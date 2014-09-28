/** 
 * @file controller_ejectors.h
 * @brief Ejectors corners controller 
 * @date 2007-09-29
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
#ifndef __CONTROLLER_EJECTORS__
#define __CONTROLLER_EJECTORS__

class controller_ejectors;

#include "../include/sprite_object.h"
#include "../include/tecnoballz.h"
#include "../include/sprite_ball.h"

class controller_ejectors:public objects_list < sprite_object,
  controller_ejectors >
{
public:
  typedef enum
  {
    TOP_LEFT_EJECTOR,
    BOTTOM_LEFT_EJECTOR,
    BOTTOM_RIGHT_EJECTOR,
    TOP_RIGHT_EJECTOR,
    MAX_OF_EJECTORS
  }
  EJECTORS_ENUM;

private:
  static const Uint32 COORD_EJECTOR_1 = 16;
  static const Uint32 COORD_EJECTOR_2 = 224;
  static bool is_pos_ball_initialized;

public:
    controller_ejectors ();
   ~controller_ejectors ();
  void create_ejectors_sprites ();
  void draw_shadow ();
  void draw ();
  sprite_object *get_ejector (Uint32 eject);
  void initialize_ball_positions (ball_ejector_coords * table);
};
#endif
