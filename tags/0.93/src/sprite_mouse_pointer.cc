/** 
 * @file sprite_mouse_pointer.cc 
 * @brief The sprite of the mouse pointer 
 * @date 2007-10-21
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
#include "../include/sprite_mouse_pointer.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"
#include "../include/list_sprites.h"

/**
 * Create the mouse pointer sprite
 */
sprite_mouse_pointer::sprite_mouse_pointer ()
{
  clear_sprite_members ();
}

/**
 * Release the mouse pointer sprite
 */
sprite_mouse_pointer::~sprite_mouse_pointer ()
{
}

/**
 * Create the sprite data and initialize it
 */
void
sprite_mouse_pointer::create_pointer_sprite (bitmap_data * bmp)
{
  Uint32 id;
  if (random_counter & 1)
    {
      id = sprite_object::MOUSE_POINTER_1;
    }
  else
    {
      id = sprite_object::MOUSE_POINTER_2;
    }
  create_sprite (id, bmp, 0);
  sprites->add (this);
  Uint32 x = game_screen->get_width () / 2;
  Uint32 y = game_screen->get_height () / 2;
  set_coordinates (x, y);
  enable ();
  set_frame_delay (3);
}

/**
 * Moving the mouse pointer
 */
void
sprite_mouse_pointer::move ()
{
  Sint32 offsX = keyboard->get_mouse_x ();
  Sint32 offsY = keyboard->get_mouse_y ();
  move_x (offsX);
  move_y (offsY);
  set_coordinates (offsX, offsY);
  clip_coordinates ();
  play_animation_loop ();
}
