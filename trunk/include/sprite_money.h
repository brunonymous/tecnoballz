/** 
 * @file sprite_money.h
 * @brief The money sprite 
 * @date 2007-04-13
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
#ifndef __SPRITE_MONEY__
#define __SPRITE_MONEY__
class sprite_money;

#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/right_panel_score.h"
#include "../include/handler_display.h"
#include "../include/controller_bricks.h"
#include "../include/sprite_ball.h"
#include "../include/sprite_projectile.h"

class sprite_money:public sprite_object
{
  friend class controller_moneys;

private:
  /** Toward bottom, right, top, left */
  Uint32 towards;
  /** Amount of money contained in this capsule */
  Uint32 money_amount;
  /** Moving speed in pixels of the money capsule */
  Uint32 speed_of_moving;
  /** Paddle sprite which goes the money caspule */
  sprite_paddle *paddle;
  /** Multiply the amount of money collected in the capsule
   * by the paddle */ 
  Uint32 money_multiplier;

public:
    sprite_money ();
   ~sprite_money ();
  void init_members ();
  bool enable_if_available (brick_redraw * briPT);
  bool enable_if_available (sprite_ball * pball);
  bool enable_if_available (sprite_projectile * pfire);
  Uint32 move ();
  Uint32 move_bottom ();
private:
  void init_money (Uint32 xcoord, Uint32 ycoord, sprite_paddle * pad);
};
#endif
