/**
 * @file sprite_wall.h
 * @brief The sprite of the wall used in bricks level 
 * @created 2007-1O-04 
 * @date 2007-1O-05
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
#ifndef __SPRITE_WALL__
#define __SPRITE_WALL__
class sprite_wall;
#include "../include/sprite_object.h"
class sprite_wall:public sprite_object
  {
  static sprite_wall* sprite_wall_singleton;

  public:
    Sint32 thecounter;

  private:
    sprite_wall ();
  public:
    ~sprite_wall ();
    void enable (Uint32 counter);
    void disable ();
  static sprite_wall * get_instance ();
  };
#endif
