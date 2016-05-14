/**
 * @file tecnoballz.cc
 * @brief Base of all classes, and main static methods of the game
 * @created 2002-08-18
 * @date 2014-08-16
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
#include "../include/tecnoballz.h"
#include "../include/handler_display.h"
#include "../include/handler_keyboard.h"
#include "../include/list_sprites.h"
#include "../include/handler_players.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/supervisor_shop.h"
#include "../include/supervisor_guards_level.h"
#include "../include/supervisor_main_menu.h"
#include "../include/bitmap_data.h"
#include "../include/handler_audio.h"
#include "../include/handler_levels.h"
#include "../include/handler_resources.h"
#include "../include/handler_high_score.h"
#include "../include/supervisor_map_editor.h"


/**
 * Once initialization, create persistent objects
 */
void
tecnoballz::first_init (configfile * pConf)
{
  config_file = pConf;
  if (is_verbose)
    {
      std::cout << ">tecnoballz::first_init() start!" << std::endl;
      std::cout << " has_background:" << has_background << std::endl;
    }
#if __WORDSIZE == 64
  random_counter = (long) first_init;
#else
  random_counter = (Sint32) first_init;
#endif
  resources = handler_resources::get_instance ();
  high_score = handler_high_score::get_instance ();
  resources->load_sinus ();
  display = new handler_display ();
  display->initialize ();
#ifndef SOUNDISOFF
  audio = handler_audio::get_instance ();
#endif
  keyboard = handler_keyboard::get_instance ();
  sprites = new list_sprites ();
  sprites->init (400);
  ptLev_data = new handler_levels ();
  current_player =
    handler_players::create_all_players (handler_players::MAX_OF_PLAYERS);
  sprite_ball::init_collisions_points ();

  /* retrieve player names */
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      handler_players::players_list[i]->set_name (pConf->get_player_name (i));
    }
  current_phase = MAIN_MENU;

  if (arg_jumper > 0)
    {
#ifdef UNDER_DEVELOPMENT
      is_enabled_cheat_mode = true;
#endif
      current_phase = arg_jumper;
    }
  if (is_verbose)
    {
      std::cout << ">tecnoballz::first_init() end!" << std::endl;
    }
}

/**
 * main loop of the game
 */
void
tecnoballz::game_begin ()
{
  supervisor *stage = NULL;
  do
    {
      if (is_verbose)
        {
          std::cout << ">tecnoballz::game_begin() phase:" << current_phase
                    << std::endl;
        }
      switch (current_phase)
        {
        case LEAVE_TECNOBALLZ:
          is_exit_game = true;
          break;
        case BRICKS_LEVEL:
          stage = new supervisor_bricks_level ();
          break;
        case SHOP:
          stage = new supervisor_shop ();
          break;
        case GUARDS_LEVEL:
          stage = new supervisor_guards_level ();
          break;
        case MAIN_MENU:
          stage = new supervisor_main_menu ();
          break;
        case MAP_EDITOR:
          stage = new supervisor_map_editor ();
          break;
        default:
          std::cerr << "(!)tecnoballz::game_begin() phase number " <<
                    current_phase << "is invalid!" << std::endl;
          throw std::runtime_error ("(!)tecnoballz::game_begin() "
                                    "invalid phase number!");
        }
      if (NULL != stage)
        {
          stage->first_init ();
          Uint32 next = 0;
          do
            {
              next = stage->main_loop ();
            }
          while (!next);
          current_phase = next;
          delete stage;
          stage = NULL;
        }
    }
  while (!is_exit_game);
}

/**
 * Game exit, relase all objects
 */
void
tecnoballz::release_all_objects (configfile * pConf)
{
  /* save player names into config file */
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      pConf->set_player_name (i,
                              handler_players::players_list[i]->get_name ());
    }
  if (is_verbose)
    {
      std::cout << "(X) 1. release all player objects " << std::endl;
    }
  handler_players::release_all_players ();
  if (is_verbose)
    {
      std::cout << "(x) 2. delete 'handler_levels' singleton" << std::endl;
    }
  delete ptLev_data;
  if (is_verbose)
    {
      std::cout << "(x) 3. delete 'list_sprites' singleton" << std::endl;
    }
  delete sprites;

  if (is_verbose)
    {
      std::cout << "(x) 4. delete 'hanbdler_keyboard' singleton" << std::endl;
    }
  delete keyboard;

  if (is_verbose)
    {
      std::
      cout << "(x) 5. delete 'handler_high_score' singleton" << std::endl;
    }
  delete high_score;

#ifndef SOUNDISOFF
  if (is_verbose)
    {
      std::cout << "(x) 7. delete 'handler_audio' singleton" << std::endl;
    }
  delete audio;
#endif

  if (is_verbose)
    {
      std::cout << "(x) 6. delete 'handler_display' singleton" << std::endl;
    }
  delete display;

  if (is_verbose)
    {
      std::cout << "(x) 8. delete 'handler_resources'" << std::endl;
    }
  delete resources;
}

/**
 * Create the object
 */
tecnoballz::tecnoballz ()
{
}

/**
 * Release object
 */
tecnoballz::~tecnoballz ()
{
}

/**
 * Initialize some members
 */
void
tecnoballz::object_init ()
{
  object_num = objects_counter;
  objects_counter++;
}

/**
 * Object destroyed
 */
void
tecnoballz::object_free ()
{
  objects_counter--;
}

/**
 * Creates a string representing an integer number
 * @param value the integer value to be converted
 * @param padding length of the string
 * @param str the string representation of the number
 */
void
tecnoballz::integer_to_ascii (Sint32 value, Uint32 padding, char *str)
{
  char *ptr = str + padding - 1;
  bool neg = (value < 0);
  if (neg)
    {
      value = -value;
      --padding;
    }
  do
    {
      *ptr-- = (value % 10) + '0';
      value /= 10;
      --padding;
    }
  while (value && padding > 0);
  for (; padding > 0; --padding)
    {
      *ptr-- = '0';
    }
  if (neg)
    {
      *ptr-- = '-';
    }
}

//------------------------------------------------------------------------------
// convert integer into ASCII string
// input => value:      number to convert
//       => strng:      pointer to ASCII string (finished by  zero)
//       => reste:      maximum length (1 = 2 chars, 2 = 3 chars, ...)
//------------------------------------------------------------------------------
/*
void
tecnoballz::intToASCII (Sint32 value, char *strng, Uint32 reste)
{
  Uint32 index = 0;
  Uint32 zsize = 1;
  if (value < 0)
    {
      value = -value;
      *(strng++) = '-';
      if (reste > 0)
        reste--;
    }
  for (index = 0; index < reste; index++)
    zsize = zsize * 10;
  index = 0;
  while (zsize > 0)
    {
      Uint32 reste = value / zsize;
      char zchar = (char) ('0' + reste);
      if (zchar > '9' || zchar < '0')
        zchar = '?';
      strng[index++] = zchar;
      value = value - (zsize * reste);
      zsize = zsize / 10;
    }
}
*/

void
tecnoballz::int_to_big_endian (Uint32 * ptsrc, Uint32 * ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  *ptdes = *ptsrc;
#else
  char *s = (char *) ptsrc;
  char *d = (char *) ptdes;
  d[0] = s[3];
  d[1] = s[2];
  d[2] = s[1];
  d[3] = s[0];
#endif
}

void
tecnoballz::big_endian_to_int (Uint32 * ptsrc, Uint32 * ptdes)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  *ptdes = *ptsrc;
#else
  char *s = (char *) ptsrc;
  char *d = (char *) ptdes;
  d[3] = s[0];
  d[2] = s[1];
  d[1] = s[2];
  d[0] = s[3];
#endif
}


Sint32 tecnoballz::arg_jumper = -1;
bool tecnoballz::force_4_colors_tiles = false;
bool tecnoballz::is_verbose = false;
Uint32 tecnoballz::objects_counter = 0;
Sint32 tecnoballz::random_counter = 0;
Uint32 tecnoballz::frame_counter = 0;
handler_high_score *
tecnoballz::high_score = NULL;
handler_resources *
tecnoballz::resources = NULL;
handler_levels *
tecnoballz::ptLev_data = NULL;
#ifndef SOUNDISOFF
handler_audio *
tecnoballz::audio = NULL;
#endif
handler_display *
tecnoballz::display = NULL;
handler_keyboard *
tecnoballz::keyboard = NULL;
list_sprites *
tecnoballz::sprites = NULL;
handler_players *
tecnoballz::current_player = NULL;
Sint16 *
tecnoballz::table_cosL = NULL;
Sint16 *
tecnoballz::table_sinL = NULL;

Uint32 tecnoballz::current_phase = BRICKS_LEVEL;
bool tecnoballz::is_exit_game = false;
bitmap_data *
tecnoballz::sprites_bitmap = NULL;
bool tecnoballz::is_enabled_cheat_mode = false;
bool tecnoballz::birth_flag = 0;
Sint32 tecnoballz::difficulty_level = DIFFICULTY_EASY;
Sint32 tecnoballz::initial_num_of_lifes = 8;
Sint32 tecnoballz::number_of_players = 1;
const char
tecnoballz::nomprefix[] = PREFIX;
Uint32 tecnoballz::resolution = 2;
bool tecnoballz::has_background = false;
bool tecnoballz::absolute_mouse_positioning = false;
offscreen_surface *
tecnoballz::game_screen = NULL;
offscreen_surface *
tecnoballz::background_screen = NULL;
configfile *
tecnoballz::config_file;
