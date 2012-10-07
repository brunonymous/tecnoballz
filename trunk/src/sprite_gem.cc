/**
 * @file sprite_gem.cc 
 * @brief The gem sprite 
 * @created 2004-04-12 
 * @date 2012-10-07
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
#include "../include/sprite_gem.h"

/**
 * Create the gem sprite
 */
sprite_gem::sprite_gem ()
{
  clear_sprite_members ();
  towards = 0;
  speed_of_moving = 0;
  is_indicator = false;
  blink_counter = 0;
  rand_count = 0;
  gem_id = GREY_SQUARE;
  paddle = (sprite_paddle *) NULL;
}

/**
 * Release the gem sprite
 */
sprite_gem::~sprite_gem ()
{
}

/**
 * Initialize a new gem from a ball 
 * @param ball a pointer to the ball sprite which destroyed
 *        the enemy ship
 * @return true if the sprite was enabled, otherwise false
 */
bool
sprite_gem::enable_if_available (sprite_ball * ball)
{
  if (is_enabled)
    {
      return false;
    }
  init_gem (ball->x_coord, ball->y_coord, ball->paddle_touched);
  return true;
}

/**
 * Initialize a new gem from a projectile
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 * @return true if the sprite was enabled, otherwise false
 */
bool
sprite_gem::enable_if_available (sprite_projectile * blast)
{
  if (is_enabled)
    {
      return false;
    }
  init_gem (blast->x_coord, blast->y_coord, blast->paddle);
  return true;
}

/**
 * Initialize the gem
 * @param xcoord x coordinate of the gem
 * @param ycoord y coordinate of the gem
 * @param paddle pointer to a paddle sprite
 */
void
sprite_gem::init_gem (Sint32 xcoord, Sint32 ycoord, sprite_paddle * pad)
{
  is_enabled = true;
  x_coord = xcoord;
  y_coord = ycoord;
  paddle = pad;
  towards = paddle->get_paddle_number ();
  speed_of_moving = resolution;
  Uint32 h = ((random_counter >> 4) + rand_count++) & 7;
  random_counter += xcoord;
  h = gem_random[h];
  gem_id = h;
  set_image (h);
  is_indicator = false;
  sprite_has_shadow = true;
  blink_counter = 0;
  right_panel_score* panel_score = right_panel_score::get_instance ();
  init_coords_max_min (panel_score->get_width());
}

/**
 * A new gem was collected
 * @param id gem identifier 0 to 5
 */
void
sprite_gem::collect (Uint32 id)
{
  gem_id = id;
  set_image (id);
  sprite_has_shadow = false;
  is_indicator = true;
  y_coord = screen_height - sprite_height - 2 * resolution;
  x_coord = 270 * resolution + sprite_width * id;
  is_enabled = true;
  blink_counter = 0;
}

/**
 * Enable the blink
 */
void
sprite_gem::enable_blink ()
{
  if (is_enabled && is_indicator)
    {
      blink_counter = 30;
    }
}

/**
 * Move or blink gem
 * @return gem identifier collected, otherwise -1
 */
Sint32
sprite_gem::move ()
{
  if (is_indicator)
    {
      blink ();
      return -1;
    }

  if (!is_enabled)
    {
      return -1;
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

              return gem_id;
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

              return gem_id;
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

              return gem_id;
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

              return gem_id;
            }
        }
      else
        {
          is_enabled = false;
        }
      break;
    }
  return -1;
}


/**
 * Blinking the gem
 */
void
sprite_gem::blink ()
{
  if (0 == blink_counter || !is_indicator)
    {
      return;
    }
  if (blink_counter > 20)
    {
      is_enabled = false;
    }
  else
    {
      is_enabled = true;
    }
  blink_counter--;
  if (0 == blink_counter)
    {
      blink_counter = 30;
    }
}


const Uint32 sprite_gem::gem_random[8] =
  {
    GREY_SQUARE,
    GREEN_SPHERE,
    YELLOW_RING,
    BLUE_TRIANGLE,
    GOLD_RHOMBUS,
    BRONZE_PENTAGON,
    GREY_SQUARE,
    GREEN_SPHERE
  };
