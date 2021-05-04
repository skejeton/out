#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include "texture.h"
#include "types.h"

Game_Texture game_read_bmp(char *path)
{
    
    File f = read_file(path);
    
    u32 width = *((u32*)(f.buf+18));
    u32 height = *((u32*)(f.buf+22));
    if (width < 4000 && height < 4000)
    {
        u32 *texloc = (u32*)(f.buf+*((u32*)(f.buf+10)));
        Game_Texture res = game_texture_from_bytes_argb(width, height, texloc);
        
        return res;
    }
    delete(f.buf);
    printf("Failed loading bitmap\n");
    exit(-1);
}
