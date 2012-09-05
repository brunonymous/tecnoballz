/** 
 * @file sprite_display_menu.h
 * @brief Sprite wich display text of the menu in the menu principal 
 * @date 2012-09-02 
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
#ifndef __SPRITE_DISPLAY_MENU__
#define __SPRITE_DISPLAY_MENU__

class sprite_display_menu;

#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"
#include "../include/handler_menu_events.h"

class sprite_display_menu:public virtual display_text_bitmap,
  public virtual sprite_object
{

public:
  typedef enum 
    {
      DO_NO_EXIT,
      PROGRAM_EXIT,
      START_GAME
    }
    EXIT_ENUM;

private:

    typedef enum
    {
      MAIN_SECTION = 0,
      OPTIONS_SECTION = 19,
      ABOUT_SECTION = 38,
      INFOS_SECTION = 57,
      GREETINGS_SECTION = 76,
      SCORE_SECTIONS = 95,
      MAX_OF_LINES = 114
    }
    MENU_TEXTS_ENUM;

  /** Object handle events of the menu */
  handler_menu_events* menu_events;
  /** Pointer to the text file loaded */
  char **texts_of_menus;
  static const unsigned char color_cycling[];
  /** Number of characters by lines */
  static const Sint32 NUM_OF_COLUMNS = 32;
  /** Number of lines of characters */
  static const Sint32 NUM_OF_ROWS = 19;
  static const Sint32 LINE_START = 5;
  static const Sint32 LINE_PARAM = 6;
  static const Sint32 LINE_ABOUT = 7;
  static const Sint32 LINE_SALUT = 8;
  static const Sint32 LINE_INFOS = 9;
  static const Sint32 LINE_SCORE = 10;
  static const Sint32 LINE_CODE1 = 11;
  static const Sint32 LINE_CODE2 = 12;
  static const Sint32 LINE_SORTI = 13;
  static const Sint32 LINE_ENTRE = 14;
  static const Sint32 YCOORDNULL = -10240;

  /** Width of font in pixels 8 or 16 */
  Uint32 font_width;
  /** Height of font in pixels 8 or 16 */
  Uint32 font_height;
  /** Space between lines in pixels */
  Uint32 line_spacing;
  /** Id of the current menu from MAIN_SECTION to SCORE_SECTIONS */
  Uint32 current_menu_section; 
  /** Current index of the first color of the line
   *  under the mouse cursor */
  Uint32 menu_first_color_index;
  Sint32 y_coord_left_down;
  Sint32 y_coord_right_down;
  bitmap_data *text_offscreen;
  /** True if currently cleaning */
  bool is_clean_zone;
  /** Height of the zone to clear */
  Uint32 clear_zone_height;
  /** Width of the zone to clear */
  Uint32 clear_zone_width;
  /** X-coordinate of the top-left zone to clear */
  Uint32 clear_zone_xcoord;
  /** Y-coordinate of the top-right zone to clear */
  Uint32 clear_zone_ycoord; 
  Uint32 blink_cursor_delay;
  /** Offsets of the first 128 ASCII characters */
  static char ascii_to_index[128];
public:
  static const char difficulte[];
public:
    sprite_display_menu ();
   ~sprite_display_menu ();
  void load_text_file();
  void first_init ();
  Uint32 check_and_display ();

private:
  void update_strings ();
  void set_current_menu_section(Uint32 current);
  Uint32 check_events ();
  void clear_text_offscreen ();
  void clear_input_zone ();
  void clear_zone_start (Uint32 xcoord, Uint32 ycoord, Uint32 width, Uint32 height);
  void start_input_string (Uint32 xcoord, Uint32 ycoord, Uint32 width, char *str);
  void clear_zone_stop ();
  void draw_input_cursor ();
  void copy_high_score_in_menu ();
};
#endif
