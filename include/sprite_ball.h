/**
 * @file sprite_ball.h
 * @brief The ball sprite
 * @date 2014-07-27 
 * @copyright 1991-2014 TLK Games
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
#ifndef __SPRITE_BALL__
#define __SPRITE_BALL__
class sprite_ball;
#include "../include/sprite_object.h"

/** Coordinates of the ball on the ejectors */
typedef struct
  {
    Sint32 x_coord;
    Sint32 y_coord;
  }
ball_ejector_coords;


#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_viewfinders.h"
#include "../include/controller_ejectors.h"

class sprite_ball:public sprite_object
  {
    friend class controller_balls;
    friend class controller_viewfinders;
    friend class controller_paddles;
    friend class sprite_money;
    friend class sprite_gem;
    friend class controller_guardians;
    friend class direction;

  public:
    /** Different types of ball */
    typedef enum
    {
      NORMAL,
      POWER_1,
      POWER_2
    }
    TYPES_OF_BALL;
    /** Different sizes of ball */
    typedef enum
    {
      SIZE_1 = 0,
      SIZE_2 = 3,
      SIZE_3 = 6
    }
    SIZES_OF_BALL;
    /** Width and height of collision of the ball size 1 in low-res */
    static const Sint32 WIDTH_1 = 5;
    /** Width and height of collision of the ball size 2 in low-res */
    static const Sint32 WIDTH_2 = 7;
    /** Width and height of collision of the ball size 3 in low-res */
    static const Sint32 WIDTH_3 = 9;
    /** Left limit of ball in low-res */
    static const Sint32 X_MINIMUM = 5;
    /** Right limit of ball in low-res */
    static const Sint32 X_MAXIMUM = 256;
    /** Top limit of ball in low-res */
    static const Sint32 Y_MINIMUM = 0;
    /** Bottom limit of ball low-res */
    static const Sint32 Y_MAXIMUM = 232;

    /** Previous X coordinate */
    Sint32 previous_x_coord;
    /** Previous Y coordinate */
    Sint32 previous_y_coord;
 
  private:
    /** Ball direction from 0 to 64 step 4
     * 64 = immobile ball */
    Sint32 direction;
    /** Previous ball direction */
    Sint32 previous_direction;
    /** Delay counter before change ball direction
     * Avoid the horizontal blockings */
    Sint32 change_direction_count;
    /** The last paddle touched by the ball */
    sprite_paddle *paddle_touched;
    /** Padlle on witch is stuck the ball */
    sprite_paddle *stick_paddle;
    /** Paddle number on which the ball is stuck */
    Uint32 sticky_paddle_num;
    /** Pointer to the current velocities table */
    Sint16 *velocities;
    /** Pointer to the current velocities table */
    Sint16 *initial_velocities;
    /** Pointer to the current points of collision with a brick */
    Sint32 *brick_collision_points;
    /* Strength of ball 1, 2 or 3 to decrease brick strength */
    Sint32 strength1;
    /* Strength of ball 32, 64 or 96 to decrease brick address */
    Sint32 strength2;
    /** Time delay before ejection of the ball */
    Sint32 ejector_delay;
    /** Table of directions possible that a ball can
     * set when it leave a ejector */
    Sint32 *ejector_table;
    /** Counter before a tilt is possible */
    Sint32 tilt_delay_counter;
    /** Counter delay before accelerating the ball */
    Sint32 accelerate_delay_counter;
    /** Time delay before accelerating the ball */
    Sint32 accelerate_delay;
    /** Delay counter before the ball leaves the paddle */
    Sint32 start_delay_counter;
    /** Initial delay before the ball leaves the paddle */
    Sint32 start_delay;
    /** Direction of the viewfinder when the ball is sticked
     * on a paddle: from 0 to 13 */
    Sint32 viewfinder_direction;
    /** Counter delay before change direction of the viewfinder */
    Sint32 viewfinder_delay;
    /** Size identfier SIZE_1, SIZE_2 or SIZE_3 */
    Sint32 size_id;
    /** Type of ball NORMAL, POWER_1 or POWER_2 */
    Sint32 type;
    /** Brick's width in pixels 16 or 32 */
    Sint32 brick_width;
    /** Previous hited wall RIGHT_WALL, LEFT_WALL, TOP_WALL or 0 if not */
    Sint32 last_hited_wall;
    /** True if collision point of ball with bricks were corrected */
    static bool is_collisions_point_initialized;
    /** Collision points of the ballz size 1 with a brick */
    static Sint32 brick_collision_points_1[8];
    /** Collision points of the ballz size 2 with a brick */
    static Sint32 brick_collision_points_2[8];
    /** Collision points of the ballz size 3 with a brick */
    static Sint32 brick_collision_points_3[8];
    /** Displacement offsets of speed 1 */
    static Sint16 velocities_speed_1[];
    /** Displacement offsets of speed 2 */
    static Sint16 velocities_speed_2[];
    /** Displacement offsets of speed 3 */
    static Sint16 velocities_speed_3[];
    /** Displacement offsets of speed 1 */
    static Sint16 velocities_speed_4[];
    /** Coordinates of the balls on the ejectors */
    static ball_ejector_coords ejector_coords[];
    /** Possible directions of a ball when
     * a player activates the tilt */
    static const Sint32 tilt_directions[16][16];
    /** Directions possible that a ball can set when it leave
     * the top-left ejector */
    static Sint32 ball_ejector_1[];
    /** Directions possible that a ball can set when it leave
     * the bottom-left ejector */
    static Sint32 ball_ejector_2[];
    /** Directions possible that a ball can set when it leave
     * the bottom-right ejector */
    static Sint32 ball_ejector_3[];
    /** Directions possible that a ball can set when it leave
     * the top-right ejector */
    static Sint32 ball_ejector_4[];
    static Sint32 *ball_ejectors[];

  public:
    sprite_ball ();
    ~sprite_ball ();
    static void init_collisions_points ();
    void once_init (Sint32 start, Sint32 speed,
                    sprite_paddle * paddle, Sint16 * table, Sint32 w);
    void starts_again (sprite_paddle * paddle);
    void remove (sprite_paddle * paddle);
    void init_first_ball (Sint32 h);
    void duplicate_from (sprite_ball * ball, Uint32 angle);
    static Sint16 *get_velocities (Sint32 speed);
    sprite_paddle *get_last_paddle_touched ();
    void set_power_1 ();
    void set_power_2 ();
    void set_size_2 ();
    void set_size_3 ();
    void set_maximum_speed ();
    void enbale_on_ejector (Uint32 eject_id, Uint32 otime);
    void set_on_ejector (Uint32 eject_id, Uint32 otime = 1);
    void disable_stick ();
    void accelerate ();
    virtual bool collision (sprite_object * sprite);

  private:
    void set_initial_values (sprite_paddle * paddle);
    void select_image ();
  };
#endif
