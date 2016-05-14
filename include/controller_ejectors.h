/**
 * @file controller_ejectors.h
 * @brief Ejectors corners controller
 * @date 2015-06-14
 * @copyright 1991-2015 TLK Games
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
#ifndef __CONTROLLER_EJECTORS__
#define __CONTROLLER_EJECTORS__

class controller_ejectors;

#include "../include/sprite_object.h"
#include "../include/tecnoballz.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_ejector.h"

class controller_ejectors:public objects_list < sprite_ejector,
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

  static const Sint32 EJECTOR0_BALL_OFFSETX = 10;
  static const Sint32 EJECTOR0_BALL_OFFSETY = 10;
  static const Sint32 EJECTOR1_BALL_OFFSETX = 10;
  static const Sint32 EJECTOR1_BALL_OFFSETY = 5;
  static const Sint32 EJECTOR2_BALL_OFFSETX = 5;
  static const Sint32 EJECTOR2_BALL_OFFSETY = 5;
  static const Sint32 EJECTOR3_BALL_OFFSETX = 5;
  static const Sint32 EJECTOR3_BALL_OFFSETY = 10;

private:
  static const Uint32 COORD_EJECTOR_1 = 16;
  static const Uint32 COORD_EJECTOR_2 = 224;

public:
    controller_ejectors ();
   ~controller_ejectors ();
  void create_ejectors_sprites ();
  void draw_shadow ();
  void draw ();
  sprite_ejector *get_ejector (Uint32 eject);
};
#endif
