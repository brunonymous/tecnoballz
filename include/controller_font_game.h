/** 
 * @file controller_font_game.h
 * @brief Handle mobile characters used for "LEVEL n COMPLETED"
 * @date 2007-10-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.3 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_font_game.h,v 1.3 2007-10-31 07:35:29 gurumeditation Exp $
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
#ifndef __CONTROLLER_FONT_GAME__
#define __CONTROLLER_FONT_GAME__
class controller_font_game;
#include "../include/objects_list.h"
#include "../include/sprite_font_game.h"
class controller_font_game:public objects_list < sprite_font_game, controller_font_game >
{
private:
  /** Number of chars in the first string ("LEVEL n") */
  Uint32 size_of_line_1;
  /** Number of chars in the second string ("COMPLETED") */
  Uint32 size_of_line_2;
  /** Number total of chars both in the first and second string */
  Uint32 total_size;
  /** Screen width in pixels 256/512 pixels */
  Uint32 horizontal_length;
  /** Horizontal offet 0 or 32/64 pixels */
  Uint32 horizontal_offset;
  static char ze_bobText[];
  static char ze_endText[];

public:
    controller_font_game ();
   ~controller_font_game ();
  void initialize (Uint32 level, Uint32 offset = 0);
  void move ();
  void enable ();
private:
  Uint32 set_start_values (Uint32 n, Uint32 a, Uint32 j, Sint32 y,
                     Uint32 i3, Sint32 y3);
};
#endif
