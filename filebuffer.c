/*
filebuffer.c
Written in 2015 by Michael Bosse metiscus@gmail.com

To the extent possible under law, the author(s) have dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#include "filebuffer.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifndef MIN
#define MIN(x, y) ((x) > (y) ? (y) : (x))
#endif

struct filebuffer_t
{
    uint32_t size;
    char *data;
    char *cursor;
    char *end;
    FILE *fp;
};

struct filebuffer_t* filebuffer_new(FILE *fp, uint32_t size)
{
    struct filebuffer_t *buffer = (struct filebuffer_t *)malloc(sizeof(struct filebuffer_t));

    if(buffer) {
        buffer->fp     = fp;
        buffer->size   = size;
        buffer->data   = (char*)calloc(1, size);
        buffer->cursor = buffer->data;
        buffer->end    = buffer->data + 1;
    }

    return buffer;
}

void filebuffer_delete(struct filebuffer_t* buffer)
{
    if(buffer) {
        free(buffer->data);
        buffer->size   = 0;
        buffer->fp     = 0;
        buffer->cursor = 0;
        buffer->end    = 0;
        buffer->data   = 0;
    }
    free(buffer);
}

void filebuffer_fill(struct filebuffer_t * buffer, uint32_t n)
{
    if(buffer && buffer->fp) {

        /* move data to front of buffer and adjust */
        ptrdiff_t leftover = buffer->end - buffer->cursor - 1;
        if(leftover > 0) {
            memmove(buffer->data, buffer->cursor, leftover);
        }
        buffer->cursor = buffer->data;
        buffer->end    = buffer->cursor + leftover + 1;

        /* determine how much to read */
        uint32_t freeSize = buffer->size - leftover;
        uint32_t toRecv = MIN(freeSize, n);

        if(toRecv >0) {
            buffer->end = buffer->data + leftover;
            size_t recvd = fread(buffer->end, 1, toRecv, buffer->fp);
            buffer->end += recvd + 1;
        }
    }
}

uint32_t filebuffer_get(struct filebuffer_t * buffer, char* data, uint32_t n)
{
    uint32_t ret = 0;
    if(buffer) {
        ret = n;
        ptrdiff_t avail = buffer->end - buffer->cursor - 1;
        ret = MIN(n, avail);

        memcpy(data, buffer->data, ret);
        buffer->cursor += ret;
    }

    return ret;
}

void filebuffer_reset(struct filebuffer_t* buffer, FILE *fp)
{
    if(buffer)
    {
        buffer->cursor = buffer->data;
        buffer->end = buffer->data + 1;
        buffer->fp = fp;
    }
}
