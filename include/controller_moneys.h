/** 
 * @file controller_moneys.h
 * @brief Moneys controller 
 * @date 2007-04-13
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
#ifndef __CONTROLLER_MONEYS__
#define __CONTROLLER_MONEYS__

class controller_moneys;

#include "../include/list_sprites.h"
#include "../include/objects_list.h"
#include "../include/handler_display.h"
#include "../include/right_panel_score.h"
#include "../include/sprite_money.h"
#include "../include/right_panel_score.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/controller_indicators.h"
#include "../include/sprite_ball.h"

class controller_moneys:public objects_list < sprite_money, controller_moneys >
{
private:
  /** time delay before sending a new money capsule */
  Uint32 send_delay;
  /** delay counter before sending a new money capsule */
  Uint32 delay_count;
  right_panel_score *ptbarreScr;
  controller_indicators *ptPrntmney;

public:
    controller_moneys ();
   ~controller_moneys ();
  void initialize (Uint32 delay, right_panel_score * score, controller_indicators * money);
  void initialize (Uint32 delay, controller_indicators * money);
  void send_money_from_brick (brick_redraw * briPT);
  void send_money (sprite_ball * ball);
  void send_money (sprite_projectile * blast);
  void send_money_from_guardian (sprite_ball * ball);
  void move ();
  void move_bottom ();
};
#endif
