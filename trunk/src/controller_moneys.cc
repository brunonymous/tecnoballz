/** 
 * @file controller_moneys.cc 
 * @brief Moneys controller 
 * @date 2007-10-17
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

#include "../include/controller_moneys.h"

/**
 * Create the moneys controller 
 */
controller_moneys::controller_moneys ()
{
  littleInit ();
  max_of_sprites = 6;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::MONEY;
}

/**
 * Release the moneys controller 
 */
controller_moneys::~controller_moneys ()
{
  release_sprites_list ();
}

/** 
 * Initialize the moneys sprites in the bricks levels
 * @param delay time delay before sending a new money capsule 
 * @param score
 * @param money
 */
void
controller_moneys::initialize (Uint32 delay, right_panel_score * score,
                               controller_indicators * money)
{
  send_delay = delay;
  ptbarreScr = score;
  ptPrntmney = money;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->init_members ();
    }
}

/**
 * Send a money capsule from a brick 
 * @param briPT a pointer to the brick which touched by a ball 
 */
void
controller_moneys::send_money_from_brick (brick_redraw * briPT)
{
  if (++delay_count <= send_delay)
    {
      return;
    }
  delay_count = 0;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->enable_if_available (briPT))
        {
          return;
        }
    }
}

/**
 * Send a money capsule from a destroyed flying enemy ship
 * @param ball a pointer to the ball sprite which destroyed the enemy ship
 */
void
controller_moneys::send_money (sprite_ball * ball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->enable_if_available (ball))
        {
          return;
        }
    }
}

/**
 * Send a money capsule from a destroyed flying enemy ship
 * @param blast a pointer to the projectile sprite which
 *        destroyed the enemy ship
 */
void
controller_moneys::send_money (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->enable_if_available (blast))
        {
          return;
        }
    }
}

/** 
 * Move money capsules and check collision with the paddles
 * in bricks levels 
 */
void
controller_moneys::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->play_animation_loop ();
      Uint32 amount = money->move ();
      if (amount > 0)
        {
          current_player->add_score (20);
          ptPrntmney->increase_money_amount (amount);
        }
    }
}

/** 
 * Initialize the moneys sprites in the guardians levels
 * @param delay time delay before sending a new money capsule 
 */
void
controller_moneys::initialize (Uint32 delay, controller_indicators * money)
{
  send_delay = delay;
  ptPrntmney = money;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->init_members ();
    }
}

/**
 * Send a money capsule from a guardian
 * @param ball a pointer to the ball sprite which touched the guardian
 */
void
controller_moneys::send_money_from_guardian (sprite_ball * ball)
{
  if (++delay_count <= send_delay)
    {
      return;
    }
  delay_count = 0;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      if (money->enable_if_available (ball))
        {
          return;
        }
    }
}

/** 
 * Move money capsules and check collision with the paddle
 * in guardians levels 
 */
void
controller_moneys::move_bottom ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_money *money = sprites_list[i];
      money->play_animation_loop ();
      Uint32 amount  = money->move_bottom ();
      if (amount > 0)
        {
          ptPrntmney->increase_money_amount (amount);
          current_player->add_score (20);
        }
    }
}
