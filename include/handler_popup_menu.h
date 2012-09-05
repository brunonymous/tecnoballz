/**
 * @file handler_popup_menu.h
 * @brief popup menu handler (When the [Esc] is pressed)
 * @created 2004-08-08 
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
#ifndef __HANDLER_POPUP_MENU__
#define __HANDLER_POPUP_MENU__
class handler_popup_menu;

#include "../include/tecnoballz.h"
#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"
#include "../include/handler_menu_events.h"

class handler_popup_menu:public virtual display_text_bitmap,
      public virtual sprite_object
  {
  private:
    /** Object handle events of the menu */
    handler_menu_events* menu_events;
    /** Pointer to the text file loaded */
    char **texts_of_menus;
    /** Offscreen used to draw the box menu */
    bitmap_data * screen_menu;
    /** Number of columns of the menu: 25 chars per line */
    Uint32 num_of_columns;
    /** Number of lines of the current menu: 3 or 4 lines */
    Uint32 num_of_lines;
    /** Space between lines in pixels */
    Uint32 vertical_space;
    /** Current index of the first color of the line
     * under the mouse cursor */
    Uint32 menu_first_color_index;
    /** True if restore background when leave menu */
    bool is_restore_background;
    /** Current menu enabled 0 or 1 */
    Uint32 menu_number;
    /** Center x-coordinate of the menu */
    Uint32 menu_xcenter;

  public:
    /** List of items avaiblables in the menu */
    typedef enum
    {
      CONTINUE_PLAY_CURRENT = 1,
      CAUSE_GAME_OVER,
      QUIT_TO_MAIN_MENU,
      QUIT_TECNOBALLZ
    }
    MENU_ITEMS_ENUM;
    /** String indexes */
    typedef enum
    {
      MENU_00,
      MENU_01 = 4,
      MAX_OF_LINES = 7
    }
    MENU_TEXTS_ENUM;
    /** Number maximum of chars by line */
    static const Uint32 MAX_OF_CHARS = 25;
  public:
    handler_popup_menu ();
    ~handler_popup_menu ();
    void first_init (bitmap_data * bmp);
    Sint32 run ();

  private:
    void initialize_palette();
    void build_menu_box(bitmap_data * bmp, Uint32 width);
    void load_text_file();
    void draw (char *source, Sint32 xcoord, Sint32 ycoord,
               Sint32 raw_src, Sint32 width, Sint32 height);
    void display_640 ();
    void display_320 ();
    static const unsigned char cycling_table[];
    static char **menu_texts_pt[2];

  };
#endif
