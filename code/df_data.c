/********************************************************
 * DANGER FORCE
 * A PSX Game written in 2017 -- what?!
 * by Jack Riales
 * (c) 2017 Jack Riales. All rights reserved.
 ********************************************************/

#include "df_data.h"

INTERNAL u64 baseaddr = INITIAL_BASE_ADDRESS;

s32 LoadExternalData(EXTERNALDATA *dat, char *path)
{
#ifndef CDROM_RELEASE
    u32 i = 0;
    u32 size = 0;
    s32 file;
    s8* buffer;

    file = PCopen(path, 0, 0);
    if (file <= 0)
        return ERROR_EXTDAT_FILE_INVALID;

    size = PClseek(file, 0, 2);
    buffer = (s8*) malloc (size);
    if (!buffer)
        return ERROR_EXTDAT_BUFFER_ALLOC;
    
    i = size;
    while (i > PC_LOAD_READ_SIZE)
    {
        PCread(file, buffer, PC_LOAD_READ_SIZE);
        buffer += PC_LOAD_READ_SIZE;
        i -= PC_LOAD_READ_SIZE;
    }
    PCread(file, buffer, i);
    PCclose(file);
    
#else
#endif
}
