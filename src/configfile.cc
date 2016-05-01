/**
 * @file configfile.cc
 * @brief Config file handler
 * @created 2005-01-22
 * @date 2014-09-28
 * @copyright 1991-2014 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2014 TLK Games all rights reserved
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
#include "config.h"
#include "../include/configfile.h"
#include "../include/handler_display.h"
#include "../include/handler_audio.h"
#include "../include/handler_players.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *
configfile::language_to_string[MAX_OF_LANGUAGES] =
{
  "en",
  "fr"
};

const
std::string
configfile::CONF_DIR_NAME ("tlk-games");
const
std::string
configfile::CONF_FILENAME ("tecnoballz.conf");

/**
 * Create object
 */
configfile::configfile ()
{
  thePlayers[0] = thePlayer1;
  thePlayers[1] = thePlayer2;
  thePlayers[2] = thePlayer3;
  thePlayers[3] = thePlayer4;
  thePlayers[4] = thePlayer5;
  thePlayers[5] = thePlayer6;
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      char *p = thePlayers[i];
      for (Uint32 j = 0; j < 8; j++)
        {
          p[j] = 0;
        }
    }
  resetvalue ();
}

/**
 * Destroy object
 */
configfile::~configfile ()
{
}

/**
 * Reset all values
 */
void
configfile::resetvalue ()
{
#ifndef SOUNDISOFF
  handler_audio::is_audio_enable = 1;
#endif
  resolution = 2;
  has_background = false;
  is_verbose = false;
  handler_display::optionfull = false;
  difficulty_level = DIFFICULTY_NORMAL;
  initial_num_of_lifes = 5;
  number_of_players = 1;
  char *user = getenv ("USER");
  if (user == NULL)
    {
      user = stringname;
    }
  for (Uint32 i = 0; i < 6; i++)
    {
      strncpy (thePlayers[i], user, 6);
    }
  language = LANGUAGE_EN;
  absolute_mouse_positioning = false;
}

/**
 * Display values
 */
void
configfile::configinfo ()
{
  bool audio;
#ifndef SOUNDISOFF
  audio = handler_audio::is_audio_enable;
#else
  audio = false;
#endif
  fprintf (stdout, "  <config info>\n"
           "- optionfull : %i\n- is_audio_enable: %i\n- resolution:%i\n"
           "- is_verbose: %i\n difficulty_level : %i\n",
           handler_display::optionfull, audio, resolution,
           is_verbose, difficulty_level);
}

/**
 * Check if config directory exists; if not create it and set config_dir
 */
bool configfile::check_and_create_dir ()
{
#ifdef _WIN32
  /* opendir don't exist on windows
   * create directory if not exist */
  MKDIR (conf_dirname.c_str (), S_IRWXU);
#else
  /* test and create .tlkgames */
  DIR* dp = opendir (conf_dirname.c_str ());
  if (!dp)
    {
      fprintf (stderr, "couldn't find/open config directory '%s'\n",
               conf_dirname.c_str ());
      fprintf (stderr, "attempting to create it... ");
      MKDIR (conf_dirname.c_str (), S_IRWXU);
      dp = opendir (conf_dirname.c_str ());
      if (!dp)
        {
          fprintf (stderr, "opendir() was failed\n");
          return false;
        }
    }
  closedir (dp);
#endif
  return true;
}

/**
 * Built the full path of the configuration file.
 */
void
configfile::get_fullpathname ()
{
  if (!conf_filename.empty ())
    {
      return;
    }
  if (getenv ("XDG_CONFIG_HOME") != NULL)
    {
      conf_dirname = getenv ("XDG_CONFIG_HOME");
      conf_dirname += "/" + CONF_DIR_NAME;
    }
  else
    {
      conf_dirname = (getenv ("HOME") ? getenv ("HOME") : ".");
      conf_dirname += "/.config/" + CONF_DIR_NAME;
    }
  conf_filename += conf_dirname + "/" + CONF_FILENAME;
}

/**
 * Load configuration file in "~/.tlkgames/tecnoballz.conf"
 */
void
configfile::load ()
{
  resetvalue ();
  get_fullpathname ();
  std::cout << conf_filename << std::endl;

  /* Check if .conf exist and create it if not */
  FILE *fi = fopen (conf_filename.c_str (), "r");
  if (fi == NULL)
    save();
  else
    fclose (fi);

  lispreader *parser = new lispreader ();
  lisp_object_t *root_obj = parser->lisp_read_file (conf_filename);
  if (root_obj == NULL)
    {
      std::
      cerr << "lispreader::lisp_read_file (" << conf_filename <<
           ") was failed" << std::endl;
      return;
    }
  if (root_obj->type == LISP_TYPE_EOF
      || root_obj->type == LISP_TYPE_PARSE_ERROR)
    {
      fprintf (stderr, "configfile::loadconfig() / conf parsing failed\n");
      return;
    }

  if (strcmp
      (parser->lisp_symbol (parser->lisp_car (root_obj)),
       "tecnoballz-config") != 0)
    {
      fprintf (stderr, "configfile::loadconfig() / conf parsing failed\n");
      return;
    }

  std::string ptStr;

  if (!parser->read_string ("lang", &ptStr))
    {
      language = LANGUAGE_EN;
    }
  else
    {
      if (ptStr == "fr")
        {
          language = LANGUAGE_FR;
        }
      else
        {
          language = LANGUAGE_EN;
        }
    }
  //exit(0);
  if (!parser->read_bool ("fullscreen", &handler_display::optionfull))
    {
      handler_display::optionfull = -1;
    }
#ifndef SOUNDISOFF
  if (!parser->read_bool ("sound", &handler_audio::is_audio_enable))
    {
      handler_audio::is_audio_enable = true;
    }
#endif
  if (!parser->read_bool ("verbose", &is_verbose))
    {
      is_verbose = false;
    }
  if (!parser->read_bool ("absolute_mouse", &absolute_mouse_positioning))
    {
      absolute_mouse_positioning = false;
    }


  // read window resolution: 1 = 320*240; 2 = 640*480
  Sint32 res = 0;
  if (!parser->read_int ("resolution", &res))
    {
      res = 2;
    }
  resolution = res;
  if (resolution < 1 || resolution > 2)
    {
      resolution = 2;
    }
  if (resolution == 2)
    {
      has_background = false;
    }
  else
    {
      has_background = true;
    }
  has_background = false;

  // read number of lifes from 1 to 9
  if (!parser->read_int ("lifes", &initial_num_of_lifes))
    {
      initial_num_of_lifes = 5;
    }
  if (initial_num_of_lifes < 1 || initial_num_of_lifes > 9)
    {
      initial_num_of_lifes = 5;
    }

  // read difficulty DIFFICULTY_EASY, DIFFICULTY_NORMAL,
  // DIFFICULTY_MEDIUM or DIFFICULTY_HARD
  if (!parser->read_int ("difficulty", &difficulty_level))
    {
      difficulty_level = DIFFICULTY_NORMAL;
    }
  if (difficulty_level < DIFFICULTY_EASY
      || difficulty_level > DIFFICULTY_HARD)
    {
      difficulty_level = DIFFICULTY_NORMAL;
    }

  //  read number of players from 1 to 6
  if (!parser->read_int ("players", &number_of_players))
    {
      number_of_players = handler_players::MAX_OF_PLAYERS;
    }
  if (number_of_players < 1
      || number_of_players > (Sint32) handler_players::MAX_OF_PLAYERS)
    {
      number_of_players = 1;
    }

  // read players names
  std::string sName (6, ' ');
  char cName[8] = { "......." };
  for (Uint32 i = 0; i < 6; i++)
    {
      sprintf (cName, "player%01d", i + 1);
      if (parser->read_string (cName, &sName))
        {
          strncpy (thePlayers[i], sName.c_str (), 6);
        }
    }
  delete parser;
#ifdef TECNOBALLZ_GP2X
  resolution = 1;
#endif
}

/**
 * Return current player name, which read from config file
 * @param playernum Player number from 0 to 5
 * @return Pointer to a player name
 */
const char *
configfile::get_player_name (Uint32 playernum)
{
  if (playernum >= handler_players::MAX_OF_PLAYERS)
    {
      playernum = handler_players::MAX_OF_PLAYERS - 1;
    }
  return thePlayers[playernum];
}

/**
 * Set player name
 * @param playernum Player number from 0 to 5
 * @param name Pointer to a string
 */
void
configfile::set_player_name (Uint32 playernum, const char *name)
{
  if (playernum >= handler_players::MAX_OF_PLAYERS)
    {
      playernum = handler_players::MAX_OF_PLAYERS - 1;
    }
  strncpy (thePlayers[playernum], name, 6);
}

/*
 * Return current language
 * @return "en" or fr
 */
const char *
configfile::get_language ()
{
  return language_to_string[language];
}

/**
 * Save config file "~/.tlkgames/tecnoballz.conf"
 */
void
configfile::save ()
{
  bool audio;
#ifndef SOUNDISOFF
  audio = handler_audio::is_audio_enable;
#else
  audio = false;
#endif
  if (!check_and_create_dir ())
    {
      return;
    }
  FILE *config = fopen_data (conf_filename.c_str (), "w");
  if (config)
    {
      fprintf (config, "(tecnoballz-config\n");
      fprintf (config,
               "\t;; the following options can be set to #t or #f:\n");
      fprintf (config, "\t(fullscreen %s)\n",
               handler_display::optionfull ? "#t" : "#f");
      fprintf (config, "\t(sound %s)\n", audio ? "#t" : "#f");
      fprintf (config, "\t(verbose %s)\n", is_verbose ? "#t" : "#f");
      fprintf (config, "\t(absolute_mouse %s)\n",
               absolute_mouse_positioning ? "#t" : "#f");
      fprintf (config, "\n\t;; window size 1 (low-res) or 2 (high-res):\n");
      fprintf (config, "\t(resolution  %d)\n", resolution);
      fprintf (config,
               "\n\t;; difficulty 1 (easy), 2 (hard), 3 (madness) or 4 (suicidal)\n");
      fprintf (config, "\t(difficulty   %d)\n", difficulty_level);
      fprintf (config, "\n\t;; number of lifes (1 to 9)\n");
      fprintf (config, "\t(lifes   %d)\n", initial_num_of_lifes);
      fprintf (config, "\n\t;; number of players (1 to 6)\n");
      fprintf (config, "\t(players   %d)\n", number_of_players);
      fprintf (config, "\n\t;; players names\n");
      for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
        {
          fprintf (config, "\t(player%i      \"%s\")\n", i + 1,
                   thePlayers[i]);
        }
      fprintf (config, "\n\t;; language en or fr\n");
      fprintf (config, "\t(lang      ");
      switch (language)
        {
        case LANGUAGE_FR:
          fprintf (config, "\"fr\")\n");
          break;
        default:
          fprintf (config, "\"en\")\n");
          break;
        }
      fprintf (config, ")\n");
      fclose (config);
    }
}

/**
 * Open a file
 * @param fname
 * @param fmode
 * @return
 */
FILE *
configfile::fopen_data (const char *fname, const char *fmode)
{
  FILE *fi;
  fi = fopen (fname, fmode);
  if (fi == NULL)
    {
      fprintf (stderr, "configfile::fopen_data(): Warning: Unable "
               "to open the file \"%s\" ", fname);
      if (strcmp (fmode, "r") == 0)
        {
          fprintf (stderr, "for read!!!\n");
        }
      else if (strcmp (fmode, "w") == 0)
        {
          fprintf (stderr, "for write!!!\n");
        }
    }
  return (fi);
}

/**
 * Scan command line arguments
 * @param arg_count the number of arguments
 * @param arg_values he command line arguments
 * @return FALSE if exit, TRUE otherwise
 */
Sint32 configfile::scan_arguments (Sint32 arg_count, char **arg_values)
{
  Sint32 i;
  for (i = 1; i < arg_count; i++)
    {
      if (*arg_values[i] != '-')
        {
          continue;
        }

      if (!strcmp (arg_values[i], "-h") || !strcmp (arg_values[i], "--help"))
        {
          printf ("\noptions:\n");
          printf ("-h, --help     print Help (this message) and exit\n");
          printf ("--version      print version information and exit\n");
          printf ("--full         full screen\n");
          printf ("--window       windowed mode\n");
          printf ("--320          game run in a 320*200 window\n");
          printf ("--640          game run in a 640*400 window\n");
          printf ("--verbose      verbose mode\n");
#ifndef SOUNDISOFF
          printf ("--nosound      force no sound\n");
          printf ("--sound        enable sound\n");
#endif
          printf ("--nosync       disable timer\n");  
          printf ("--bg4          Force using a four-color background\n");  
          printf
          ("--------------------------------------------------------------\n");
          printf ("keys recognized during the game:\n");
          printf ("CTRL+S         enable/disable sound\n");
          printf ("CTRL+D         enable/disable music\n");
          printf ("CTRL+F         enable/disable sound effects\n");
          printf ("CTRL+X         finish the play current\n");
          printf ("CTRL+Q         return to the main menu\n");
          printf ("CTRL+ESC       quit TecnoballZ\n");
          printf ("P              enable/disable pause\n");
          printf ("F              full screen/window mode\n");
          printf ("\n");
          return 0;
        }

      if (!strcmp (arg_values[i], "--version"))
        {
          printf (TECNOBALLZ_VERSION);
          printf ("\n");
          printf ("copyright (c) 1991-2014 TLK Games\n");
          printf ("website: http://linux.tlk.fr/games/TecnoballZ/\n");
          return 0;
        }

      if (!strcmp (arg_values[i], "--full"))
        {
          handler_display::optionfull = true;
          continue;
        }

      if (!strcmp (arg_values[i], "--window"))
        {
          handler_display::optionfull = false;
          continue;
        }

      if (!strcmp (arg_values[i], "--verbose") ||
          !strcmp (arg_values[i], "-v"))
        {
          is_verbose = true;
          continue;
        }

      if (!strcmp (arg_values[i], "--320"))
        {
          resolution = 1;
          continue;
        }
      if (!strcmp (arg_values[i], "--640"))
        {
          resolution = 2;
          continue;
        }

      if (!strcmp (arg_values[i], "--nosync"))
        {
          handler_display::optionsync = false;
          continue;
        }
#ifndef SOUNDISOFF
      if (!strcmp (arg_values[i], "--sound"))
        {
          handler_audio::is_audio_enable = true;
          continue;
        }
      if (!strcmp (arg_values[i], "--nosound"))
        {
          handler_audio::is_audio_enable = false;
          continue;
        }
#endif
      /* use 4 colors background in 640x480 */
      if (!strcmp (arg_values[i], "--bg4"))
        {
          force_4_colors_tiles = true;
          continue;
        }

#ifdef UNDER_DEVELOPMENT
      /* start at brick or guard level */
      if (!strcmp (arg_values[i], "--guard"))
        {
          arg_jumper = 3;
          continue;
        }
      if (!strcmp (arg_values[i], "--brick"))
        {
          arg_jumper = 1;
          continue;
        }
#endif
    }
  return 1;
}

char
configfile::stringname[7] = "YANIS ";
