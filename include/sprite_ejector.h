/**
 * @file sprite_ejector.h
 * @brief Sprite ejector located in the corners
 * @created 2015-04-12
 * @date 2015-06-14
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
#ifndef __SPRITE_EJECTOR__
#define __SPRITE_EJECTOR__
class sprite_ejector;
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/controller_ejectors.h"
class sprite_ejector:public sprite_object
{
  friend class controller_balls;
  friend class controller_ejectors;

private:
    Sint32 ball_offsetx;
  Sint32 ball_offsety;

public:
    sprite_ejector (Sint32 ball_offx, Sint32 ball_offy);
   ~sprite_ejector ();
  void stick (sprite_ball * ball);
};
#endif
