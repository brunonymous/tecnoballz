/**
 * @file sprite_font_game.cc 
 * @brief The sprite font used to display a char of the
 *        "LEVEL COMPLETED" string
 * @date 2007-10-17
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
#include "../include/sprite_font_game.h"

/**
 * Create a fonte game sprite
 */
sprite_font_game::sprite_font_game ()
{
  clear_sprite_members ();
  initialize (0, 0, 0, 0, 0, 0);
}

/**
 * Release a fonte game sprite
 */
sprite_font_game::~sprite_font_game ()
{}

/**
 * Initialize radius, stop coordinate and start y coordinate
 * @param radius Pointer to sinus table from 0 to 511
 * @param x_stop Final x-coordinate
 * @param y_start First y-coordinate
 * @param xinc Horizontal speed in pixels
 * @param yinc Vertical speed in pixel 
 * @param y_stop Final y-coordinate
 */
void
sprite_font_game::initialize (Uint32 radius, Sint32 x_stop, Sint32 y_start,
                              Sint32 xinc, Sint32 yinc, Sint32 y_stop)
{
  current_radius = radius;
  xcoord_final = x_stop;
  ycoord_current = y_start;
  x_inc = xinc;
  y_inc = yinc;
  ycoord_final = y_stop;
}

/**
 * Character moving
 */
void
sprite_font_game::move ()
{
  if (is_enabled)
    {
      /* ordinate linear moving */
      Sint32 d = ycoord_current;
      if (d > ycoord_final)
        {
          d += y_inc;
          ycoord_current = d;
        }

      /* ordinate sinus moving */
      Sint32 a = current_radius + 5;
      a &= SINUS_MASK;
      current_radius = a;
      Sint16 *s = table_sinL + a;
      a = *s;
      a *= 10 * resolution;
      a >>= SINUS_DECA;
      a = a + d;
      y_coord = a;

      /* absciss moving */
      if (x_coord != xcoord_final)
        {
          x_coord += x_inc;
        }
    }
  clip_coordinates ();
}
