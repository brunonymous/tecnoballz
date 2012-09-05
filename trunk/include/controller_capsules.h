/** 
 * @file controller_capsules.h
 * @brief Capsules controller 
 * @date 2012-09-02 
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
#ifndef __CONTROLLER_CAPSULES__
#define __CONTROLLER_CAPSULES__
class controller_capsules;

#include "../include/objects_list.h"
#include "../include/sprite_capsule.h"
#include "../include/controller_paddles.h"
#include "../include/controller_balls.h"

class controller_capsules:public objects_list < sprite_capsule, controller_capsules >
{
public:
  /** X-coordinate of the first capsule in the shop */
  static const Sint32 SHOP_XCOORD_CAPSULE = 146;
  /** Y-coordinate of the first capsule in the shop */
  static const Sint32 SHOP_YCOORD_CAPSULE = 2;
  /** X-coordinate of the indicator capsule in the shop */
  static const Sint32 SHIP_XCOORD_INDICATOR = 280;
  /** Y-coordinate of the indicator capsule in the shop */
  static const Sint32 SHIP_YCOORD_INDICATOR = 204;

private:
  /** Paddle selected, used in the cheat mode */
  sprite_paddle * paddle_selected;
  /** Counter time delay before next image */
  Sint32 frame_delay;
  /** Time delay before next image of the animation */
  Sint32 frame_period;
  /** Index of the current image */
  Uint32 frame_index;
  /** Overview capsule of a option in the shop */
  sprite_capsule *overview_capsule;
  /** Counter delay before dropping a penalty/bonus capsule */
  Uint32 capsule_delay_counter;
  /** Appearance frequency of the penalty/bonus capsules */
  Uint32 capsule_delay;
  /** Random list of capsules, bonuses or penalties which can
   * fall in the current level */
  const Uint32 *random_list; 
  /** Counter number of bricks breaked before release a new
   * bonus caspule bought in the shop */ 
  Uint32 bricks_breaked_count;
  /** Number of bonus capsules bought in the shop */
  Uint32 num_of_caspules_bought;
  /** Current number of bonus capsules bought in the shop released */
  Uint32 capsules_released_count;
  /** Delay before dropping a bonus capsule bought in the shop */
  Uint32 bonus_delay;
  /** Index of the next bonus capsule of the shopping cart */
  Uint32 shopping_cart_index;
  /** List of the bonus capsules bought in the shop which will fall
   * regularly to the destruction of bricks */
  Sint32 *shopping_cart;
  /** Cheat keys to enable options in bricks levels,
   * only under development */
  static Sint16 cheat_keys[];
  static const Uint16 randomlist[128];

public:
    controller_capsules ();
   ~controller_capsules ();
  void initialize (Uint32 delay, const Uint32* random);
  void send_capsule_from_brick (brick_redraw * brick);
  void send_penalty_or_bonus (sprite_ball *ball);
  void send_penalty_or_bonus (sprite_projectile *blast);
  void check_if_send_capsule (sprite_ball * ball);
  void create_shop_sprites_list ();
  void set_overview_capsule (Uint32 id);
  void move_in_bricks_level ();
  void move_in_guardians_level ();
  void play_animation_in_shop (Uint32 value = 0);
  void check_cheat_keys ();

private:
  void run_in_bricks_level (sprite_paddle * paddle, Uint32 capsule_id);
  void run_in_guards_level (sprite_paddle * paddle, Uint32 capsule_id);

};
#endif
