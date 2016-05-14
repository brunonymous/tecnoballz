/** 
 * @file controller_explosions.h
 * @brief Guardians explosions controller  
 * @created 2003-03-02 
 * @date 2007-02-08
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
#ifndef __CONTROLLER_EXPLOSIONS__
#define __CONTROLLER_EXPLOSIONS__

class controller_explosions;

#include "../include/list_sprites.h"
#include "../include/sprite_object.h"
#include "../include/objects_list.h"

class controller_explosions:public objects_list < sprite_object, controller_explosions >
{

public:
  controller_explosions ();
  ~controller_explosions ();
  void create_explosions_list ();
  void add (Uint32 xcoord, Uint32 ycoord);
  void play_animation ();

private:
  Uint32 sound_delay;
};
#endif
