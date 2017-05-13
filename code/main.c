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
#include <stdlib.h>
#include "df_types.h"
#include "df_constants.h"
#include "df_main.h"

/* Assets */
#include "imageh/DFBlue.h" // NOTE(Jack): defines `DFBlue`

/* For convenience, PXFINDER */
/* https://github.com/apiraino/psx_pxfinder */
#ifdef PXFINDER
#include "pxfinder/pxfinder.h"
float move_offset = 1.3;
#endif

/* Globals */

// Actually important stuff
GsOT sgMainOrderTable[BUFFER_COUNT];
GsOT_TAG sgMainOrderTableTag[BUFFER_COUNT][1<<OT_LENGTH];
PACKET sgGPUPacketArea[BUFFER_COUNT][GPUPACKETSIZE * GPUPACKETMAX];
INTERNAL u8 ClearColor[3];
INTERNAL s8 CurrentBuffer; /* Holds the ID of the current buffer */
INTERNAL u64 SystemTimer;

// Testing stuff
GsSPRITE image;

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
    printf("\n\n DANGER FORCE by Jack Riales ((NotebookDeviant)) [2017] \nhttps://github.com/JackRiales/Danger-Force-Psyq");

    u64 videomode = 0; // NTSC by default
    u16 screen_height = SCREEN_HEIGHT_NTSC; // NTSC by default
    u16 intermode = 0;

    /* Check for european */
    if (*(char*) 0xbfc7ff52 == 'E')
    {
        videomode = 1;
        screen_height = SCREEN_HEIGHT_PAL;
    }

    /* Set interlaced flag */
    if (interlaced)
        intermode = GsINTER;
    else
        intermode = GsNONINTER;
    
    ResetCallback();
    PadInit(0);

    SetGraphDebug(0);
    SetVideoMode(videomode);
    GsInitGraph(SCREEN_WIDTH, screen_height, intermode | GsOFSGPU, dithermode, bitmode24);
    
    /* Set background color */
    ClearColor[0] = 0;
    ClearColor[1] = 0;
    ClearColor[2] = 0;
}

INTERNAL void
CreateSprite( GsSPRITE *sprite, u64 image_addr )
{
    #if 1
    GsIMAGE timdata;
    s32 image_type;
    s32 width_compression;

    // Get the information about the TIM from the given address
    GsGetTimInfo((u64*) image_addr + TIM_HEADER_SIZE, &timdata);

    // Gather what kind of width compression it has
    image_type = timdata.pmode & 3;
    switch (image_type)
    {
        case 0: { width_compression = 4; break; }
        case 1: { width_compression = 2; break; }
        case 2: { width_compression = 1; break; }
        case 3: { printf("\n24-bit TIMs are unsupported right now."); break; }
    }

    // Set attribute color depth and size
    sprite->attribute = (image_type << 24);
    sprite->w = timdata.pw * width_compression;
    sprite->h = timdata.ph;

    // Set texture page
    sprite->tpage = GetTPage(image_type, 0, timdata.px, timdata.py);

    // VRAM offset
    sprite->u = (timdata.px % 64) * width_compression;
    sprite->v = timdata.py % 256;

    // Set CLUT position if it's 4 or 8 bit
    if (image_type < 2)
    {
        sprite->cx = timdata.cx;
        sprite->cy = timdata.cy;
    }

    // Set all brightnesses to midvalue
    sprite->r = 128;
    sprite->g = 128;
    sprite->b = 128;

    // Set scale and rotation (defaults)
    // 4096 is the GPU equiv of 1
    sprite->scalex = 4096;
    sprite->scaley = 4096;
    sprite->rotate = 0;

    // Set the origin (m) to be the center of the sprite by default
    sprite->mx = timdata.pw * width_compression / 2;
    sprite->my = timdata.ph / 2;

    // Set position to 0 by default
    sprite->x = 0;
    sprite->y = 0;
    #else
    RECT r;
    GsIMAGE *image;
    GsGetTimInfo((u64*) (image_addr + TIM_HEADER_SIZE), image);

    r.x = image->px;
    r.y = image->py;
    r.w = image->pw;
    r.h = image->ph;
    LoadImage(&r, image->pixel);
    if ((image->pmode & 3) < 2)
    {
        r.x = image->cx;
        r.y = image->cy;
        r.w = image->cw;
        r.h = image->ch;
        LoadImage(&curRect, image->clut);
    }
    #endif
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
  Initialize the videomode, graphs, and ordertables
*/
INTERNAL void
InitializeGraphics( void )
{    
    // Check for NTSC mode
    s32 offset = 0; /* Can't remember where this goes */
    u32 flags = GsNONINTER | GsOFSGPU;
    b32 pal_mode = FALSE;
    u32 screen_height = SCREEN_HEIGHT_NTSC;

    SetDispMask(1);
    ResetGraph(0);
    InitGeom();
    VRAMClear();
    
    pal_mode = (*(char*) 0xbfc7ff52 == 'E'); /* Check ROM for Europe */
    if (pal_mode)
    {
        if (DEBUG) printf("Setting playstation to PAL video mode.");
        flags = GsINTER | GsOFSGPU;
        screen_height = SCREEN_HEIGHT_PAL;
    }
        
    // Set video mode and offset
    SetVideoMode(pal_mode);
    GsInitGraph(SCREEN_WIDTH, screen_height, flags, 1, 0);

    // set the top left coordinates of the two buffers in video memory
    GsDefDispBuff(0, 0, 0, screen_height);
    
    // Initialize the order tables
    sgMainOrderTable[0].length = OT_LENGTH;
    sgMainOrderTable[1].length = OT_LENGTH;
    sgMainOrderTable[0].org = sgMainOrderTableTag[0];
    sgMainOrderTable[1].org = sgMainOrderTableTag[1];

    // Clear order tables
    GsClearOt(0, 0, &sgMainOrderTable[0]);
    GsClearOt(0, 0, &sgMainOrderTable[1]);
}

/*
  TODO(Jack): \\
  This is a peculiar case of he-said-she-said.
  The sequence of events that needs to happen here seems to vary in examples.
  PXFINDER performs waits _after_ the buffer swap, where other examples don't, yet it works.
  Find out what really needs to happen here.
 */
INTERNAL void
Display( void )
{
    if (DEBUG) FntPrint("DANGER-FORCE\nBY JACK RIALES");

    // refresh the font
    FntFlush(-1);
    
    // get the current buffer
    CurrentBuffer = GsGetActiveBuff();
    
    // setup the packet workbase
    GsSetWorkBase((PACKET*)sgGPUPacketArea[CurrentBuffer]);
    
    // clear the ordering table
    GsClearOt(0,0, &sgMainOrderTable[CurrentBuffer]);

    // clear the ordering table with a background color (R,G,B)
    GsSortClear(50,50,50, &sgMainOrderTable[CurrentBuffer]);

    // Draw sprite
    GsSortFastSprite(&image, &sgMainOrderTable[CurrentBuffer], 0);

    #ifdef PXFINDER
    pxfinder_draw(sgMainOrderTable[CurrentBuffer], 1);
    #endif
        
    // flip the double buffers
    GsSwapDispBuff();
    
    // wait for all drawing to finish
    DrawSync(0);
    
    // wait for v_blank interrupt
    VSync(0);
    
    // draw the ordering table
    GsDrawOt(&sgMainOrderTable[CurrentBuffer]);
}

int main( void )
{
#if DEBUG
    printf("\n\nRUNNING DANGER-FORCE ON PSX\n");
#endif
    ResetCallback();
    InitializeGraphics();
    PadInit(0);
    
    /* Upload the default system font pattern to the VRAM and set it as our debug font */
    FntLoad(960, 256);
    SetDumpFnt(FntOpen(45, 0, SCREEN_WIDTH, 64, 0, 512));

    #ifdef PXFINDER
    pxfinder_init();
    #endif
    
    // Sprite
    CreateSprite(&image, (u64) &DFBlue);
    
    while (TRUE)
    {
        Display();
        image.x = (image.x + 1) % SCREEN_WIDTH;
    }

    PadStop();
    ResetGraph(0);
    StopCallback();
    
    return 0;
}
