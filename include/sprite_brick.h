/** 
 * @file sprite_brick.h
 * @brief The sprite of the brick 
 * @created 2007-09-12
 * @date 2007-09-15
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
#ifndef __SPRITE_BRICK__
#define __SPRITE_BRICK__

class sprite_brick;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
class sprite_brick:public sprite_object
{
private:
  /** Color of brick */
  Uint32 original_color;
  Uint32 current_color;

public:
  sprite_brick ();
  ~sprite_brick ();
  void update_image (Uint32 h_pos);
  void set_color (Uint32 color); 
  void touch ();
  bool is_cycling();
  virtual void draw ();
};
#endif
