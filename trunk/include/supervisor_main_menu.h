/**
 * @file supervisor_main_menu.h
 * @brief TecnoballZ's main menu supervisor 
 * @date 2007-10-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_main_menu.h,v 1.13 2007-10-11 05:20:26 gurumeditation Exp $
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
#ifndef __SUPERVISOR__MAIN_MENU__
#define __SUPERVISOR__MAIN_MENU__

#include "../include/tilesmap_scrolling.h"
#include "../include/supervisor.h"
#include "../include/handler_keyboard.h"
#include "../include/sprite_object.h"
#include "../include/list_sprites.h"
#include "../include/controller_font_menu.h"
#include "../include/display_text_bitmap.h"
#include "../include/sprite_display_menu.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/handler_audio.h"

class supervisor_main_menu:public virtual supervisor
  {
  public:
    static const Uint32 AREA_CODE_LENGTH = 10;

  private:
    tilesmap_scrolling * tiles_map;
    /** Big TecnoballZ logo at the top of the screen */
    sprite_object *tecnoballz_logo;
    /** Scrolltext at the bottom of the screen */
    controller_font_menu *font_scrolling;
    /** Handle the text of the main menu */
    sprite_display_menu *text_menu;
    /** Angle from 0 to 511 used for the horizontal displacement
     * of the TecnoballZ logo */
    Uint32 tecnoballz_logo_angle;
    /** Sprite of the mouse cursor */
    sprite_mouse_pointer *mouse_pointer;
    /** All areas code for every areas and every difficulty levels */
    static const char area_codes[241];
    /** Current input area code used to jump
     * directly to the end of a area */
    static char current_area_code[AREA_CODE_LENGTH + 1];

  public:
    supervisor_main_menu ();
    ~supervisor_main_menu ();
    void first_init ();
    Uint32 main_loop ();
    static const char *get_area_code (Uint32 aera_num, Uint32 difficulty);
    static char *get_current_area_code ();
    static void copy_current_area_code (char *destination);

  private:
    void move_tecnoballz_logo ();
    Sint32 start_new_game ();
    Uint32 check_area_code ();
  };
#endif
