/** 
 * @file sprite_money.cc 
 * @brief The money sprite 
 * @date 2007-09-25
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.15 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_money.cc,v 1.15 2007-09-26 06:02:01 gurumeditation Exp $
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
#include "../include/sprite_money.h"

/**
 * Create the money sprite
 */
sprite_money::sprite_money ()
{
  clear_sprite_members ();
  towards = 0;
  money_amount = 0;
  speed_of_moving = 0;
  paddle = (sprite_paddle *) NULL;
}

/**
 * Release the money sprite
 */
sprite_money::~sprite_money ()
{
}

/** 
 * perform some initialization of members
 */
void
sprite_money::init_members ()
{
  x_maximum = screen_width - ((64 + 16) * resolution);
  x_minimum = 3 * resolution;
  y_maximum = screen_height - 10 * resolution;
  y_minimum = 0;
  frame_period = 5;
  frame_delay = 1;

  /* initialize multiplier of the value of the capsules of money  */
  switch (current_player->get_area_number ())
    {
    default:
      money_multiplier = 1;
      break;
    case 3:
    case 4:
      money_multiplier = 2;
      break;
    case 5:
      money_multiplier = 4;
      break;
    }
}

/**
 * Initialize a new capsule of money from a bricks
 * @param brick a pointer to the brick which touched by a ball
 * @return true if the sprite was enabled, otherwise false
 */
bool sprite_money::enable_if_available (brick_redraw * brick)
{
  if (is_enabled)
    {
      return false;
    }
  init_money (brick->xcoord_collision, brick->ycoord_collision, brick->paddle);
  return true;
}

/**
 * Initialize a new capsule of money from a ship or a guardian 
 * @param ball a pointer to the ball sprite which destroyed
 *        the enemy ship or touched the guardian
 * @return true if the sprite was enabled, otherwise false
 */
bool sprite_money::enable_if_available (sprite_ball * ball)
{
  if (is_enabled)
    {
      return false;
    }
  init_money (ball->x_coord, ball->y_coord, ball->paddle_touched);
  return true;
}

/**
 * Initialize a new capsule of money from a projectile
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 * @return true if the sprite was enabled, otherwise false
 */
bool sprite_money::enable_if_available (sprite_projectile * blast)
{
  if (is_enabled)
    {
      return false;
    }
  init_money (blast->x_coord, blast->y_coord, blast->paddle);
  return true;
}

/** 
 * Initialize a new capsule of money
 * @param xcoord x coordinate of the money capsule
 * @param ycoord y coordinate of the money caspule
 * @parm pad pointer to the paddle sprite which goes the money caspule
 */
void
sprite_money::init_money (Uint32 xcoord, Uint32 ycoord, sprite_paddle * pad)
{
  is_enabled = true;
  x_coord = xcoord;
  y_coord = ycoord;
  paddle = pad;
  Uint32 value = random_counter & 0x003;
  random_counter += value;
  towards = paddle->get_paddle_number ();
  switch (value)
    {
    case 0:
      money_amount = 10 * money_multiplier;
      speed_of_moving = resolution;
      break;
    case 2:
      money_amount = 30 * money_multiplier;
      speed_of_moving = resolution * 3;
      break;
    case 1:
    default:
      money_amount = 20 * money_multiplier;
      speed_of_moving = resolution * 2;
      break;
    }
}

/**
 * Displacement and collision of the money in the bricks levels
 * @return the money amount collected
 */
Uint32 sprite_money::move ()
{
  if (!is_enabled)
    {
      return 0;
    }
  switch (towards)
    {
    case controller_paddles::BOTTOM_PADDLE:
      y_coord += speed_of_moving;
      if (y_coord < y_maximum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::COLLECT_MONEY);
#endif
              return (money_amount);
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    case controller_paddles::RIGHT_PADDLE:
      x_coord += speed_of_moving;
      if (x_coord < x_maximum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::COLLECT_MONEY);
#endif
              return (money_amount);
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    case controller_paddles::TOP_PADDLE:
      y_coord -= speed_of_moving;
      if (y_coord > y_minimum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::COLLECT_MONEY);
#endif
              return (money_amount);
            }
        }
      else
        {
          is_enabled = false;
        }
      break;

    case controller_paddles::LEFT_PADDLE:
      x_coord -= speed_of_moving;
      if (x_coord > x_minimum)
        {
          if (collision (paddle))
            {
              is_enabled = false;
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::COLLECT_MONEY);
#endif
              return (money_amount);
            }
        }
      else
        {
          is_enabled = false;
        }
      break;
    }
  return 0;
}

/**
 * Displacement and collision of the money in the guards levels
 * @return the money amount collected
 */
Uint32 sprite_money::move_bottom ()
{
  if (!is_enabled)
    {
      return 0;
    }
  y_coord += speed_of_moving;
  if (y_coord >= y_maximum)
    {
      is_enabled = false;
      return 0;
    }
  if (!collision (paddle))
    {
      return 0;
    }
  is_enabled = false;
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::COLLECT_MONEY);
#endif
  return money_amount;
}
