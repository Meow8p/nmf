/******************************************************************************!
 *    Improved allocation library
 *    Copyright (C) 2017 meow & samel
 ****************************************************************************/

#include "../src/libnmf.h"

void pish(void)
{
 nmf_malloc(30);
 nmf_malloc(30);
 nmf_malloc(30);
 nmf_malloc(30);
 nmf_print_status();
}

void maddaffakka(void)
{
 nmf_malloc(70);
 nmf_malloc(70);
 nmf_malloc(70);
 nmf_malloc(70);
 nmf_print_status();
}

int main(int argc, char **argv)
{
 void *a;

 printf("NMF LIBRARY TEST\n");
 printf("__________________________________________________________________\n");

 printf("malloc test\n");
 printf("__________________________________________________________________\n");
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 a = nmf_malloc(10);
 pish();
 pish();
 maddaffakka();
}

