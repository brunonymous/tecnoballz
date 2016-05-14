/**
 * @file sprite_wall.cc 
 * @brief The sprite of the wall used in bricks level 
 * @created 2007-1O-04 
 * @date 2007-1O-04
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
#include "../include/sprite_wall.h"


sprite_wall * sprite_wall::sprite_wall_singleton = NULL;

/**
 * Create the object
 */
sprite_wall::sprite_wall()
{
  clear_sprite_members();
  thecounter = 0;
}

/**
 * Release the object
 */
sprite_wall::~sprite_wall()
{
  sprite_wall_singleton = NULL;
}

/**
 * Get the object instance
 * sprite_wall is a singleton
 * @return the sprite_wall object 
 */
sprite_wall *
sprite_wall::get_instance ()
{
  if (NULL == sprite_wall_singleton)
    {
      sprite_wall_singleton = new sprite_wall ();
    }
  return sprite_wall_singleton;
}

/**
 * Enable the wall
 * @param counter Delay counter before disabling the wall
 */
void
sprite_wall::enable (Uint32 counter)
{
    thecounter += counter;
    is_enabled = true;
}

/**
 * Disable the wall
 */
void
sprite_wall::disable ()
{
    thecounter = 0;
    is_enabled = false;
}





