/** 
 * @file controller_font_game.cc 
 * @brief Handle mobile characters used for "LEVEL n COMPLETED"
 * @date 2007-10-31
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
#include "../include/controller_font_game.h"

/**
 * Create the object
 */
controller_font_game::controller_font_game ()
{
  littleInit ();
  size_of_line_1 = 0;
  size_of_line_2 = 0;
  horizontal_length = 0;
  max_of_sprites = 20;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::FONT_GAME;
  horizontal_offset = 0;
}

/**
 * Release the object
 */
controller_font_game::~controller_font_game ()
{
  release_sprites_list ();
}

/**
 * Perform some initializations
 * @param level
 * @param offset
 */
void
controller_font_game::initialize (Uint32 level, Uint32 offset)
{
  horizontal_offset = offset;
  horizontal_length = 256 * resolution;
  char *monPT = ze_bobText;
  //if(level <= 12)
  {
    Sint32 d = level / 10;
    Sint32 u = level - d * 10;
    ze_bobText[6] = (char) d + '\\';
    ze_bobText[7] = (char) u + '\\';
  }
  /*else
     {    monPT = ze_endText;
     } */

  /* count the number of characters */
  size_of_line_1 = 0;
  while (*(monPT++) != 0)
    {
      size_of_line_1++;
    }
  size_of_line_2 = 0;
  while (*(monPT++) != 0)
    {
      size_of_line_2++;
    }
  total_size = size_of_line_1 + size_of_line_2;

  /* initialize the images for each character */
  monPT = ze_bobText;
  for (Uint32 i = 0; i < total_size; i++)
    {
      sprite_font_game *sprite_char = sprites_list[i];
      char c = *(monPT++);
      if (c == '\0')
	{
          c = *(monPT++);
	}
      sprite_char->new_offset (c - 'A');
    }

  /* initialize characters of "LEVEL n" */
  Sint32 yStrt = 200 * resolution;
  Sint32 a = set_start_values (size_of_line_1, 0, 0,
                         yStrt, -6 * resolution, 60 * resolution);

  /* initialize characters of "COMPLETED" */
  yStrt += 10 * resolution;
  set_start_values (size_of_line_2, a, size_of_line_1,
              yStrt, -8 * resolution, 80 * resolution);
}

/**
 * Initialize start values for each char of a string
 * @param length Number of chars
 * @param zerad Radius
 * @paral index
 * @param yStrt Start y-coordinate
 * @param yOffs Y move offset
 * @param yStop Stop y-coordinate
 * @return
 */
Uint32
controller_font_game::set_start_values (Uint32 length, Uint32 zeRad, Uint32 index,
                                    Sint32 yStrt, Uint32 yOffs, Sint32 yStop)
{
  Sint32 width = sprites_list[0]->get_sprite_height ();
  Sint32 e = (horizontal_length) / length;
  Sint32 x_stop = (horizontal_length - (length * width)) / 2;
  Sint32 xOffs = 0;             //X move offset (1, -1 or 0)
  Sint32 xStrt = e / 2;         //start X coordinate

  x_stop += horizontal_offset;
  xStrt += horizontal_offset;

  for (Uint32 i = index; i < (length + index); i++, xStrt += e, x_stop += width)
    {
      sprite_font_game *chara = sprites_list[i];
      chara->set_coordinates (xStrt, yStrt);
      if (xStrt > x_stop)
	{
          xOffs = -1;
	}
      else
        {
          if (xStrt == x_stop)
	    {
              xOffs = 0;
	    }
          else
	    {
              xOffs = 1;
	    }
        }
      chara->initialize (zeRad, x_stop, yStrt, xOffs, yOffs, yStop);
      zeRad += 8;
      zeRad &= SINUS_MASK;
    }
  return zeRad;
}

/**
 * Animation of characters sprites
 */
void
controller_font_game::move ()
{
  for (Uint32 i = 0; i < total_size; i++)
    {
      sprite_font_game *sprite_char = sprites_list[i];
      sprite_char->move ();
    }
}

/**
 * Enable character sprites
 */
void
controller_font_game::enable ()
{
  for (Uint32 i = 0; i < total_size; i++)
    {
      sprite_font_game *sprite_char = sprites_list[i];
      sprite_char->enable ();
    }
}

//-----------------------------------------------------------------------------
// string to display
//-----------------------------------------------------------------------------
char
  controller_font_game::ze_bobText[] = "LEVEL[//\0COMPLETED\0";
char
  controller_font_game::ze_endText[] = "[[GAME[[\0FINISHED[\0";
