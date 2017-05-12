#ifndef DF_TYPES_H_
/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/
#define DF_TYPES_

// My typename prefs
typedef char   s8;
typedef u_char u8;
typedef short  s16;
typedef u_short u16;
typedef int    s32;
typedef u_int  u32;
typedef long   s64;
typedef u_long u64;
typedef int    b32;
typedef int    BOOL;
typedef float  f32;
typedef double f64;
#define TRUE 1
#define FALSE 0
#define INTERNAL static
#define GLOBAL_PERSIST static

typedef union
{
    struct
    {
        f32 x, y;
    };
    struct
    {
        f32 u, v;
    };
    f32 P[2];
} VEC2;

typedef struct
{
    u8 r;
    u8 g;
    u8 b;
} RGB8;

typedef struct
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} RGBA8;

#endif
