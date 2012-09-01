/** 
 * @file controller_indicators.cc 
 * @brief Controller of money amount, reverse penalty   
 * @created 2002-11-28 
 * @date 2007-10-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_indicators.cc,v 1.10 2007-10-29 13:18:53 gurumeditation Exp $
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
#include "../include/controller_indicators.h"

/**
 * Create the indicators controller
 */
controller_indicators::controller_indicators ()
{
  littleInit ();
  max_of_sprites = 8;
  sprites_have_shades = false;
  y_position = YCOORD_INDICATORS * resolution;
  sprite_type_id = sprite_object::FONT_MONEY;
  money_posy = y_position;
  life_sprite = NULL;
  reverse_sprite = NULL;
  money_sprite = NULL;
}

/**
 * Release the indicators controller
 */
controller_indicators::~controller_indicators ()
{
  if (NULL != life_sprite)
    {
      delete life_sprite;
      life_sprite = NULL;
    }
  if (NULL != reverse_sprite)
    {
      delete reverse_sprite;
      reverse_sprite = NULL;
    }
  if (NULL != money_sprite)
    {
      delete money_sprite;
      money_sprite = NULL;
    }
  release_sprites_list ();
}

void
controller_indicators::create_indicators_sprites ()
{

  create_sprites_list ();

  /*
   * money sprite indicator
   */
  controller_moneys *moneys = controller_moneys::get_instance ();
  sprite_money *money = moneys->get_first_sprite ();
  money_sprite = new sprite_money ();
  money->duplicate_to (money_sprite);
  money_sprite->set_shadow (false);
  sprites->add (money_sprite);
  init_money ();
  
  /*
   * reverse sprite is only enable in the bricks levels
   */
  controller_capsules *capsules = controller_capsules::get_instance ();
  sprite_capsule *caps = capsules->get_first_sprite ();
  if (current_phase == BRICKS_LEVEL)
    {
      reverse_sprite = new sprite_capsule ();
      caps->duplicate_to (reverse_sprite);
      reverse_sprite->set_shadow (false);
      sprites->add (reverse_sprite);

      Sint32 x = 215 * resolution;
      reverse_sprite->enable_indicator_capsule (sprite_capsule::INVERSE_CONTROL);
      reverse_sprite->set_coordinates (x, money_posy);
      reverse_sprite->set_frame_delay (5);
      x += reverse_sprite->get_sprite_width ();
      sprite_object **liste = sprites_list + 6;
      for (Uint32 i = 0; i < 2; i++)
        {
          sprite_object *x_bob = *(liste++);
          x_bob->set_coordinates (x, money_posy);
          x += 8 * resolution;
        }
    }
  /*
   * life sprite is only enable in the guards levels
   */
  if (current_phase == GUARDS_LEVEL)
    {
      life_sprite = new sprite_capsule ();
      caps->duplicate_to (life_sprite);
      sprites->add (life_sprite);
      life_sprite->set_shadow (false);
      Sint32 x = 264 * resolution;
      life_sprite->enable_indicator_capsule (sprite_capsule::EXTRA_LIFE);
      life_sprite->set_coordinates (x, money_posy);
      life_sprite->set_frame_delay (5);
      x += life_sprite->get_sprite_width ();
      sprite_object **liste = sprites_list + 6;
      for (Uint32 i = 0; i < 2; i++)
        {
          sprite_object *x_bob = *(liste++);
          x_bob->set_coordinates (x, money_posy);
          x += 8 * resolution;
          x_bob->enable ();
        }
    }
}

/**
 * initialize credit of money sprites
 */
void
controller_indicators::init_money ()
{
  Uint32 x = 24 * resolution;
  money_sprite->set_coordinates (x, money_posy);
  money_sprite->set_frame_delay (5);
  money_sprite->enable ();
  x += money_sprite->get_sprite_width ();

  /* chars sprites */
  sprite_object **liste = sprites_list;
  for (Sint32 i = 0; i < 6; i++)
    {
      sprite_object *x_bob = *(liste++);
      x_bob->set_coordinates (x, money_posy);
      x_bob->enable ();
      x += 8 * resolution;
    }
}

/**
 * Display amount of money and reverse penalty
 */
void
controller_indicators::display_money_and_reverse ()
{
  display_money_amount ();
  controller_paddles* paddles = controller_paddles::get_instance ();

  /* display reverse penalty if enable */
  sprite_object **chars = sprites_list + 6;
  Uint32 counter = paddles->get_reverse_counter ();
  if (counter > 0)
    {
      counter--;
      paddles->set_reverse_counter (counter);
      counter = counter / 2;
      Uint32 digits = 10;
      while (digits > 0)
        {
          Uint32 i = 0;
          while (counter >= digits)
            {
              counter -= digits;
              i++;
            }
          digits /= 10;
          sprite_object *character = *(chars++);
          character->set_image (i);
          character->enable ();
        }
      reverse_sprite->enable ();
      reverse_sprite->play_animation_loop ();
    }
  else
    {
      for (Sint32 i = 0; i < 2; i++)
        {
          sprite_object *character = *(chars++);
          character->disable ();
        }
      reverse_sprite->disable ();
    }
}

/**
 * Display amount of money and number of lifes 
 */
void
controller_indicators::display_money_and_lifes ()
{
  display_money_amount ();
  Uint32 lifes = current_player->number_of_lifes;
  Uint32 digits = 10;
  sprite_object **chars = sprites_list + 6;
  while (digits > 0)
    {
      Sint32 i = 0;
      while (lifes >= digits)
        {
          lifes -= digits;
          i++;
        }
      digits /= 10;
      sprite_object *character = *(chars++);
      character->set_image (i);
    }
  life_sprite->play_animation_loop ();
}

/**
 * Display amount of money
 */
void
controller_indicators::display_money_amount ()
{
  Uint32 amount = current_player->amount_of_money;
  Uint32 digits = 100000;
  sprite_object **chars = sprites_list;
  while (digits > 0)
    {
      Uint32 i = 0;
      while (amount >= digits)
        {
          amount -= digits;
          i++;
        }
      digits /= 10;
      sprite_object *character = *(chars++);
      character->set_y_coord (money_posy);
      character->set_image (i);
    }
  money_sprite->set_y_coord (money_posy);
  money_sprite->play_animation_loop ();
  if (money_posy < y_position)
    {
      money_posy += resolution;
    }
}

/**
 * Increase the amount of money
 * @param value money amount, 10, 20, 30, ...
 */
void
controller_indicators::increase_money_amount (Uint32 value)
{
  money_posy = y_position - 5 * resolution;
  current_player->amount_of_money += value;
  if (current_player->area_number >= 3)
    {
      current_player->amount_of_money += value;
    }
  if (current_player->area_number >= 5)
    {
      current_player->amount_of_money += value;
    }
}
