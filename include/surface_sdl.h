/**
 * @file surface_sdl.h
 * @brief an surface drawing surface
 * @created 2007-02-15
 * @date 2012-09-15
 * @copyright 1991-2016 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2016 TLK Games all rights reserved
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
#ifndef __SURFACE_SDL__
#define __SURFACE_SDL__
class surface_sdl;
#include "../include/tecnoballz.h"

class surface_sdl:public virtual tecnoballz
  {

  protected:
    SDL_Surface* surface;
    /** The number of bytes used to represent each pixel in a surface */
    Uint32 bytes_per_pixel;
    char* pixel_data;

  public:
    surface_sdl ();
    surface_sdl (Uint32 w, Uint32 h, Uint32 depth);
    ~surface_sdl ();
    SDL_Surface* get_surface ();
    char* get_pixel_data ();
    char* get_pixel_data (Sint32 xcoord, Sint32 ycoord);
    Uint32 get_row_size ();
    Uint32 get_offset (Sint32 xcoord, Sint32 ycoord);
    void clear (Uint32 color = 0);
    void clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 w,
                Uint32 height);
    Uint32 get_width ();
    Uint32 get_height ();
    Uint32 get_line_modulo (Uint32 w);
    void lock_surface ();
    void unlock_surface ();
    void blit_to_surface (surface_sdl *dest);
    void blit_to_surface (surface_sdl *dest, Uint32 xcoord,
                          Uint32 ycoord, Uint32 w, Uint32 h);
    void blit_to_surface (surface_sdl *dest, Uint32 x1, Uint32 y1,
                          Uint32 x2, Uint32 y2, Uint32 w, Uint32 h);
    void set_palette (SDL_Color *colors);
    void set_palette (surface_sdl *dest);
    void blit_surface (surface_sdl *dest, Uint32 x1, Uint32 y1,
                       Uint32 x2, Uint32 y2, Uint32 w, Uint32 h);
    void fill_shadow_rect (Uint32 xcoord, Uint32 ycoord, Uint32 w,
                           Uint32 h);

  public:
    void create_surface (Uint32 w, Uint32 h, Sint32 depth,
                         Uint32 flags = SDL_ANYFORMAT,
                         Uint32 red_mask = 0xf00,
                         Uint32 green_mask = 0x0f0,
                         Uint32 blue_mask = 0x00f,
                         Uint32 alpha_mask = 0x00);
    surface_sdl *cut_to_surface (Sint32 xcoord, Sint32 ycoord, Uint32 w,
                                 Uint32 h);
    void cut_to_surface (surface_sdl *dest, Sint32 xcoord,
                         Sint32 ycoord, Uint32 w, Uint32 h);
  };
#endif
