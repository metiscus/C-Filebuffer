/*
filebuffer_test.c
Written in 2015 by Michael Bosse metiscus@gmail.com

To the extent possible under law, the author(s) have dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#include "filebuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int g_fail = 0;

#define BeginTest(msg)                                              \
    do {                                                            \
        printf("[Test %s]\n", msg);
#define TestAssert(cond)                                            \
        {                                                                   \
            int result = (cond);                                            \
            printf("\t[%-50s] : %s\n", #cond, (result) ? "true" : "false"); \
            g_fail += !result;                                              \
        }
#define EndTest()\
    } while(0);

int main(int argc, char** argv)
{

    //Test 1
    //Technically opening this file should fail
    BeginTest("Null File")
    FILE *fp = fopen(tmpnam(0), "rt");
    TestAssert(fp == 0);
    struct filebuffer_t *buffer = filebuffer_new(fp, 1000);
    filebuffer_fill(buffer, 100);
    char data[100];
    TestAssert(filebuffer_get(buffer, data, 100) == 0);
    filebuffer_delete(buffer);
    EndTest();


    //Test 2
    BeginTest("Small File")
    char *filename = strdup(tmpnam(0));
    FILE *fp = fopen(filename, "wt");
    TestAssert(fp != 0);
    char data[100];
    memset(data, 'c', 100);
    TestAssert(fwrite(data, 1, 100, fp) == 100);
    fclose(fp);

    fp = fopen(filename, "rt");
    TestAssert(fp != 0);

    struct filebuffer_t *buffer = filebuffer_new(fp, 1000);
    filebuffer_fill(buffer, 1000);
    char data2[1000];
    TestAssert(filebuffer_get(buffer, data2, 1000) == 100);
    filebuffer_delete(buffer);
    free(filename);
    fclose(fp);
    TestAssert(memcmp(data, data2, 100) == 0);
    EndTest();

    //Test 3
    BeginTest("Large File, multi reads")
    char *filename = strdup(tmpnam(0));
    FILE *fp = fopen(filename, "wt");
    TestAssert(fp != 0);
    char data[10500];
    int i;
    for(i=0; i<21; ++i)
    {
        memset(&data[i*500], i, 500);
    }

    TestAssert(fwrite(data, 1, 10500, fp) == 10500);
    fclose(fp);

    fp = fopen(filename, "rt");
    TestAssert(fp != 0);

    struct filebuffer_t *buffer = filebuffer_new(fp, 1000);
    filebuffer_fill(buffer, 1000);
    char data2[1000];
    for(i=0; i<10; ++i){
        filebuffer_fill(buffer, 1000);
        TestAssert(filebuffer_get(buffer, data2, 1000) == 1000);
    }
    filebuffer_fill(buffer, 1000);
    TestAssert(filebuffer_get(buffer, data2, 1000) == 500);
    filebuffer_delete(buffer);
    free(filename);
    fclose(fp);
    TestAssert(memcmp(&data[10000], data2, 500) == 0);
    EndTest();
    return g_fail;
}
