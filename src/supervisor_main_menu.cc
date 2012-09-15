/**
 * @file supervisor_main_menu.cc
 * @brief TecnoballZ's main menu supervisor
 * @date 2012-09-15
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
#include "../include/supervisor_main_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"
#include "../include/controller_guardians.h"

/**
 * Create the main menu supervisor
 */
supervisor_main_menu::supervisor_main_menu ()
{
  initialize ();
  /* vertical background scrolling */
  tiles_map = new tilesmap_scrolling ();
  /* big TecnoballZ logo */
  tecnoballz_logo = new sprite_object ();
  font_scrolling = new controller_font_menu ();
  text_menu = new sprite_display_menu ();
  mouse_pointer = new sprite_mouse_pointer ();
  tecnoballz_logo_angle = 0;
}

/**
 * Release the main menu supervisor
 */
supervisor_main_menu::~supervisor_main_menu ()
{
  delete mouse_pointer;
  delete text_menu;
  delete font_scrolling;
  delete tecnoballz_logo;
  delete tiles_map;
  release ();
}

/**
 * Perform some initializations
 */
void
supervisor_main_menu::first_init ()
{
  if (is_verbose)
    {
      std::cout << "supervisor_main_menu::first_init() Begin!" << std::endl;
    }
  sprites->reset ();
#ifndef SOUNDISOFF
  audio->play_music (handler_audio::MUSICINTRO);
#endif

  /*
   * create sprites data
   */
  resources->load_sprites_bitmap ();
  tecnoballz_logo->create_sprite (sprite_object::TECNOBALLZ_LOGO,
                                  sprites_bitmap, 1);
  sprites->add (tecnoballz_logo);
  tecnoballz_logo->enable ();
  tecnoballz_logo->set_coordinates (64 * resolution, 13 * resolution);
  font_scrolling->create_fontes_list ();
  mouse_pointer->create_pointer_sprite (sprites_bitmap);
  resources->release_sprites_bitmap ();
  tiles_map->initialize (tilesmap_scrolling::TILES_COLOR_MENU,
                         tilesmap_scrolling::MAP_MENU);

  text_menu->first_init ();
  keyboard->set_grab_input (false);
  if (is_verbose)
    {
      std::cout << "supervisor_main_menu::first_init() End!" << std::endl;
    }
}

/**
 * Main loop of the main menu
 */
Uint32
supervisor_main_menu::main_loop ()
{

  display->wait_frame ();
  /* vertical scrolling of the screen background */
  tiles_map->scroll (-1);
  display->lock_surfaces ();
  move_tecnoballz_logo ();
  /* scroll text of the menu */
  font_scrolling->move_chars ();
  mouse_pointer->move ();
  sprites->draw ();

  /* check and draw the menu text */
  switch (text_menu->check_and_display ())
    {
    case sprite_display_menu::PROGRAM_EXIT:
      next_phase = LEAVE_TECNOBALLZ;
      break;
    case sprite_display_menu::START_GAME:
      next_phase = start_new_game ();
      break;
    }

  text_menu->draw_copy_from_bitmap ();
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TECNOBALLZ))
    {
      next_phase = LEAVE_TECNOBALLZ;
    }
  display->unlock_surfaces ();

  /* copy whole game surface into screen surface */
  display->window_update ();

  /* [F5] key jump to map editor */
#ifdef UNDER_DEVELOPMENT
  if (keyboard->key_is_pressed (SDLK_F5))
    {
      next_phase = MAP_EDITOR;
    }
#endif
  return next_phase;
}

/**
 * Move the logo of TecnoballZ sprite
 */
void
supervisor_main_menu::move_tecnoballz_logo ()
{
  tecnoballz_logo_angle = (tecnoballz_logo_angle + 6) & SINUS_MASK;
  /* convert unsigned to signed */
  Sint32 res = (Sint32) resolution;
  Uint32 a = tecnoballz_logo_angle;
  tecnoballz_logo->set_x_coord (((table_sinL[a] * 20 * res) >> SINUS_DECA) +
                                32 * res);
  if (!birth_flag)
    {
      return;
    }
  a = (a * 4) & SINUS_MASK;
  tecnoballz_logo->set_y_coord (((table_sinL[a] * 5 * res) >> SINUS_DECA) +
                                7 * res);
}

/**
 * Start new TecnoballZ game
 */
Sint32 supervisor_main_menu::start_new_game ()
{
  is_enabled_cheat_mode = false;
#ifdef UNDER_DEVELOPMENT
  is_enabled_cheat_mode = true;
#endif

  /*
   * check area password validity
   */
  Uint32
  area_num;
  Uint32
  level_num;
  Uint32
  grdPt;
  Uint32
  area_count = check_area_code ();
  if (area_count == 0)
    {
      area_num = 1;
      level_num = 1;
      grdPt = 0;
    }
  else
    {
      if (area_count == 6)
        {
          area_num = 5;
          level_num = 13;
        }
      else
        {
          area_num = area_count;
          level_num = 12;
        }
      if (is_verbose)
        {
          std::cout << "*supervisor_main_menu::start_new_game() " <<
                    "password is valid! Password: " << &current_area_code[0] <<
                    "; area number:" << area_num << "; level number:" << level_num <<
                    "; difficulty level_num: " << difficulty_level << std::endl;

        }
      grdPt = controller_guardians::level2gdpt (area_num, level_num);
    }

  /* initialize and enable the player(s) */
  Sint32
  iplay;
  for (iplay = 0; iplay < number_of_players; iplay++)
    {
      Uint32
      nlife = initial_num_of_lifes;
      handler_players *
      player = handler_players::players_list[iplay];
      if (birth_flag)
        {
          nlife = nlife + 10;
        }
      if (strcmp (player->get_name (), "ETB   ") == 0)
        {
          nlife += 5;
        }
      if (strcmp (player->get_name (), "DJI   ") == 0)
        {
          nlife += 4;
        }
      if (strcmp (player->get_name (), "JMM   ") == 0)
        {
          nlife += 3;
        }
      if (strcmp (player->get_name (), "ZBB   ") == 0)
        {
          nlife += 2;
        }
      if (strcmp (player->get_name (), "REG   ") == 0)
        {
          nlife += 1;
        }
      player->initialize (nlife, area_num, level_num, 600, grdPt);
    }


  /* disable other player(s) */
  for (Uint32 i = iplay; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      handler_players::players_list[i]->initialize (0, 1, 1, 0, 0);
    }
  current_player = handler_players::players_list[0];

  Uint32
  next = current_player->get_next_phase ();
  if (next == SHOP)
    {
      next = BRICKS_LEVEL;
    }
  return next;
}

/**
 * Check area area code validity
 * @return if the code is valid return the area number from 1 to 5
 *         or 6 for the very last guardian of the area 5.
 *         Return 0 if the code is not valid
 */
Uint32 supervisor_main_menu::check_area_code ()
{
  Uint32 area_max = 4;
  Uint32 index = 0;
#ifdef UNDER_DEVELOPMENT
  /* The number of zones really goes from 1 to 5.
   * The value 6 makes it possible to jump directly to the
   * very last guardian of the area 5 */
  area_max = 6;
#else
  if (birth_flag)
    {
      area_max = 6;
    }
#endif
  for (Uint32 area_count = 1; area_count <= area_max; area_count++)
    {
      for (Uint32 difficulty_count = 1; difficulty_count <= 4;
           difficulty_count++)
        {
          bool
          is_valid = true;
          for (Uint32 i = 0; i < 10; i++)
            {
              if (current_area_code[i] != area_codes[index + i])
                {
                  is_valid = false;
                  break;
                }
            }
          index += 10;
          if (is_valid)
            {
              difficulty_level = difficulty_count;
              return area_count;
            }
        }
    }
  return 0;
}

/**
 * Static method which return a area code
 * @param area_num area number from 2 to 5
 * @param difficulty 0 to 3
 * @return the area code
 */
const char *
supervisor_main_menu::get_area_code (Uint32 area_num, Uint32 difficulty)
{
  if (area_num < 2)
    {
      return NULL;
    }
  return &area_codes[(area_num - 2) * 40 + (difficulty - 1) * 10];
}

/**
* Static method which return the current area code
* @return the current area code
*/
char *
supervisor_main_menu::get_current_area_code ()
{
  return current_area_code;
}

/**
 * Static method which copy current area code
 * @param destination destination string
 */
void
supervisor_main_menu::copy_current_area_code (char *destination)
{
  for (Uint32 i = 0; i < AREA_CODE_LENGTH; i++)
    {
      destination[i] = current_area_code[i];
    }
}

/** All areas code for every areas and every difficulty levels */
const char
supervisor_main_menu::area_codes[241] =
{
  /* level 12 area 1 */
  "LARRYHEARD" "SAUNDERSON" "JUANATKINS" "STEPHENSON"
  /* level 12 area 2 */
  "DANCEFLOOR" "REVOLUTION" "LOOKTOSEXY" "REACHINGUP"
  /* level 12 area 3 */
  "ZULUNATION" "HOUSEPIMPS" "ANDRONICUS" "DEFINITIVE"
  /* level 12 area 4 */
  "DANCEMANIA" "PEPPERMINT" "SOLARTRIBE" "PROJECTXYZ"
  /* level 12 area 5 (with cheat code enabled) */
  "RINGOFFIRE" "POINTBLANK" "TEMPTATION" "BLUEMONDAY"
  /* level 13 area 5 (with cheat code enabled) */
  "SHELLSHOCK" "HOUSEMUSIC" "DAVECLARKE" "CYBERACTIF"
};

/** Current input area code used to jump directly to the
 * end of a area */
char
supervisor_main_menu::current_area_code[AREA_CODE_LENGTH + 1] =
  "          ";
