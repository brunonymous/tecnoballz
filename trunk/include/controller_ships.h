/** 
 * @file controller_ships.h
 * @brief Ship controller 
 * @date 2007-02-13
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
#ifndef __CONTROLLER_SHIPS__
#define __CONTROLLER_SHIPS__
class controller_ships;

#include "../include/list_sprites.h"
#include "../include/sprite_ship.h"
#include "../include/objects_list.h"
#include "../include/handler_display.h"
#include "../include/controller_moneys.h"
#include "../include/controller_gems.h"
#include "../include/controller_bricks.h"

class controller_ships:public objects_list < sprite_ship, controller_ships >
{
  friend class sprite_ship;
private:
  Sint32 apparition[4];
  Sint32 reappTemps;
  Sint32 power_init;
public:
    controller_ships ();
   ~controller_ships ();
  void initialise (Sint32 time0, Sint32 time1, Sint32 time2,
                   Sint32 time3, Sint32 time4, Sint32 power);
  void move ();
  void force_explosion ();
};
#endif
