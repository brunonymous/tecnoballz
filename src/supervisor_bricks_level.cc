/**
 * @file supervisor_bricks_level.cc 
 * @brief Bricks levels supervisor 
 * @date 2007-11-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.66 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_bricks_level.cc,v 1.66 2007-11-18 16:13:20 gurumeditation Exp $
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
#include "../include/supervisor_bricks_level.h"
#include "../include/handler_resources.h"

/**
 * Create the bricks level supervisor
 */
supervisor_bricks_level::supervisor_bricks_level ()
{
  initialize ();
  sides_bricks = controller_sides_bricks::get_instance ();
  tiles_ground = tiles_background::get_instance ();
  panel_score = right_panel_score::get_instance ();
  ejectors_corners = controller_ejectors::get_instance ();
  money_capsules = controller_moneys::get_instance ();
  power_up_capsules = controller_capsules::get_instance ();
  gem_stones = controller_gems::get_instance ();
  bricks = controller_bricks::get_instance ();
  head_anim = head_animation::get_instance ();
  ships = controller_ships::get_instance ();
  magnetic_eyes = controller_magnetic_eyes::get_instance ();
  bottom_wall = sprite_wall::get_instance ();
  info_messages = short_info_messages::get_instance ();
  balls = controller_balls::get_instance ();
  viewfinders_paddles = controller_viewfinders::get_instance ();
  paddles = controller_paddles::get_instance ();
  font_game = controller_font_game::get_instance ();
  gigablitz = controller_gigablitz::get_instance ();
  player_indicators = controller_indicators::get_instance ();
  game_over = controller_game_over::get_instance ();

  popup_menu = new handler_popup_menu ();

  sprite_projectile::start_list ();
  level_number = 1;
  area_number = 1;
  gameover_counter = 0;
#ifdef UNDER_DEVELOPMENT
  backgound_index = 0;
  devel_keyw = false;
  devel_keyx = false;
#endif
}

/**
 * Release the bricks level supervisor
 */
supervisor_bricks_level::~supervisor_bricks_level ()
{
  delete popup_menu;
  delete game_over;
  delete player_indicators;
  delete gigablitz;
  delete font_game;
  delete paddles;
  delete viewfinders_paddles;
  delete balls;
  delete info_messages;
  delete bottom_wall;
  delete magnetic_eyes;
  delete ships;
  delete head_anim;
  delete bricks;
  delete gem_stones;
  delete power_up_capsules;
  delete money_capsules;
  delete ejectors_corners;
  delete panel_score;
  delete tiles_ground;
  delete sides_bricks;
  release ();
}

/**
 * Initialization of a bricks level
 */
void
supervisor_bricks_level::first_init ()
{
  game_screen->clear ();
  sprites->reset ();
  area_number = current_player->get_area_number ();
  level_number = current_player->get_level_number ();
#ifndef SOUNDISOFF
  audio->play_level_music (area_number, level_number);
  audio->enable_sound ();
#endif

  count_next = 0;
  next_phase = 0;
  gameover_counter = 0;
  if (is_verbose)
    {
      std::cout << "supervisor_bricks_level::first_init() area_number:"
      << area_number << "level_number:" << level_number
      << " difficulty_level:" << difficulty_level << std::endl;
    }

  /* generation of paddles graphics shapes tables */
  paddles->create_paddles_sprites ();

  /* generation of gigablitz graphics shapes tables */
  gigablitz->create_gigablitz_sprites ();

  /* load bitmap of sprites in memory (all other sprites) */
  resources->load_sprites_bitmap ();

  /*
   * generate the data of the spites
   */
  bottom_wall->create_sprite (sprite_object::BOTTOM_WALL, sprites_bitmap, 0);
  sprites->add (bottom_wall);
  bottom_wall->set_coordinates (32 * resolution, 232 * resolution);
  paddles->initialize_robot ();
  sides_bricks->initialize ();
  ejectors_corners->create_ejectors_sprites ();

  initialize_background ();
  bricks->add_to_sprites_list();

  game_over->create_sprites_list ();
  balls->create_sprites_list ();
  ships->create_sprites_list ();
  magnetic_eyes->create_eyes_list ();
  money_capsules->create_sprites_list ();
  power_up_capsules->create_sprites_list (6);
  gem_stones->create_sprites_list ();
  font_game->create_sprites_list ();
  paddles->create_projectiles_list ();
  player_indicators->create_indicators_sprites ();
  viewfinders_paddles->create_sprites_list ();
  popup_menu->first_init (sprites_bitmap);
  resources->release_sprites_bitmap ();
  panel_score->first_init ();

  display->lock_surfaces ();

  /* initialize controller of the big letters animated composing the word
   * "game over"  */
  game_over->first_init ();
  head_anim->load_bitmap ();
  init_level ();
  /* draw ejectors and side walls */
  paddles->init_paddles (gigablitz, balls);

  /* balls initialization */
  balls->init (
    /* time before the ball leaves paddle, at the level beginning */
    level_desc->ball_release_time,
    /* time before the ball leaves (glue option) */
    level_desc->glue_time / difficulty_level,
    /* time before the ball accelerates */
    level_desc->acceleration_delay / difficulty_level,
    /* time before "tilt" is available */
    level_desc->tilt_delay, level_desc->starting_speed);

  ships->initialise (level_desc->reappearance / difficulty_level,
                     level_desc->ship_appearance_delay1 / difficulty_level,
                     level_desc->ship_appearance_delay2 / difficulty_level,
                     level_desc->ship_appearance_delay3 / difficulty_level,
                     level_desc->ship_appearance_delay4 / difficulty_level,
                     level_desc->ships_strength * difficulty_level);

  money_capsules->initialize (level_desc->moneys_frequency * difficulty_level,
                              panel_score, player_indicators);

  /* initialize the object which handles bonus and penalty capsules */
  power_up_capsules->initialize (
    level_desc->penalties_frequency * difficulty_level,
    level_desc->malusListe
    );
  gem_stones->initialize ();
  /* initialize sprite fonts "LEVEL x COMPLETED" */
  font_game->initialize (level_number);
  viewfinders_paddles->initialize ();
  display->unlock_surfaces ();
  /* copy the background offscreen to the game offscreen */
  if (has_background)
    {
      background_screen->blit_to_surface (game_screen);
    }
  keyboard->clear_command_keys ();
  keyboard->set_grab_input (true);
  info_messages->send_message_request (short_info_messages::ARE_YOU_READY);
}

/**
 * Reads the parameters of the current level
 */
void
supervisor_bricks_level::init_level ()
{
  level_desc = ptLev_data->get_bricks_levels_params (area_number, level_number);
}

/**
 * Main loop in the bricks level
 * @return
 */
Uint32
supervisor_bricks_level::main_loop ()
{
  Sint32 Ecode = -1;

  /*
   * the player has no more lives: Game Over
   */
  if (current_player->get_num_of_lifes () <= 0)
    {
      if (gameover_counter == 0)
        {
#ifndef SOUNDISOFF
          audio->disable_sound ();
          audio->stop_music ();
#endif
          paddles->disable_all_paddles ();
          bricks->clr_bricks ();
          font_game->disable_sprites ();
          gem_stones->disable_sprites ();
          power_up_capsules->disable_sprites ();
          money_capsules->disable_sprites ();
          balls->disable_sprites ();
          sprite_projectile::disable_sprites ();
          info_messages->clear_messages_request ();
          tiles_ground->set_scroll_type(tiles_background::TILES_SCROLL_GAMEOVER);
          gameover_counter++;
        }
      if (has_background)
	{
          info_messages->run ();
	}
      display->wait_frame ();
      head_anim->play ();
      display->lock_surfaces ();
      gigablitz->run_in_bricks_levels ();
      if (has_background)
        {
          sprites->clear ();
        }
      if (!(random_counter & 0x00f))
        {
          head_anim->start_interference ();
        }

      sides_bricks->run ();
      viewfinders_paddles->run ();
      ships->move ();
      draw_tilesmap ();
      sprites->draw ();

      if (gameover_counter >= 2)
        {
          gameover_counter++;
          game_over->run ();
        }
      if (!bricks->update () && gameover_counter < 2)
        {
          gameover_counter = 2;
        }


      panel_score->draw_gigablizt_gauge ();
      player_indicators->display_money_and_reverse ();
      display->unlock_surfaces ();
      panel_score->text_refresh ();
      display->window_update ();
      if (keyboard->wait_key () && gameover_counter > 60)
        {
          current_player = handler_players::get_next_player (current_player, &next_phase, 1);
        }
    }

  /*
   * game is running!
   */
  else
    {
      display->wait_frame ();
      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
          head_anim->play ();
        }
      sides_bricks->run ();
      display->lock_surfaces ();
      if (has_background)
        {
          sprites->clear ();
        }
      bricks->color_cycling ();
      /* draw or clear bricks
       * send a money and/or a capsule */
      bricks->update ();
      switch_background ();

      if (!keyboard->command_is_pressed (handler_keyboard::COMMAND_KEY_PAUSE))
        {
	  if (has_background)
	    {
              info_messages->run ();
	    }
          gigablitz->run_in_bricks_levels ();
          /* handle the "less bricks" option */
          bricks->less_bricks ();
          paddles->move_paddles ();
          if (panel_score->get_bricks_counter () > 0)
            {
              paddles->check_if_release_balls ();
              paddles->fire_projectiles ();
            }
          paddles->move_robot ();
          balls->run_in_bricks_levels ();
          viewfinders_paddles->run ();
          sprite_projectile::gestionTir ();
          ships->move ();
          magnetic_eyes->move ();
          money_capsules->move ();
          power_up_capsules->move_in_bricks_level ();
          power_up_capsules->check_cheat_keys ();
          gem_stones->move ();
          font_game->move ();
          if (bottom_wall->thecounter < 1)
            {
              bottom_wall->disable ();
            }
          else
            {
              bottom_wall->thecounter--;
            }

          panel_score->draw_gigablizt_gauge ();
          player_indicators->display_money_and_reverse ();
        }

      draw_tilesmap ();
      sprites->draw ();
      Ecode = popup_menu->run ();
      display->unlock_surfaces ();
      panel_score->text_refresh ();
      display->window_update ();

      /*
       * jump to next level or next player
       */
      if (panel_score->get_bricks_counter () == 0)
        {
          if (count_next > 0)
            {
              count_next++;
              bool music_finished = false;
              if (count_next > 350)
                {
                  music_finished = true;
#ifndef SOUNDISOFF
                  music_finished = audio->is_win_music_finished ();
#endif
                }
              if (count_next > 20000000 ||
                  keyboard->wait_key() || music_finished)
                {
                  sides_bricks->save_state_of_walls ();
                  current_player = handler_players::get_next_player (current_player,
                                   &next_phase);
#ifndef SOUNDISOFF
                  audio->stop_music ();
#endif
                }
              balls->disable_sprites ();
              sprite_projectile::disable_sprites ();
            }
          else
            {
              font_game->enable ();
              sprite_projectile::disable_sprites ();
              balls->disable_sprites ();
#ifndef SOUNDISOFF
              audio->play_win_music ();
#endif
              info_messages->send_message_request (short_info_messages::NEXT_LEVEL);
#ifndef SOUNDISOFF
              audio->disable_sound ();
#endif
              tiles_ground->set_scroll_type(tiles_background::TILES_SCROLL_WIN);
              count_next = 1;
            }
        }
    }

  /* escape key to quit the game! */
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TECNOBALLZ) ||
      Ecode == handler_popup_menu::QUIT_TECNOBALLZ)
    {
      next_phase = LEAVE_TECNOBALLZ;
    }
  if (keyboard->command_is_pressed (handler_keyboard::CAUSE_GAME_OVER) ||
      Ecode == handler_popup_menu::CAUSE_GAME_OVER)
    {
      current_player->remove_all_lifes ();
    }
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TO_MAIN_MENU) ||
      Ecode == handler_popup_menu::QUIT_TO_MAIN_MENU)
    {
      next_phase = MAIN_MENU;
    }

  /* control position music's module */
#ifndef SOUNDISOFF
  Uint32 phase = audio->get_portion_music_played ();
  if (phase == handler_audio::LOST_PORTION &&
      phase != audio->get_portion_music_played ())
    {
      info_messages->send_message_request (short_info_messages::YEAH_YOU_WHERE);
      paddles->release_all_balls ();
    }
#endif
  return next_phase;
}

/**
 * Change the tiles background
 */
void
supervisor_bricks_level::switch_background ()
{
#ifdef UNDER_DEVELOPMENT
  if (keyboard->key_is_pressed (SDLK_RSHIFT) ||
      keyboard->key_is_pressed (SDLK_LSHIFT) ||
      keyboard->key_is_pressed (SDLK_RCTRL) ||
      keyboard->key_is_pressed (SDLK_LCTRL) ||
      keyboard->key_is_pressed (SDLK_RALT) ||
      !keyboard->key_is_pressed (SDLK_LALT))
    {
      return;
    }

  if (keyboard->key_is_pressed (SDLK_w))
    {
      devel_keyw = true;
    }
  if (keyboard->key_is_pressed (SDLK_x))
    {
      devel_keyx = true;
    }

  if ((keyboard->key_is_released (SDLK_w) && devel_keyw) ||
      (keyboard->key_is_released (SDLK_x) && devel_keyx))
    {
      game_screen->clear ();

      if (devel_keyw)
        {
          devel_keyw = false;
          if (--backgound_index < 0)
            {
              backgound_index = 49;
            }
        }
      if (devel_keyx)
        {
          devel_keyx = false;
          if (++backgound_index > 49)
            {
              backgound_index = 0;
            }
        }
      if (is_verbose)
        {
          std::cout << ">supervisor_bricks_level::switch_background: " <<
          "backgound_index: " << backgound_index << std::endl;
        }
      initialize_background (backgound_index);
      background_screen->blit_to_surface (game_screen);
    }

  if (keyboard->key_is_pressed (SDLK_v))
    {
      head_anim->start_yawn ();
    }
  if (keyboard->key_is_pressed (SDLK_b))
    {
      head_anim->start_yawn ();
    }
  if (keyboard->key_is_pressed (SDLK_n))
    {
      head_anim->start_interference ();
    }
  if (keyboard->key_is_pressed (SDLK_g))
    {
      gigablitz->shoot_paddle ();
    }
  /*
   
  	if(keyboard->key_is_pressed(SDLK_w))
   { 
   }
   
   if(keyboard->key_is_pressed(SDLK_w))
   { keyw = 1;
   }
   
   
   if(keyboard->key_is_released(SDLK_w) && keyw == 1)
   { tiles_ground->prev_color();
   keyw = 0;
   }
   if(keyboard->key_is_released(SDLK_x) && keyx == 1)
   { tiles_ground->next_color();
   keyx = 0;
   }
  */


#endif
}

/**
 * Initialize the background, draw tiles side bricks and bicks
 * @param bkg_num tileset number, if negative then the
 *                tilesset number depends on the current level number 
 */
void
supervisor_bricks_level::initialize_background (Sint32 bkg_num)
{
  if (is_verbose)
    {
      std::cout << ">supervisor_bricks_level::initialize_background() start! "
      << std::endl;
    }
  if (bkg_num < 0)
    {
      bkg_num = ((area_number - 1) * 10) + level_number;
      if (level_number > 5)
        {
          bkg_num--;
        }
      if (is_verbose)
        {
          std::cout << "supervisor_bricks_level::initialize_background() " <<
          "background number: " << bkg_num << std::endl;
        }
    }
  /* initialize and draw the tiles background */
  tiles_ground->setup (bkg_num);
  /* short info messages displayed */
  info_messages->initialize ();
  /* draw shadows of ejectors */
  ejectors_corners->draw_shadow ();
  /* save background under small bricks */
  sides_bricks->save_background ();
  /* draw shadows of small bricks */
  sides_bricks->draw_shadows_to_brackground ();
  /* draw small bricks of the three walls */
  sides_bricks->draw_to_brackground ();
  ejectors_corners->draw ();
  /* initialize the bricks level */
  bricks->first_init ();
  bricks->initialize ();
  if (is_verbose)
    {
      std::cout << "/supervisor_bricks_level::initialize_background() start! " 
      << std::endl;
    }
}

/**
 * Draw the tiles background 
 */
void
supervisor_bricks_level::draw_tilesmap()
{
  if (has_background)
    {
      return;
    }
  display->unlock_surfaces ();
  tiles_ground->draw();
  info_messages->run ();
  display->lock_surfaces ();
}

