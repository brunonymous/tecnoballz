/** 
 * @file controller_capsules.cc 
 * @brief Capsules controller 
 * @date 2007-10-31
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.34 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_capsules.cc,v 1.34 2007-10-31 07:35:29 gurumeditation Exp $
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
#include "../include/controller_capsules.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_players.h"
#include "../include/sprite_projectile.h"
#include "../include/handler_players.h"

/**
 * Create the capsules controller
 */
controller_capsules::controller_capsules ()
{
  littleInit ();
  frame_delay = 0;
  frame_period = 5;
  frame_index = 0;
  capsule_delay_counter = 0;
  capsule_delay = 0;
  bricks_breaked_count = 0;
  num_of_caspules_bought = 0;
  capsules_released_count = 0;
  bonus_delay = 0;
  shopping_cart_index = 0;
  max_of_sprites = 0;
  sprites_have_shades = true;
  /* additional table to drawing pixel by pixel is required
   * for color cycling of the chance capsule */
  is_draw_pixel_by_pixel = true;
  sprite_type_id = sprite_object::CAPSULES;
  shopping_cart = NULL;
}

/**
 *  Release capsules controller
 */
controller_capsules::~controller_capsules ()
{
  release_sprites_list ();
}

/**
 * Perform some initializations
 * @param delay Time interval between two penalty/bonus capsules 
 * @param random Pointer to the list of penalties/bonuses capsules
 */
void
controller_capsules::initialize (Uint32 delay,
                                 const Uint32 * random)
{
  controller_paddles *paddles = controller_paddles::get_instance ();
  paddle_selected = paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  capsule_delay = delay;
  bonus_delay = 0;
  random_list = random;
  capsule_delay_counter = 0;
  bricks_breaked_count = 0;
  capsules_released_count = 0;
  sprite_capsule *capsule = sprites_list[0];
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      capsule = sprites_list[i];
      capsule->init_members ();
    }
  frame_period = capsule->frame_period;

  if (current_phase == BRICKS_LEVEL)
    {
      controller_bricks *bricks = controller_bricks::get_instance ();
      Uint32 numof_bricks = bricks->get_num_of_bricks ();
      shopping_cart = current_player->get_shopping_cart ();
      num_of_caspules_bought = current_player->get_numof_items_in_shopping_cart ();
      /* at least a bought bonus? */
      if (num_of_caspules_bought > 0)
        {
          bonus_delay =
            (numof_bricks - numof_bricks / 2) / num_of_caspules_bought;
        }
      else
        {
          /* no bonus bought in the shop,
           * initialize with the maximum value */
          bonus_delay = numof_bricks + 1;
        }
    }
}

/**
 * Check if send a bonus or penalty capsule from brick
 * @param brick Pointer to the brick which touched by a ball
 */
void
controller_capsules::send_capsule_from_brick (brick_redraw * brick)
{
  bricks_breaked_count++;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *caspule = sprites_list[i];
      if (caspule->is_enabled)
        {
          continue;
        }

      /*
       * Send a random penalty or bonus capsule
       */
      capsule_delay_counter++;
      if (capsule_delay_counter > capsule_delay)
        {
          capsule_delay_counter = 0;
          caspule->enable_capsule (brick, random_list[random_counter & 0x3F]);
          return;
        }

      /*
       * Send a bonus capsule bought in th shop
       */
      if (num_of_caspules_bought > capsules_released_count
          && bricks_breaked_count > bonus_delay)
        {
          bricks_breaked_count = 0;
          capsules_released_count++;
          Sint32 id = shopping_cart[shopping_cart_index];
          if (id == 0)
            {
              id = shopping_cart[0];
              shopping_cart_index = 0;
            }
          shopping_cart_index++;
          caspule->enable_capsule (brick, id);
        }
    }
  return;
}

/**
 * Send a penalty from a ship in bricks level
 * @param ball Pointer to the ball sprite which touched the ship 
 */
void
controller_capsules::send_penalty_or_bonus (sprite_ball * ball)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      if (capsule->is_enabled)
        {
          continue;
        }
      capsule->enable_capsule (ball, random_list[random_counter & 0x3F]);
      return;
    }
}

/**
 * Send a penalty from a ship in bricks level
 * @param ball Pointer to the projectile sprite which touched the ship 
 */
void
controller_capsules::send_penalty_or_bonus (sprite_projectile * blast)
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      if (capsule->is_enabled)
        {
          continue;
        }
      capsule->enable_capsule (blast, random_list[random_counter & 0x3F]);
      return;
    }
}

/**
 * Check if send a bonus capsule in the guards level 
 * @param ball Pointer to a ball sprite which touched the guardian
 */
void
controller_capsules::check_if_send_capsule (sprite_ball * ball)
{
  capsule_delay_counter++;
  if (capsule_delay_counter <= capsule_delay)
    {
      return;
    }
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      if (capsule->is_enabled)
        {
          continue;
        }
      capsule_delay_counter = 0;
      capsule->enable_guardian_capsule (ball,
                                        random_list[random_counter & 0x3F]);
      return;
    }
}

/**
 * Initialize the coordinattes of the bonus capsules in the shop 
 */
void
controller_capsules::create_shop_sprites_list ()
{
  create_sprites_list (supervisor_shop::MAX_OF_CAPSULES_BOUGHT + 2, false);
  Uint32 maxof = supervisor_shop::MAX_OF_CAPSULES_BOUGHT;
  sprite_capsule **caspules = sprites_list;
  sprite_capsule *caspule = caspules[0];
  Uint32 h = caspule->sprite_height + 1;
  Sint32 x = SHOP_XCOORD_CAPSULE * resolution;
  Sint32 y = SHOP_YCOORD_CAPSULE * resolution;
  for (Uint32 i = 0; i < maxof; i++)
    {
      caspule = *(caspules++);
      caspule->set_coordinates (x, y);
      caspule->disable ();
      y += h;
    }

  /* initialize caspule sprite object for the drag and drop */
  caspule = *(caspules++);
  caspule->set_coordinates (0, 0);
  caspule->disable ();

  /* initialize caspule sprite object that display indicator */
  caspule = *caspules;
  caspule->set_coordinates (SHIP_XCOORD_INDICATOR * resolution,
                            SHIP_YCOORD_INDICATOR * resolution);
  caspule->disable ();
  overview_capsule = caspule;
}

/**
 * Set the overview capsule in the shop
 * @id id capsule identifier
 */
void
controller_capsules::set_overview_capsule (Uint32 id)
{
  overview_capsule->set_in_shop (id);
}

/**
 *  Move and collision with paddles in bricks levels
 */
void
controller_capsules::move_in_bricks_level ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      capsule->play_animation_loop ();
      sprite_paddle *paddle = capsule->move ();
      if (NULL != paddle)
        {
          Uint32 id = capsule->get_id ();
          run_in_bricks_level (paddle, id);
        }
    }
}

/**
 * Move and collision with paddle in guardians levels
 */
void
controller_capsules::move_in_guardians_level ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *capsule = sprites_list[i];
      capsule->play_animation_loop ();
      sprite_paddle *paddle = capsule->move ();
      if (NULL != paddle)
        {
          Uint32 id = capsule->get_id ();
          run_in_guards_level (paddle, id);
        }
    }
}

/**
 * Play capsules animation in shop
 * @param speed speed of the animation 1 or 2
 */
void
controller_capsules::play_animation_in_shop (Uint32 speed)
{
  if (--frame_delay <= 0)
    {
      frame_delay = frame_period / speed;
      if (++frame_index >= sprite_capsule::NUM_OF_IMAGES)
        {
          frame_index = 0;
        }
    }
  Uint32 offset = frame_index;
  sprite_capsule **caspules = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_capsule *caspule = *(caspules++);
      caspule->set_image (caspule->frame_index_min + offset);
    }
}

/**
 * Enable a bonus or a penalty with the keyboard
 */
void
controller_capsules::check_cheat_keys ()
{
  printf("controller_capsules::check_cheat_keys 1\n");
  if (!is_enabled_cheat_mode)
    {
      return;
    }
  printf("controller_capsules::check_cheat_keys 2\n");
  if (keyboard->key_is_pressed (SDLK_LSHIFT) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
#ifndef UNDER_DEVELOPMENT
      !keyboard->key_is_pressed (SDLK_RSHIFT) ||
      !keyboard->key_is_pressed (SDLK_RCTRL) ||
#else
      keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_RCTRL) ||
#endif
      keyboard->key_is_pressed (SDLK_LALT))
    {
      return;
    }
  printf("controller_capsules::check_cheat_keys 3\n");
  Sint16 *keys = cheat_keys;
  /* read SDL key code */
  while (Sint16 key = *(keys++))
    {
      Sint16 capsule_id = *(keys++);
      if (keyboard->key_is_pressed (key))
        {
          *(keys++) = 1;
        }
      else
        {
          if (*keys == 1)
            {
              run_in_bricks_level (paddle_selected, capsule_id);
            }
          *(keys++) = 0;
        }
    }
}

/**
 * Enable an option, bonus or penalty, which can possibly come from
 * a capsule collected by a paddle in the bricks levels
 * @param paddle Paddle which collected the caspule 
 * @param capsule_id Option identifier
 */
void
controller_capsules::run_in_bricks_level (sprite_paddle * paddle,
                                          Uint32 capsule_id)
{
  paddle_selected = paddle;
  controller_paddles *paddles = controller_paddles::get_instance ();
  short_info_messages *messages = short_info_messages::get_instance ();
  controller_balls *balls = controller_balls::get_instance ();
  sprite_wall *wall = sprite_wall::get_instance ();
  if (capsule_id == sprite_capsule::CHANCE)
    {
      capsule_id = randomlist[random_counter & 127];
    }

  switch (capsule_id)
    {
    case sprite_capsule::GLUE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_TRANSFORMATION);
#endif
      messages->send_message_request (short_info_messages::GLUE_OPTION);
      paddle->set_glue ();
      break;

    case sprite_capsule::NEXT_LEVEL:
      {
        messages->send_message_request (short_info_messages::NEXT_LEVEL);
        right_panel_score *panel = right_panel_score::get_instance ();
        panel->set_bricks_counter (0);
      }
      break;

    case sprite_capsule::FIRE_POWER_1:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_TRANSFORMATION);
#endif
      messages->send_message_request (short_info_messages::SIMPLE_FIRE);
      paddle->set_fire_1 ();
      break;

    case sprite_capsule::FIRE_POWER_2:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_TRANSFORMATION);
#endif
      messages->send_message_request (short_info_messages::COOL_FIRE);
      paddle->set_fire_2 ();
      break;

    case sprite_capsule::SHRINK_PADDLE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_TRANSFORMATION);
#endif
      messages->send_message_request (short_info_messages::SMALL_PADDLE);
      paddles->shrink_paddles ();
      break;

    case sprite_capsule::EXPAND_PADDLE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_TRANSFORMATION);
#endif
      messages->send_message_request (short_info_messages::BIG_PADDLE);
      paddles->expand_paddles ();
      break;

    case sprite_capsule::LOSE_A_LIFE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::LOST_LIFE);
#endif
      messages->send_message_request (short_info_messages::LOST_FILE);
      current_player->remove_life (1);
      break;

    case sprite_capsule::EXTRA_LIFE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::EXTRA_LIFE);
#endif
      messages->send_message_request (short_info_messages::WIN_LIFE);
      current_player->add_life (1);
      break;

    case sprite_capsule::EXTRA_BALLS:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      messages->send_message_request (short_info_messages::EXTRA_BALLS);
      balls->extra_balls ();
      break;

    case sprite_capsule::MULTI_BALLS:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      messages->send_message_request (short_info_messages::MULTI_BALLS);
      balls->multi_balls ();
      break;

    case sprite_capsule::POWER_BALL_1:
      messages->send_message_request (short_info_messages::POWERBALLS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_power_1 ();
      break;

    case sprite_capsule::POWER_BALL_2:
      messages->send_message_request (short_info_messages::MEGA_POWERBALLS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_power_2 ();
      break;

    case sprite_capsule::INVERSE_CONTROL:
      messages->send_message_request (short_info_messages::INVERSE_COMMANDS);
      paddles->set_reverse_counter (50 * 4);
      break;

      /* increase the speed of the balls to the maximum 
       * this option is only available as a part of chance capsule */
    case sprite_capsule::MAXIMUM_SPEED_OF_BALLS:
      messages->
        send_message_request (short_info_messages::MAXIMUM_ACCELERATION);
      balls->set_maximum_speed ();
      break;

      /* this option is never used */
    case sprite_capsule::ENABLE_BOTTOM_PADDLE:
      paddle_selected =
        paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
      break;

      /* the option which enables the right paddle is only
       * available as a part of chance capsule */
    case sprite_capsule::ENABLE_RIGHT_PADDLE:
      messages->send_message_request (short_info_messages::RIGHT_PADDLE);
      paddle_selected =
        paddles->get_paddle (controller_paddles::RIGHT_PADDLE);
      paddle_selected->enable ();
      break;

      /* the option which enables the top paddle is only
       * available as a part of chance capsule */
    case sprite_capsule::ENABLE_TOP_PADDLE:
      messages->send_message_request (short_info_messages::TOP_PADDLE);
      paddle_selected = paddles->get_paddle (controller_paddles::TOP_PADDLE);
      paddle_selected->enable ();
      break;

      /* the option which enables the left paddle is only
       * available as a part of chance capsule */
    case sprite_capsule::ENABLE_LEFT_PADDLE:
      messages->send_message_request (short_info_messages::LEFT_PADDLE);
      paddle_selected = paddles->get_paddle (controller_paddles::LEFT_PADDLE);
      paddle_selected->enable ();
      break;

    case sprite_capsule::BALL_SIZE_2:
      messages->send_message_request (short_info_messages::BIG_BALLS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_size_2 ();
      break;

    case sprite_capsule::BALL_SIZE_3:
      messages->send_message_request (short_info_messages::HUGE_BALLS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_size_3 ();
      break;

    case sprite_capsule::CHANCE:
      break;

    case sprite_capsule::ENABLE_HUGELY_OPTIONS:
      messages->send_message_request (short_info_messages::MAXIMUM_OPTIONS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      paddles->set_maximum_paddles_size ();
      balls->add_balls ();
      balls->set_power_2 ();
      balls->set_size_3 ();

      paddle->set_glue ();
      paddle->set_fire_2 ();

      paddle_selected = paddles->get_paddle (2);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();

      paddle_selected = paddles->get_paddle (3);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();

      paddle_selected = paddles->get_paddle (4);
      paddle_selected->enable ();
      paddle_selected->set_glue ();
      paddle_selected->set_fire_2 ();
      break;

      /* options prices will be to 1 in the next shop
       * this bonus is only available as a part of chance capsule */
    case sprite_capsule::SET_THE_PRICES_TO_1:
      messages->send_message_request (short_info_messages::BUDGET_PRICES);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      current_player->set_budget_prices (true);
      break;

      /* enable the bottom wall */
    case sprite_capsule::BOTTOM_WALL:
      messages->send_message_request (short_info_messages::WALL_ENABLE);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      wall->enable (500);
      paddles->disable_robot ();
      break;

      /* enable the paddle robot */
    case sprite_capsule::ROBOT_PADDLE:
      messages->send_message_request (short_info_messages::ROBOT_ENABLE);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      paddles->enable_robot ();
      wall->disable ();
      break;

      /* enable the balls control */
    case sprite_capsule::BALLS_CONTROL:
      balls->enable_balls_control ();
      messages->send_message_request (short_info_messages::CONTROL_BALLS);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      break;

      /* enable the magnetic eye, no capsule exists to activate it 
       * this bonus is only available as a part of chance capsule */
    case sprite_capsule::MAGNETIC_EYE:
      {
        controller_magnetic_eyes *eyes =
          controller_magnetic_eyes::get_instance ();
        eyes->create_eye ();
      }
      break;
    }
}

/**
 * Enable the capsule option in guardians level
 * @param paddle Paddle which collected the caspule 
 * @param capsule_id Option identifier
 */
void
controller_capsules::run_in_guards_level (sprite_paddle * paddle,
                                          Uint32 capsule_id)
{
  paddle_selected = paddle;
  controller_balls *balls = controller_balls::get_instance ();
  switch (capsule_id)
    {
    case sprite_capsule::PADDLE_INVINCIBILITY:
      paddle->set_invincibility (200);
      break;

    case sprite_capsule::EXTRA_LIFE:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::EXTRA_LIFE);
#endif
      current_player->add_life (1);
      break;

    case sprite_capsule::MULTI_BALLS:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->multi_balls ();
      break;

      /* power ball 1 (ball size 2) */
    case sprite_capsule::POWER_BALL_1:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_size_2 ();
      break;

      /* power ball 2 (ball size 3) */
    case sprite_capsule::POWER_BALL_2:
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::COLLECT_CAPSULE);
#endif
      balls->set_size_3 ();
      break;
    }
}

/**
 * List of the options which can be enabled by a chance capsule
 */
const Uint16
  controller_capsules::randomlist[128] = { sprite_capsule::BOTTOM_WALL,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::BALLS_CONTROL,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::ENABLE_LEFT_PADDLE,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::LOSE_A_LIFE,
  sprite_capsule::BALLS_CONTROL,
  sprite_capsule::GLUE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::EXPAND_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::ENABLE_LEFT_PADDLE,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::NEXT_LEVEL,
  sprite_capsule::BALLS_CONTROL,
  sprite_capsule::LOSE_A_LIFE,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::ENABLE_RIGHT_PADDLE,
  sprite_capsule::ENABLE_RIGHT_PADDLE,
  sprite_capsule::ENABLE_TOP_PADDLE,
  sprite_capsule::INVERSE_CONTROL,
  sprite_capsule::EXTRA_LIFE,
  sprite_capsule::EXTRA_LIFE,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::SET_THE_PRICES_TO_1,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::ENABLE_TOP_PADDLE,
  sprite_capsule::ENABLE_LEFT_PADDLE,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::MAGNETIC_EYE,
  sprite_capsule::LOSE_A_LIFE,
  sprite_capsule::INVERSE_CONTROL,
  sprite_capsule::GLUE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::EXPAND_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::ENABLE_LEFT_PADDLE,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::NEXT_LEVEL,
  sprite_capsule::INVERSE_CONTROL,
  sprite_capsule::LOSE_A_LIFE,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::EXPAND_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::MAXIMUM_SPEED_OF_BALLS,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::ENABLE_RIGHT_PADDLE,
  sprite_capsule::ENABLE_RIGHT_PADDLE,
  sprite_capsule::ENABLE_TOP_PADDLE,
  sprite_capsule::INVERSE_CONTROL,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::GLUE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::SET_THE_PRICES_TO_1,
  sprite_capsule::SHRINK_PADDLE,
  sprite_capsule::EXPAND_PADDLE,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1,
  sprite_capsule::BOTTOM_WALL,
  sprite_capsule::BOTTOM_WALL
};

/**
 * Cheat keys to enable options in bricks levels, only under development
 */
Sint16
  controller_capsules::cheat_keys[] = {
  SDLK_F1, sprite_capsule::GLUE, 0,
  SDLK_F2, sprite_capsule::NEXT_LEVEL, 0,
  SDLK_F3, sprite_capsule::FIRE_POWER_1, 0,
  SDLK_F4, sprite_capsule::FIRE_POWER_2, 0,
  SDLK_F5, sprite_capsule::SHRINK_PADDLE, 0,
  SDLK_F6, sprite_capsule::EXPAND_PADDLE, 0,
  SDLK_F7, sprite_capsule::LOSE_A_LIFE, 0,
  SDLK_F8, sprite_capsule::EXTRA_LIFE, 0,
  SDLK_F9, sprite_capsule::EXTRA_BALLS, 0,
  SDLK_F10, sprite_capsule::MULTI_BALLS, 0,
  SDLK_F11, sprite_capsule::POWER_BALL_1, 0,
  SDLK_F12, sprite_capsule::POWER_BALL_2, 0,
  SDLK_a, sprite_capsule::INVERSE_CONTROL, 0,
  SDLK_z, sprite_capsule::MAXIMUM_SPEED_OF_BALLS, 0,
  SDLK_e, sprite_capsule::ENABLE_BOTTOM_PADDLE, 0,
  SDLK_r, sprite_capsule::ENABLE_RIGHT_PADDLE, 0,
  SDLK_t, sprite_capsule::ENABLE_TOP_PADDLE, 0,
  SDLK_y, sprite_capsule::ENABLE_LEFT_PADDLE, 0,
  SDLK_u, sprite_capsule::BALL_SIZE_2, 0,
  SDLK_i, sprite_capsule::BALL_SIZE_3, 0,
  SDLK_o, sprite_capsule::CHANCE, 0,
  SDLK_q, sprite_capsule::ENABLE_HUGELY_OPTIONS, 0,
  SDLK_s, sprite_capsule::SET_THE_PRICES_TO_1, 0,
  SDLK_d, sprite_capsule::BOTTOM_WALL, 0,
  SDLK_g, sprite_capsule::BALLS_CONTROL, 0,
  SDLK_h, sprite_capsule::MAGNETIC_EYE, 0,
  SDLK_j, sprite_capsule::MAGNETIC_EYE, 0,
  SDLK_k, sprite_capsule::ROBOT_PADDLE, 0,
  0, 0, 0
};
