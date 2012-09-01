/** 
 * @file sprite_paddle.cc 
 * @brief A paddle sprite 
 * @date 2007-10-23
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.16 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_paddle.cc,v 1.16 2007-10-29 13:18:54 gurumeditation Exp $
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
#include "../include/sprite_paddle.h"

/**
 * Create a paddle sprite object
 * @param has_projectiles true if the paddle can fire
 */
//sprite_paddle::sprite_paddle (controller_projectiles * fBump)
sprite_paddle::sprite_paddle (bool has_projectiles)
{
  clear_sprite_members ();
  length = 32 * resolution;
  is_vertical = false;
  fire_state = NOT_OWN_GUN;
  sticky_state = NOT_STICKY_PADDLE;
  enable_counter = 0;
  is_hit_ball = false;
  paddle_number = 0;
  stuck_ball = (sprite_ball *) NULL;
  if (has_projectiles)
    {
      projectiles = new controller_projectiles (); 
    }
  else
    {
      projectiles = NULL;
    }
  invincible_counter = 0;
  blink_counter = 0;
  width_mini = 0;
  width_maxi = 0;
  shift_width = 0;
  if (resolution == 1)
    {
      shift_width = 3;
    }
  else
    {
      shift_width = 4;
    }
}

/**
 * Release a paddle sprite object
 */
sprite_paddle::~sprite_paddle ()
{
  /* Paddle had projectiles */
  if (NULL != projectiles)
    {
      delete projectiles;
      projectiles = NULL;
    }
}

/**
 * Create and initializes the list of the projectiles for this paddle
 */ 
void
sprite_paddle::create_projectiles_list ()
{
  projectiles->create_projectiles_list (this);
}

/**
 * Paddle fires projectiles
 */
void
sprite_paddle::fire_projectiles ()
{
  if (fire_state != NOT_OWN_GUN)
    {
      projectiles->disponible ();
    }
}

/**
 * Move the paddle's projectiles 
 */
void
sprite_paddle::move_projectiles ()
{
  projectiles->fire ();
  projectiles->move ();
}

/**
 * Determine if the paddle must to be enabled and enable it if ok
 * @param is_team True if mode team enabled
 * @param size Paddle's width (8,16,24,32,40,48,56 or 64)
 * @param counter Counter which enable paddle if it 
 *                is greater than zero
 */
void
sprite_paddle::enable_if_ok (bool is_team, Sint32 size, Uint32 counter)
{
  length = size;
  is_enabled = false;
  enable_counter = counter;
  if (enable_counter > 0)
    {
      enable_counter--;
      is_enabled = true;
    }
  if (paddle_number == controller_paddles::BOTTOM_PADDLE
      || (paddle_number == controller_paddles::LEFT_PADDLE 
          && is_team))
    {
      enable_counter = 1;
      is_enabled = true;
    }
  select_image (size);
}

/**
 * Set the width and image of a horizontal paddle
 * @param size width of the paddle in pixels
 */
void
sprite_paddle::set_width (Uint32 size)
{
  length = size;
  collision_width = length;
  select_image (length);
  projectiles->disable_sprites ();
}

/**
 * Set the height and image of a vertical paddle
 * @param h Height of the paddle in pixels
 */
void
sprite_paddle::set_height (Uint32 h)
{
  length = h;
  collision_height = length;
  select_image (length);
  projectiles->disable_sprites ();
}

/**
 * Select the sprite image of the paddle
 * @param size Length of the paddle
 */
void
sprite_paddle::select_image (Uint32 size)
{
  Sint32 i = (size >> shift_width) - 2;
  if (fire_state != NOT_OWN_GUN)
    {
      i += 7;
    }
  if (sticky_state != NOT_STICKY_PADDLE)
    {
      i += 14;
    }
  set_image (i);
}

/**
 * Select the sprite image of the paddle
 */
void
sprite_paddle::select_image ()
{
  select_image (length);
}

/**
 * Return the paddle number
 * @return 1  1 (bottom), 2 (right), 3 (top), 4 (left) or 5 (robot)
 */
Uint32
sprite_paddle::get_paddle_number ()
{
  return paddle_number;
}

/**
 * Transform into paddle glue in the bricks levels
 */
void
sprite_paddle::set_glue ()
{
  sticky_state = FREE_STICKY_PADDLE;
  select_image ();
}

/**
 * Transform into paddle fire 1 in the bricks levels
 */ 
void
sprite_paddle::set_fire_1 ()
{
  fire_state = OWN_GUN;
  select_image ();
  projectiles->set_fire_1 ();
}

/**
 * Transform into paddle fire 2 in the bricks levels
 */
void
sprite_paddle::set_fire_2 ()
{
  fire_state = OWN_GUN;
  select_image ();
  projectiles->set_fire_2 ();
}

/**
 * Release ball if glued in the bricks levels
 */
void
sprite_paddle::release_ball ()
{
  /* is the ball glued on the paddle? */
  if (sticky_state == BUSY_STICKY_PADDLE)
    {
      /* paddle is free */
      sticky_state = FREE_STICKY_PADDLE;
    }
  sprite_ball *ball = stuck_ball;
  if (NULL != ball)
    {
      stuck_ball = (sprite_ball *) NULL;
      ball->disable_stick ();
    }
}

/*
 * Glue a ball to the paddle into the bricks level
 * @param ball pointer to sprite_ball object
 */
void
sprite_paddle::stick_ball (sprite_ball * ball)
{
  if (stuck_ball != NULL)
    {
      stuck_ball->disable_stick ();
    }
  stuck_ball = ball;
  if (sticky_state == FREE_STICKY_PADDLE)
    {
      sticky_state = BUSY_STICKY_PADDLE;
    }
}

/**
 * Return paddle's length
 * @return length of the paddle in pixels
 */
Uint32
sprite_paddle::get_length ()
{
  return length;
}

/**
 * Check if the paddle is invincible (into guards levels)
 * @return True the paddle is invincible, false otherwise
 */
bool
sprite_paddle::is_invincible ()
{
  return invincible_counter > 0 ? true : false;
}

/**
 * Set the temporary invincibility of the paddle
 * @param delay time delay of the invincibility
 */
void
sprite_paddle::set_invincibility (Sint32 delay)
{
  invincible_counter = delay;
}

/**
 * Handle invincible paddle in the guardians level 
 */
void
sprite_paddle::blink ()
{
  if (invincible_counter > 0)
    {
      invincible_counter--;
      if (blink_counter > 0)
        {
          is_enabled = false;
          blink_counter = 0;
        }
      else
        {
          is_enabled = true;
          blink_counter = 1;
        }
    }
  else
    {
      is_enabled = true;
    }
}
