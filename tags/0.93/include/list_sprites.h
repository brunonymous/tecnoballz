/** 
 * @file list_sprites.h
 * @brief Call the drawing methods of all sprites
 * @date 2007-09-15
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
#ifndef __LIST_SPRITES__
#define __LIST_SPRITES__

class list_sprites;
#include "../include/sprite_object.h"
#include "../include/tecnoballz.h"

class list_sprites:public tecnoballz
{
private:
  Sint32 num_of_shapes;
  Sint32 num_of_shadows;
  Sint32 max_of_shapes;
  sprite_object **shapes;
  sprite_object **shadows;

public:
    list_sprites ();
   ~list_sprites ();
  void init (Uint32 numof);
  void reset ();
  Uint32 get_sprites_remaining ();
  void add (sprite_object * sprite);
  void draw ();
  void clear ();
};

extern list_sprites *sprites;

#endif
