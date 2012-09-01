/** 
 * @file controller_font_menu.h
 * @brief Controller of the menu scroll text 
 * @date 2007-11-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_font_menu.h,v 1.2 2007-11-18 16:13:19 gurumeditation Exp $
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
#ifndef __CONTROLLER__FONTS_MENU__
#define __CONTROLLER__FONTS_MENU__

#include "../include/objects_list.h"
#include "../include/sprite_font_menu.h"
class controller_font_menu:public objects_list < sprite_font_menu, controller_font_menu >
{
private:
  /** Numbers maximum characters */
  static const Uint32 MAX_OF_FONTS = 14;
  static char asciiToBob[128];

private:
  sprite_font_menu * characters_list[MAX_OF_FONTS + 2];
  sprite_font_menu *objectLeft;
  sprite_font_menu *objectRigh;
  sprite_font_menu **object_ptr;
  char *scroll_ptr;
  Sint32 offset_xx1;
  Sint32 offset_yy1;
  char* scrollText;
  char** scrolltexts;

public:
    controller_font_menu ();
   ~controller_font_menu ();
  void create_fontes_list ();
  void move_chars ();

};
#endif
