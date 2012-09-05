/**
 * @file sprite_ball.cc 
 * @brief The ball sprite
 * @date 2007-10-01
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
#include "../include/sprite_ball.h"

/**
 * Create the ball sprite
 */
sprite_ball::sprite_ball ()
{
  clear_sprite_members ();
  once_init (0, 0, (sprite_paddle *) NULL, velocities_speed_1, 0);
}

/**
 * Release the ball sprite
 */
sprite_ball::~sprite_ball ()
{}

bool
sprite_ball::is_collisions_point_initialized = false;


/**
 * Correct the collisions point of ball adapted from 
 * resolution
 */
void
sprite_ball::init_collisions_points ()
{
  if (is_collisions_point_initialized)
    {
      return;
    }
  is_collisions_point_initialized = true;
  for (Uint32 i = 0; i < 8; i++)
    {
      brick_collision_points_1[i] *= resolution;
      brick_collision_points_2[i] *= resolution;
      brick_collision_points_3[i] *= resolution;
    }
}

/**
 * perform some initializations
 * @param start Time before the ball leaves the paddle (first time)
 * @param speed Time before the ball accelerates
 * @param paddle Default paddle
 * @param table Speed of the ball from 1 to 4
 * @param w Width of a brick in pixels 
*/
void
sprite_ball::once_init (Sint32 start, Sint32 speed,
                        sprite_paddle * paddle, Sint16 * table, Sint32 w)
{
  start_delay = start;
  accelerate_delay = speed;
  initial_velocities = table;
  disable ();
  sticky_paddle_num = 0;
  start_delay_counter = 0;
  brick_width = w;
  set_initial_values (paddle);
}

/**
 * Restarts the ball 
 * @param paddle pointer to a paddle sprite
 */
void
sprite_ball::starts_again (sprite_paddle * paddle)
{
  enable ();
  start_delay_counter = start_delay;
  sticky_paddle_num = paddle->get_paddle_number ();
  set_initial_values (paddle);
  select_image ();
}

/**
 * Remove the ball
 */
void
sprite_ball::remove (sprite_paddle * paddle)
  {
    disable ();
    sticky_paddle_num = 0;
    start_delay_counter = 0;
    set_initial_values (paddle);
    select_image ();
  }

/**
 * Set initial values of the ball
 * @param paddle pointer to a paddle sprite
 */
void
sprite_ball::set_initial_values (sprite_paddle * paddle)
{
  accelerate_delay_counter = accelerate_delay;
  collision_width = WIDTH_1 * resolution;
  collision_height = WIDTH_1 * resolution;
  direction = 0;
  previous_direction = direction;
  change_direction_count = 0;
  paddle_touched = paddle;
  stick_paddle = paddle;
  velocities = initial_velocities;
  brick_collision_points = brick_collision_points_1;
  strength1 = 1;
  strength2 = brick_width;
  ejector_delay = 0;
  ejector_table = NULL;
  tilt_delay_counter = 0;
  viewfinder_direction = 1;
  viewfinder_delay = 1;
  size_id = SIZE_1;
  type = NORMAL;
  last_hited_wall = 0;
}

/**
 * Initialize a first ball in a level sticked on the paddle
 * @param w Width of the paddle
 */
void
sprite_ball::init_first_ball (Sint32 w)
{
  enable ();
  sprite_paddle *paddle = stick_paddle;
  starts_again (paddle);
  x_coord =
    paddle->get_x_coord () + ((w >> 1) - ((collision_width >> 1) + 1));
  y_coord = paddle->get_y_coord () - collision_height - 1;
}

/**
 * Disable sticked ball
 */
void
sprite_ball::disable_stick ()
{
  sticky_paddle_num = 0;
  start_delay_counter = 0;
}

/**
 * Return speed ball
 * @param speed Speed of the ball from 1 to 4
 * @return Pointer to a displacement offsets table 
 */
Sint16 *
sprite_ball::get_velocities (Sint32 speed)
{
  switch (speed)
    {
    case 1:
      return velocities_speed_1;
    case 2:
      return velocities_speed_2;
    case 3:
      return velocities_speed_3;
    case 4:
      return velocities_speed_4;
    }
  return velocities_speed_1;
}

/**
 * Return the last touched paddle
 * @return a pointer to a paddle sprite
 */
sprite_paddle *
sprite_ball::get_last_paddle_touched ()
{
  return paddle_touched;
}

/**
 * Duplicate this ball from another ball
 * @param ball ball template
 * @param angle angle from 0 to 63
 */
void
sprite_ball::duplicate_from (sprite_ball * ball, Uint32 angle)
{
  is_enabled = true;
  x_coord = ball->x_coord;
  y_coord = ball->y_coord;
  direction = angle;
  sticky_paddle_num = 0;
  tilt_delay_counter = 0;
  size_id = ball->size_id;
  type = ball->type;
  collision_width = ball->collision_width;
  collision_height = ball->collision_height;
  paddle_touched = ball->paddle_touched;
  stick_paddle = ball->stick_paddle;
  velocities = ball->velocities;
  brick_collision_points = ball->brick_collision_points;
  strength1 = ball->strength1;
  strength2 = ball->strength2;
  select_image ();
}

/**
 * Enable the ball power 1
 */
void
sprite_ball::set_power_1 ()
{
  type = POWER_1;
  select_image ();
}

/**
 * Enable the ball power 2
 */
void
sprite_ball::set_power_2 ()
{
  type = POWER_2;
  select_image ();
}

/**
 * Enable the ball size 
 */
void
sprite_ball::set_size_2 ()
{
  strength1 = 2;
  strength2 = brick_width * 2;
  collision_width = WIDTH_2 * resolution;
  collision_height = WIDTH_2 * resolution;
  size_id = SIZE_2;
  brick_collision_points = brick_collision_points_2;
  select_image ();
}

/**
 * Enable the ball size 3
 */
void
sprite_ball::set_size_3 ()
{
  strength1 = 3;
  strength2 = brick_width * 3;
  collision_width = WIDTH_3 * resolution;
  collision_height = WIDTH_3 * resolution;
  size_id = SIZE_3;
  brick_collision_points = brick_collision_points_3;
  select_image ();
}

/**
 * Set the fastest speed
 */
void
sprite_ball::set_maximum_speed ()
{
  velocities = velocities_speed_3;
}

/**
 * Enable a ball on a ejector
 * @param id Ejector identifier from 0 to 3
 * @param delay Time delay before the ejection of the ball
 */
void
sprite_ball::enbale_on_ejector (Uint32 id, Uint32 delay)
{
  is_enabled = true;
  id &= 3;
  ejector_delay = delay;
  ejector_table = ball_ejectors[id];
  x_coord = ejector_coords[id].x_coord;
  y_coord = ejector_coords[id].y_coord;
  /* the ball's motionless */
  direction = 64;
}

/**
 * Set the ball on an ejector
 * @param id Ejector identifier from 0 to 3
 * @param delay Time delay before the ejection of the ball
 */
void
sprite_ball::set_on_ejector (Uint32 id, Uint32 delay)
{
  id &= 3;
  ejector_table = ball_ejectors[id];
  ejector_delay = delay;
  direction = 64;
  current_player->add_score (10);
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::ECJECTOR_IN);
#endif
}

/**
 * Select the ball image
 */
void
sprite_ball::select_image ()
{
  frame_index = size_id + type;
  set_image (frame_index);
}

/**
 * Accelerate the ball
 */
void
sprite_ball::accelerate ()
{
  if (--accelerate_delay_counter > 1)
    {
      return;
    }
  accelerate_delay_counter = accelerate_delay;
  if (velocities == velocities_speed_1)
    {
      velocities = velocities_speed_2;
      return;
    }
  if (velocities == velocities_speed_2)
    {
      velocities = velocities_speed_3;
      return;
    }
  if (velocities == velocities_speed_3)
    {
      velocities = velocities_speed_4;
    }
}

/** Collision points of the ball 1 with a brick */
Sint32
sprite_ball::brick_collision_points_1[8] = {
      5, 3,
      3, 0,
      0, 3,
      3, 5
    };

/** Collision points of the ball 2 with a brick */
Sint32
sprite_ball::brick_collision_points_2[8] = {
      7, 4,
      4, 0,
      0, 4,
      4, 7
    };

/** Collision points of the ball 3 with a brick */
Sint32
sprite_ball::brick_collision_points_3[8] = {
      9, 5,
      5, 0,
      0, 5,
      5, 9
    };

/* directions of a ball from 0 to 60
*              16
*            20/\ 12
*          24  ||   08
*        28    ||     04
*      32<=====64=====> 00
*        36    ||    60
*          40  ||  56
*            44\/52 
*              48
*/

/** Displacement offsets of speed 1 */
Sint16
sprite_ball::velocities_speed_1[] = {
                                      /* 0 */
                                      +2, 0,
                                      /* 4 */
                                      +2, -1,
                                      /* 8 */
                                      +2, -2,
                                      /* 12 */
                                      +1, -2,
                                      /* 16 */
                                      0, -2,
                                      /* 20 */
                                      -1, -2,
                                      /* 24 */
                                      -2, -2,
                                      /* 28 */
                                      -2, -1,
                                      /* 32 */
                                      -2, 0,
                                      /* 36 */
                                      -2, +1,
                                      /* 40 */
                                      -2, +2,
                                      /* 44 */
                                      -1, +2,
                                      /* 48 */
                                      0, +2,
                                      /* 52 */
                                      +1, +2,
                                      /* 56 */
                                      +2, +2,
                                      /* 60 */
                                      +2, +1,
                                      /* 64 */
                                      0, 0
                                    };

Sint16 sprite_ball::velocities_speed_2[] =
  {
    3, 0, 3, -1, 2, -2, 1, -3, 0, -3, -1, -3, -2, -2, -3, -1, -3,
    0, -3, 1, -2, 2, -1, 3, 0, 3, 1, 3, 2, 2, 3, 1, 0, 0};

Sint16 sprite_ball::velocities_speed_3[] =
  {
    4, 0, 4, -1, 3, -3, 1, -4, 0, -4, -1, -4, -3, -3, -4, -1, -4,
    0, -4, 1, -3, 3, -1, 4, 0, 4, 1, 4, 3, 3, 4, 1, 0, 0};

Sint16 sprite_ball::velocities_speed_4[] =
  {
    5, 0, 5, -2, 4, -4, 2, -5, 0, -5, -2, -5, -4, -4, -5, -2,
    -5, 0, -5, 2, -4, 4, -2, 5, 0, 5, 2, 5, 4, 4, 5, 2, 0, 0};

/** Coordinates of the balls on the ejectors */
ball_ejector_coords sprite_ball::ejector_coords[] =
  {
    /* 1: top-left */
    {8, 8}
    ,
    /* 3: bottom-left */
    {8, 3}
    ,
    /* 4: bottom-right */
    {3, 3}
    ,
    /* 2: top-right */
    {3, 8}
  };

/** Possible directions of a ball when
 * a player activates the tilt */
const
Sint32
sprite_ball::tilt_directions[16][16] =
  {
    {4, 4, 8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},
    {8, 8, 8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},
    {4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60},
    {4, 4, 8, 16, 16, 20, 24, 28, 28, 36, 40, 48, 48, 52, 56, 60},
    {4, 4, 8, 12, 20, 20, 24, 28, 28, 36, 40, 44, 52, 52, 56, 60},
    {4, 4, 8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},
    {4, 4, 8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 60, 60},
    {4, 4, 8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56},
    {4, 4, 8, 12, 16, 20, 24, 28, 28, 36, 40, 44, 48, 52, 56, 60},
    {8, 8, 8, 12, 16, 20, 24, 28, 28, 40, 40, 44, 48, 52, 56, 60},
    {4, 4, 12, 12, 16, 20, 24, 28, 28, 36, 44, 44, 48, 52, 56, 60},
    {4, 4, 8, 16, 16, 20, 24, 28, 28, 36, 40, 40, 48, 52, 56, 60},
    {4, 4, 8, 12, 12, 20, 24, 28, 28, 36, 40, 44, 44, 52, 56, 60},
    {4, 4, 8, 12, 16, 24, 24, 28, 28, 36, 40, 44, 48, 56, 56, 60},
    {4, 4, 8, 12, 16, 20, 20, 28, 28, 36, 40, 44, 48, 52, 52, 60},
    {4, 4, 8, 12, 16, 20, 24, 24, 36, 36, 40, 44, 48, 52, 56, 56}
  };


/** Directions possible that a ball can set when it leave
 * the top-left ejector */
Sint32
sprite_ball::ball_ejector_1[] = {
                                  52, 56, 60, 60, 52, 56, 60, 60, 52,
                                  52, 56, 52, 52, 60, 56, 52, 56, 56
                                };

/** Directions possible that a ball can set when it leave
 * the bottom-left ejector */
Sint32
sprite_ball::ball_ejector_2[] = {
                                  8, 4, 12, 12, 8, 4, 4, 12, 8,
                                  4, 12, 4, 8, 12, 4, 8, 12, 4, 4
                                };

/** Directions possible that a ball can set when it leave
 * the bottom-right ejector */
Sint32
sprite_ball::ball_ejector_3[] = {
                                  20, 28, 24, 20, 20, 28, 28, 24, 20, 28,
                                  24, 24, 28, 28, 20, 20, 24, 24, 28
                                };

/** Directions possible that a ball can set when it leave
 * the top-right ejector */
Sint32
sprite_ball::ball_ejector_4[] = {
                                  36, 44, 40, 36, 36, 44, 44, 40, 40,
                                  36, 44, 40, 40, 36, 36, 44, 44, 40, 36
                                };

/** Pointers of directions possible that a ball can
 * set when it leave one a ejector */
Sint32 *
sprite_ball::ball_ejectors[4] = {
                                  ball_ejector_1,
                                  ball_ejector_2,
                                  ball_ejector_3,
                                  ball_ejector_4
                                };
