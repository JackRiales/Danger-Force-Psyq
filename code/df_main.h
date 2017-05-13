#if !defined(DF_MAIN_H)
/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/
#define DF_MAIN_H

typedef struct
{
    VEC2 position;
    s32 rotation;
    GsSPRITE sprite;
} PLAYERDATA;

typedef enum
{
    DF_STATE_MAINMENU,
    DF_STATE_LVL0,
    DF_STATE_LVL1,
    DF_STATE_LVL2,
    DF_STATE_LVL3,
    DF_STATE_LVL4,
    DF_STATE_LVL5,
    DF_STATE_LVL6,
    DF_STATE_LVLEXTRA,
} GAMESTATE;

typedef struct
{

} GAMEDATA;

typedef struct
{
    GsOT     ot[BUFFER_COUNT];
    GsOT_TAG zsort[BUFFER_COUNT][1 << OT_LENGTH];
    PACKET   gpu_packet[BUFFER_COUNT][GPUPACKETSIZE * GPUPACKETMAX];
    RGB8     clear;
    u8       interlaced;
    u8       dither;
    u8       bitmode;
    u8       current_buffer;
    u64      timer;
} SYSTEM;

#endif
