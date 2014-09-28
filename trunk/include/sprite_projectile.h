/** 
 * @file sprite_projectile.h 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-09-27
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
#ifndef __SPRITE_PROJECTILE__
#define __SPRITE_PROJECTILE__

class sprite_projectile;

#include "../include/sprite_object.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_bricks.h"
#include "../include/controller_ships.h"
#include "../include/sprite_ship.h"
#include "../include/right_panel_score.h"

class sprite_projectile:public sprite_object
{
  friend class controller_projectiles;
  friend class sprite_money;
  friend class sprite_capsule;
  friend class sprite_gem;

private:
  static const Uint32 MAXI_TOTAL_OF_PROJECTILES = 200;
  static Uint32 total_fire;
  static sprite_projectile *projectiles_list[MAXI_TOTAL_OF_PROJECTILES];
  /** Fire is on the paddle, it used only for the fire 7 */
  bool on_paddle;
  Sint32 indexSinus;
  Sint32 fire_Xscie;
  Sint32 fire_Yscie;
  /* Paddle to which the projectile belongs. Used for the capsules */
  sprite_paddle *paddle;
  /** If true projectile destroys the
   * indestructible-destructibles bricks */
  bool can_destroy_indestructible;
  /** Power of the projectile 1 or 2 */
  Uint32 power;

public:
    sprite_projectile ();
   ~sprite_projectile ();
  void init_members (sprite_paddle * pad);
  void set_power1 ();
  void set_power2 ();
  static void start_list ();
  static void gestionTir ();
  static void check_outside ();
  static void disable_sprites ();
private:
  static void play_projectiles_animations ();
  static void check_collisions_with_bricks ();
  static void check_collisions_with_ships ();
};
#endif
