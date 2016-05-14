/**
 * @file sprite_ejector.cc
 * @brief Sprite ejector located in the corners
 * @created 2015-04-10
 * @date 2015-05-14
 * @copyright 1991-2015 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2015 TLK Games all rights reserved
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
#include "../include/sprite_ejector.h"

/**
 * Create the object
 */
sprite_ejector::sprite_ejector (Sint32 ball_offx, Sint32 ball_offy)
{
  ball_offsetx = ball_offx * resolution;
  ball_offsety = ball_offy * resolution;
  if (resolution == 2)
    {
      ball_offsetx++;
      ball_offsety++;
    }
  clear_sprite_members ();
}

/**
 * Release the object
 */
sprite_ejector::~sprite_ejector ()
{
}

/**
 * Places the ball on the ejector
 * @param ball pointer to sprite_ball object
 */
void
sprite_ejector::stick (sprite_ball * ball)
{
  ball->pull (this, ball_offsetx, ball_offsety);
}
