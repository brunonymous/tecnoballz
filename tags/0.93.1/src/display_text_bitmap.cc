/** 
 * @file display_text_bitmap.cc 
 * @brief Display bitmap strings 
 * @date 2007-09-13
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
#include "../include/display_text_bitmap.h"
#include "../include/handler_resources.h"


/**
 * Create the display text object
 */
display_text_bitmap::display_text_bitmap ()
{
  for (Uint32 i = 0; i < 16; i++)
    {
      int_string_tmp[i] = 0;
    }
  initial_me ();
}

/**
 * Release the display text object
 */
display_text_bitmap::~display_text_bitmap ()
{
  destroy_me ();
}

//------------------------------------------------------------------------------
// small initialization
//------------------------------------------------------------------------------
void
display_text_bitmap::initial_me ()
{
  object_init ();
  bitmap_fonts = (bitmap_data *) NULL;
  off_source = 0;
  off_desti1 = 0;
}

/**
 * Release display text object
 */  
void
display_text_bitmap::destroy_me ()
{
  if (NULL != bitmap_fonts)
    {
      delete bitmap_fonts;
      bitmap_fonts = (bitmap_data *) NULL;
    }
  object_free ();
}

/**
 * Load fontes used in the shop and the right panel
 */
void
display_text_bitmap::initialize ()
{
  load_bitmap_fonts (handler_resources::BITMAP_GAME_FONTS);
}

/**
 * Return height of chars in pixels
 * @return height of the chars in pixels 
 */
Uint32
display_text_bitmap::get_char_height ()
{
  return char_height;
}

/**
 * Load bitmap fontes and initialize the object
 * @param resource_id bitmap fonts resource identifier
 */
void
display_text_bitmap::load_bitmap_fonts (Uint32 resource_id)
{
  bitmap_fonts = new bitmap_data ();
  bitmap_fonts->load (resource_id);
  surface_fonts = bitmap_fonts->get_surface ();
  caract_adr = bitmap_fonts->get_pixel_data (0, 0);       //characters 'A' to 'Z'
  off_desti1 = game_screen->get_row_size ();    //modulo destination
  off_source = bitmap_fonts->get_row_size ();     //modulo source
  
  char_width = CHAR_WIDTH * resolution;
  char_height = bitmap_fonts->get_height ();

  rect_fonts.y = 0; 
  rect_fonts.w = rect_destination.w = char_width;
  rect_destination.h = rect_fonts.h = char_height;
}

/**
 * Draw a integer into a offscreen surface 
 * @param dest destination offscreen 
 * @param xcoord x-coordinate on the offscreen 
 * @param ycoord y-coordinate on the offscreen
 * @param value the integer value to be draw 
 * @param padding length of the string
 */
void
display_text_bitmap::draw (offscreen_surface *dest, Uint32 xcoord, Uint32 ycoord, Sint32 value, Uint32 padding)
{
  integer_to_ascii (value, padding, int_string_tmp);
  draw (dest, xcoord, ycoord, int_string_tmp, padding);
}

/**
 * Draw a integer into a SDL_Surface 
 * @param dest destination surface
 * @param xcoord x-coordinate on the surface
 * @param ycoord y-coordinate on the surface
 * @param value the integer value to be draw 
 * @param padding length of the string
 */
void
display_text_bitmap::draw (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, Sint32 value, Uint32 padding)
{
  integer_to_ascii (value, padding, int_string_tmp);
  draw (dest, xcoord, ycoord, int_string_tmp, padding);
}

/**
 * Draw a line of text into a offscreen surface 
 * @param dest destination offscreen 
 * @param xcoord x-coordinate on the offscreen 
 * @param ycoord y-coordinate on the offscreen
 * @param str string to draw
 * @param length length of the string
 */
void
display_text_bitmap::draw (offscreen_surface *dest, Uint32 xcoord, Uint32 ycoord, const char* str, Uint32 length)
{
  Uint32 yoffset = dest->get_vertical_offset ();
  draw ((surface_sdl *) dest, xcoord, ycoord + yoffset, str, length);
}

/**
 * Draw a line of text into a SDL_Surface 
 * @param dest destination surface
 * @param xcoord x-coordinate on the surface
 * @param ycoord y-coordinate on the surface
 * @param str string to draw
 * @param length length of the string
 */
void
display_text_bitmap::draw (surface_sdl *dest, Uint32 xcoord, Uint32 ycoord, const char* str, Uint32 length)
{
  SDL_Surface *surface_dest = dest->get_surface ();
  rect_destination.x = xcoord;
  rect_destination.y = ycoord;
  if (0 == length) 
    {
      length = strlen(str);
    }


  for (Uint32 i = 0; i < length; i++, rect_destination.x += char_width)
    {
      Uint32 x = str[i] - 32;
      if (0 == x) 
        {  
           continue;
        }
       rect_fonts.x = ascii2code[x] * char_width;
       if (SDL_BlitSurface
           (surface_fonts, &rect_fonts, surface_dest, &rect_destination) < 0)
       {
         std::cerr << "(!)display_text_bitmap::draw() " <<
           "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
         break;
       }
    }
}

/**
 * Replace dots string by another string
 * @param src String source
 * @param dst String destination with dots
 * @param lenght Length of the string
 */
void
display_text_bitmap::print_to_string (char *src, char *dst, Uint32 length)
{
  char *d = strchr (dst, '.');
  if (NULL == d)
    {
      d = dst;
    }
  if (length == 0)
    {
      length = strlen(src);
    }
  strncpy (d, src, length);
}

void
display_text_bitmap::print_int_to_string (Sint32 value, Sint32 padding, char *dst)
{
  char *d = strchr (dst, '.');
  if (NULL == d)
    {
      d = dst;
    }
  integer_to_ascii (value, padding, d);
}



//------------------------------------------------------------------------------
// create a BOB to display string (used into guards levels)
//------------------------------------------------------------------------------
sprite_object *
display_text_bitmap::string2bob (const char *ptStr)
{
  Sint32 length = strlen (ptStr);
  bitmap_data *bmp = new bitmap_data ();
  bmp->create_surface (length * char_height, char_height);
  Sint32 *basPT = (Sint32 *) caract_adr;
  char *desP1 = bmp->get_pixel_data ();
  Sint32 offSc = off_source;
  Sint32 offDs = bmp->get_row_size ();
  char *c = ascii2code;
  Sint32 a, b, h;
  if (resolution == 1)
    {
      for (h = 0; h < length; h++)
        {
          a = *(ptStr++) - 32;
          b = c[a];
          Sint32 *s = (Sint32 *) basPT;
          Sint32 *d = (Sint32 *) desP1;
          b = b << 3;
          s = (Sint32 *) ((char *) s + b);
          for (b = 0; b < 8; b++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              s = (Sint32 *) ((char *) s + offSc);
              d = (Sint32 *) ((char *) d + offDs);
            }
          desP1 = desP1 + 8;
        }
    }
  else
    {
      for (h = 0; h < length; h++)
        {
          a = *(ptStr++) - 32;
          b = c[a];
          Sint32 *s = (Sint32 *) basPT;
          Sint32 *d = (Sint32 *) desP1;
          b = b << 4;
          s = (Sint32 *) ((char *) s + b);
          for (b = 0; b < 16; b++)
            {
              d[0] = s[0];
              d[1] = s[1];
              d[2] = s[2];
              d[3] = s[3];
              s = (Sint32 *) ((char *) s + offSc);
              d = (Sint32 *) ((char *) d + offDs);
            }
          desP1 = desP1 + 16;
        }
    }

  sprite_object *ptBob = new sprite_object ();
  char *pixel = bmp->duplicate_pixel_data ();
  ptBob->set_pixel_data (pixel, 1);
  ptBob->sprite_width = bmp->get_width ();
  ptBob->sprite_height = bmp->get_height ();
  ptBob->row_size = bmp->get_row_size ();
  ptBob->max_of_images = 1;
  ptBob->offscreen_pitch = game_screen->get_row_size ();
  delete bmp;
  return ptBob;
}

//------------------------------------------------------------------------------
// offsets  of the first 128 ASCII characters 
// ; = .
// : = '
// ' = :
// - = ,
// . = -
//------------------------------------------------------------------------------
char
  display_text_bitmap::ascii2code[128] = { 26,  // 32 ' '
  37,                           // 33 '!'
  40,                           // 34 '"'
  26,                           // 35 '#' space
  26,                           // 36 '$' space
  26,                           // 37 '%' space
  26,                           // 38 '&' space
  39,                           // 39 "'"               //40
  26,                           // 40 '(' space
  26,                           // 41 ')' space
  26,                           // 42 '*' space
  26,                           // 43 '+' space
  41,                           // 44 ','       //26
  42,                           // 45 '-'       //41
  38,                           // 46 '.'               //42
  26,                           // 47 '/' space
  27,                           // 48 '0'
  28,                           // 49 '1'
  29,                           // 50 '2'
  30,                           // 51 '3'
  31,                           // 52 '4'
  32,                           // 53 '5'
  33,                           // 54 '6'
  34,                           // 55 '7'
  35,                           // 56 '8'
  36,                           // 57 '9'
  40,                           // 58 ':'               //39
  42,                           // 59 ';'               //38
  26,                           // 60 '<' space
  26,                           // 61 '=' space
  26,                           // 62 '>' space
  26,                           // 63 '?' space
  26,                           // 64 '@' space
  0,                            // 65 'A'
  1,                            // 66 'B'
  2,                            // 67 'C'
  3,                            // 68 'D'
  4,                            // 69 'E'
  5,                            // 70 'F'
  6,                            // 71 'G'
  7,                            // 72 'H'
  8,                            // 73 'I'
  9,                            // 74 'J'
  10,                           // 75 'K'
  11,                           // 76 'L'
  12,                           // 77 'M'
  13,                           // 78 'N'
  14,                           // 79 'O'
  15,                           // 80 'P'
  16,                           // 81 'Q'
  17,                           // 82 'R'
  18,                           // 83 'S'
  19,                           // 84 'T'
  20,                           // 85 'U'
  21,                           // 86 'V'
  22,                           // 87 'W'
  23,                           // 88 'X'
  24,                           // 89 'Y'
  25,                           // 90 'Z'
  26,                           // 91 '[' space
  26,                           // 92 '\' space
  26,                           // 93 ']' space
  26,                           // 94 '^' space
  26,                           // 95 '_' space
  26,                           // 96 '`' space
  26,                           // 97 'a' space
  26,                           // 98 'b' space
  26,                           // 99 'c' space
  26,                           // 100 'd' space
  26,                           // 101 'e' space
  26,                           // 102 'f' space
  26,                           // 103 'g' space
  26,                           // 104 'h' space
  26,                           // 105 'i' space
  26,                           // 106 'j' space
  26,                           // 107 'k' space
  26,                           // 108 'l' space
  26,                           // 109 'm' space
  26,                           // 110 'n' space
  26,                           // 111 'o' space
  26,                           // 112 'p' space
  26,                           // 113 'q' space
  26,                           // 114 'r' space
  26,                           // 115 's' space
  26,                           // 116 't' space
  26,                           // 117 'u' space
  26,                           // 118 'v' space
  26,                           // 119 'w' space
  26,                           // 120 'x' space
  26,                           // 121 'y' space
  26,                           // 122 'z' space
  26,                           // 123 '{' space
  26,                           // 124 '|' space
  26,                           // 125 '}' space
  26,                           // 126 '~' space
  26                            // 127 ' ' space
};
