/*
filebuffer.h
Written in 2015 by Michael Bosse metiscus@gmail.com

To the extent possible under law, the author(s) have dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with
this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#ifndef FILEBUFFER_H_INCLUDED
#define FILEBUFFER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

struct filebuffer_t;

extern struct filebuffer_t* filebuffer_new(FILE *fp, uint32_t size);
extern void filebuffer_delete(struct filebuffer_t* buffer);
extern void filebuffer_fill(struct filebuffer_t * buffer, uint32_t n);
extern uint32_t filebuffer_get(struct filebuffer_t * buffer, char* data, uint32_t n);
extern void filebuffer_reset(struct filebuffer_t* buffer, FILE *fp);

#endif // FILEBUFFER_H_INCLUDED
