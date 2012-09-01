/**
 * @file sprite_ship.h
 * @brief A flying enemy ships sprite
 * @date 2007-11-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.10 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_ship.h,v 1.10 2007-11-18 16:13:19 gurumeditation Exp $
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
#ifndef __SPRITE_SHIP__
#define __SPRITE_SHIP__

class sprite_ship;

#include "../include/sprite_object.h"
#include "../include/handler_audio.h"
#include "../include/controller_moneys.h"
#include "../include/controller_capsules.h"
#include "../include/controller_gems.h"
#include "../include/sprite_projectile.h"
#include "../include/sprite_ball.h"
#include "../include/controller_bricks.h"

enum
{ CODE_GEMME,
  CODE_MALUS,
  CODE_MONEY
};

class sprite_ship:public sprite_object
  {
    friend class controller_ships;
    friend class sprite_projectile;
    friend class controller_balls;

  private:
    static const Sint32 ATOM_XMINI = 32;
    static const Sint32 ATOM_XMAXI = 191;
    static const Sint32 ATOM_YMINI = 40;
    static const Sint32 ATOM_YMAXI = 191;
    static const Sint32 ATOM_ANIMA = 8;

  private:
    /** Strength delay counter */
    Sint32 strength;
    /** Initial strength delay */
    Sint32 strength_initial;
    Sint32 x_offset;            // offset X
    Sint32 y_offset;            // offset Y
    Sint32 atom_count;            // trajectory change counter
    Sint32 atom_ghost;            // pointer on "tableGhost"
    Sint32 *tableGhost;           // table time keep same trajectory
    /** counter activation, if 0 ship is enabled */
    Sint32 enable_counter;
    /** True if the ship exploding */
    bool is_exploding;
    Sint32 atom_oexpl;            // number of the first image's explosion
    Sint32 atom_traje;            // pointeur sur la trajectoir
    Sint32 apparition;            // time before reactivation
    Sint32 atom_xmini;
    Sint32 atom_xmaxi;
    Sint32 atom_ymini;
    Sint32 atom_ymaxi;
    Sint32 collisionT[8];

    static Sint32 ghostWait1[16];
    static Sint32 ghostWait2[16];
    static Sint32 ghostWait3[16];
    static Sint32 ghostWait4[16];
    static Sint32 *ghost_wait[4];
    static Sint32 ghost_traj[32];
    static Sint32 ghost_bobs[32]; // index animations images of sprites
    static Sint32 bonus_or_penalty[16];

  public:
    sprite_ship ();
    ~sprite_ship ();
    void move ();
    void littleInit (Sint32 time0, Sint32 appar, Sint32 index,
                     Sint32 power, Sint32 pos_x, Sint32 pos_y, Sint32 offst);
    void destroy (sprite_projectile *);
    void destroy (sprite_ball *);
    void explode ();

  private:
    bool is_collisions_with_bricks (Uint32 xcoord, Uint32 ycoord);


  };
#endif
