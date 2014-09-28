/** 
 * @file sprite_bullet.h
 * @brief The bullet sprite from a guardian 
 * @date 2007-09-27
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
#ifndef __SPRITE_BULLET__
#define __SPRITE_BULLET__

class sprite_bullet;

#include "../include/sprite_object.h"
#include "../include/sprite_guardian.h"
#include "../include/sprite_paddle.h"

class sprite_bullet:public sprite_object
{
  friend class controller_bullets;

private:
  /** Type of displacement from 1 to 7 */
  Uint32 displacement;
  Sint32 flagDepla1;
  Sint32 flagDepla2;
  Sint32 flagDepla3;
  Sint32 flagDepla4;
  Sint32 flagDepla5;
  Sint32 flagDepla6;
  Sint32 flagDepla7;
  const short *tablesinus;
  const short *tablecosin;
  sprite_paddle *paddle_target;
  sprite_guardian *ptguardian;
  Sint32 tir_maxi_x;
  Sint32 tir_maxi_y;
  Sint32 tir_minixy;

public:
    sprite_bullet ();
   ~sprite_bullet ();
  void move ();

private:
  void trajectory_01 ();
  void trajectory_02 ();
  void trajectory_03 ();
  void trajectory_04 ();
  void trajectory_05 ();
  void trajectory_06 ();
  void trajectory_07 ();
  void trajectory_08 ();
  void trajectory_09 ();
  void trajectory_10 ();
  void trajectory_11 ();
  void trajectory_12 ();
  void screenOver (Sint32 xmini);
  void screenStop (Sint32 vmini);
};
#endif
