//
// Created by magox on 7/8/20.
//

#ifndef PROCESSPOOL_BASE_H
#define PROCESSPOOL_BASE_H

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

enum
{
    RET_OK,
    RET_FAIL
};

#define get_thiz_parent(P,T,Z) (P *)((char *) Z + offsetof( P, T ))

#define get_thiz(P,T,t,B) (T *)((char *) B - offsetof( P, t ))

#endif //PROCESSPOOL_BASE_H
