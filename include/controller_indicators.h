/** 
 * @file controller_indicators.h
 * @brief Controller of money amount, reverse penalty   
 * @created 2002-11-28 
 * @date 2007-02-18
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
#ifndef __CONTROLLER_INDICATORS__
#define __CONTROLLER_INDICATORS__
class controller_indicators;

#include "../include/objects_list.h"
#include "../include/handler_players.h"
#include "../include/controller_paddles.h"
#include "../include/sprite_capsule.h"
#include "../include/sprite_money.h"

class controller_indicators:public objects_list < sprite_object,
  controller_indicators >
{
private:
  static const Uint32 YCOORD_INDICATORS = 232;
  Uint32 y_position;
  Uint32 money_posy;
  /** Sprite of the money capsule to the left of amount money */
  sprite_money *money_sprite;
  sprite_capsule *reverse_sprite;
  sprite_capsule *life_sprite;

public:
    controller_indicators ();
   ~controller_indicators ();
  void create_indicators_sprites ();
  void display_money_and_reverse ();
  void display_money_and_lifes ();
  void increase_money_amount (Uint32 amount);

private:
  void init_money ();
  void display_money_amount ();
};
#endif
