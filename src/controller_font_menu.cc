/** 
 * @file controller_font_menu.cc 
 * @brief Controller of the menu scroll text 
 * @date 2007-10-21
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
#include "../include/controller_font_menu.h"
#include "../include/handler_resources.h"

/**
 * Create the scroll text controller
 */
controller_font_menu::controller_font_menu ()
{
  littleInit ();
  max_of_sprites = MAX_OF_FONTS + 2;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::FONT_MENU;
  offset_xx1 = 0;
  offset_yy1 = 0;
}

/**
 * Release the scroll text controller
 */
controller_font_menu::~controller_font_menu ()
{
  if (NULL != scrolltexts)
    {
      delete[](char *)scrolltexts;
      scrolltexts = NULL;
    }
  release_sprites_list ();
}

/**
 * Perform some initializations
 */
void
controller_font_menu::create_fontes_list ()
{
  create_sprites_list ();
  Uint32 i;
  /* fisrt element of the list is NULL */ 
  characters_list[0] = NULL;
  Uint32 j = 0;
  for (i = 0; i < MAX_OF_FONTS; i++, j = (j + 18) & SINUS_MASK)
    {
      sprite_font_menu *sprite = sprites_list[i];
      characters_list[i + 1] = sprite;
      sprite->zeCosValue = j;
      sprite->set_x_coord (100);
      sprite->set_y_coord (300);
      sprite->set_image (26);
    }
  characters_list[i + 1] = NULL;
  objectLeft = sprites_list[i++];
  objectRigh = sprites_list[i];
  objectLeft->set_image (42);
  objectRigh->set_image (42);
  enable_sprites ();
  object_ptr = &characters_list[MAX_OF_FONTS];
  scrolltexts = resources->load_texts (handler_resources::TEXTS_SCROLL_MENU, 1, 0, 0);
  scrollText = scrolltexts[0];
  scroll_ptr = scrollText;
}

/**
 * Moving of the fonts
 */ 
void
controller_font_menu::move_chars ()
{
  Sint32 a, b, width;
  Sint16 *table, *sinPT, *cosPT;
  sprite_font_menu *sprite;
  sprite_font_menu *zzBOB;
  /* convert unsigned to signed */
  Sint32 res = (Sint32) resolution;

  sinPT = table_sinL;
  cosPT = table_cosL;

  /*
   * vertical moving of the characters 
   */
  /* determine the width of scrolling text */
  a = (offset_xx1 + 3) & SINUS_MASK;
  offset_xx1 = a;
  table = sinPT + a;
  width = ((*table * 10 * res) >> SINUS_DECA) + 132 * res;
  Sint32 depla = 1;
  if (birth_flag)
    {
      depla *= 2;
    }
  sprite_font_menu **sprite_fonts = object_ptr;
  for (Uint32 i = 0; i < MAX_OF_FONTS; i++)
    {
      if (NULL == *sprite_fonts)
        {
          sprite_fonts = &characters_list[MAX_OF_FONTS];
        }
      sprite = *(sprite_fonts--);
      a = sprite->zeCosValue + depla;

      /* read a new character */
      if (a >= 256)
        {
          char *sText = scroll_ptr;
          Uint32 c = (Uint32) * sText;
          /* end of text, restart it! */
          if (0 == c)
            {
              sText = scrollText;
              scroll_ptr = sText;
              c = *sText;
            }
          scroll_ptr++;
          c -= 32;
          b = 127;
          c = c & b;
          b = asciiToBob[c];
          sprite->set_image (b);
          /* fisrt element of the list? */
          if (NULL == *(object_ptr - 1))
            {
              object_ptr = &characters_list[MAX_OF_FONTS + 1];
            }
          object_ptr--;
        }
      /* radius only varies on 180 degree */
      a = a & 255;
      sprite->zeCosValue = a;
      table = cosPT + a;
      a = ((*table * width) >> SINUS_DECA) + 152 * res;
      sprite->set_x_coord (a);
    }

  /*
   * vertical moving of the characters 
   */
  /* and 0x1ff => value 0 to 511 */
  a = (offset_yy1 + 2) & SINUS_MASK;
  offset_yy1 = a;
  depla = 4;
  if (birth_flag)
    {
      depla *= 2;
    }
  sprite_fonts = object_ptr;
  Sint32 zerad = 25 * res;
  Sint32 zeoff = 200 * res;
  for (Uint32 i = 0; i < MAX_OF_FONTS; i++)
    {
      if (*sprite_fonts == NULL)
        {
          sprite_fonts = &characters_list[MAX_OF_FONTS];
        }
      sprite = *(sprite_fonts--);
      a = (a + depla) & SINUS_MASK;
      table = sinPT + a;
      b = ((*table * zerad) >> SINUS_DECA) + zeoff;
      sprite->set_y_coord (b);
    }

  /* move the left mask */
  sprite = objectRigh;
  table = cosPT;
  a = ((*table * width) >> SINUS_DECA) + 152 * res;
  sprite->set_x_coord (a);
  zzBOB = *(++sprite_fonts);
  a = zzBOB->get_y_coord ();
  sprite->set_y_coord (a);

  /* move the right mask */
  sprite = objectLeft;
  table = cosPT + 255;
  a = ((*table * width) >> SINUS_DECA) + 152 * res;
  sprite->set_x_coord (a);
  zzBOB = *object_ptr;
  a = zzBOB->get_y_coord ();
  sprite->set_y_coord (a);
}

char controller_font_menu::asciiToBob[128] =
{
  26,       // 32 ' ' space
  37,                           // 33 '!'
  39,                           // 34 '''
  26,                           // 35 '#' (space)
  26,                           // 36 '$' (space)
  26,                           // 37 '%' (space)
  26,                           // 38 '&' (space)
  39,                           // 39 "'"
  26,                           // 40 '(' (space)
  26,                           // 41 ')' (space)
  26,                           // 42 '*' (space)
  26,                           // 43 '+' (space)
  40,                           // 44 ',' 
  41,                           // 45 '-'
  38,                           // 46 '.'
  26,                           // 47 '/' (space)
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
  26,                           // 58 ':' (space)
  26,                           // 59 ';' (space)
  26,                           // 60 '<' (space)
  26,                           // 61 '=' (space)
  26,                           // 62 '>' (space)
  26,                           // 63 '?' (space)
  26,                           // 64 '@' (space)
  00,                           // 65 'A'
  01,                           // 66 'B'
  02,                           // 67 'C'
  03,                           // 68 'D'
  04,                           // 69 'E'
  05,                           // 70 'F'
  06,                           // 71 'G'
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
  26,                           // 91 '[' (space)
  26,                           // 92 '\' (space)
  26,                           // 93 ']' (space)
  26,                           // 94 '^' (space)
  26,                           // 95 '_' (space)
  26,                           // 96 '`' (space)
  26,                           // 97 'a' (space)
  26,                           // 98 'b' (space)
  26,                           // 99 'c' (space)
  26,                           // 100 'd' (space)
  26,                           // 101 'e' (space)
  26,                           // 102 'f' (space)
  26,                           // 103 'g' (space)
  26,                           // 104 'h' (space)
  26,                           // 105 'i' (space)
  26,                           // 106 'j' (space)
  26,                           // 107 'k' (space)
  26,                           // 108 'l' (space)
  26,                           // 109 'm' (space)
  26,                           // 110 'n' (space)
  26,                           // 111 'o' (space)
  26,                           // 112 'p' (space)
  26,                           // 113 'q' (space)
  26,                           // 114 'r' (space)
  26,                           // 115 's' (space)
  26,                           // 116 't' (space)
  26,                           // 117 'u' (space)
  26,                           // 118 'v' (space)
  26,                           // 119 'w' (space)
  26,                           // 120 'x' (space)
  26,                           // 121 'y' (space)
  26,                           // 122 'z' (space)
  26,                           // 123 '{' (space)
  26,                           // 124 '|' (space)
  26,                           // 125 '}' (space)
  26,                           // 126 '~' (space)
  26                            // 127 ' ' (space)
};
