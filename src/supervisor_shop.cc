/**
 * @file supervisor_shop.cc
 * @brief Shop supervisor
 * @date 2012-08-19
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.55 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_shop.cc,v 1.55 2012-08-19 19:14:25 gurumeditation Exp $
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
#include "../include/supervisor_shop.h"
#include "../include/handler_resources.h"
#include "../include/supervisor_main_menu.h"
#include "../include/sprite_display_menu.h"

/**
 * Create the shop supervisor
 */
supervisor_shop::supervisor_shop ()
{
  initialize ();
  menu_events = new handler_menu_events();
  tiles_ground = tiles_background::get_instance ();
  mouse_pointer = new sprite_mouse_pointer ();
  led_indicator = new sprite_object ();
  power_up_capsules = controller_capsules::get_instance ();
  display_text = new display_text_bitmap ();
  popup_menu = new handler_popup_menu ();
  current_selected_option = 0;
  some_infos_index = INFO_PADDLES;
  is_already_view_info = false;
  current_price = 0;
  num_of_bought_capsules = 0;
  led_indicator_xcoord = 0;
  led_indicator_ycoord = 0;
  is_caspule_drag = false;
  blink_capsule = (sprite_capsule *) NULL;

  options_frame_xmin = 6 * resolution;
  options_frame_xmax1 = 283 * resolution;
  options_frame_xmax2 = 51 * resolution;
  options_frame_ymax1 = 173 * resolution;
  options_frame_ymax2 = 218 * resolution;

  selected_capsule_index = -1;
  is_drawn_select_cursor = false;
  capsules_frame_ymin = 3 * resolution;
  capsules_frame_ymax = 171 * resolution + capsules_frame_ymin;
  select_cursor_xcoord = 292 * resolution;
  select_cursor_ycoord = 0;
  select_cursor_width = 21 * resolution;
  select_cursor_height = 9 * resolution;
  select_cursor_color = 0;
  select_cursor_sin_index = 0;
  /* last key pressed */
  previous_key_code_down = 0;
  /* if cheat_code_input == cheat_code then is_enabled_cheat_mode = 1 */
  cheat_code_input = 0;
  cheat_code = SDLK_e << 24 | SDLK_t << 16 | SDLK_b << 8 | SDLK_RETURN;
  box_texts = NULL;
}

/**
 * Release the shop supervisor
 */
supervisor_shop::~supervisor_shop ()
{
  delete popup_menu;
  delete display_text;
  delete power_up_capsules;
  if (NULL != led_indicator)
    {
      delete led_indicator;
      led_indicator = (sprite_object *) NULL;
    }
  if (NULL != box_texts)
    {
      delete[](char *) box_texts;
      box_texts = NULL;
    }
  delete mouse_pointer;
  delete tiles_ground;
  delete menu_events;
  release ();
}

/**
 * Initialize the shop supervisor
 */
void
supervisor_shop::first_init ()
{

  box_texts =
    resources->load_texts (handler_resources::TEXTS_SHOP, 0,
                           BOX_LENGTH_STRING, 3);
  //for (Uint32 i = 0; i < 36; i++)
  //  {
  //    printf ("%02d): %s \n", i, box_texts[i]);
  //  }


  Uint32 area_num = current_player->get_area_number ();
  Uint32 level_num = current_player->get_level_number ();
#ifndef SOUNDISOFF
  audio->play_level_music (area_num, level_num);
  audio->play_shop_music (area_num);
#endif
  sprites->reset ();

  /* copy name player into menu text */
  display_text->print_to_string (current_player->get_name (),
                                 box_texts[TEXT_WELCOME]);
  //char * dest;
  //dest = current_player->get_name ();
  display_text->print_int_to_string (current_player->get_num_of_lifes (),
                                     2, box_texts[TEXT_LIVES_LEFT]);
  display_text->print_int_to_string (MAX_OF_CAPSULES_BOUGHT, 2,
                                     box_texts[TEXT_CANNOT_BUY_MORE]);

  /* copy area code */
  if (area_num > 1)
    {
      char * code = (char *)
                    supervisor_main_menu::get_area_code (area_num, difficulty_level);
      display_text->print_to_string (code,
                                     box_texts[TEXT_AREA_CODE],
                                     supervisor_main_menu::AREA_CODE_LENGTH);
    }


  /*
   dest = &info_text1[6 * BOX_LENGTH_STRING];
   for (Uint32 i = 0; i < (BOX_LENGTH_STRING * 2); i++)
     {
       dest[i] = source[i];
     }


   source = &sprite_display_menu::difficulte[(difficulty_level - 1) * 4];
   dest = &info_text1[8 * BOX_LENGTH_STRING + 16];
   for (Sint32 i = 0; i < 4; i++)
     {
       dest[i] = source[i];
     }
     */

  resources->load_sprites_bitmap ();

  /* initialize the led indicator */
  if (resolution == 1)
    {
      led_indicator->create_sprite (sprite_object::SHOP_LED_INDICATOR_320,
                                    sprites_bitmap, false);
    }
  else
    {
      led_indicator->create_sprite (sprite_object::SHOP_LED_INDICATOR_640,
                                    sprites_bitmap, false);
    }
  sprites->add (led_indicator);
  led_indicator->enable ();

  /* initialize the power-up capsules  */
  power_up_capsules->create_shop_sprites_list ();
  current_player->clear_shopping_cart ();
  Sint32 * tp = temporary_shopping_cart;
  for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
    {
      *(tp++) = 0;
    }
  sprite_capsule **capsules = power_up_capsules->get_sprites_list ();
  drag_sprite = capsules[(MAX_OF_CAPSULES_BOUGHT + 2) - 1 - 1];

  /* initialize the mouse pointer */
  mouse_pointer->create_pointer_sprite (sprites_bitmap);

  /* intialize the "escape menu" */
  popup_menu->first_init (sprites_bitmap);
  resources->release_sprites_bitmap ();
  display_text->initialize ();

  /* load bitmap background of the shop */
  bitmap_data *
  bmp = new bitmap_data ();
  bmp->load (handler_resources::BITMAP_SHOP);
  background_screen->blit_surface (bmp, 0, 0, 0, 0, bmp->get_width (),
                                   bmp->get_height ());
  delete bmp;
  background_screen->blit_to_surface (game_screen);


  put_current_text (box_texts[TEXT_WELCOME]);
  if (!current_player->is_budget_prices ())
    {
      char *str = box_texts[TEXT_WELCOME] + BOX_LENGTH_STRING * 2;
      for (Uint32 i = 0; i < BOX_LENGTH_STRING; i++)
        {
          str[i] = ' ';
        }
    }

  keyboard->set_grab_input (false);
  tiles_ground->set_4_color_palette ();


  menu_events->begin(
    48 * resolution,
    44 * resolution,
    0,
    0,
    5,
    3,
    options_frame_xmin,
    0
  );

}

/**
 * The main loop of the shop
 */
Uint32
supervisor_shop::main_loop ()
{
  display->wait_frame ();

  /* copy the background offscreen to the game offscreen */
  background_screen->blit_to_surface (game_screen, 290 * resolution,
                                      3 * resolution, 26 * resolution,
                                      180 * resolution);


  /* display the 3 lines of text  */
  display_box_text ();
  /* display current price and credit */
  display_text->draw (game_screen, 263 * resolution, 227 * resolution,
                      current_price, 6);
  display_text->draw (game_screen, 263 * resolution, 183 * resolution,
                      current_player->get_money_amount (), 6);

  display->lock_surfaces ();
  next_phase = 0;
  sprites->clear ();

  if (!popup_menu->is_enable ())
    {
      set_select_cursor_coordinates ();
      Sint32 x = mouse_pointer->get_x_coord ();
      Sint32 y = mouse_pointer->get_y_coord ();
      if (!is_caspule_drag)
        {
          Sint32 x2, y2;
          bool is_left_up = keyboard->is_left_button_up (&x2, &y2);
          is_left_up |= menu_events->check();
          if (is_left_up)
            {
              Sint32 option = get_option_over_mouse_cursor (x, y);
              if (option == current_selected_option)
                {
                  Sint32 option_id = available_options_id[option];
                  purchase_option_or_capsule (option_id);
                }
            }
          else
            {
              if (!keyboard->is_left_button ())
                {
                  current_selected_option =
                    get_option_over_mouse_cursor (x, y);
                  current_price =
                    get_price_and_update_led (current_selected_option);
                }
            }

        }
      else
        {
          current_price = get_price_and_update_led (-1);
        }
      capsule_drag_and_drop ();
    }

  mouse_pointer->move ();
  if (is_enabled_cheat_mode)
    {
      power_up_capsules->play_animation_in_shop (2);
    }
  else
    {
      power_up_capsules->play_animation_in_shop (1);
    }

  /* display the cursor of the bonus selected in the list on the right */
  draw_select_cursor ();
  sprites->draw ();
  display_capsules_bought ();
  Sint32 popup_event = popup_menu->run ();

  /* copy whole game surface into screen surface */
  display->unlock_surfaces ();
  display->window_update ();

  /* escape key to quit the game! */
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TECNOBALLZ) ||
      popup_event == handler_popup_menu::QUIT_TECNOBALLZ)
    {
      next_phase = LEAVE_TECNOBALLZ;
    }
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TO_MAIN_MENU) ||
      popup_event == handler_popup_menu::QUIT_TO_MAIN_MENU)
    {
      next_phase = MAIN_MENU;
    }

  check_if_enable_cheat ();
  return next_phase;
}

/**
 * Display list bonus capsule(s) bought in the shop,
 * on the right of the screen
 */
void
supervisor_shop::display_capsules_bought ()
{
  Sint32 *cart = current_player->get_shopping_cart ();
  sprite_capsule **capsules = power_up_capsules->get_sprites_list ();
  Sint32 pos_y = 4 * resolution;
  for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
    {
      sprite_capsule *capsule = *(capsules++);
      capsule->set_coordinates (294 * resolution, pos_y);
      pos_y = pos_y + 9 * resolution;
      Sint32 id = *(cart++);
      capsule->set_in_shop (id);
    }
}

/**
 * Return selected bonus, and calculate position of LED indicator
 * @input x X-coordinate of the mouse pointer
 * @input y Y-coordinate of the mouse pointer
 * @return index of the selected bonus from 0 to 24
 *         or otherwise -1 if no bonus is selected
 * */
Sint32
supervisor_shop::get_option_over_mouse_cursor (Sint32 x, Sint32 y)
{
  if (x < options_frame_xmin || x > options_frame_xmax1 || y > options_frame_ymax2 ||
      (x > options_frame_xmax2 && y > options_frame_ymax1))
    {
      return -1;
    }
  else
    {
      x = (x - (6 * resolution)) / (48 * resolution);
      y = (y / (44 * resolution));
      if (y > 4)
        {
          x = 0;
        }
      Sint32 i = x + 6 * y;
      led_indicator_xcoord = (x * (48 * resolution)) + (17 * resolution);
      if (resolution == 1)
        {
          led_indicator_ycoord = (y * (44 * resolution)) + (36 * resolution);
        }
      else
        {
          led_indicator_ycoord = (y * (44 * resolution)) + (35 * resolution);
        }
      return i;
    }
}

/**
 * Update LED indicator state, and return the price of
 * the current option
 * @param index Index on the selected option from 0 to 24,
 *              otherwise -1 if not option selected
 * @return The price of the selected option
*/
Uint32
supervisor_shop::get_price_and_update_led (Sint32 index)
{
  if (index < 0)
    {
      led_indicator->disable ();
      power_up_capsules->set_overview_capsule (0);
      return 0;
    }

  /* set capsule overview */
  power_up_capsules->set_overview_capsule (available_options_id[index]);

  /* set LED indicator */
  led_indicator->enable ();
  led_indicator->set_coordinates (led_indicator_xcoord, led_indicator_ycoord);

  Sint32 price;
  /* info already seen at least once? */
  if (index == 10 && is_already_view_info)
    {
      price = 0;
    }
  else if (current_player->is_budget_prices ())
    {
      price = 1;
    }
  else
    {
      price = options_prices[index];
    }
  return price;
}

/**
 * Bought a bonus capsule or an option
 * @param option_id Option identifier
 */
void
supervisor_shop::purchase_option_or_capsule (Uint32 option_id)
{
  if (option_id != sprite_capsule::SOME_INFOS)
    {
      some_infos_index = INFO_PADDLES;
    }

  switch (option_id)
    {
    case sprite_capsule::EXPAND_PADDLE:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::FIRE_POWER_1:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::FIRE_POWER_2:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::EXTRA_BALLS:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::MULTI_BALLS:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::POWER_BALL_1:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::POWER_BALL_2:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::EXTRA_LIFE:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::BOTTOM_WALL:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::BALL_SIZE_2:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::BALL_SIZE_3:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::ROBOT_PADDLE:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::BALLS_CONTROL:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::GLUE:
      purchase_bonus_capsule (option_id);
      break;

    case sprite_capsule::SOME_INFOS:
      display_info ();
      break;

      /* rebuild the wall */
    case sprite_capsule::REBUILD_THE_WALL:
      if (current_player->get_area_number () < 5)
        {
          put_current_text (box_texts[TEXT_ONLY_FOR_AREA_5]);
        }
      else
        {
          if (!current_player->is_rebuild_walls ()
              && decrease_money_amount ())
            {
              current_player->set_rebuild_walls (true);
              display_sales_confirmation (current_selected_option);
            }
        }
      break;

      /* less bricks option */
    case sprite_capsule::LESS_BRICKS:
      if (current_player->get_less_bricks () <= 0
          && decrease_money_amount ())
        {
          current_player->set_less_bricks (10);
          display_sales_confirmation (current_selected_option);
        }
      break;

      /* exit from the shop */
    case sprite_capsule::LEAVE_SHOP:
      current_player->set_budget_prices (false);
      current_player =
        handler_players::get_next_player (current_player, &next_phase);
      break;

      /* buy a right, up or left paddle */
    default:
    {
      if (option_id >= sprite_capsule::ENABLE_RIGHT_PADDLE
          && option_id <= sprite_capsule::ENABLE_LEFT_PADDLE)
        {
          Sint32 i = (option_id - sprite_capsule::ENABLE_RIGHT_PADDLE) / 2 + 2;
          if (current_player->get_paddle_alive_counter (i) <= 0
              && decrease_money_amount ())
            {
              current_player->set_paddle_alive_counter (i, 3);
              display_sales_confirmation (current_selected_option);
            }
        }
    }
    break;
    }
}

/**
 * Display info message
 */
void
supervisor_shop::display_info ()
{
  if (!is_already_view_info)
    {
      if (!decrease_money_amount ())
        {
          return;
        }
    }
  is_already_view_info = true;

  switch (some_infos_index)
    {
    case INFO_PADDLES:
    {
      Uint32 area_num = current_player->get_area_number ();
      Uint32 index = TEXT_PADDLE_RIGHT;
      Uint32 line = 0;
      char *dest = box_texts[TEXT_PADDLES];
      char *src;
      for (Uint32 i = controller_paddles::RIGHT_PADDLE;
           i <= controller_paddles::LEFT_PADDLE; i++)
        {
          src = box_texts[index];
          if (current_player->get_paddle_alive_counter (i) == 0)
            {
              src += BOX_LENGTH_STRING;
              if ((area_num >= 2 && i == controller_paddles::TOP_PADDLE) ||
                  (area_num >= 3 && i == controller_paddles::RIGHT_PADDLE)
                  || (area_num >= 4
                      && i == controller_paddles::LEFT_PADDLE))
                {
                  src += BOX_LENGTH_STRING;
                }
            }
          index++;
          line++;
          strncpy (dest, src, BOX_LENGTH_STRING);
          dest += BOX_LENGTH_STRING;
        }
    }
    put_current_text (box_texts[TEXT_PADDLES]);
    break;

    case INFO_LIVES:
      put_current_text (box_texts[TEXT_LIVES_LEFT]);
      break;

    case INFO_AREA_CODE:
      if (current_player->get_area_number () > 1)
        {
          put_current_text (box_texts[TEXT_AREA_CODE]);
        }
      else
        {
          put_current_text (box_texts[TEXT_NO_AREA_CODE]);
        }
      break;

    case INFO_END:
    default:
      if (is_enabled_cheat_mode)
        {
          put_current_text (box_texts[TEXT_ENABLED_CHEAT_MODE]);
        }
      else if (birth_flag)
        {
          put_current_text (box_texts[TEXT_WAITING_CHEAT_MODE]);
        }
      else
        {
          put_current_text (box_texts[TEXT_HOPING_HELP]);
        }
      put_current_text (box_texts[TEXT_HOPING_HELP]);
      break;
    }
  if (++some_infos_index > INFO_END)
    {
      some_infos_index = INFO_PADDLES;
    }

}

/**
 * Check if a purchase is possible, so decrement the money amount
 * @return true if the money amount could be descreased
 */
bool supervisor_shop::decrease_money_amount ()
{
  if (current_player->decrease_money_amount (current_price))
    {
      return true;
    }
  else
    {
      put_current_text (box_texts[TEXT_NOT_ENOUGH_MONEY]);
      return false;
    }
}

/**
 * Purchase a bonus capsule if possible
 * @param capsule_id Capsule identifier
 */
void
supervisor_shop::purchase_bonus_capsule (Sint32 capsule_id)
{
  /* maximum number of capsules reached */
  if (num_of_bought_capsules >= MAX_OF_CAPSULES_BOUGHT)
    {
      put_current_text (box_texts[TEXT_CANNOT_BUY_MORE]);
      return;
    }

  /* purchase is possible? */
  if (!decrease_money_amount ())
    {
      return;
    }

  Sint32 *cart = current_player->get_shopping_cart ();
  cart[num_of_bought_capsules] = capsule_id;
  sprite_capsule **caspules = power_up_capsules->get_sprites_list ();
  sprite_capsule *capsule = caspules[num_of_bought_capsules++];
  capsule->set_in_shop (capsule_id);
  display_sales_confirmation (current_selected_option);
  current_player->set_numof_items_in_shopping_cart (num_of_bought_capsules);
}

/**
 * Display the sales confirmation message
 * @param option_index Option index
 */
void
supervisor_shop::display_sales_confirmation (Sint32 option_index)
{
  char *text = box_texts[(int)led_index_to_text_index[option_index]];
  put_current_text (text);
}

/**
 * Update the pointers of three lines currently displayed
 * @param str Pointer to a string containing three lines
 */
void
supervisor_shop::put_current_text (char *str)
{
  current_text_displayed[0] = str;
  str += BOX_LENGTH_STRING;
  current_text_displayed[1] = str;
  str += BOX_LENGTH_STRING;
  current_text_displayed[2] = str;
}

/**
 * Display the three lines of text in the box in bottom
 */
void
supervisor_shop::display_box_text ()
{
  Uint32 height = display_text->get_char_height ();
  Uint32 x_pos = 60 * resolution;
  Uint32 y_pos = 180 * resolution;
  Uint32 yspac = height + resolution;
  game_screen->clear (0, x_pos, y_pos, 22 * 8 * resolution, 3 * yspac);
  display_text->draw (game_screen, x_pos, y_pos, current_text_displayed[0],
                      BOX_LENGTH_STRING);
  display_text->draw (game_screen, x_pos, y_pos + yspac, current_text_displayed[1],
                      BOX_LENGTH_STRING);
  display_text->draw (game_screen, x_pos, y_pos + yspac * 2, current_text_displayed[2],
                      BOX_LENGTH_STRING);
}

/**
 * Drag and drop the bonus capsule
 */
void
supervisor_shop::capsule_drag_and_drop ()
{
  /* index on 'options_prices' list, if -1 no drag objet  */
  if (!is_caspule_drag)
    /* grab an object with the mouse */
    {
      drag_sprite->disable ();
      if (keyboard->is_left_button ()
          && selected_capsule_index >= 0)
        {
          dragged_capsule_pt = current_player->get_shopping_cart ()
                               + selected_capsule_index;
          sprite_capsule **capsules =
            power_up_capsules->get_sprites_list ();
          blink_capsule = *(capsules + selected_capsule_index);
          is_caspule_drag = true;
          drag_sprite->clone_from_capsule (blink_capsule);
          blink_capsule->is_enabled = true;
        }
      return;
    }

  /* drag the bonus capsule */
  if (keyboard->is_left_button ())
    {
      drag_sprite->enable ();
      drag_sprite->set_coordinates (mouse_pointer->get_x_coord (),
                                    mouse_pointer->get_y_coord ());
      if (blink_capsule->is_enabled)
        {
          blink_capsule->is_enabled = false;
        }
      else
        {
          blink_capsule->is_enabled = true;
        }
      return;
    }


  /*
   * drop a bonus capsule
   */
  drag_sprite->disable ();
  blink_capsule->is_enabled = true;
  Sint32 i = selected_capsule_index;

  /* change position of a bonus capsule in the capsules list */
  if (i >= 0)
    {
      if (i >= (Sint32)num_of_bought_capsules)
        {
          i = num_of_bought_capsules - 1;
        }
      Sint32 *player_cart = current_player->get_shopping_cart ();
      Sint32 *source = dragged_capsule_pt;
      Sint32 *dest = player_cart + i;
      Sint32 *cart = temporary_shopping_cart;
      Sint32 drop_id = *source;
      if (source != dest)
        {
          Uint32 i = 0;
          do
            {
              if (player_cart == source)
                {
                  player_cart++;
                  i++;
                }
              else
                {
                  if (player_cart == dest)
                    {
                      if (source <= dest)
                        {
                          if (i++ < MAX_OF_CAPSULES_BOUGHT)
                            {
                              *(cart++) = *(player_cart++);
                            }
                        }
                      if (i++ < MAX_OF_CAPSULES_BOUGHT)
                        {
                          *(cart++) = drop_id;
                        }
                    }
                  if (i++ < MAX_OF_CAPSULES_BOUGHT)
                    {
                      *(cart++) = *(player_cart++);
                    }
                }
            }
          while (i < MAX_OF_CAPSULES_BOUGHT);
          cart = temporary_shopping_cart;
          player_cart = current_player->get_shopping_cart ();
          for (Uint32 i = 0; i < MAX_OF_CAPSULES_BOUGHT; i++)
            {
              *(player_cart++) = *(cart++);
            }
        }
    }

  /* resell a bonus capsule to the shop */
  else
    {
      Sint32 *source = dragged_capsule_pt;
      Sint32 *dest = source;
      Sint32 drop_id = *source;
      *(source++) = 0;
      while (*source >= 0)
        {
          *(dest++) = *source;
          *(source++) = 0;
        }
      num_of_bought_capsules--;
      current_player->set_numof_items_in_shopping_cart (num_of_bought_capsules);
      Sint32 price = 0;
      if (current_player->is_budget_prices ())
        {
          /* the player collected a chance capsule containing
           * a budget prices bonus in the previous bricks level.
           * All the options are thus for the price of 1 in
           * the current shop */
          price = 1;
        }
      else
        {
          i = 0;
          while (available_options_id[i] != sprite_capsule::LEAVE_SHOP)
            {
              if (available_options_id[i] == drop_id)
                {
                  price = options_prices[i];
                  break;
                }
              i++;
            }
        }
      current_player->increase_money_amount (price);
    }
  is_caspule_drag = false;
}

/**
 * Calculate coordinates the
 * Calculate the coordinates of the cursor which selects a capsule in the list
 * of the bought capsules
 *
 */
void
supervisor_shop::set_select_cursor_coordinates ()
{
  Sint32 y = mouse_pointer->get_y_coord () - capsules_frame_ymin;
  Sint32 offset = (y / select_cursor_height);
  y = offset * select_cursor_height + capsules_frame_ymin;
  if (y < capsules_frame_ymin)
    {
      y = capsules_frame_ymin;
    }
  if (y > capsules_frame_ymax)
    {
      y = capsules_frame_ymax;
    }
  /* cursor y coordinate */
  select_cursor_ycoord = y;

  /* don't display cursor by default */
  is_drawn_select_cursor = false;
  selected_capsule_index = -1;
  /* at least one bonus capsule? */
  if (num_of_bought_capsules < 1)
    {
      return;
    }

  /*
   * calculate maximum y coordinate
   */
  Sint32 ymax = num_of_bought_capsules * select_cursor_height
                + capsules_frame_ymin;
  /* if an object is drag, then adds a supplementary place */
  if (is_caspule_drag)
    {
      ymax = ymax + select_cursor_height;
    }

  Sint32 x = mouse_pointer->get_x_coord ();
  y = mouse_pointer->get_y_coord ();
  if (x >= select_cursor_xcoord && y < ymax)
    {
      is_drawn_select_cursor = true;
      selected_capsule_index = offset;
    }
}

/**
 * Draw the cursor of the bonus selected in the list on the right
 */
void
supervisor_shop::draw_select_cursor ()
{
  if (select_cursor_color++ > 32)
    {
      select_cursor_color = 0;
    }

  /* calculation the height and the width of the cursor */
  Sint32 a = select_cursor_sin_index + 5;
  a &= 511;
  select_cursor_sin_index = a;
  if (!is_drawn_select_cursor > 0)
    {
      return;
    }
  Sint16 *s = table_cosL + a;
  a = *s;
  a *= (select_cursor_width / 5);
  a >>= 7;
  a = a + select_cursor_width / 5;
  Sint32 b = select_cursor_sin_index;
  s = table_cosL + b;
  b = *s;
  b *= (select_cursor_height / 5);
  b >>= 7;
  b = b + select_cursor_height / 5;
  Sint32 w = select_cursor_width - a;
  Sint32 h = select_cursor_height - b;

  /* calculation of the coordinates of the cursor */
  Sint32 x = select_cursor_xcoord + (select_cursor_width - w) / 2;
  Sint32 y = select_cursor_ycoord + (select_cursor_height - h) / 2;

  /* draw the cursor */
  Uint32 delay = 0;
  Uint32 color = select_cursor_color;
  char *dest = game_screen->get_pixel_data (x, y);
  Uint32 next_line = game_screen->get_row_size ();
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < w; j++)
        {
          unsigned char pixel = color_cycling[color];
          dest[j] = pixel;
        }
      if (++delay == 5)
        {
          delay = 0;
          if (color++ > 32)
            {
              color = 0;
            }
        }
      dest += next_line;
    }
}

/**
 * Check if the player enables the cheating mode
 */
void
supervisor_shop::check_if_enable_cheat ()
{
  mouse_pointer->set_frame_period (3);
  if (is_enabled_cheat_mode)
    {
      /* the cheating mode is already enabled */
      return;
    }
  if (!birth_flag)
    {
      return;
    }
  if (!mouse_pointer->get_x_coord () && !mouse_pointer->get_y_coord ())
    {
      mouse_pointer->set_frame_period (20);
      Uint32 code = keyboard->get_key_down_code ();
      if (previous_key_code_down != code && code > 0)
        {
          previous_key_code_down = code;
          cheat_code_input = cheat_code_input << 8 | code;
        }
    }
  if (cheat_code_input == cheat_code)
    {
      is_enabled_cheat_mode = true;
    }
  else
    {
      is_enabled_cheat_mode = false;
    }
}

/**
 * Identifiers of the available options and bonus capsules
 */
Sint32
supervisor_shop::available_options_id[] =
{
  sprite_capsule::EXPAND_PADDLE,
  sprite_capsule::FIRE_POWER_1,
  sprite_capsule::FIRE_POWER_2,
  sprite_capsule::REBUILD_THE_WALL,
  sprite_capsule::EXTRA_BALLS,
  sprite_capsule::MULTI_BALLS,
  sprite_capsule::POWER_BALL_1, sprite_capsule::POWER_BALL_2,
  sprite_capsule::LESS_BRICKS, sprite_capsule::EXTRA_LIFE,
  sprite_capsule::SOME_INFOS, sprite_capsule::BOTTOM_WALL,
  sprite_capsule::ENABLE_LEFT_PADDLE,
  sprite_capsule::ENABLE_TOP_PADDLE,
  sprite_capsule::ENABLE_RIGHT_PADDLE,
  sprite_capsule::BALL_SIZE_2,
  sprite_capsule::BALL_SIZE_3,
  sprite_capsule::ROBOT_PADDLE,
  sprite_capsule::BALLS_CONTROL,
  sprite_capsule::GLUE,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP,
  sprite_capsule::LEAVE_SHOP
};

/**
 * Prices of all the available options in the shop
 */
Uint32
supervisor_shop::options_prices[] =
{
  60, 75, 150, 350, 25, 50,
  250, 500, 400, 450, 10, 75,
  100, 100, 100, 60, 75, 100,
  60, 150, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

/**
 * Indexes of the texts used for sales confirmation messages
 */
unsigned char
supervisor_shop::led_index_to_text_index[] =
{
  /* S+/F1/F2/RW/B2/B3 */
  0, 1, 2, 3, 4, 5,
  /* P1/P2/LB/L+/??/WA */
  6, 7, 8, 9, 10, 11,
  /* BL/BU/BR/S2/S3/RB */
  12, 13, 14, 15, 16, 17,
  /* CT/GL/XX/XX/XX/XX */
  18, 19, 20, 20, 20, 20,
  /* XX/XX/XX/XX/XX/XX */
  20, 20, 20, 20, 20, 20,
};

const unsigned char
supervisor_shop::color_cycling[] =
{
  239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};
