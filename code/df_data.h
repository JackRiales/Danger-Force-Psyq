#if !defined(DF_DATA_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Jack Riales $
   $Notice: (C) Copyright 2015 by Jack Riales. All Rights Reserved. $
   ======================================================================== */

#define DF_DATA_H

/* Define this to emulate CD ROM loading speed when loading assets on an emulator */
#define PC_EMULATE_CDROM_LOAD_SPEED

#define INITIAL_BASE_ADDRESS 0x80010000 /* First base address for loading data;
                                           can be overwritten to something else */

#define ALIGNMENT 16
#define CD_LOAD_RETRIES 10
#define PC_LOAD_READ_SIZE 1024

/* Error codes */
enum
{
    ERROR_EXTDAT_FILE_INVALID,
    ERROR_EXTDAT_BUFFER_ALLOC
};

typedef struct
{
    char *filepath;
    void *address;
    u64   size;
} EXTERNALDATA;

#endif
