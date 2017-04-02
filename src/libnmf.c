/** NMF: No More Free
  * Improved allocation library
  * Copyright (C) 2017 meow & samel
  */

#include "libnmf.h"

#ifdef NMF_DETAILED_DEBUG
#define DEBUG(...); printf(__VA_ARGS__);
#else
#define DEBUG(...);
#endif

//memory chunk data structure
struct dummy_nmf_mem_chunk
{
 int allocation_id;
 void* pointer;
#ifdef NMF_DETAILED_DEBUG
 size_t size;
 char file_name[NMF_MAX_FILENAME_LEN];
 unsigned int file_line;
#endif
};
typedef struct dummy_nmf_mem_chunk nmf_mem_chunk;

nmf_mem_chunk* nmf_malloc_pool=NULL;
unsigned int nmf_malloc_pool_position=0;
unsigned int nmf_malloc_actual_id=0;
unsigned int nmf_malloc_max=NMF_MAX_MALLOC_PER_RESIZE;
int nmf_initialized=0;

void __attribute__((__no_instrument_function__)) compact_malloc(unsigned int i);
int __attribute__((__no_instrument_function__)) nmf_init(void);
int __attribute__((__no_instrument_function__)) nmf_deinit(void);

int __attribute__((__no_instrument_function__)) nmf_init()
{
 DEBUG("libnmf - Init nmf_malloc_max=%d\n",nmf_malloc_max);
 if(nmf_initialized)
 {
  DEBUG("libnmf - Failed,library already initialized\n");
  return 0;
 }
 if((nmf_malloc_pool=malloc(nmf_malloc_max*sizeof(nmf_mem_chunk)))==NULL)
 {
  DEBUG("libnmf - Failed during nmf_malloc_pool allocation\n");
  exit(-1);
 }
 memset(nmf_malloc_pool,0,nmf_malloc_max*sizeof(nmf_mem_chunk));
 nmf_initialized=1;
 DEBUG("libnmf - Init success\n");
 return 1;
}

int __attribute__((__no_instrument_function__)) nmf_deinit(void)
{
 unsigned int i=0;
 DEBUG("libnmf - Deinit\n");
 if(!nmf_initialized)
 {
  DEBUG("libnmf - Failed,library not initialized\n");
  return 0;
 }
 for(i=0;i<nmf_malloc_max;i++)
 {
  DEBUG("libnmf - Pointer at %d is %p during nmf_deinit\n",i,nmf_malloc_pool[i].pointer);
  if (nmf_malloc_pool[i].pointer!=NULL)
  {
#ifdef NMF_DETAILED_DEBUG
   DEBUG("libnmf - Freeing %p at position %d, file=%s, line=%d during nmf_deinit\n",nmf_malloc_pool[i].pointer,i,nmf_malloc_pool[i].file_name,nmf_malloc_pool[i].file_line);
#else
   DEBUG("libnmf - Freeing %p at position %d during nmf_deinit\n",nmf_malloc_pool[i].pointer,i);
#endif
   free(nmf_malloc_pool[i].pointer);
  }
 }
 free(nmf_malloc_pool);
 nmf_initialized=0;
 nmf_malloc_pool_position=0;
 DEBUG("libnmf - Deinit success\n");
 return 1;
}

void* __attribute__((__no_instrument_function__)) nmf_malloc_internal(size_t size,char* file_name,unsigned int file_line)
{
 void* ret=NULL;
 DEBUG("libnmf - Malloc size=%ld byte, file=%s, line=%d\n",size,file_name,file_line);
 //do nothing if the library is not initialized
 if(!nmf_initialized)
 {
  DEBUG("libnmf - Failed,library not initialized\n");
  return NULL;
 }
 if((ret=malloc(size))==NULL)
 {
  DEBUG("libnmf - Failed during memory allocation\n");
  return NULL;
 }
 if(nmf_malloc_pool_position>=nmf_malloc_max)
 {
  nmf_malloc_max+=NMF_MAX_MALLOC_PER_RESIZE;
  DEBUG("libnmf - Resizing malloc pool to %d\n",nmf_malloc_max);
  if((nmf_malloc_pool=realloc(nmf_malloc_pool,nmf_malloc_max*sizeof(nmf_mem_chunk)))==NULL)
  {
   DEBUG("libnmf - Failed during nmf_malloc_pool allocation\n");
   exit(-1);
  }
  nmf_print_status();
 }
 nmf_malloc_pool[nmf_malloc_pool_position].pointer=ret;
 nmf_malloc_pool[nmf_malloc_pool_position].allocation_id=nmf_malloc_actual_id;
#ifdef NMF_DETAILED_DEBUG
 nmf_malloc_pool[nmf_malloc_pool_position].size=size;
 strncpy(nmf_malloc_pool[nmf_malloc_pool_position].file_name,file_name,NMF_MAX_FILENAME_LEN);
 nmf_malloc_pool[nmf_malloc_pool_position].file_line=file_line;
#endif
 DEBUG("libnmf - Malloc success (%p) at position: %d\n",ret,nmf_malloc_pool_position);
 nmf_malloc_pool_position++;
 return ret;
}

int __attribute__((__no_instrument_function__)) nmf_free_internal(void* pointer,char* file_name,unsigned int file_line)
{
 int i=0;
 DEBUG("libnmf - Free pointer=%p, file=%s, line=%d\n",pointer,file_name,file_line);
 if(!nmf_initialized)
 {
  DEBUG("libnmf - Failed,library not initialized\n");
  return 0;
 }
 while(i<nmf_malloc_pool_position)
 {
  if(nmf_malloc_pool[i].pointer==pointer)
  {
   DEBUG("libnmf - Pointer allocated at file=%s, line=%d was at position=%d\n",nmf_malloc_pool[i].file_name,nmf_malloc_pool[i].file_line,i);
   free(pointer);
   nmf_malloc_pool[i].pointer=NULL;
   nmf_malloc_pool[i].allocation_id=0;
#ifdef NMF_DETAILED_DEBUG
   nmf_malloc_pool[i].size=0;
   memset(nmf_malloc_pool[i].file_name,0,NMF_MAX_FILENAME_LEN);
   nmf_malloc_pool[i].file_line=-1;
#endif
   compact_malloc(i);
   nmf_malloc_pool_position--;
   DEBUG("libnmf - Free success\n");
   return 1;
  }
  i++;
 }
 DEBUG("libnmf - Failed pointer not found\n");
 return 0;
}

int __attribute__((__no_instrument_function__)) nmf_free_allocation_id(unsigned int id)
{
 int i=0;
 DEBUG("libnmf - Free allocation id=%d\n",id);
 if(!nmf_initialized)
 {
  DEBUG("libnmf - Failed,library not initialized\n");
  return 0;
 }
 for(i=nmf_malloc_pool_position-1;i>=0;i--)
 {
  if (nmf_malloc_pool[i].allocation_id==id)
  {
#ifdef NMF_DETAILED_DEBUG
   nmf_free_internal(nmf_malloc_pool[i].pointer,nmf_malloc_pool[i].file_name,nmf_malloc_pool[i].file_line);
#else
   nmf_free(nmf_malloc_pool[i].pointer);
#endif
  }
  else
  {
   break;
  }
 }
 DEBUG("libnmf - Free allocation id success\n");
 return 1;
}

void __attribute__((__no_instrument_function__)) nmf_print_status(void)
{
 int i=0;
 DEBUG("libnmf - Status\n");
 if(!nmf_initialized)
 {
  DEBUG("libnmf - Failed,library not initialized\n");
  return;
 }
 DEBUG("libnmf - Malloc pool %d%% (%d/%d) full\n",
       (nmf_malloc_pool_position*100)/nmf_malloc_max,
       nmf_malloc_pool_position,
       nmf_malloc_max);
 for(i=0;i<nmf_malloc_pool_position;i++)
 {
  DEBUG("libnmf - Pointer at position=%d has pointer=%p, size=%ld",i,nmf_malloc_pool[i].pointer,nmf_malloc_pool[i].size);
#ifdef NMF_DETAILED_DEBUG
  DEBUG(", file=%s, line:%d\n",nmf_malloc_pool[i].file_name,nmf_malloc_pool[i].file_line);
#else
  DEBUG("\n");
#endif
 }
}

void __attribute__((__no_instrument_function__)) compact_malloc(unsigned int i)
{
 unsigned int k=i;
 DEBUG("libmnf - Compacting malloc pool from %d\n",i);
 while(k<nmf_malloc_pool_position-1)
 {
  DEBUG("libmnf - Compacting malloc pool[%d]=pool[%d]\n",k,k+1);
  nmf_malloc_pool[k]=nmf_malloc_pool[k+1];
  k++;
 }
}

void __attribute__((__no_instrument_function__)) __cyg_profile_func_enter(void *this_fn,void *call_site)
{
 if (nmf_malloc_actual_id==0)
 {
  nmf_init();
 }
 nmf_malloc_actual_id++;
}

void __attribute__((__no_instrument_function__)) __cyg_profile_func_exit(void *this_fn,void *call_site)
{
 nmf_free_allocation_id(nmf_malloc_actual_id);
 nmf_malloc_actual_id--;
 if (nmf_malloc_actual_id==0)
 {
  nmf_deinit();
 }
}

