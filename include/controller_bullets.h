/** 
 * @file controller_bullets.h 
 * @brief Bullets controller 
 * @date 2007-02-19
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
#ifndef __CONTROLLER_BULLETS__
#define __CONTROLLER_BULLETS__
class controller_bullets;

#include "../include/list_sprites.h"
#include "../include/sprite_bullet.h"
#include "../include/objects_list.h"
#include "../include/sprite_guardian.h"

class controller_bullets:public objects_list < sprite_bullet, controller_bullets >
{
  friend class controller_guardians;

private:
  static const Sint16 tir01_posi[62];
  static const Sint16 tir02_posi[60];
  static const Sint16 tir04_posi[14];
  static const Sint16 tir10_posi[12];
  static const Sint16 fire_sinus[60];

public:
    controller_bullets ();
   ~controller_bullets ();
  void move ();
  void play_animation_loop ();
  void check_paddle_collisions ();
  sprite_bullet *get_last_bullet ();
  void fire (Uint32 fire_id, sprite_guardian * guardian);

private:
  void init_fire_01 (sprite_guardian * pgard);
  void init_fire_02 (sprite_guardian * pgard);
  void init_fire_03 (sprite_guardian * pgard);
  void init_fire_04 (sprite_guardian * pgard);
  void init_fire_05 (sprite_guardian * pgard);
  void init_fire_06 (sprite_guardian * pgard);
  void init_fire_07 (sprite_guardian * pgard);
  void init_fire_08 (sprite_guardian * pgard);
  void init_fire_09 (sprite_guardian * pgard);
  void init_fire_10 (sprite_guardian * pgard);
  void init_fire_11 (sprite_guardian * pgard);
};
#endif
