/** 
 * @file controller_balls.cc 
 * @brief Control the balls. Move and collisions 
 * @date 2012-09-15 
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
#include "../include/controller_balls.h"
#include "../include/controller_paddles.h"
#include "../include/handler_players.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/list_sprites.h"

/** 
 * Create the balls controller into guards levels
 */
controller_balls::controller_balls ()
{
  littleInit ();
  glue_delay = 60;
  tilt_delay = 60;
  max_of_sprites = 20;
  sprites_have_shades = true;
  balls_are_controlled = false;
  sprite_type_id = sprite_object::BALLS;
}

/** 
 * Release the balls controller
 */
controller_balls::~controller_balls ()
{
  release_sprites_list ();
}

/**
 * Initialize the balls before one level
 * @param start Time delay before the ball leaves the paddle (first time)
 * @param glue Time delay before the ball leaves (glue option)
 * @param speed Time delay before the ball accelerates
 * @param tilt Time delay before "tilt" is available
 * @param table speed ball (1 to 4)
 */
void
controller_balls::init (Uint32 start,
                        Uint32 glue, Uint32 speed, Uint32 tilt,
                        Uint32 table)
{

  controller_paddles *paddles = controller_paddles::get_instance ();

  glue_delay = glue;
  tilt_delay = tilt;
  Sint16* velocities = sprite_ball::get_velocities (table);
  paddle_bottom = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);

  Sint32 w;
  if (current_phase == GUARDS_LEVEL)
    {
      /* the width of a brick in pixels */
      w = 16 * resolution;
    }
  else
    {
      paddle_right = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle_top = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle_left = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle_robot = paddles->get_paddle (controller_paddles::ROBOT_PADDLE);
      controller_bricks *bricks = controller_bricks::get_instance ();
      w = bricks->get_brick_width ();
    }
  
  /* initialize all balls */
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      ball->once_init (start, speed, paddle_bottom, velocities, w);
    }

  /* first ball special initialization */
  sprite_ball *ball = sprites_list[0];
  paddle_bottom->stuck_ball = ball;
  ball->init_first_ball (paddle_bottom->collision_width);
  /* one ball on the screen */
  num_of_sprites = 1;
  if (current_phase == BRICKS_LEVEL)
    {
      controller_ejectors *ejectors = controller_ejectors::get_instance ();
      ejectors->initialize_ball_positions (&sprite_ball::ejector_coords[0]);
    }
}

/**
 * Move the balls and check the collisions in the bricks levels
 */
void
controller_balls::run_in_bricks_levels ()
{
  activate_tilt ();
  bricks_collision ();
  move_balls ();
  collisions_with_paddles ();
  collisions_with_robot ();
  /* collisions between balls and the 3 walls */
  collisions_with_walls ();
  /* collisions between balls and the 4 ejectors */
  handle_ejectors ();
  collisions_with_ships ();
  collisions_with_eyes ();
  /* control balls with the left mouse button */
  controll_balls ();
  accelerate ();
  check_outside_balls ();
  if (!balls_are_controlled)
    {
      check_tilt_availability ();
    }
}

/**
 * Move the balls and check the collisions in the guardians levels
 */
void
controller_balls::run_in_guardians_level ()
{
  check_outside_balls_guards_level ();
  activate_tilt ();
  move_balls_in_guards_level ();
  collisions_with_sides ();
  /* prevent that the ball remains blocked horizontally */
  prevent_horizontal_blocking ();
  collisions_with_paddle ();
  collisions_with_guardians ();
  check_tilt_availability ();
  accelerate ();
}

/** 
 * Check if balls go out of the screen of game
 */
void
controller_balls::check_outside_balls ()
{
  Sint32 min_x = sprite_ball::X_MINIMUM * resolution;
  Sint32 max_x = sprite_ball::X_MAXIMUM * resolution;
  Sint32 min_y = sprite_ball::Y_MINIMUM * resolution;
  Sint32 max_y = sprite_ball::Y_MAXIMUM * resolution;
  controller_ships *ships = controller_ships::get_instance ();
  right_panel_score *panel = right_panel_score::get_instance ();
  head_animation *head_anim = head_animation::get_instance ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      sprite_paddle *paddle = NULL;
      Sint32 j = ball->x_coord;
      if (j < min_x)
        {
          paddle = paddle_left;
        }
      else
        {
          if (j >= (max_x - ball->collision_width))
            {
              paddle = paddle_right;
            }
          else
            {
              j = ball->y_coord;
              if (j < min_y)
                {
                  paddle = paddle_top;
                }
              else if (j > max_y)
               { 
                  paddle = paddle_bottom;
               }
            }
        }

      /* ball is out of screen? */
      if (NULL == paddle)
        {
          /* no, continue */
          continue;
        }

      /* paddle id enabled ? */
      if (!paddle->is_enabled)
        {
          /* no, sey he bottom paddle by default */
          paddle = paddle_bottom;
        }

      /*
       * there is still at least one ball
       */
      if (--num_of_sprites > 0)
        {
          ball->remove (paddle_bottom);
          continue;
        }

      /*
       * the player loses his last ball and a life
       */
      /* one starts again with only one ball  */
      num_of_sprites = 1;
      ball->paddle_touched->stick_ball (ball);
      ball->starts_again (ball->paddle_touched);
      head_anim->start_interference ();
      current_player->remove_life (1);
      ships->force_explosion ();
#ifndef SOUNDISOFF
      audio->play_lost_music ();
      audio->play_sound (handler_audio::LOST_LIFE);
#endif
      short_info_messages* messages = short_info_messages::get_instance ();
      messages->send_message_request (short_info_messages::LOST_FILE);
      messages->send_message_request (short_info_messages::ARE_YOU_READY);
      panel->reset_gigablitz_countdown ();
      controller_bricks *bricks = controller_bricks::get_instance ();
      bricks->start_cycling ();
      tiles_background *tiles = tiles_background::get_instance ();
      tiles->set_scroll_type(tiles_background::TILES_SCROLL_LOST);
    }
}

/**
 * Check if balls go out of the screen of game
 */
void
controller_balls::check_outside_balls_guards_level ()
{
  Sint32 max_y = sprite_ball::Y_MAXIMUM * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      if (ball->y_coord <= max_y)
        {
          continue;
        }
      if (--num_of_sprites > 0)
        {
          ball->remove (paddle_bottom);
          continue;
        }

      /*
       * the player loses a life 
       */
      /* one starts again with only one ball */
      num_of_sprites = 1;
      paddle_bottom->stick_ball (ball);
      ball->starts_again (paddle_bottom);
      ball->set_power_2 ();
      current_player->remove_life (1);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::LOST_LIFE);
#endif
    }
}

/**
 * Activate the tilt if 2 buttons are pressed simultaneously
 */
void
controller_balls::activate_tilt ()
{
  if (!keyboard->is_gigablitz_or_tilt ())
    {
      return;
    }
  bool is_tilt = false;
  Sint32 delay = tilt_delay;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled  || ball->tilt_delay_counter < delay)
        {
          continue;
        }
      is_tilt = true;
      break;
    }

  if (!is_tilt)
    {
      return;
    }


  Sint32 rand = random_counter;
  rand = rand & 15;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      if (ball->direction < 64)
        {
          Sint32 d = (ball->direction >> 2) & 0xf;
          ball->direction = sprite_ball::tilt_directions[d][rand];
        }
      ball->tilt_delay_counter = 0;
    }
  
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::TECNOBALL);
#endif
  display->tilt_screen ();
}

/**
 * handle the acceleration of the balls 
 */
void
controller_balls::accelerate ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *balle = sprites_list[i];
      if (balle->is_enabled)
        balle->accelerate ();
    }
}

/**
 * Displacement of the balls in bricks level
 */
void
controller_balls::move_balls ()
{
  Sint32 j;
  sprite_paddle *paddle;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
	{
	  continue;
	}

      /*  the ball is not sticked */
      if (ball->sticky_paddle_num == 0)
	{
	  /* the balle moves */
	  j = ball->direction;
	  if (j > 64)
	    {
	      std::cerr << "controller_balls::" <<
		"move_balls() ball->direction = " <<
		j << std::endl;
	      j = 60;
	    }
	  Sint16 *velocities = ball->velocities;
	  velocities = (Sint16 *) ((char *) velocities + j);
	  Sint32 offset = *(velocities++);
	  ball->x_coord += (offset * resolution);
	  offset = *velocities;
	  ball->y_coord += (offset * resolution);
	  continue;
	}

      paddle = ball->paddle_touched;
      if (--ball->start_delay_counter == 0)
	{
	  tiles_background *tiles = tiles_background::get_instance ();
	  tiles->set_scroll_type(tiles_background::TILES_NO_SCROLL);
	  ball->sticky_paddle_num = 0;
	  if (paddle->sticky_state == sprite_paddle::BUSY_STICKY_PADDLE)
	    {
	      paddle->sticky_state = sprite_paddle::FREE_STICKY_PADDLE;
	    }
	  paddle->stuck_ball = (sprite_ball *) NULL;
	  continue;
	}

      /* displacement of the balls sticked to the paddle */
      switch (ball->sticky_paddle_num)
	{
	case controller_paddles::BOTTOM_PADDLE:
	  j = (paddle->collision_width >> 1) -
	    ((ball->collision_width >> 1) + 1);
	  j += paddle->x_coord;
	  ball->x_coord = j;
	  j = (paddle->y_coord) - (ball->collision_height + 1);
	  ball->y_coord = j;
	  break;

	case controller_paddles::RIGHT_PADDLE:
	  j = (paddle->x_coord) - (ball->collision_width - 1);
	  ball->x_coord = j;
	  j =
	    (paddle->collision_height >> 1) -
	    ((ball->collision_height >> 1) + 1);
	  j += paddle->y_coord;
	  ball->y_coord = j;
	  break;

	case controller_paddles::TOP_PADDLE:
	  j =
	    (paddle->collision_width >> 1) -
	    ((ball->collision_width >> 1) + 1);
	  j += paddle->x_coord;
	  ball->x_coord = j;
	  j = (paddle->y_coord) + paddle->collision_height + 1;
	  ball->y_coord = j;
	  break;

	case controller_paddles::LEFT_PADDLE:
	  j = (paddle->x_coord) + (paddle->collision_width) + 1;
	  ball->x_coord = j;
	  j =
	    (paddle->collision_height >> 1) -
	    ((ball->collision_height >> 1) + 1);
	  j += paddle->y_coord;
	  ball->y_coord = j;
	  break;

	}


      if (--ball->viewfinder_delay < 0)
	{
	  ball->viewfinder_delay = 8;
	  if (++ball->viewfinder_direction > 13)
	    {
	      ball->viewfinder_direction = 0;
	    } 
	  paddle = ball->paddle_touched;
    ball->direction = paddle->direct_tab[ball->viewfinder_direction];

	}
    }
}

/**
 * Displacement of the balls in guardians level
 */
void
controller_balls::move_balls_in_guards_level ()
{
  Sint32 j;
  sprite_paddle *paddle;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
	{
	  continue;
	}

      /*  the ball is not sticked */
      if (ball->sticky_paddle_num == 0)
	{
	  /* the balle moves */
	  j = ball->direction;
	  if (j > 64)
	    {
	      std::cerr << "controller_balls::" <<
		"move_balls_in_guards_level() ball->direction = " <<
		j << std::endl;
	      j = 60;
	    }
	  Sint16 *velocities = ball->velocities;
	  velocities = (Sint16 *) ((char *) velocities + j);
	  Sint32 offset = *(velocities++);
	  ball->x_coord += (offset * resolution);
	  offset = *velocities;
	  ball->y_coord += (offset * resolution);
	  continue;
	}

      paddle = ball->paddle_touched;
      if (--ball->start_delay_counter == 0)
	{
	  ball->sticky_paddle_num = 0;
	  if (paddle->sticky_state == sprite_paddle::BUSY_STICKY_PADDLE)
	    {
	      paddle->sticky_state = sprite_paddle::FREE_STICKY_PADDLE;
	    }
	  paddle->stuck_ball = (sprite_ball *) NULL;
	  continue;
	}
    
      /* displacement of the balls sticked to the paddle */
      switch (ball->sticky_paddle_num)
	{
	case controller_paddles::BOTTOM_PADDLE:
	  j = (paddle->collision_width >> 1) -
	    ((ball->collision_width >> 1) + 1);
	  j += paddle->x_coord;
	  ball->x_coord = j;
	  j = (paddle->y_coord) - (ball->collision_height + 1);
	  ball->y_coord = j;
	  break;
	}

      if (--ball->viewfinder_delay < 0)
	{
	  ball->viewfinder_delay = 8;
	  if (++ball->viewfinder_direction > 13)
	    {
	      ball->viewfinder_direction = 0;
	    }
	  paddle = ball->paddle_touched;
    ball->direction = paddle->direct_tab[ball->viewfinder_direction];
	}
    }
}

/**
 * Collisions betweeb balls and paddles in bricks level 
 */
void
controller_balls::collisions_with_paddles ()
{
  Sint32 j, x, y;
  sprite_paddle *paddle, *touched_paddle;
  paddle_bottom->is_hit_ball = false;
  paddle_right->is_hit_ball = false;
  paddle_top->is_hit_ball = false;
  paddle_left->is_hit_ball = false;
  right_panel_score *panel = right_panel_score::get_instance ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled || ball->sticky_paddle_num > 0 || ball->direction >= 64)
        {
          continue;
        }
          /* bottom paddle */
          paddle = paddle_bottom;
          touched_paddle = NULL;
          if (paddle->is_enabled)
            {
              x = paddle->x_coord;
              y = paddle->y_coord;
              if (ball->x_coord + (Sint32)ball->collision_width > x &&
                  ball->y_coord + (Sint32)ball->collision_height > y &&
                  ball->x_coord < x + (Sint32)paddle->collision_width &&
                  ball->y_coord < y + (Sint32)paddle->collision_height)
                {
                  ball->y_coord = y - ball->collision_height;
                  touched_paddle = paddle;
                  panel->reset_gigablitz_countdown ();
                }
            }

          /* right paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_right;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->x_coord = x - ball->collision_width;
                      touched_paddle = paddle;
                    }
                }
            }

          /* top paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_top;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->y_coord = y + paddle->collision_height;
                      touched_paddle = paddle;
                    }
                }
            }

          /* left paddle */
          if (NULL == touched_paddle)
            {
              paddle = paddle_left;
              if (paddle->is_enabled)
                {
                  x = paddle->x_coord;
                  y = paddle->y_coord;
                  if (ball->x_coord + (Sint32)ball->collision_width > x &&
                      ball->y_coord + (Sint32)ball->collision_height > y &&
                      ball->x_coord < x + (Sint32)paddle->collision_width &&
                      ball->y_coord < y + (Sint32)paddle->collision_height)
                    {
                      ball->x_coord = x + paddle->collision_width;
                      touched_paddle = paddle;
                    }
                }
            }

          /* does the ball touch a paddle?  */
          if (NULL != touched_paddle)
            {
              touched_paddle->is_hit_ball = true;
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::BALL_HIT_PADDLE);
#endif
              ball->paddle_touched = touched_paddle;
              ball->tilt_delay_counter = 0;
              j = ball->direction;
              if (j > 64)
                {
                  std::cerr <<
                    "controller_balls::collisions_with_paddles() "
                    << "(1) ball->direction " << j;
                  j = 64;
                }
              const Sint32 *bounces = touched_paddle->current_bounces;
              bounces = (Sint32 *) ((char *) bounces + j);
              j = *bounces;
              ball->direction = j;
              if (touched_paddle->sticky_state == sprite_paddle::FREE_STICKY_PADDLE && !touched_paddle->stuck_ball)
                {
                  /* ball is sticked on the paddle */
                  touched_paddle->sticky_state = sprite_paddle::BUSY_STICKY_PADDLE;
                  touched_paddle->stuck_ball = (sprite_ball *) ball;
                  ball->stick_paddle = touched_paddle;
                  /* time of the stick */
                  ball->start_delay_counter = glue_delay;
                  ball->sticky_paddle_num = paddle->paddle_number;
                }
        }
    }
}

/**
 * Collisions balls and the paddle in guardians level
 */
void
controller_balls::collisions_with_paddle ()
{
  Sint32 j, x, y;
  const Sint32 *monPT;
  sprite_paddle *paddle, *touched_paddle;
  paddle_bottom->is_hit_ball = false;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      paddle = paddle_bottom;
      if (!paddle->is_enabled)
        {
          continue;
        }
      touched_paddle = NULL;
      x = paddle->x_coord;
      y = paddle->y_coord;
      if (ball->x_coord + (Sint32)ball->collision_width > x &&
          ball->y_coord + (Sint32)ball->collision_height > y &&
          ball->x_coord < x + (Sint32)paddle->collision_width &&
          ball->y_coord < y + (Sint32)paddle->collision_height)
        {
          ball->y_coord = y - ball->collision_height;
          touched_paddle = paddle;
        }
      
      /* does the ball touch a paddle */
      if (touched_paddle == NULL)
        {
          continue;
        }
      touched_paddle->is_hit_ball = true;
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::BALL_HIT_PADDLE);
#endif
      ball->paddle_touched = touched_paddle;
      ball->tilt_delay_counter = 0;
      j = ball->direction;
      monPT = touched_paddle->current_bounces;
      monPT = (Sint32 *) ((char *) monPT + j);
      ball->direction = *monPT;
      if (touched_paddle->sticky_state == sprite_paddle::FREE_STICKY_PADDLE)
        {
          /* ball glued to the paddle */
          touched_paddle->sticky_state = sprite_paddle::BUSY_STICKY_PADDLE;
          touched_paddle->stuck_ball = (sprite_ball *) ball;
          ball->stick_paddle = touched_paddle;
          /* time of the glue */
          ball->start_delay_counter = glue_delay;
          ball->sticky_paddle_num = paddle->paddle_number;
        }
    }
}

/**
 * Handle collisions balls with robot paddle
 */
void
controller_balls::collisions_with_robot ()
{
  if (paddle_robot->enable_counter == 0)
    {
      return;
    }
  sprite_paddle *paddle = paddle_robot;
  paddle->is_hit_ball = false;
  Sint32 x1 = paddle->x_coord;
  Sint32 y1 = paddle->y_coord;
  Sint32 x2 = paddle->x_coord + paddle->collision_width;
  Sint32 y2 = paddle->y_coord + paddle->collision_height + 8;
  const Sint32 *monPT;
  Sint32 j;
  for (Uint32 i = 0; i < max_of_sprites; i++)
  {
    sprite_ball *ball = sprites_list[i];
    if (!ball->is_enabled)
    {
      continue;
    }
    if (ball->x_coord + (Sint32)ball->collision_width > x1 &&
        ball->y_coord + (Sint32)ball->collision_height > y1 &&
        ball->x_coord < x2 && ball->y_coord < y2)
    {
      ball->y_coord = y1 - ball->collision_height;
      paddle->is_hit_ball = true;
      j = ball->direction;
      monPT = paddle->current_bounces;
      monPT = (Sint32 *) ((char *) monPT + j);
      ball->direction = *monPT;
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::BALL_HIT_PADDLE);
#endif
    }
  }
}

/**
 * Handle ejectors in bricks level
 */
void
controller_balls::handle_ejectors ()
{
  controller_ejectors *ejectors = controller_ejectors::get_instance ();
  sprite_object *eject0 =
    ejectors->get_ejector (controller_ejectors::TOP_LEFT_EJECTOR);
  sprite_object *eject1 =
    ejectors->get_ejector (controller_ejectors::BOTTOM_LEFT_EJECTOR);
  sprite_object *eject2 =
    ejectors->get_ejector (controller_ejectors::BOTTOM_RIGHT_EJECTOR);
  sprite_object *eject3 =
    ejectors->get_ejector (controller_ejectors::TOP_RIGHT_EJECTOR);

  for (Uint32 i = 0; i < max_of_sprites; i++)
  {
    sprite_ball *ball = sprites_list[i];
    if (!ball->is_enabled)
    {
      continue;
    }
    
   /* the ball is on an ejector */
    if (ball->ejector_table != NULL)
      {
	ball->ejector_delay++;
	if (ball->ejector_delay >= 200)
	  {
	    ball->ejector_delay = 0;
	    ball->ejector_table = NULL;
	  }
	/* time before ejection */
	else if (ball->ejector_delay == 160)
	  {
             ball->direction = ball->ejector_table[random_counter & 0xF];
	  }
	continue;
      }

    /*
     * if not test if a ball is aspired by an ejector
     */
      /* top-left */
      if (ball->collision (eject0))
      {
        ball->pull (eject0, 10 * resolution, 10 * resolution);
        ball->set_on_ejector(controller_ejectors::TOP_LEFT_EJECTOR);
      }
      else
      {
        /* top-right */
        if (ball->collision (eject3))
        {
          ball->pull (eject3, 5 * resolution,
              10 * resolution);
          ball->set_on_ejector(controller_ejectors::TOP_RIGHT_EJECTOR);
        }
        else
        {
          /* bottom-left */
          if (ball->collision (eject1))
          {
            ball->pull (eject1, 10 * resolution,
                5 * resolution);
            ball->set_on_ejector(controller_ejectors::BOTTOM_LEFT_EJECTOR);
          }
          else
          {
            /* bottom-right */
            if (ball->collision (eject2))
            {
              ball->pull (eject2, 5 * resolution,
                  5 * resolution);
              ball->set_on_ejector(controller_ejectors::BOTTOM_RIGHT_EJECTOR);
            }
          }
        }
      }
  }
}

/**
 * Collisions of the balls with the 4 walls in the bricks levels
 */
void
controller_balls::collisions_with_walls ()
{
  controller_sides_bricks *sides_bricks =
    controller_sides_bricks::get_instance ();
  Sint32 left_xcoord = sides_bricks->get_left_xcoord ();
  Sint32 right_xcoord = sides_bricks->get_right_xcoord ();
  Sint32 top_ycoord = sides_bricks->get_top_ycoord ();
  sprite_wall *wall = sprite_wall::get_instance ();
  Sint32 bottom_ycoord = wall->get_y_coord ();
  bool is_wall = wall->is_enable ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 x = ball->x_coord;
      Sint32 y = ball->y_coord;
      Sint32 *monPT = NULL;

      /* collision with the bottom wall, if it's enable */
      if (is_wall && y > (bottom_ycoord - (Sint32)ball->collision_height))
        {
          monPT = rb7;
        }
      else
        {
          /* collision with the left wall */
          if (x < left_xcoord)
            {
              if (sides_bricks->collision_with_left_wall (y))
                {
                  monPT = rb5;
                }
            }
          else
            {
              /* collision with the right wall */
              x += ball->collision_width;
              if (x > right_xcoord)
                {
                  if (sides_bricks->collision_with_right_wall (y))
                    monPT = rb1;
                }
              else
                {
                  /* collision with the up wall */
                  if (y < top_ycoord)
                    {
                      if (sides_bricks->collision_with_top_wall (x))
                        monPT = rb3;
                    }
                }
            }
        }

      /* collision dectected */
      if (NULL != monPT)
        {
          monPT = (Sint32 *) ((char *) monPT + ball->direction);
          ball->direction = *monPT;
#ifndef SOUNDISOFF
          audio->play_sound (handler_audio::BALL_HIT_SIDE);
#endif
        }
    }
}

/**
 * Collisions of the balls with the 3 sides in the guarduans levels
 */
void
controller_balls::collisions_with_sides ()
{
  Sint32 left_xcoord, right_xcoord;
  if (resolution == 1)
    {
      left_xcoord = 16;
      right_xcoord = 300;
    }
  else
    {
      left_xcoord = 20;
      right_xcoord = 612;
    }

  Sint32 top_ycoord = 8 * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
	{
	  continue;
	}
      Sint32 x = ball->x_coord;
      Sint32 y = ball->y_coord;
      Sint32 *bounce = NULL;
      if (x < left_xcoord)
	{
	  bounce = rb5;
	  ball->x_coord = left_xcoord;
#ifndef SOUNDISOFF
	  audio->play_sound (handler_audio::BALL_HIT_SIDE);
#endif
	  ball->last_hited_wall = controller_sides_bricks::LEFT_WALL;
	}
      else
	{
	  if (x > right_xcoord)
	    {
	      bounce = rb1;
	      ball->x_coord = right_xcoord;
#ifndef SOUNDISOFF
	      audio->play_sound (handler_audio::BALL_HIT_SIDE);
#endif
	      ball->last_hited_wall = controller_sides_bricks::RIGHT_WALL;
	    }
	  else
	    {
	      if (y < top_ycoord)
		{
		  bounce = rb3;
		  ball->y_coord = top_ycoord;
#ifndef SOUNDISOFF
		  audio->play_sound (handler_audio::BALL_HIT_SIDE);
#endif
		  ball->last_hited_wall = controller_sides_bricks::TOP_WALL;
		}
	      else
		{
		  ball->last_hited_wall = 0;
		}
	    }
	}
      if (bounce != NULL)
	{
	  bounce = (Sint32 *) ((char *) bounce + ball->direction);
	  ball->direction = *bounce;
	}
    }
}

/**
 * Prevent that the ball remains blocked horizontally in guardians level
 */
void
controller_balls::prevent_horizontal_blocking ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = sprites_list[i];
      if (!ball->is_enabled)
	{
          continue;
	}
      Sint32 direction = ball->direction;
      if (direction >= 32)
	{
          direction -= 32;
	}
      if (direction == ball->previous_direction)
        {
          if (ball->change_direction_count++ > 360 && direction == 0 && ball->last_hited_wall > 0)
            {
              if (ball->direction == 32)
                {
                  ball->direction = 28;
                }
              else
                {
                  ball->direction = 4;
                }
            }
        }
      else
        {
          ball->previous_direction = direction;
          ball->change_direction_count = 0;
        }
    }
}

/** 
 * Check for collision balls with bricks 
 */
void
controller_balls::bricks_collision ()
{
  controller_bricks *bricks = controller_bricks::get_instance ();
  /* brick's width in pixels */
  Uint32 brick_width = bricks->get_brick_width ();
  /* y-offset between 2 bricks: 8 or 16 pixels */
  Sint32 byoff = bricks->getYOffset ();
  /* first indestructible brick */
  Sint32 indestructible = bricks->get_indestructible_offset ();
  sprite_ball **balls = sprites_list;
  brick_info *bricks_map = bricks->get_bricks_map ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      /* true if collision with a indestructible brick */
      bool indes_col = false;
      /* 4 points of collision with a brick */
      Sint32 *points = ball->brick_collision_points;
      Sint32 bounce_inc = 1;
      /* index used for bounce */
      Sint32 bounce_index = 0;
      /* for the 4 collision pointof the ball */
      for (Sint32 j = 0; j < 4; j++, bounce_inc += bounce_inc)
        {
          Sint32 x = ball->x_coord;
          x += *(points++);
          Sint32 y = ball->y_coord;
          y += *(points++);
          /* list of bricks to clear or redraw */
          brick_redraw *redraw = bricks->get_bricks_redraw ();
          redraw->xcoord_collision = x;
          redraw->ycoord_collision = y;
          x /= brick_width;
          y /= byoff;
	  /* 16 bricks per lines */
          y *= controller_bricks::MAX_OF_BRICKS_HORIZONTALLY;
          x += y;
          brick_info *brick = (bricks_map + x);
          //x = brick->source_offset;
          /* collision between a ball and a brick? */
          //if (0 == x)
          if (brick->source_offset == 0)
            {
	      /* no collision */
              continue;
            }
          redraw->is_gigablitz_destroyed = false;
          redraw->paddle = ball->paddle_touched;
	  if (!has_background)
	    {
	       brick->sprite->touch();
	    }
          //x = x - indestructible; x = x - 25088
          //if (x >= 0)
          if (brick->source_offset >= indestructible)
            {
              /* 
               * indestructible brick touched!
               */
              /* collision with indestructible brick */
              indes_col = true;
              /* indestructible/destructible bricks? */
              //if ((x -= brick_width) > 0)
              if (brick->source_offset > (Sint32)(indestructible + brick_width))
                {
                  if (ball->type == sprite_ball::POWER_2)
                    {
                      redraw->pixel_offset = brick->pixel_offset;
                      redraw->brick_map = brick;
                      redraw->is_indestructible = true;
                      redraw->number = brick->number;
                      redraw->sprite = brick->sprite;
                      redraw->is_background = true;
                      /* clear brick code */
                      brick->source_offset = 0;
		      /* restore background under brick */
		      bricks->bricks_redraw_next ();
                    }
                  else
                    {
                      //x = 2;
#ifndef SOUNDISOFF
                      audio->play_sound (handler_audio::HIT_INDESTRUCTIBLE_BRICK2);
#endif
                    }
                }
              else
                {
		  /* brick's really indestructible */
                  //x = 1; 
#ifndef SOUNDISOFF
                  audio->play_sound (handler_audio::HIT_INDESTRUCTIBLE_BRICK1);
#endif
                }
            }

          /* 
           * normal brick touched
           */
          else
            {
              redraw->is_indestructible = false;
              redraw->pixel_offset = brick->pixel_offset;
              redraw->brick_map = brick;
              redraw->sprite = brick->sprite;
              brick->h_pos = brick->h_pos - (ball->strength1 * 2);
              if (brick->h_pos <= 0)
                {
                  brick->h_pos = 0;
                  /* clear brick code */
                  brick->source_offset = 0;
                  redraw->number = brick->number;
                  /* restore background */
                  redraw->is_background = true;
                }
              else
                {
                  brick->source_offset = brick->source_offset - ball->strength2;
                  redraw->number = brick->pixel_offset;
                  /* redraw a new brick */
                  redraw->is_background = false;
                }
	      bricks->bricks_redraw_next ();
            }
          /* inc. bounce index */
          bounce_index += bounce_inc;
        }
      if (--bounce_index >= 0)
        {
          if (indes_col || ball->type == sprite_ball::NORMAL)
            {
              Sint32 *rebPT = *(brick_jump + bounce_index);
              rebPT = (Sint32 *) ((char *) rebPT + ball->direction);
              ball->direction = *rebPT;
            }
        }
  }
}

/**
 * Collisions between balls and eyes
 */
void
controller_balls::collisions_with_eyes ()
{
  controller_magnetic_eyes *eyes = controller_magnetic_eyes::get_instance ();
  Sint32 hypo = eyes->hypotenuse;
  sprite_ball **liste = sprites_list;
  Sint32 numof_eyes = eyes->get_max_of_sprites ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(liste++);
      if (!ball->is_enabled)
        {
          continue;
        }
      sprite_eye **eyes_list = eyes->get_sprites_list ();
      for (Sint32 j = 0; j < numof_eyes; j++)
        {
          sprite_eye *eye = *(eyes_list++);
          if (!eye->is_enabled)
            {
              continue;
            }
          Sint32 center_x = eye->x_coord + eyes->center_x;
          Sint32 center_y = eye->y_coord + eyes->center_y;
          Sint32 delta_x =
            ball->x_coord + (ball->collision_width / 2) - center_x;
          delta_x = delta_x * delta_x;
          Sint32 delta_y =
            ball->y_coord + (ball->collision_width / 2) - center_y;
          delta_y = delta_y * delta_y;
          if (delta_x + delta_y >= hypo)
            {
              continue;
            }
          delta_x = ball->x_coord + (ball->collision_width / 2) - center_x;
          delta_y = ball->y_coord + (ball->collision_width / 2) - center_y;
          if (delta_y == 0)
            {
              if (delta_x < 0)
               {
                  ball->direction = 32;
               }
              else
               {
                  ball->direction = 0;
               }
            }

          if (delta_y > 0)
            {
              if (delta_x == 0)
                {
                  ball->direction = 48;
                }
              if (delta_x < 0)
                {
                  delta_x = -delta_x;
                  if (delta_x == delta_y)
                    {
                      ball->direction = 40;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->direction = 44;
                    }
                  else
                    {
                      ball->direction = 36;
                    }
                }
              else
                {
                  if (delta_x == delta_y)
                    {
                      ball->direction = 56;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->direction = 52;
                    }
                  else
                    {
                      ball->direction = 56;
                    }
                }
            }

          if (delta_y < 0)
            {
              delta_y = -delta_y;
              if (delta_x == 0)
                {
                  ball->direction = 16;
                }
              if (delta_x < 0)
                {
                  delta_x = -delta_x;
                  if (delta_x == delta_y)
                    {
                      ball->direction = 24;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->direction = 20;
                    }
                  else
                    {
                      ball->direction = 28;
                    }
                }
              else
                {
                  if (delta_x == delta_y)
                    {
                      ball->direction = 8;
                    }
                  if (delta_x < delta_y)
                    {
                      ball->direction = 12;
                    }
                  else
                    {
                      ball->direction = 4;
                    }
                }
            }
        }
    }
}

/**
 * Collisions between balls and ships
 */
void
controller_balls::collisions_with_ships ()
{
  controller_ships *ships = controller_ships::get_instance ();
  sprite_ball **balls = sprites_list;
  Uint32 t = ships->get_max_of_sprites ();
  sprite_ship **ships_list = ships->get_sprites_list ();
  Sint32 nouve = (random_counter & 0xF) << 2;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 h = ball->collision_width;
      h = h - 2;
      Sint32 x1 = ball->x_coord;
      Sint32 x2 = x1 + h;
      x1 -= 20;
      Sint32 y1 = ball->y_coord;
      Sint32 y2 = y1 + h;
      y1 -= 26;
      sprite_ship **ships = ships_list;
      for (Uint32 j = 0; j < t; j++)
        {
          sprite_ship *ship = *(ships++);
          if (ship->enable_counter > 0)
            {
              continue;
            }

          Sint32 k = ship->y_coord;
          if (k < y2 && k > y1)
            {
              k = ship->x_coord;
              if (k < x2 && k > x1)
                {
                  current_player->add_score (100);
#ifndef SOUNDISOFF
                  audio->play_sound (handler_audio::HIT_SHIP);
#endif
                  k = (ball->type + 1) * 4;
                  ship->strength -= k;
                  if (ship->strength < 1)
                    {
                      ship->destroy (ball);
                    }
                  ball->direction = nouve;
                }
            }
        }
    }
}

/**
 * Collision of balls with the guardians in the guardians levels
 */
void
controller_balls::collisions_with_guardians ()
{
  controller_guardians *guards = controller_guardians::get_instance ();
  controller_capsules *capsules = controller_capsules::get_instance ();
  controller_moneys *moneys = controller_moneys::get_instance ();

  /* number of balls from 1 to n */
  Uint32 numof_balls = max_of_sprites;
  sprite_ball **balls = sprites_list;
  /* number of guardians from 1 to 2 */
  Uint32 numof_guards = guards->get_max_of_sprites ();
  sprite_guardian **guards_list = guards->get_sprites_list ();
  for (Uint32 j = 0; j < numof_guards; j++)
    {
      sprite_guardian *guardian = guards_list[j];
      if (!guardian->is_enabled || guardian->energy_level <= 0)
        {
          continue;
        }
      sprite_ball *ball_hit = NULL;
      Sint32 grdx1 = guardian->x_coord;
      Sint32 grdx2 = grdx1 + guardian->gard_colx2;
      grdx1 += guardian->gard_colx1;
      Sint32 grdy1 = guardian->y_coord;
      Sint32 grdy2 = grdy1 + guardian->gard_coly2;
      grdy1 += guardian->gard_coly1;
      for (Uint32 i = 0; i < numof_balls; i++)
        {
          sprite_ball *ball = balls[i];
          if (!ball->is_enabled)
            {
              continue;
            }
          Sint32 x = ball->x_coord;
          if (x <= grdx2)
            {
              Sint32 y = ball->y_coord;
              if (y <= grdy2)
                {
                  x += ball->collision_width;
                  if (x > grdx1)
                    {
                      y += ball->collision_width;
                      if (y > grdy1)
                        {
                          x = ((random_counter + i) & 0xF) << 2;
#ifndef SOUNDISOFF
                          audio->play_sound (handler_audio::HIT_GUARDIAN);
#endif
                          ball->direction = x;
                          guardian->recently_touched = 5;
                          guardian->energy_level -= ball->strength1;
                          if (guardian->energy_level <= 0)
                            {
                              /* guardian is dead */
                              guardian->energy_level = 0;
                              /* make exploding guardian! */
                              guardian->explode_delay_counter = 500;
                            }
                          ball_hit = ball;
                        }
                    }
                }
            }
        }
      if (NULL != ball_hit)
        {

          moneys->send_money_from_guardian (ball_hit);
          capsules->check_if_send_capsule (ball_hit);
        }
    }
}

/**
 * Return the first enable ball
 * @return a pointer to ball sprite
 */
sprite_ball *
controller_balls::get_first_ball ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          return ball;
        }
    }
  return sprites_list[0];
}

/**
 * Enable control of the balls with left mouse button into bricks levels
 */
void
controller_balls::enable_balls_control ()
{
  balls_are_controlled = true;
}

/**
 * Add 2 ball into the ejectors in the bricks levels
 */
void
controller_balls::extra_balls ()
{
  add_balls (2);
}

/**
 * Add n ball(s) into the ejector(s) in the bricks levels
 * @param nball from 1 to n
 */
void
controller_balls::add_balls (Uint32 nball)
{
  if (nball < 1)
    {
      nball = max_of_sprites;
    }
  sprite_ball **balls = sprites_list;
  Uint32 count = 0;
  Uint32 ejector_id = random_counter & 3;
  Uint32 delay = 1;
  for (Uint32 i = 0; i < max_of_sprites && count < nball; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          continue;
        }
      ball->enbale_on_ejector (ejector_id++, delay);
      count++;
      num_of_sprites++;
      delay += 2;
    }
}

/** 
 * Add 3 balls starting from the first enable ball
 */
void
controller_balls::multi_balls ()
{
  sprite_ball *model = get_first_ball ();
  /* direction of the current ball */
  Uint32 j = model->direction;
  Uint32 i = 0;
  Uint32 count = 0;
  sprite_ball **balls = sprites_list;
  while (i < max_of_sprites && count < 3)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          j += 8;
          j &= 60;
          ball->duplicate_from (model, j);
          num_of_sprites++;
          count++;
        }
      i++;
    }
}

/** 
 * Transform all the enable balls into balls power 1
 */
void
controller_balls::set_power_1 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_power_1 ();
        }
    }
}

/** 
 * Transform all the enable balls into balls power 2
 */
void
controller_balls::set_power_2 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_power_2 ();
        }
    }
}

/** 
 * Transform all the enable balls into balls size 2 (7*7 or 14*14)
 */
void
controller_balls::set_size_2 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_size_2 ();
        }
    }
}

/**
 * Transform all the enable balls into balls size 3 (10*10 or 20*20)
 */
void
controller_balls::set_size_3 ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_size_3 ();
        }
    }
}

/**
 * Increase the speed of the balls to the maximum
 */
void
controller_balls::set_maximum_speed ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->is_enabled)
        {
          ball->set_maximum_speed ();
        }
    }
}

/**
 * Check if the player can use the tilt
 */
void
controller_balls::check_tilt_availability ()
{
  bool is_tilted = false;
  sprite_ball **balls = sprites_list;
  Sint32 delay = tilt_delay;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled || ball->sticky_paddle_num > 0)
        {
          continue;
        }
      if (ball->tilt_delay_counter == delay)
        {
          is_tilted = true;
        }
      ball->tilt_delay_counter++;
    }
  if (is_tilted)
    {
      if (current_phase == BRICKS_LEVEL)
        {
          head_animation *head_anim = head_animation::get_instance ();
          head_anim->start_yawn ();
        }
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::TILT_ALARM);
#endif
    }
}

/**
 * Handle the control of the balls with the left mouse button
 */
void
controller_balls::controll_balls ()
{
  if (!balls_are_controlled or ! keyboard->is_right_button ())
    {
      return;
    }
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (!ball->is_enabled)
        {
          continue;
        }
      Sint32 dball = ball->direction;
      if (dball < 64)
        {
          dball = dball + 4;
          dball = dball & 60;
          ball->direction = dball;
        }
    }
}

/**
 * Check if there remains at least a ball glue
 * @return True if remains at least a ball glue
 */
bool
controller_balls::is_sticky_balls_remains ()
{
  sprite_ball **balls = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ball *ball = *(balls++);
      if (ball->sticky_paddle_num > 0)
        {
          return 1;
        }
    }
  return 0;
}

/*
 * directions of the ball after a rebound on a brick
 */
Sint32 controller_balls::rb0[16] =
{
  64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};
/* right rebound */
Sint32 controller_balls::rb1[16] =
{
  32, 28, 24, 20, 20, 24, 24, 28, 32, 36, 40, 40, 44, 44, 40, 36
};
Sint32 controller_balls::rb2[16] =
{
  48, 36, 40, 44, 32, 44, 24, 28, 32, 36, 40, 44, 48, 48, 44, 40
};
/* top rebond */
Sint32 controller_balls::rb3[16] = {
  60, 60, 56, 52, 48, 44, 40, 36, 36, 40, 40, 44, 48, 52, 56, 56
};
Sint32 controller_balls::rb4[16] =
{
  0, 4, 8, 0, 0, 52, 56, 60, 48, 52, 56, 44, 48, 52, 56, 60
};
/* left rebond */
Sint32 controller_balls::rb5[16] =
{
  0, 4, 8, 8, 12, 12, 8, 4, 0, 60, 56, 52, 52, 36, 56, 60
};
Sint32 controller_balls::rb6[16] = {
  0, 4, 8, 12, 16, 20, 24, 12, 16, 12, 8, 4, 0, 4, 8, 60
};
/* bottom rebond */
Sint32 controller_balls::rb7[16] =
{
  4, 8, 12, 12, 16, 20, 20, 24, 28, 28, 24, 20, 16, 12, 8, 4
};
Sint32 controller_balls::rb8[16] =
{
  16, 20, 24, 12, 16, 20, 24, 28, 32, 36, 40, 28, 32, 20, 24, 28
};
Sint32 *
  controller_balls::brick_jump[15] =
  { rb1, rb3, rb2, rb5, rb1, rb4, rb3, rb7, rb8, rb2, rb1, rb6, rb7, rb5,
  rb1
};
