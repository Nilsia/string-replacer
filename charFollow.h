#ifndef __CHAR_FOLLOW__H__
#define __CHAR_FOLLOW__H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct CharFollow {
    char c;
    struct CharFollow *charFollowing;
} *CharF, CharFollow;

typedef enum Bool
{
    False,
    True
} Bool;


CharF CF_create();
Bool CF_is_empty(CharF cf);
CharF CF_add(CharF cf, char c);
CharF CF_remove(CharF cf);
CharF CF_get_last(CharF cf);
CharF CF_clear(CharF cf);
void CF_print(CharF cf);
Bool CF_contains(CharF cf, char* str);


#endif