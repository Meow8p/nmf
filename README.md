# NMF: No More Free

## Introduction 

The NMF library acts like a garbage collector: it keeps a list of all allocation you have in your code and automatically free all of them at the end.

## Usage

1. Download the library code or clone it from this GitHub repository
1. Move the code into your project source directory
1. Add it to your makefile (or add it to your IDE project)
1. Include the libnmf.h header into the source files where you want to use the NMF library
1. In those files, replace all of your malloc() calls with nmf_malloc()
1. In those files, replace all of your free() calls with nmf_free(), or comment them at all (the library will free all buffers for you)
1. Add to your compiler flags `-finstrument-functions` 
1. Compile!

That’s it!

## Important note
This library collects all allocations done into every function. At the end of the function, all buffer allocated from the start of the same function will be freed.
If you usually alloc buffers into function and use them as return values, you should avoid that. Instead, you should alloc the buffer in the caller function, pass the pointer as parameter and just fill it.

## Debug mode

If you want to see a verbose library output, to check all operations, add this define to your compilers flags `-DNMF_DETAILED_DEBUG`.

## Tuning

If you want to tune the library perfectly on your project, you should check this compiler flags:
* `NMF_MAX_MALLOC_PER_RESIZE`, this is the initial length of the allocation list. The library will automatically increase the size when needed, but this operation is not cost-less. If you want, you can tune this value depending on the number of allocations you need. 
* `NMF_MAX_FILENAME_LEN`, this is the size of the buffer containing the file name. It is used only in debug mode. If your files have names longer than this value and you want to see them whole, tune this value.
 
