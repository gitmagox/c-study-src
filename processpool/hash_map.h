//
// Created by magox on 7/7/20.
//

#ifndef PROCESSPOOL_HASH_MAP_H
#define PROCESSPOOL_HASH_MAP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "map.h"


static inline char* string_join(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    if (result == NULL) exit (1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static inline char* int2string(int i){
    int count=0;
    int tmp=i;
    char *type="";
    while(i!=0){
        i /= 10;
        count++;
    }
    int len = count+1;
    char a[len];
    snprintf(a, 13, "%d", tmp);
    type = a;
    return type;
}

static inline char * hash_map_create_string(int * count,...){
    char * key;
    char * tmp;
    va_list str;
    va_start(str, count);
    for(int i = 0; i < count; i++)
    {
        tmp = va_arg(str, char *);
        key = string_join(key,tmp);
    }
    va_end(str);
    return key;
}

static inline char * hash_map_create_int(int * count,...){
    char *type="";
    char * key="";
    char * join = "k";
    int j;
    va_list str;
    va_start(str, count);
    for(int i = 0; i < count; i++)
    {
        j = va_arg(str, int);
        type = int2string(j);
        key = string_join(key,join);
        key = string_join(key,type);
    }
    va_end(str);
    return key;
}


#define hash_map_get_key(T,count, ...) hash_map_create_##T( count,__VA_ARGS__)

#define hash_map_t(T) map_t(T)

#define hash_map_init(m) map_init(m)

#define hash_map_deinit(m) map_deinit(m)

#define hash_map_get(m, key) map_get(m, key)

#define hash_map_set(m, key, value) map_set(m, key, value)

#define hash_map_remove(m, key) map_remove(m, key)



#endif //PROCESSPOOL_HASH_MAP_H
