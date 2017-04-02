/** NMF: No More Free
  * Improved allocation library
  * Copyright (C) 2017 meow & samel
  */

#ifndef __LIBNMF__
#define __LIBNMF__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NMF_MAX_MALLOC_PER_RESIZE
/** Initial length of the allocation list
  */
#define NMF_MAX_MALLOC_PER_RESIZE (2)
#endif

#ifndef NMF_MAX_FILENAME_LEN
/** Size of the buffer containing the file name
  */
#define NMF_MAX_FILENAME_LEN (32)
#endif

/** @defgroup Public Macros to expose NMF functions to the world
  * @{
  */

/** Macro for the NMF allocation function: on debug mode this function has 2 more parameters (file name and line number)
  */
#define nmf_malloc(A) nmf_malloc_internal((A),__FILE__,__LINE__)

/** Macro for the NMF free function: on debug mode this function has 2 more parameters (file name and line number)
  */
#define nmf_free(A) nmf_free_internal((A),__FILE__,__LINE__)

/** @}
  */


/** @defgroup Internal Some internal function prototype
  * @{
  */

/**
  * Allocate and register a chunk of memory _ (internal use only) _
  * @param size The size of new allocated chunck
  * @param file_name The name of the file that call this function
  * @param file_line The line of the file that call this function
  * @return The pointer to new chunk
  */
void* nmf_malloc_internal(size_t size,char* file_name,unsigned int file_line);

/**
  * Free an allocated chunk of memory _ (internal use only) _
  * @param pointer the pointer that will be deleted
  * @param file_name The name of the file that call this function
  * @param file_line The line of the file that call this function
  * @return The function's results
  */
int nmf_free_internal(void* pointer,char* file_name,unsigned int file_line); 

/**
  * Print the library status
  */
void nmf_print_status(void);

/** @}
  */

#endif

