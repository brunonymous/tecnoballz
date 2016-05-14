/** 
 * @file supervisor_guards_level.cc 
 * @brief Guardians level supervisor 
 * @created 2003-01-09
 * @date 2007-11-18
 * @copyright 1991-2016 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/* 
 * copyright (c) 1991-2016 TLK Games all rights reserved
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
#include "../include/supervisor_guards_level.h"
#include "../include/handler_resources.h"

/**
 * Create the guards level supervisor
 */
supervisor_guards_level::supervisor_guards_level ()
{
  initialize ();
  tiles_map = new tilesmap_scrolling ();
  guards = controller_guardians::get_instance ();
  paddles = controller_paddles::get_instance ();
  font_game = controller_font_game::get_instance ();
  explosions = controller_explosions::get_instance ();
  bullets = controller_bullets::get_instance ();
  money_capsules = controller_moneys::get_instance ();
  power_up_capsules = controller_capsules::get_instance ();
  balls = controller_balls::get_instance ();
  viewfinders_paddles = controller_viewfinders::get_instance ();
  player_indicators = controller_indicators::get_instance ();
  gigablitz = controller_gigablitz::get_instance ();
  game_over = controller_game_over::get_instance ();
  metallic_spheres = new controller_spheres ();
  popup_menu = new handler_popup_menu ();
  sprite_playername = NULL;
}

/**
 * Release the guards level supervisor
 */
supervisor_guards_level::~supervisor_guards_level ()
{
  if (NULL != sprite_playername)
    {
      delete sprite_playername;
      sprite_playername = NULL;
    }
  delete popup_menu;
  delete metallic_spheres;
  delete game_over;
  delete gigablitz;
  delete player_indicators;
  delete viewfinders_paddles;
  delete balls;
  delete power_up_capsules;
  delete money_capsules;
  delete bullets;
  delete explosions;
  delete font_game;
  delete paddles;
  delete guards;
  delete tiles_map;
  release ();
}

/** 
 * Initializations of the guardian's level
 */
void
supervisor_guards_level::first_init ()
{
  game_screen->clear ();
#ifndef SOUNDISOFF
  audio->play_music (handler_audio::MUSICGUARD);
  audio->enable_sound ();
#endif
  sprites->reset ();
  next_phase = 0;
  gameover_counter = 0;
  count_next = 0;
  is_victory_initialized = false;
  is_victory = false;
  area_number = current_player->get_area_number ();
  level_number = current_player->get_level_number ();
  Sint32 grdP = current_player->getGuardPt ();
  if (is_verbose)
  {
    std::cout << ">supervisor_guards_level::first_init() " <<
      "area_number: " << area_number << "; level_number: " <<
      level_number << "; grdP: " << grdP << std::endl;
  }

  /* gigablitz sprites are contained separately and in its own bitmap */
  gigablitz->create_gigablitz_sprites ();

  /* 
   * sprites who are contained into the large bitmap
   */
  resources->load_sprites_bitmap ();
  bullets->create_sprites_list ();
  guards->create_guardians_list (grdP);
  paddles->create_paddles_sprites ();
  balls->create_sprites_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list (6);
  player_indicators->create_indicators_sprites ();
  explosions->create_explosions_list ();
  viewfinders_paddles->create_sprites_list ();
  /* mobile characters at the end of the level */
  font_game->create_sprites_list ();
  game_over->create_sprites_list ();
  metallic_spheres->create_sprites_list ();

  /* initialize escape menu */
  popup_menu->first_init (sprites_bitmap);
  resources->release_sprites_bitmap ();

 /* initialize controller of the big letters animated composing the word
  * "game over"  */
  game_over->first_init (32 * resolution);
  init_level ();
  
  /* initialize background vertical scrolling */
  tiles_map->initialize ();
  display->lock_surfaces ();

  /*
   * initialize the balls controller *
   */
  balls->init (
    /* time delay before ball leaves paddle */
    level_desc->ball_release_time,
    0,
    /* time delay before ball accelerates */
    50 * 99,
   /* time delay before titl available */
   level_desc->tilt_delay,
   /* ball speed 3 or 4 */
   level_desc->starting_speed);
  /* in the guardians levels, the balls are always forced to power 2 */
  balls->set_power_2 ();

  money_capsules->initialize (3 + difficulty_level, player_indicators);

  /* initialize le capsules controller */
  power_up_capsules->initialize (
                          /* delay of appearance of a penalty capsule */
                          level_desc->capsules_frequency * difficulty_level,
                          /* list of penalties capsules */ 
                          level_desc->capsules_list);

  /* initialize mobile characters at the end of the level */
  font_game->initialize (level_number, 32 * resolution);
  viewfinders_paddles->initialize ();
  display->unlock_surfaces ();
  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);
  sprite_display_scores *pOver = game_over->get_sprite_high_score ();
  sprite_playername = pOver->string2bob (current_player->get_name ());
  sprites->add (sprite_playername);
  sprite_playername->enable ();
  sprite_playername->
    set_coordinates ((display->get_width () -
                      sprite_playername->get_sprite_width ()) / 2, resolution);
  sprite_playername->set_draw_method (sprite_object::COPY_FROM_BITMAP);
  if (is_verbose)
  {
    std::cout << ">supervisor_guards_level::first_init() End!" << std::endl;
  }
}

/**
 * The main loop of the guardians phase
 */
Uint32
supervisor_guards_level::main_loop ()
{
  Sint32 popup_event = -1;
  
  /*
   * gameover: the player has no more lives
   */
  if (current_player->get_num_of_lifes () <= 0)
    {
      if (gameover_counter > 0)
        {
#ifndef SOUNDISOFF
          audio->disable_sound ();
#endif
          paddles->disable_all_paddles ();
          power_up_capsules->disable_sprites ();
          money_capsules->disable_sprites ();
          guards->disable_sprites ();
          explosions->disable_sprites ();
          font_game->disable_sprites ();
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (is_victory && !is_victory_initialized)
            {
              metallic_spheres->initialize ();
              tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_CONGRATULATIONS,
                                     tilesmap_scrolling::MAP_CONGRATULATIONS);
              scroll_speed = 1;
              scroll_start_delay = 300;
              is_victory_initialized = true;
            }
        }
      gameover_counter++;
      display->wait_frame ();

      if (scroll_start_delay > 0)
        {
          scroll_start_delay--;
          tiles_map->scroll (0);
        }
      else
        {
          tiles_map->scroll (scroll_speed);
        }
      
      display->lock_surfaces ();
      viewfinders_paddles->run ();
      player_indicators->display_money_and_lifes ();
      font_game->move ();
      sprites->draw ();
      if (gameover_counter >= 1)
        {
          game_over->run (is_victory);
          if (is_victory)
            {
              /* animate the metal spheres */
              metallic_spheres->run ();
            }
        }
      display->unlock_surfaces ();
      display->window_update ();
      if (keyboard->wait_key () && gameover_counter > 150)
        {
          current_player = handler_players::get_next_player (current_player,
                                                    &next_phase,
                                                    guards->
                                                    get_max_of_sprites () +
                                                    1);
        }
    }
  else
    {
      /*
       * game running !
       */
      display->wait_frame ();
      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          run_scroll ();
          tiles_map->scroll (scroll_speed);
          display->lock_surfaces ();
          paddles->move_paddle ();
          paddles->check_if_release_ball ();
          balls->run_in_guardians_level ();
          viewfinders_paddles->run ();
          /* moving guards, and fire bullets and gigablitz */
          guards->run ();
          bullets->move ();
          bullets->check_paddle_collisions ();
          money_capsules->move_bottom ();
          power_up_capsules->move_in_guardians_level ();
          font_game->move ();
          player_indicators->display_money_and_lifes ();
          /* move Gigablitz and check collision with paddle */
          gigablitz->run_in_guardians_level ();
          explosions->play_animation ();
          bullets->play_animation_loop ();

        }
      else
        {
          bullets->play_animation_loop ();
          tiles_map->scroll (0);
          display->lock_surfaces ();
        }

      /* display all sprites in the game offscreen */
      sprites->draw ();
      popup_event = popup_menu->run ();
      display->unlock_surfaces ();
      display->window_update ();
    }


  /* next level */
  if (guards->is_guardians_destroyed () && !is_victory)
    {
      if (count_next > 0)
        {
          count_next++;
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          if (count_next > 500 || keyboard->wait_key())
            {
              is_victory = current_player->zlastlevel ();
              if (is_victory)
                {
                  current_player->remove_all_lifes ();
                  count_next = 0;
                }
              else
                {
                  current_player = handler_players::get_next_player
                    (current_player,
                     &next_phase, guards->get_max_of_sprites () + 1);
                }
            }
        }
      else
        {
          font_game->enable ();
          gigablitz->disable_sprites ();
          balls->disable_sprites ();
          bullets->disable_sprites ();
          count_next = 1;
        }
    }

  /* cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 */
  cheat_keys ();

  /* escape key to quit the game! */
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TECNOBALLZ) ||
      popup_event == handler_popup_menu::QUIT_TECNOBALLZ)
    {
      next_phase = LEAVE_TECNOBALLZ;
    }
  if (keyboard->command_is_pressed (handler_keyboard::CAUSE_GAME_OVER) ||
      popup_event == handler_popup_menu::CAUSE_GAME_OVER)
    {
      current_player->remove_all_lifes ();
    }
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TO_MAIN_MENU) ||
      popup_event == handler_popup_menu::QUIT_TO_MAIN_MENU)
    {
      next_phase = MAIN_MENU;
    }



  return next_phase;
}

/**
 * Determine the pointer on the guardians difficulty level 
 */
void
supervisor_guards_level::init_level ()
{
  level_desc = ptLev_data->get_guardians_levels_params (area_number, level_number);
  scroll_type = level_desc->scroll_id;
  scroll_start_delay = level_desc->scroll_delay;
  scroll_speed = 0;
}

/**
 * Handle speed et behavior of the background scrolling
 */
void
supervisor_guards_level::run_scroll ()
{
  if (scroll_start_delay > 0)
    {
      scroll_start_delay--;
      scroll_speed = 0;
      return;
    }
  sprite_bullet *bullet = bullets->get_last_bullet ();
  sprite_ball *ball = balls->get_first_ball ();
  scroll_speed = guards->get_scrolling_speed (scroll_type, scroll_speed, ball, bullet);
}

/** 
 * Cheat mode: F2=destroyed the two guards / F3=guard 1 / F4=guard 2 
 */
void
supervisor_guards_level::cheat_keys ()
{
  if (!is_enabled_cheat_mode)
    {
      return;
    }
  if (!keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_LSHIFT) ||
      !keyboard->key_is_pressed (SDLK_RCTRL) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
      keyboard->key_is_pressed (SDLK_LALT))
    {
      return;
    }

  if (keyboard->key_is_pressed (SDLK_F2))
    {
      guards->killguards ();
    }
  if (keyboard->key_is_pressed (SDLK_F3))
    {
      guards->killguards (1);
    }
  if (keyboard->key_is_pressed (SDLK_F4))
    {
      guards->killguards (2);
    }
}

