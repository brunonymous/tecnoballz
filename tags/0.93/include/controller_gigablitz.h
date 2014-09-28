/** 
 * @file controller_gigablitz.h
 * @brief Gigablitz controller 
 * @date 2007-04-03
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
#ifndef __CONTROLLER_GIGABLITZ__
#define __CONTROLLER_GIGABLITZ__
class controller_gigablitz;

#include "../include/sprite_gigablitz.h"
#include "../include/list_sprites.h"
#include "../include/right_panel_score.h"
#include "../include/handler_keyboard.h"
#include "../include/controller_viewfinders.h"
#include "../include/controller_projectiles.h"
#include "../include/handler_players.h"
#include "../include/supervisor_bricks_level.h"
#include "../include/objects_list.h"
#include "../include/controller_bricks.h"
#include "../include/controller_explosions.h"

class controller_gigablitz:public objects_list < sprite_gigablitz, controller_gigablitz >
{
  friend class sprite_gigablitz;

private:
  static const Uint32 MAX_OF_GIGABLITZ = 7;
  static Uint32 numeroBOBs[MAX_OF_GIGABLITZ];
  sprite_paddle *paddle_bottom;
  sprite_paddle *paddle_top;
  sprite_gigablitz *current_gigablitz;
  Sint32 blitz_colx;
  Sint32 gigablitz_xcoord;
  /* Height of the gigablitz in pixels */
  Uint32 gigablitz_height;
  Sint32 bitz_ystop;            //"gigablitz" Y max.
  Sint32 bitz_maxiy;            // 
  Sint32 bitz_miniy;            // 
  Sint32 blitz_seta;
  Uint32 num_of_bricks;            //width of collison bricks in row
  Uint32 blitz_xsin;

public:
    controller_gigablitz ();
   ~controller_gigablitz ();
  void create_gigablitz_sprites ();
  void shoot_paddle ();
  void run_in_bricks_levels ();
  void collision1 ();
  void run_in_guardians_level ();
  bool shoot_guardian (Uint32 id, Sint32 xcoord, Sint32 ycoord,
                      Uint32 width);
  bool is_enable ();
private: 
  void collision_with_paddle ();
};
#endif
