/** 
 * @file controller_magnetic_eyes.h
 * @brief Magectic eyes controller 
 * @created 2004-09-17 
 * @date 2007-04-08
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
#ifndef __CONTROLLER_MAGNECTIC_EYES__
#define __CONTROLLER_MAGNECTIC_EYES__
class controller_magnetic_eyes;

#include "../include/list_sprites.h"
#include "../include/sprite_eye.h"
#include "../include/objects_list.h"

class controller_magnetic_eyes:public objects_list < sprite_eye, controller_magnetic_eyes >
{
  friend class sprite_eye;

private:
  static const Uint16 x_coordinates[32];
  static const Uint16 y_coordinates[32];

public:
  /** Value used for the collisions with balls */
  Sint32 hypotenuse;
  /** X-coordinate of the eyes centers */
  Sint32 center_x;
  /** Y-coordinate of the eyes centers */
  Sint32 center_y;

public:
    controller_magnetic_eyes ();
   ~controller_magnetic_eyes ();
  bool create_eye ();
  void create_eyes_list ();
  void move ();
};
#endif
