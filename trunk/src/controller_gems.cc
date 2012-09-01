/** 
 * @file controller_gems.cc 
 * @brief The gems controller 
 * @created 2004-04-12 
 * @date 2007-10-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_gems.cc,v 1.10 2007-10-29 13:18:53 gurumeditation Exp $
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

#include "../include/controller_gems.h"

/**
 * Create the gems controller
 */
controller_gems::controller_gems ()
{
  littleInit ();
  max_of_sprites = MAX_OF_GEMS;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::GEM;
}

/**
 * Release the gems controller
 */
controller_gems::~controller_gems ()
{
  release_sprites_list ();
}

/**
 * Initialize the gemstones sprites
 */
void
controller_gems::initialize ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (current_player->is_collected_gem (i))
        {
          gem->collect (i);
        }
    }
}

/**
 * Send a gem stone
 * @param ball a pointer to the ball sprite which destroyed the enemy ship
 */
void
controller_gems::send (sprite_ball * ball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (gem->enable_if_available (ball))
        {
          return;
        }
    }
}

/**
 * Send a gem stone
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 */
void
controller_gems::send (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      if (gem->enable_if_available (blast))
        {
          return;
        }
    }
}

/**
 * Move gems and collision with the paddles
 */
void
controller_gems::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_gem *gem = sprites_list[i];
      Sint32 gem_id = gem->move ();
      if (gem_id < 0)
        {
          /* this gem is disabled or was not collected by a paddle  */
          continue;
        }
      if (current_player->is_collected_gem (gem_id))
        {
          /* this gem was already collected */
          continue;
        }
      current_player->add_score (500);
      gem->collect (gem_id);
      if (!current_player->are_collected_all_gems (gem_id))
        {
          /* the six gems were not collected yet */
          continue;
        }
      /*
       * the six gems were collected!
       */
      for (Uint32 k = 0; k < max_of_sprites; k++)
        {
          sprite_gem *gem = sprites_list[k];
          gem->enable_blink ();
        }
      current_player->add_score (2500);
      controller_indicators* indicators = controller_indicators::get_instance ();
      indicators->increase_money_amount (500);
      current_player->add_life (3);
      /* jump to the next level */
      right_panel_score* panel = right_panel_score::get_instance ();
      panel->set_bricks_counter (0);
      /* enable right paddle */
      current_player->set_paddle_alive_counter (controller_paddles::RIGHT_PADDLE, 3);
      /* enable top paddle */
      current_player->set_paddle_alive_counter (controller_paddles::TOP_PADDLE, 3);
      /* enable left paddle */
      current_player->set_paddle_alive_counter (controller_paddles::LEFT_PADDLE, 3);
      sprite_paddle *paddle;
      controller_paddles* paddles = controller_paddles::get_instance ();
      paddle = paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle->enable ();
      paddle = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle->enable ();
      paddle = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle->enable ();
      return;
    }
}
