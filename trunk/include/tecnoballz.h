/** 
 * @file tecnoballz.h
 * @brief Base of all classes, and main static methods of the game 
 * @created 2002-08-18
 * @date 2012-10-07
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
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef WIN32
#include <io.h>
#include "SDL/SDL.h"
#else
#include <unistd.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <pthread.h>
#include <unistd.h>
#include "SDL/SDL.h"
#endif
#include "../include/configfile.h"
#include "config.h"
#ifndef __TECNOBALLZ__
#define __TECNOBALLZ__

#if !defined(PACKAGE_STRING)
#define PACKAGE_STRING "TECNOBALLZ 0.93"
#endif
#define TECNOBALLZ_VERSION  PACKAGE_STRING " (2012-10-07)"


/** Force bytes copy (SPARC unaligned memory access) */
/* #define BYTES_COPY */
#ifndef SCOREFILE
#define SCOREFILE "tecnoball"
#endif
#ifndef PREFIX
#define PREFIX ""
#endif


/** PlayStation Portable port, PSP is a handheld game console released
 * and currently manufactured by Sony Computer Entertainment */
/* #define TECNOBALLZ_PSP */
/** GP2X port, GP2X is an open-source,
 * Linux-based handheld video game console */
/* #define TECNOBALLZ_GP2X */
#ifdef TECNOBALLZ_GP2X
#undef TECNOBALLZ_PSP
#define TECNOBALLZ_HANDHELD_CONSOLE
#endif
#ifdef TECNOBALLZ_PSP
#undef TECNOBALLZ_GP2X
#define TECNOBALLZ_HANDHELD_CONSOLE
#endif

class tecnoballz;
class configfile;
class handler_high_score;
class handler_resources;
class handler_levels;
class handler_display;
class handler_keyboard;
#ifndef SOUNDISOFF
class handler_audio;
#endif
class list_sprites;
class handler_players;
class supervisor_bricks_level;
class supervisor_shop;
class supervisor_guards_level;
class supervisor_main_menu;
class supervisor_map_editor;
class bitmap_data;
class offscreen_surface;

const Sint32 SINUS_MASK = 511;
const Sint32 SINUS_DECA = 7;
class tecnoballz
{
public:

  /** Different phases of the game */
  typedef enum
  {
    BRICKS_LEVEL = 1,
    SHOP,
    GUARDS_LEVEL,
    MAIN_MENU,
    MAP_EDITOR,
    LEAVE_TECNOBALLZ
  }
  GAME_PHASE;
  /** Different levels of difficulty */
  typedef enum 
  {
    DIFFICULTY_EASY = 1,
    DIFFICULTY_NORMAL,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
  }
  DIFFICULTIES_ENUM;

public:
  /** 4 levels of difficulty */
  static const Uint32 MAX_OF_DIFFICULTY_LEVELS = 4;

public:
  /** True if verbose enable */
  static bool is_verbose;
  /** Force 4 colors tiles set in bricks level */
  static bool force_4_colors_tiles;
  /** 1=320x240 or 2=640x480 pixels */ 
  static Uint32 resolution;
  /** True if static background tiles with bricks and ejectors
   * draw inside. False if bricks and ejectors are sprites */
  static bool has_background;
  /** Control paddle with absolute mouse position */
  static bool absolute_mouse_positioning;
  static Sint32 arg_jumper;

protected:
  /* object number */
  Uint32 object_num;

  /*
   * static members
   */

  /** Current number of objects */
  static Uint32 objects_counter;
  /** Counter the number of frames */
  static Uint32 frame_counter;
  /** Random counting variable */
  static Sint32 random_counter;
  /** Current phase of the game
   * BRICKS_LEVEL, SHOP, GUARDS_LEVEL,
   * MAIN_MENU, or MAP_EDITOR */
  static Uint32 current_phase;
  /** Difficulty with which an average player may complete a game
   * 1: easy; 2:normal 3:medium; 4:hard */
  static Sint32 difficulty_level;
  /** If true then leaves the game definitively */
  static bool is_exit_game;
  /** If true cheat mode is enabled. Before everything in the menu, 
   * the six names must be 040670. Also the cheat mode must be enabled
   * in the shop. You can moving the pointer to the top left screen
   * corner, and type [E], [T], [B] and [ENTER].
   */
  static bool is_enabled_cheat_mode;
  /** True if six names of menu initialized to 040670 */
  static bool birth_flag;
  /** Number of initial lifes */
  static Sint32 initial_num_of_lifes;
  /** Number of players from 1 to 6 */
  static Sint32 number_of_players;
  static const char nomprefix[];
  static handler_high_score *high_score;
  /** Handler of the files resources */
  static handler_resources *resources;
  static handler_levels *ptLev_data;
#ifndef SOUNDISOFF
  static handler_audio *audio;
#endif
  static handler_display *display;
  static handler_keyboard *keyboard;
  static list_sprites *sprites;
  static handler_players *current_player;
  static Sint16 *table_cosL;
  static Sint16 *table_sinL;
  /** A bitmap containing the current images of sprites */
  static bitmap_data *sprites_bitmap;
  /**  The main offscreen surface of the game */
  static offscreen_surface *game_screen;
  static offscreen_surface *background_screen;
  static configfile *config_file;

public:
  static void first_init (configfile * pConf);
  static void game_begin ();
  static void release_all_objects (configfile * pConf);
  static void integer_to_ascii (Sint32 value, Uint32 padding, char *str); 
    tecnoballz ();
  virtual ~tecnoballz ();
  void object_init ();
  void object_free ();
  void int_to_big_endian (Uint32 *, Uint32 *);
  void big_endian_to_int (Uint32 *, Uint32 *);
};
#endif
