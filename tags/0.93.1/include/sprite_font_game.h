/**
 * @file sprite_font_game.h
 * @brief The sprite font used to display a char of the
 *        "LEVEL COMPLETED" string
 * @date 2007-10-17
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
#ifndef __SPRITE_FONTE_GAME__
#define __SPRITE_FONTE_GAME__
class sprite_font_game;
#include "../include/sprite_object.h"

class sprite_font_game:public sprite_object
  {
    friend class controller_font_game;

  private:
    /** Radius value from 0 to 511, index on a sinus table */
    Uint32 current_radius;
    /** Final y-coordinate */
    Sint32 ycoord_current;
    /** Final x-coordinate */
    Sint32 xcoord_final;
    /** X increase: 1, -1, or 0 */
    Sint32 x_inc;
    /** Y increase: -12 or -16 */
    Sint32 y_inc;
    /** Final y-coordinate */
    Sint32 ycoord_final;

  public:
    sprite_font_game ();
    ~sprite_font_game ();
    void initialize (Uint32 radius, Sint32 x_stop, Sint32 y_start,
                     Sint32 xinc, Sint32 yinc, Sint32 y_stop);
    void move ();
  };
#endif
