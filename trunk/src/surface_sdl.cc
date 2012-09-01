/** 
 * @file surface_sdl.cc 
 * @brief an drawing surface
 * @created 2007-02-15
 * @date 2007-09-30
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: surface_sdl.cc,v 1.13 2007-09-30 07:23:39 gurumeditation Exp $
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
#include "../include/tecnoballz.h"
#include "../include/surface_sdl.h"
#include "../include/handler_display.h"

/**
 * Create the surface surface object
 */
surface_sdl::surface_sdl ()
{
  object_init ();
}

/**
 * Create the surface surface object
 * @param w width of the surface in pixels 
 * @param h height of the surface in pixels
 * @param depth number of bits per pixel: 8, 16, or 24
 * @param voffset 
 */
surface_sdl::surface_sdl (Uint32 w, Uint32 h, Uint32 depth)
{
  object_init ();
  create_surface (w, h, depth);
}

surface_sdl::~surface_sdl ()
{
  if (NULL == surface)
    {
      SDL_FreeSurface (surface);
      surface = NULL;;
    }
  object_free ();
}

/**
 * Return the SDL surface
 * @return a pointer to the SDL surface structure
 */
SDL_Surface*
surface_sdl::get_surface ()
{
  return surface;
}

/** 
 * Create an empty SDL surface
 * @param w width of the surface in pixels 
 * @param h height of the surface in pixels
 * @param depth number of bits(s) per pixel (8 to 32)
 * @param flags specifies the type of surface
 * @param red_mask
 * @param green_mask
 * @param blue_mask
 * @param alpha_mask
 */
void
surface_sdl::create_surface (Uint32 w, Uint32 h, Sint32 depth, Uint32 flags, Uint32 red_mask, Uint32 green_mask, Uint32 blue_mask, Uint32 alpha_mask)
{
  if (is_verbose)
    {
      std::cout << " surface_sdl::create_surface() width: " << w << 
        " height: " << h << " depth: " << depth << std::endl;
    } 
  surface = SDL_CreateRGBSurface (flags, w, h, depth, red_mask, green_mask, blue_mask, alpha_mask);
  if (NULL == surface)
    {
      std::cerr << "(!)surface_sdl::create_surface() " <<
        "SDL_CreateRGBSurface return  " << SDL_GetError ();
      throw std::runtime_error ("SDL_CreateRGBSurface() failed!");
    }
  pixel_data = (char *) surface->pixels; 
  bytes_per_pixel = surface->format->BytesPerPixel;
}

/** 
 * Return surface memory address
 * @return a pointer to the buffer data
 */
char*
surface_sdl::get_pixel_data ()
{
  return pixel_data;
}

/**
 * Return surface memory address from the corresponding coordinates
 * @param xcoord x coordinate in the surface
 * @param ycoord y coordinate in the surface 
 * @return a pointer to the pixel data
 */
char*
surface_sdl::get_pixel_data (Uint32 xcoord, Uint32 ycoord)
{
  return pixel_data + ycoord * surface->pitch + xcoord * bytes_per_pixel;
}

/**
 * Return size of line in bytes
 * @return row size in bytes
 */
Uint32
surface_sdl::get_row_size ()
{
  return surface->pitch;
}

/**
 * Return surface memory offset from the corresponding coordinate
 * @param xcoord x coordinate in the surface
 * @param ycoord y coordinate in the surface 
 * @return offset to the pixel data
 */
Uint32
surface_sdl::get_offset (Uint32 xcoord, Uint32 ycoord)
{
  return ycoord * surface->pitch + xcoord * bytes_per_pixel;
}

/**
 * Clear surface
 * @param color pixel color value
 */
void
surface_sdl::clear (Uint32 color)
{
  if (SDL_FillRect (surface, NULL, color) < 0)
    {
      std::cerr << "(!)surface_sdl::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Clear surface
 * @param color pixel color value
 * @param xcoord x coordinate in the surface
 * @param ycoord y coordinate in the surface 
 * @param w the width in pixels to clear
 * @param h the height in pixels to clear
 */
void
surface_sdl::clear (Uint32 color, Uint32 xcoord, Uint32 ycoord, Uint32 w, Uint32 h)
{
  SDL_Rect rect = {xcoord, ycoord, w, h};
  SDL_FillRect (surface, &rect, color);
}

/**
 * Return the width of the surface 
 * @return width in pixels
 */
Uint32
surface_sdl::get_width ()
{
  return surface->w;
}

/**
 * Return the height of the surface
 * @return the height of the surface in pixels
 */
Uint32
surface_sdl::get_height ()
{
  return surface->h;
}

/** 
 * Return amount to add to get to the next line  
 * @param w width of source element in bytes
 * return modulo line
 */
Uint32
surface_sdl::get_line_modulo (Uint32 w)
{
  return surface->pitch - w * bytes_per_pixel;
}

/**
 * Lock surface
 */
void
surface_sdl::lock_surface ()
{
  if (SDL_LockSurface (surface))
    {
      std::cerr << "(!)surface_sdl::lock_surface() " <<
        " SDL_LockSurface return " << SDL_GetError () << std::endl; 
    }
}

/**
 * Unlock surface
 */
void
surface_sdl::unlock_surface ()
{
  SDL_UnlockSurface (surface);
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param surface pointer to a surface surface object
 */
void
surface_sdl::blit_to_surface (surface_sdl *dest)
{
  SDL_Surface* surface_dest = dest->get_surface ();
  SDL_Rect rect = {0, 0, surface->w, surface->h};
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "(!)surface_sdl::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param dest pointer to a surface object
 * @param xcoord x coordinate in the source and destination
 * @param ycoord y coordinate in the source and destination
 * @param w the width in pixels to copy 
 * @param h the height in pixels to copy
 */
void
surface_sdl::blit_to_surface (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, Uint32 w, Uint32 h)
{
  SDL_Surface* surface_dest = dest->get_surface ();
  SDL_Rect rect = {xcoord, ycoord, w, h};
  if (SDL_BlitSurface (surface, &rect, surface_dest, &rect) < 0)
    {
      std::cerr << "(!)surface_sdl::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param dest pointer to a surface object
 * @param x1 source x coordinate in the source and destination
 * @param y1 source y coordinate in the source and destination
 * @param x2 destination x coordinate in the source and destination
 * @param y2 destination y coordinate in the source and destination
 * @param w the width in pixels to copy 
 * @param h the height in pixels to copy
 */
void
surface_sdl::blit_to_surface (surface_sdl *dest, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2, Uint32 w, Uint32 h)
{
  /*
  std::cout << "surface_sdl::blit_to_surface() source(" << x1 << ", " << y1
    << ") dest(" << x2 << "," << y2 << ") size(" << w << ", " << h << ")" << std::endl;
  */
  SDL_Surface* dest_surface = dest->get_surface ();
  SDL_Rect src_rect = {x1, y1, w, h};
  SDL_Rect dest_rect = {x2, y2, w, h};
  if (SDL_BlitSurface (surface, &src_rect, dest_surface, &dest_rect) < 0)
    {
      std::cerr << "(!)surface_sdl::blit_to_surface() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
* Set the colors in the palette of an 8-bit surface
* @param colors pointer to aSDL_Color structure
*/
void
surface_sdl::set_palette (SDL_Color *colors)
{
  if (bytes_per_pixel > 1)
    {
      return;
    }
  if (!SDL_SetPalette (surface, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256))
   {
      std::cerr << "(!)surface_sdl::set_palette() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
   }
}

/**
* Set the colors in the palette of an 8-bit surface
 * @param dest pointer to a surface object
*/
void
surface_sdl::set_palette (surface_sdl *dest)
{
  if (bytes_per_pixel > 1)
    {
      return;
    }
  if (is_verbose)
    {
      std::cout << " surface_sdl::set_palette surface_sdl" << std::endl;
    }
  if (!SDL_SetPalette (dest->get_surface (), SDL_LOGPAL | SDL_PHYSPAL, surface->format->palette->colors, 0, 256))
   {
      std::cerr << "(!)surface_sdl::set_palette() " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
   }
}

/**
 * Perform a blit from the source surface to the destination surface
 * @param source pointer to a source surface object
 * @param x1 source x coordinate in the source and destination
 * @param y1 source y coordinate in the source and destination
 * @param x2 destination x coordinate in the source and destination
 * @param y2 destination y coordinate in the source and destination
 * @param w the width in pixels to copy 
 * @param h the height in pixels to copy
 */
void
surface_sdl::blit_surface (surface_sdl *dest, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2, Uint32 w, Uint32 h)
{
  /*
  std::cout << "surface_sdl::blit_to_surface() source(" << x1 << ", " << y1
    << ") dest(" << x2 << "," << y2 << ") size(" << w << ", " << h << ")" << std::endl;
  */
  SDL_Surface* source_surface = dest->get_surface ();
  SDL_Rect src_rect = {x1, y1, w, h};
  SDL_Rect dest_rect = {x2, y2, w, h};
  if (SDL_BlitSurface (source_surface, &src_rect, surface, &dest_rect) < 0)
    {
      std::cerr << "(!)surface_sdl::blit_to_surface(x1=" << x1 << 
        ",y1=" << y1 << ",x2= " << x2 << ", y2=" << y2 << ", w=" << 
        w << " , h=" << h <<  ") " <<
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}

/**
 * Fill a shadow rectangle
 * @param xcoord x coordinate the upper-left corner of the rectangle
 * @param ycoord y coordinate the upper-left corner of the rectangle
 * @param w width of the rectangle
 * @param h height of the rectangle
 */
void
surface_sdl::fill_shadow_rect (Uint32 xcoord, Uint32 ycoord, Uint32 w,
                             Uint32 h)
{
  char k = (char) handler_display::SHADOW_PIX;
  Uint32 length = w;
  Uint32 ymax = ycoord + h;
  for (Uint32 y = ycoord; y < ymax; y++)
    {
      char *data = get_pixel_data (xcoord, y);
      for (Uint32 i = 0; i < length; i++)
        {
          *(data)++ |= k;
        }
    }
}

/**
 * Copy a part of the surface in a new  surface
 * @param xcoord x-coordinate in source surface
 * @param ycoord y-coordinate in source surface
 * @param w width of the detination surface
 * @param h height of the destination surface
 */
surface_sdl *
surface_sdl::cut_to_surface (Sint32 xcoord, Sint32 ycoord, Uint32 w, Uint32 h)
{
  surface_sdl *dest = new surface_sdl ();
  dest->create_surface (w, h, surface->format->BitsPerPixel);
  cut_to_surface (dest, xcoord, ycoord, w, h);
  return dest;
}

/**
 * Copy a part of the surface in a new  surface
 * @param xcoord x-coordinate in source surface
 * @param ycoord y-coordinate in source surface
 * @param w width of the detination surface
 * @param h height of the destination surface
 */
void
surface_sdl::cut_to_surface (surface_sdl *dest, Sint32 xcoord, Sint32 ycoord, Uint32 w, Uint32 h)
{
  SDL_Surface *surface_dest = dest->get_surface ();
  SDL_Rect rect = { xcoord, ycoord, w, h };
  if (1 == bytes_per_pixel)
    {
      SDL_SetPalette (surface_dest, SDL_LOGPAL | SDL_PHYSPAL,
                      surface->format->palette->colors, 0, 256);
    }
  if (SDL_BlitSurface (surface, &rect, surface_dest, NULL) < 0)
    {
      std::cerr << "(!)surface_sdl::cut_to_surface(xcoord=" << xcoord <<
        ", ycoord=" << ycoord << ", w= " << w << ", h=" << h << ") " << 
        "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
}




