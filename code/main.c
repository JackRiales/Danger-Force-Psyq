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
#include "asset/DFBLUE.h" // NOTE(Jack): defines `dfblue`

/* Globals */
/* TODO(Jack): Localize to main or something. Minimize globals! */
char version[5]="V0.1";

GsOT sgMainOrderTable[2];
GsOT_TAG sgMainOrderTableTag[2][1<<OT_LENGTH];
PACKET sgGPUPacketArea[2][PACKETMAX];
INTERNAL s8 CurrentBuffer = 0; /* Holds the ID of the current buffer */
GsSPRITE image;

INTERNAL void
CreateSprite(GsSPRITE *sprite, const char imageAddress[])
{
    RECT rect;
    GsIMAGE tdata;

    GsGetTimInfo((u_long*)(imageAddress+4), &tdata);

    // load the image into the frame buffer
   rect.x = tdata.px;                                    // tim start X coord
   rect.y = tdata.py;                                    // tim start Y coord
   rect.w = tdata.pw;                                    // data width
   rect.h = tdata.ph;                                    // data height
   // load the tim data into the frame buffer
   LoadImage(&rect, tdata.pixel);       

   // load the CLUT into the frame buffer
   rect.x = tdata.cx;                                    // x pos in frame buffer
   rect.y = tdata.cy;                                    // y pos in frame buffer
   rect.w = tdata.cw;                                    // width of CLUT
   rect.h = tdata.ch;                                    // height of CLUT
   // load data into frame buffer (DMA from DRAM to VRAM)
   LoadImage(&rect, tdata.clut);                

   // initialise sprite
   sprite->attribute=0x2000000;                           // 16 bit CLUT, all options off (0x1 = 8-bit, 0x2 = 16-bit)
   sprite->x = 0;                                         // draw at x coord 0
   sprite->y = 0;                                         // draw at y coord 0
   sprite->w = 320;                                       // width of sprite
   sprite->h = tdata.ph;                               // height of sprite
   
   // texture page | texture mode (0 4-bit, 1 8-bit, 2 16-bit), semi-transparency rate, texture x, texture y in the framebuffer
   sprite->tpage=GetTPage(1, 2, 320, 0);
   sprite->r = 128;                                       // RGB Data
   sprite->g = 128;
   sprite->b = 128;
   sprite->u=0;                                           // position within timfile for sprite
   sprite->v=0;                                           
   sprite->cx = tdata.cx;                              // CLUT location x coord
   sprite->cy = tdata.cy;                              // CLUT location y coord
   sprite->r=sprite->g=sprite->b=128;                 // normal luminosity
   sprite->mx = 0;                                        // rotation x coord
   sprite->my = 0;                                        // rotation y coord
   sprite->scalex = ONE;                                  // scale x coord (ONE = 100%)
   sprite->scaley = ONE;                                  // scale y coord (ONE = 100%)
   sprite->rotate = 0;                                    // degrees to rotate   

   // wait for all drawing to finish
   DrawSync(0);
}

INTERNAL void
VRAMClear()
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

    SetDispMask(1);
    ResetGraph(0);
    VRAMClear();
    
    pal_mode = (*(char*) 0xbfc7ff52 == 'E'); /* Check ROM for Europe */
    if (pal_mode)
    {
        if (DEBUG) printf("Setting playstation to PAL video mode.");
        flags = GsINTER | GsOFSGPU;
    }
        
    // Set video mode and offset
    SetVideoMode(pal_mode);
    GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, flags, 1, 0);

// set the top left coordinates of the two buffers in video memory
    GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
    
    // Initialize the order tables
    sgMainOrderTable[0].length = OT_LENGTH;
    sgMainOrderTable[1].length = OT_LENGTH;
    sgMainOrderTable[0].org = sgMainOrderTableTag[0];
    sgMainOrderTable[1].org = sgMainOrderTableTag[1];

    // Clear order tables
    GsClearOt(0, 0, &sgMainOrderTable[0]);
    GsClearOt(0, 0, &sgMainOrderTable[1]);
}

INTERNAL void
Display( void )
{
    // refresh the font
    if (DEBUG) FntFlush(-1);
    
    // get the current buffer
    CurrentBuffer = GsGetActiveBuff();
    
    // setup the packet workbase
    GsSetWorkBase((PACKET*)sgGPUPacketArea[CurrentBuffer]);
    
    // clear the ordering table
    GsClearOt(0,0, &sgMainOrderTable[CurrentBuffer]);

    // Draw sprite
    GsSortSprite(&image, &sgMainOrderTable[CurrentBuffer], 0);
    
    // wait for all drawing to finish
    DrawSync(0);
    
    // wait for v_blank interrupt
    VSync(0);
    
    // flip the double buffers
    GsSwapDispBuff();
    
    // clear the ordering table with a background color (R,G,B)
    GsSortClear(50,50,50, &sgMainOrderTable[CurrentBuffer]);
    
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
    FntLoad(960, 256);
    SetDumpFnt(FntOpen(5, 20, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 512));

    // Sprite
    CreateSprite(&image, dfblue);
    
    while (TRUE)
    {
        if (DEBUG) FntPrint("DANGER-FORCE\nBY JACK RIALES");
        Display();
    }
    return 0;
}
