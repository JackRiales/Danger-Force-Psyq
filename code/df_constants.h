#if !defined(DF_CONSTANTS_H_)
/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/
#define DF_CONSTANTS_H_

/* System and region variables  */
#define DEBUG 1                 /* Show and print debug information */
#define OT_LENGTH 10            /* Maximum OrderTable length */
#define BUFFER_COUNT 2          /* Graphics system will be double-buffered */
#define GPUPACKETSIZE 64        /* Size of each GPU packet (?)*/
#define GPUPACKETMAX 1000       /* Maximum number of objects that can be on screen (?) */
#define SCREEN_WIDTH 320        /* Screen width, in pixels (standard) */
#define SCREEN_HEIGHT_NTSC 240  /* NTSC (American) screen height in pixels */
#define SCREEN_HEIGHT_PAL  256  /* PAL (International) screen height */

/* System constants */
#define __ramsize   0x00200000; /* 2 MB of RAM */
#define __stacksize 0x00004000; /* 16 KB of Stack Space */
#define TIM_HEADER_SIZE 4       /* Size of the header of a tim file.
                                   Used for when we only want the actual image data */

/* Game specific variables */
#define MAX_BULLETS 1000        /* The absolute maximum amount of bullets that
                                   can be made at once                        */

#endif
