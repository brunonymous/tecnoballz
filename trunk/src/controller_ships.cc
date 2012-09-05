/** 
 * @file controller_ships.cc 
 * @brief Ship controller 
 * @date 2007-10-17
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
#include "../include/controller_ships.h"

/**
 * Create a ships controller
 */
controller_ships::controller_ships ()
{
  littleInit ();
  max_of_sprites = 8;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::SHIPS;
}

/**
 * Release a ships controller
 */
controller_ships::~controller_ships ()
{
  release_sprites_list ();
}

/**
 * Initialize the ships
 * @param time0 time delay before the first apparition 
 * @param time1 time delay before the first apparition 
 * @param time2 time delay before the first apparition
 * @param time3 time delay before the first apparition
 * @param time4 time delay before the first apparition
 * @param power strength
 */
void
controller_ships::initialise (Sint32 time0, Sint32 time1, Sint32 time2,
                              Sint32 time3, Sint32 time4, Sint32 power)
{
  reappTemps = time0;
  apparition[0] = time1;
  apparition[1] = time2;
  apparition[2] = time3;
  apparition[3] = time4;
  power_init = power;
  Sint32 x = sprites_list[0]->atom_xmini;
  Sint32 y = sprites_list[0]->atom_ymini;
  Sint32 j = 0;
  Sint32 offs1 = 20 * sprite_ship::ATOM_ANIMA;
  Sint32 offs2 = 21 * sprite_ship::ATOM_ANIMA;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ship *ship = sprites_list[i];
      ship->littleInit (
        /* time delay before activation */
        time0,
        /* time delay before first activation */
        apparition[j] + i,
        /* table number of standby values, from 0 to 3 */
        j,
        /* strength */
        power,
        /* x coordinate */
        x + (i * 16 * resolution),
        /* y coordinate */
        y + (i * 16 * resolution),
        /* image number of the explosion */
        offs1);
      j++;
      j &= 3;
      Sint32 k = offs2;
      offs2 = offs1;
      offs1 = k;
    }
}

/**
 * Move all ships
 */
void
controller_ships::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ship *ship = sprites_list[i];
      ship->move ();
    }
}

/**
 * Force the explosion of all ships
 */
void
controller_ships::force_explosion ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_ship *ship = sprites_list[i];
      ship->explode ();
    }
}
