/** 
 * @file bitmap_data.h 
 * @briefi Handle the bitmap 
 * @created 1996-06-29 
 * @date 2007-04-09
 * @copyright 1991-2014 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/* 
 * copyright (c) 1991-2014 TLK Games all rights reserved
 * $Id$
 *
 * TecnoballZ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * TecnoballZ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef __BITMAP_DATA__
#define __BITMAP_DATA__
class bitmap_data;
#include "../include/tecnoballz.h"
#include "../include/surface_sdl.h"

class bitmap_data:public virtual surface_sdl 
{
private:
  /** Height in linges */
  Uint32 height;
  /** Width in pixels */
  Uint32 width;
  /** Real row length in bytes */
  Uint32 row_size;
  /** Bytes peer pixel 1 = 256 colors */
  Uint32 depth;
  /** Size of the bitmap in bytes */
  Uint32 bytes_size;
  /** Palette of 256 colors */
  unsigned char palette[768];

public:
    bitmap_data ();
   ~bitmap_data ();

  void clear_members ();
  void release ();
  Uint32 get_width ();
  Uint32 get_row_size ();
  Uint32 get_height ();
  char *get_pixel_data (Sint32 xcoord, Sint32 ycoord);
  char *get_pixel_data ();
  Sint32 get_offset (Sint32 xcoord, Sint32 ycoord);
  Sint32 get_line_modulo (Sint32 w);
  void create_surface (Uint32 w, Uint32 h);
  char *duplicate_pixel_data ();
  void enable_palette ();
  unsigned char *get_palette ();
  void load (char *filename);
  void load (Sint32 id);
  bitmap_data *cut_to_bitmap (Sint32 xcoord, Sint32 ycoord, Uint32 w,
                               Uint32 h);

private:
  void sdl_load_bmp (char *filename);
};
#endif
