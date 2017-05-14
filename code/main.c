/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/

/* Headers */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <libgs.h>
#include <libsn.h>
#include <libcd.h>
#include <stdlib.h>
#include "df_types.h"
#include "df_constants.h"
#include "df_main.h"

/* Assets */
// #include "imageh/DFBlue.h" // NOTE(Jack): defines `DFBlue`
u8 DFBlue[] = {0x0}; /* just for now */

/* Single translation unit for now, don't freak out */
#include "df_data.c"

/* For convenience, PXFINDER */
/* https://github.com/apiraino/psx_pxfinder */
#ifdef PXFINDER
#include "pxfinder/pxfinder.h"
float move_offset = 1.3;
#endif

/* Globals */
GsOT     OT[BUFFER_COUNT];
GsOT_TAG ZSort[BUFFER_COUNT][1 << OT_LENGTH];
PACKET   GPUPackets[BUFFER_COUNT][GPUPACKETSIZE * GPUPACKETMAX];
RGB8     BackgroundColor;
u8       ActiveBuffer;
u64      SystemTimer;
s32      SystemPad, SystemPadT;

// Sprite
static GsIMAGE image;
static GsSPRITE sprite;

INTERNAL void
VSYNCCounterWait( void )
{
    if (GetVideoMode() == MODE_PAL)
        SystemTimer += 5120; // (1000 / 50fps) << 8
    else
        SystemTimer += 4267; // (1000 / 60fps) << 8
}

INTERNAL void
InitializeSystem(BOOL bitmode24,
                 BOOL interlaced,
                 BOOL dithermode)
{
    DISPENV display;
    u64 videomode = 0; // NTSC by default
    u16 screen_height = SCREEN_HEIGHT_NTSC; // NTSC by default
    u16 intermode = 0;

    display.screen.x = 0;
    display.screen.y = 0;
    display.screen.w = 256;
    display.screen.h = 256;
    
    /* Check for european */
    if (*(char*) 0xbfc7ff52 == 'E')
    {
        videomode = 1;
        screen_height = SCREEN_HEIGHT_PAL;
        display.screen.y = 16; /* PAL offset (288-256) / 2 */
    }

    PutDispEnv(&display);

    /* Set interlaced flag */
    if (interlaced)
        intermode = GsINTER;
    else
        intermode = GsNONINTER;

    /* Initialize callbacks and pads */
    ResetCallback();
    PadInit(0);

    /* Setup video system */
    SetGraphDebug(0);
    SetVideoMode(videomode);
    GsInitGraph(SCREEN_WIDTH, screen_height, intermode | GsOFSGPU, dithermode, bitmode24);
    if (interlaced)
        GsDefDispBuff(0, 0, 0, 0);
    else
        GsDefDispBuff(0, 0, 0, screen_height);
    GsClearDispArea(0, 0, 0);
    
    /* Setup the order tables */
    OT[0].length = OT_LENGTH;
    OT[1].length = OT_LENGTH;
    OT[0].org    = ZSort[0];
    OT[1].org    = ZSort[1];
    GsClearOt(0, 0, &OT[0]);
    GsClearOt(0, 0, &OT[1]);

    /* Upload the default system font pattern to the VRAM and set it as our debug font */
    FntLoad(960, 256);
    FntOpen(0, 0, SCREEN_WIDTH, screen_height, 0, 512);
    
    /* Setup the counter */
    SystemTimer = 0;
    VSyncCallback(VSYNCCounterWait);
    
    /* Set background color */
    BackgroundColor.r = 50;
    BackgroundColor.g = 50;
    BackgroundColor.b = 50;
}

INTERNAL void
ExitSystem( void )
{
    PadStop();
    ResetGraph(0);
    StopCallback();
}

GsIMAGE LoadTIM(u64 *memaddr)
{
    RECT r;
    GsIMAGE tim;

    GsGetTimInfo(memaddr+1, &tim);

    r.x = tim.px;
    r.y = tim.py;
    r.w = tim.pw;
    r.h = tim.ph;
    LoadImage(&r, tim.pixel);
    DrawSync(0);

    if ((tim.pmode >> 3) & 0x01)
    {
        r.x = tim.cx;
        r.y = tim.cy;
        r.w = tim.cw;
        r.h = tim.ch;
        LoadImage(&r, tim.clut);
        DrawSync(0);
    }
    
    return tim;
}

INTERNAL GsSPRITE
MakeSprite(GsIMAGE tim)
{
    GsSPRITE spr;

    switch (tim.pmode & 3)
    {
        case 0: // 4-bit
            spr.w = tim.pw << 2;
            spr.u = (tim.px & 0x3f) * 4;
            break;
        case 1: // 8-bit
            spr.w = tim.pw << 1;
            spr.u = (tim.px & 0x3f) * 2;
            break;
        default: // 16-bit
            spr.w = tim.pw;
            spr.u = tim.px & 0x3f;
    }

    spr.h = tim.ph;
    spr.v = tim.py & 0xff;

    spr.tpage = GetTPage((tim.pmode & 3), 0, tim.px, tim.py);
    spr.attribute = (tim.pmode & 3) << 24;

    spr.cx = tim.cx;
    spr.cy = tim.cy;

    spr.r = 128;
    spr.g = 128;
    spr.b = 128;

    spr.x = 0;
    spr.y = 0;
    spr.mx = 0;
    spr.my = 0;
    spr.scalex = 1;
    spr.scaley = 1;
    spr.rotate = 0;

    return spr;
}

INTERNAL void
VRAMClear( void )
{    
    RECT rectTL;
    
    setRECT(&rectTL, 0, 0, 1024, 512);
    ClearImage2(&rectTL, 0, 0, 0);
    
    DrawSync(0); // ensure that the VRAM is clear before exiting
    
    if (DEBUG) printf("VRAM cleared!\n");
}

/*
  TODO(Jack): \\
  This is a peculiar case of he-said-she-said.
  The sequence of events that needs to happen here seems to vary in examples.
  PXFINDER performs waits _after_ the buffer swap, where other examples don't, yet it works.
  Find out what really needs to happen here.
 */
INTERNAL void
UpdateAndDisplay( void )
{
#if 0
    // refresh the font
    FntFlush(-1);
    
    // get the current buffer
    ActiveBuffer = GsGetActiveBuff();
    
    // setup the packet workbase
    GsSetWorkBase((PACKET*)GPUPackets[ActiveBuffer]);
    
    // clear the ordering table
    GsClearOt(0,0, &OT[ActiveBuffer]);

    // clear the ordering table with a background color (R,G,B)
    GsSortClear(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, &OT[ActiveBuffer]);

    // Draw sprite
    GsSortFastSprite(&image, &OT[ActiveBuffer], 0);

    #ifdef PXFINDER
    pxfinder_draw(OT[ActiveBuffer], 0);
    #endif
        
    // flip the double buffers
    GsSwapDispBuff();
    
    // wait for all drawing to finish
    DrawSync(0);
    
    // wait for v_blank interrupt
    VSync(0);
    
    // draw the ordering table
    GsDrawOt(&OT[ActiveBuffer]);
#else

    // Update input
    s32 pad = PadRead(0);
    SystemPadT = pad & (pad ^ SystemPad);
    SystemPad  = pad;
    
    // Set and clear the buffer
    ActiveBuffer = GsGetActiveBuff();
    GsSetWorkBase((PACKET*)GPUPackets[ActiveBuffer]);
    GsClearOt(0, 0, &OT[ActiveBuffer]);
    
    // Display
    if (DEBUG) FntFlush(-1);
    DrawSync(0);
    VSync(0);
    GsSwapDispBuff();
    GsSortClear(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, &OT[ActiveBuffer]);
    GsDrawOt(&OT[ActiveBuffer]);

    
    // Draw
#ifdef PXFINDER
    pxfinder_draw(OT[ActiveBuffer], 1);
#endif
    
    GsSortFastSprite(&sprite, &OT[ActiveBuffer], 0);
#endif
}

int main( void )
{
    printf("\n\n DANGER FORCE by Jack Riales ((NotebookDeviant)) [2017] \nhttps://github.com/JackRiales/Danger-Force-Psyq");

    InitializeSystem(0, 0, 1);

    image = LoadTIM((u64*)&DFBlue);
    sprite = MakeSprite(image);
    
    #ifdef PXFINDER
    pxfinder_init();
    #endif
    
    while (TRUE)
    {
        sprite.x = (sprite.x + 1) % SCREEN_WIDTH;
        UpdateAndDisplay();
    }

    ExitSystem();
    return 0;
}
