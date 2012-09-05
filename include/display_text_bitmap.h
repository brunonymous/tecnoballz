/** 
 * @file display_text_bitmap.h
 * @brief Display bitmap strings 
 * @date 2007-11-16
 * @copyright 1991-2012 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/* 
 * copyright (c) 1991-2012 TLK Games all rights reserved
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
#ifndef __DISPLAY_TEXT_BITMAP__
#define __DISPLAY_TEXT_BITMAP__

class display_text_bitmap;

#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/tecnoballz.h"
#include "../include/sprite_object.h"

class display_text_bitmap:public virtual tecnoballz
{
protected:
  static const Uint32 CHAR_WIDTH = 8;
  static char ascii2code[128];
  Sint32 off_desti1;            // modulo destination affichage ecran
  Sint32 off_source;            // modulo source page graphique
  bitmap_data *bitmap_fonts;
  SDL_Surface *surface_fonts;
  SDL_Rect rect_fonts;
  SDL_Rect rect_destination;
  char int_string_tmp[16]; 
  char *caract_adr;

  /** Height of the chars in pixels */
  Uint32 char_height;
  /** Width of the chars in pixels */
  Uint32 char_width;

public:
    display_text_bitmap ();
   ~display_text_bitmap ();
  void initial_me ();
  void destroy_me ();
  void initialize ();
  Uint32 get_char_height ();
  void load_bitmap_fonts (Uint32 resource_id);
  void draw (offscreen_surface *dest, Uint32 xcoord, Uint32 ycoord, Sint32 value, Uint32 padding);
  void draw (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, Sint32 value, Uint32 padding);
  void draw (offscreen_surface *dest, Uint32 xcoord, Uint32 ycoord, const char* str, Uint32 length = 0);
  void draw (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, const char* str, Uint32 length = 0);
  void print_to_string (char *src, char *dst, Uint32 length = 0);
  void print_int_to_string (Sint32 value, Sint32 padding, char *dst);
  sprite_object *string2bob (const char *ptStr);
};
#endif
