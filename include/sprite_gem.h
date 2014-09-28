/** 
 * @file sprite_gem.h
 * @brief The gem sprite 
 * @created 2004-04-12 
 * @date 2007-03-09
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
#ifndef __SPRITE_GEM__
#define __SPRITE_GEM__
class sprite_gem;

#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_projectile.h"

typedef enum
{
  GREY_SQUARE,
  GREEN_SPHERE,
  YELLOW_RING,
  BLUE_TRIANGLE,
  GOLD_RHOMBUS,
  BRONZE_PENTAGON
}
GEMS_ENUM;

class sprite_gem:public sprite_object
{
  friend class controller_moneys;

private:
  /** Toward bottom, right, top, left */
  Uint32 towards;
  /** Moving speed in pixels of the gem */
  Uint32 speed_of_moving;
  /** If true the gem is positionned as indicator on the score panel */
  bool is_indicator;
  /** Gem identifier GREY_SQUARE to BRONZE_PENTAGON */
  Uint32 gem_id;
  sprite_paddle *paddle;
  Uint32 blink_counter;
  Uint32 rand_count;

public:
    sprite_gem ();
   ~sprite_gem ();
  bool enable_if_available (sprite_ball *ball);
  bool enable_if_available (sprite_projectile *blast);
  void collect (Uint32 id);
  void enable_blink ();
  Sint32 move ();

private:
  void init_gem (Sint32 pos_x, Sint32 pos_y, sprite_paddle *pad);
  void blink ();

private:
  static const Uint32 gem_random[8];
};
#endif
