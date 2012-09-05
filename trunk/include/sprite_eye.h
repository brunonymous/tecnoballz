/**
 * @file sprite_eye.h
 * @brief The sprite of the eye used in bricks level 
 * @created 2004-09-17
 * @date 2007-09-12
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
#ifndef __SPRITE_EYE__
#define __SPRITE_EYE__
class sprite_eye;
#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/controller_bricks.h"
#include "../include/controller_balls.h"
#include "../include/controller_magnetic_eyes.h"
class sprite_eye:public sprite_object
  {
    friend class controller_balls;
    friend class controller_magnetic_eyes;

  private:
    Sint32 finishPosx;
    Sint32 finishPosy;
    Sint32 centerPosx;
    Sint32 centerPosy;
    Sint32 radius_360;

  public:
    sprite_eye ();
    ~sprite_eye ();
  };
#endif
