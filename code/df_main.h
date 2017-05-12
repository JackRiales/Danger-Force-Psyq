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

#endif
