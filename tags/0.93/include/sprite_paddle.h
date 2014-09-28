/** 
 * @file sprite_paddle.h
 * @brief A paddle sprite 
 * @date 2007-10-23
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
#ifndef __SPRITE_PADDLE__
#define __SPRITE_PADDLE__

class sprite_paddle;

#include "../include/sprite_object.h"
#include "../include/sprite_ball.h"
#include "../include/controller_projectiles.h"
#include "../include/bitmap_data.h"
#include "../include/sprite_bullet.h"
#include "../include/controller_gigablitz.h"

class sprite_paddle:public sprite_object
{
  friend class controller_paddles;
  friend class controller_balls;
  friend class controller_projectiles;
  friend class sprite_bullet;
  friend class controller_viewfinders;
  friend class controller_gigablitz;
public:
  typedef enum
   {
     NOT_OWN_GUN,
     OWN_GUN,
     FIRE = 3
   } FIRE_PADDLE_STATUS;

   typedef enum 
   {
    NOT_STICKY_PADDLE,
    FREE_STICKY_PADDLE,
    BUSY_STICKY_PADDLE
   } STICKY_PADDLE_STATUS;


private:
  /** Paddle is enabled if the counter is greater than zero */
  Uint32 enable_counter;
  /** True if the paddle is vertical, otherwise horizontal */
  bool is_vertical;
  /** Paddle number from 0 to 5 */
  Uint32 paddle_number;
  /** Paddle length in pixels */
  Uint32 length;
  /** Minimum with of the paddle 32 or 64 pixels */
  Uint32 width_mini;
  /** Maximum with of the paddle 64 or 128 pixels */
  Uint32 width_maxi;
  /** Shift binary right value (3 or 4) used to convert the length to
   * convert the paddle's length in a interger from 0 to 6 */
  Sint32 shift_width;
  /** Fire state: NOT_OWN_GUN, OWN_GUN, or FIRE */
  Sint32 fire_state;
  /** Stick paddle state: NOT_STICKY_PADDLE, FREE_STICKY_PADDLE, or
   * BUSY_STICKY_PADDLE */
  Uint32 sticky_state;
  const Sint32 **rebonds_Ga;    //ball rebounds table (move on the left)
  const Sint32 **rebonds_Dr;    //ball rebounds table (move on the right)
  /** Current directions used for bounce a ball */
  const Sint32 *current_bounces;
  Sint32 *direct_tab;           // table direction si la balle collee
  /** Current stuck ball, NULL if not */
  sprite_ball *stuck_ball;
  /** If true the paddle touched a ball */
  bool is_hit_ball;
  /** Paddle is invinciblei f  the counter is greater than zero
   * available only in the guadians phase */
  Sint32 invincible_counter;
  /** Counter used to blink the padde when it is invincible */
  Sint32 blink_counter;
  /** Used for fire power 1 or fire power 2 */
  controller_projectiles *projectiles;
  /** Used to increase x-coord of a projectile */
  Sint32 projectile_xinc0;
  /** Used to increase y-coord of a projectile */
  Sint32 projectile_yinc0;
  /** Used to increase x-coord of a projectile */
  Sint32 projectile_xinc1;
  /** Used to increase y-coord of a projectile */
  Sint32 projectile_yinc1;
  /** Used to increase x-coord of a projectile */
  Sint32 projectile_xinc2;
  /** Used to increase y-coord of a projectile */
  Sint32 projectile_yinc2;
  /** X-coordinate of center of the circle fire (fire 7) */
  Sint32 projectile_xcenter;
  /** Y-coordinate of center of the circle fire (fire 7) */
  Sint32 projectile_ycenter;
  /** x-offset used for the fire 6 */
  Sint32 projectile_xoffset;
  /** x-offset used for the fire 6 */
  Sint32 projectile_yoffset;

public:
    sprite_paddle (bool has_projectiles = true);
   ~sprite_paddle ();
  void create_projectiles_list ();
  void fire_projectiles ();
  void move_projectiles ();
  void enable_if_ok (bool is_team, Sint32 size, Uint32 counter);
  void set_width (Uint32 size);
  void set_height (Uint32 h);
  void select_image (Uint32 size);
  void select_image ();
  Uint32 get_paddle_number ();
  void set_glue ();
  void set_fire_1 ();
  void set_fire_2 ();
  void release_ball ();
  void stick_ball (sprite_ball * ball);
  Uint32 get_length ();
  bool is_invincible ();
  void set_invincibility (Sint32 delay);
  void blink ();
};
#endif
