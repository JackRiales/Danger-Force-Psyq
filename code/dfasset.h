#if !defined(DF_ASSET_H)
/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/
#define DF_ASSET_H

typedef struct
{
    char *name;
    char *metapath;
} ASSETHEADER;

typedef struct
{
    ASSETHEADER header;
    u64  *tim;
} IMAGEDATA;



#endif
