/** 
 * @file controller_balls.h
 * @brief Control the balls. Move and collisions 
 * @date 2007-10-05
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
#ifndef __CONTROLLER_BALLS__
#define __CONTROLLER_BALLS__

class controller_balls;

#include "../include/sprite_ball.h"
#include "../include/objects_list.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_ships.h"
#include "../include/right_panel_score.h"
#include "../include/controller_guardians.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_paddles.h"
#include "../include/controller_magnetic_eyes.h"
#include "../include/sprite_eye.h"

class controller_balls:public objects_list < sprite_ball, controller_balls >
{
  friend class controller_ejectors;
  friend class sprite_ball;

private:
  /** Time delay of the glue */
  Uint32 glue_delay;
  /** Time delay before a tilt be possible */
  Sint32 tilt_delay;
  /** True if ball controlled by the left mouse button */
  bool balls_are_controlled;
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_right;
  sprite_paddle *paddle_top;
  sprite_paddle *paddle_left;
  sprite_paddle *paddle_robot;

private:
  static Sint32 rb0[16];
  static Sint32 rb1[16];
  static Sint32 rb2[16];
  static Sint32 rb3[16];
  static Sint32 rb4[16];
  static Sint32 rb5[16];
  static Sint32 rb6[16];
  static Sint32 rb7[16];
  static Sint32 rb8[16];
  static Sint32 *brick_jump[15];

public:
   ~controller_balls ();
  controller_balls ();
  void init (Uint32 start, Uint32 glueC,
                   Uint32 speed, Uint32 tiltC, Uint32 table);
  void run_in_bricks_levels ();
  void run_in_guardians_level ();
  sprite_ball *get_first_ball ();

private:
  void check_outside_balls ();
  void check_outside_balls_guards_level ();
  void activate_tilt ();
  void move_balls ();
  void accelerate ();
  void move_balls_in_guards_level ();
  void collisions_with_paddles ();
  void collisions_with_paddle ();
  void collisions_with_robot ();
  void handle_ejectors ();
  void collisions_with_walls ();
  void collisions_with_sides ();
  void prevent_horizontal_blocking ();
  void bricks_collision ();
  void collisions_with_ships ();
  void collisions_with_eyes ();
  void collisions_with_guardians ();
  void controll_balls ();


public:
  void enable_balls_control ();
  void extra_balls ();
  void add_balls (Uint32 nball = 0);
  void multi_balls ();
  void set_power_1 ();
  void set_power_2 ();
  void set_size_2 ();
  void set_size_3 ();
  void set_maximum_speed ();
  void check_tilt_availability ();
  bool is_sticky_balls_remains ();

};
#endif
