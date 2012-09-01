/** 
 * @file sprite_mouse_pointer.h
 * @brief The sprite of the mouse pointer 
 * @date 2007-02-05
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_mouse_pointer.h,v 1.2 2007-09-12 06:32:48 gurumeditation Exp $
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
#ifndef __SPRITE_MOUSE_POINTER__
#define __SPRITE_MOUSE_POINTER__
class sprite_mouse_pointer;
#include "../include/sprite_object.h"
class sprite_mouse_pointer:public sprite_object
{
public:
  sprite_mouse_pointer ();
  ~sprite_mouse_pointer ();
  void create_pointer_sprite (bitmap_data * bmp);
  void move ();
};
#endif
