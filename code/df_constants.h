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
#define OT_LENGTH 1             /* Maximum OrderTable length */
#define PACKETMAX 18            /* Maximum number of objects that can be on screen */
#define SCREEN_WIDTH 320        /* Screen width, in pixels (standard) */
#if REGION_NTSC
#define SCREEN_HEIGHT 240       /* NTSC (American) screen height in pixels */
#else
#define SCREEN_HEIGHT 256       /* PAL (International) screen height */
#endif

/* System constants */
#define __ramsize   0x00200000; /* 2 MB of RAM */
#define __stacksize 0x00004000; /* 16 KB of Stack Space */
#define TIM_HEADER_OFFSET 4     /* Size of the header of a tim file.
                                   Used for when we only want the actual image data */

/* Game specific variables */
#define MAX_BULLETS 1000        /* The absolute maximum amount of bullets that
                                   can be made at once                        */

#endif
