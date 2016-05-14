/**
 * @file sprite_ship.cc 
 * @brief A flying enemy ships sprite 
 * @date 2007-11-03
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
#include "../include/sprite_ship.h"

/**
 * Create the flying enemy ships sprite
 */
sprite_ship::sprite_ship ()
{
  atom_xmini = ATOM_XMINI * resolution;
  atom_xmaxi = ATOM_XMAXI * resolution;
  atom_ymini = ATOM_YMINI * resolution;
  atom_ymaxi = ATOM_YMAXI * resolution;
  clear_sprite_members ();
  littleInit (0, 0, 0, 0, 0, 0, 0);
}

/**
 * Release the flying enemy ships sprite
 */
sprite_ship::~sprite_ship ()
{}

/**
 * Displacement of the ship
 */
void
sprite_ship::move ()
{
  /* explosion of the ship */
  if (is_exploding)
    {
      is_exploding = play_animation_once ();
      return;
    }

  /*
   * the ship is not currently enable 
   */
  if (enable_counter > 0)
    {
      /* the ship appears ? */
      if (--enable_counter == 0)
        {
          if (!is_collisions_with_bricks (x_coord, y_coord))
            {
              is_enabled = true;
              Sint32 *monPT = ghost_bobs + (random_counter & 31);
#if __WORDSIZE == 64
              random_counter += (long) this;
#else
              random_counter += (Sint32) this;
#endif
              Sint32 k = *monPT;
              frame_index_min = k;
              frame_index = k;
              frame_index_max = k + ATOM_ANIMA - 1;
              sprite_has_shadow = 1;
              set_image (k);
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::SHIP_APPEAR);
#endif
            }
          else
            {
              enable_counter = 1;
            }
        }
    }

  /*
   * the ship change its trajectory
   */
  if (!(--atom_count))
    {
      atom_ghost++;
      /* 16 values possible of standby */
      atom_ghost &= 15;
      Sint32 *depla = tableGhost + atom_ghost;
      atom_count = *depla;
      atom_traje += 2;
      /* 16 values possible of trajectory */
      atom_traje &= 30;
      depla = ghost_traj + atom_traje;
      x_offset = *(depla++) * resolution;
      y_offset = *depla * resolution;
      if (object_pos > 3)
        {
          x_offset = -x_offset;
          y_offset = -y_offset;
        }

    }
  play_animation_loop ();

  /* initialize the coordinates of the ship */
  Sint32 x = x_coord + x_offset;
  if (x < atom_xmini)
    {
      x = atom_xmini;
    }
  else if (x > atom_xmaxi)
    {
      x = atom_xmaxi;
    }
  Sint32 y = y_coord + y_offset;
  if (y < atom_ymini)
    {
      y = atom_ymini;
    }
  else if (y > atom_ymaxi)
    {
      y = atom_ymaxi;
    }
  if (!is_collisions_with_bricks (x, y) || enable_counter)
    {
      x_coord = x;
      y_coord = y;
    }
}

/**
 * Check collisions between a ship and bricks
 * @param xcoord x coordinate of the ship
 * @param ycoord y coordinate of the ship
 * @return true if collision detected, otherwise false
 */
bool
sprite_ship::is_collisions_with_bricks (Uint32 xcoord, Uint32 ycoord)
{
  controller_bricks* bricks = controller_bricks::get_instance ();
  Sint32 brick_width = bricks->get_brick_width ();
  Sint32 y_space = bricks->getYOffset ();
  brick_info *bricks_map = bricks->get_bricks_map ();
  Sint32 c = 0;
  for (Sint32 i = 0; i < 4; i++)
    {
      Sint32 x = xcoord + collisionT[c++];
      Sint32 y = ycoord + collisionT[c++];
      /* width of brick: 32 pixels */
      x /= brick_width;
      /* y-offset between 2 bricks: 16 lines */
      y /= y_space;
      y *= controller_bricks::MAX_OF_BRICKS_HORIZONTALLY;
      x += y;
      brick_info *map = (bricks_map + x);
      /* collision between the ship and a brick? */
      if (map->source_offset != 0)
        {
          return true;
        }
    }
  return false;
}

/**
 * Perform some initializations
 * @param time0
 * @param appar Time before the first activation
 * @param index Number of the table of standby values (0 to 3)
 * @param power Strength
 * @param pos_x X-coordinate 
 * @param pos_y Y-coordinate
 * @param offst Number of the image's explosion
 */
void
sprite_ship::littleInit (Sint32 time0, Sint32 appar, Sint32 index,
                         Sint32 power, Sint32 pos_x, Sint32 pos_y,
                         Sint32 offst)
{
  enable_counter = appar;
  apparition = time0;
  tableGhost = ghost_wait[index];       //table of the 16 standby values
  strength = power;
  strength_initial = power;
  x_coord = pos_x;
  y_coord = pos_y;
  atom_oexpl = offst;           //number of the image's explosion
  is_exploding = false;
  atom_ghost = 0;               //pointer on "tableGhost"
  x_offset = 0;
  y_offset = 0;
  Sint32 i = random_counter & 15;
  i++;
  i *= object_pos;
  atom_count = i;               //trajectory change counter
  frame_period = 3;             //rate animation
  frame_delay = 1;              //rate animation counter
  i *= 2;
  i &= 31;
  atom_traje = i;
#if __WORDSIZE == 64
  random_counter += (long) this;
#else
  random_counter += (Sint32) this;
#endif
  i = 4 * resolution;

  collisionT[0] = i;
  collisionT[1] = i;

  collisionT[2] = collision_width - i;
  collisionT[3] = i;

  collisionT[4] = collision_width - i;
  collisionT[5] = collision_height - i;

  collisionT[6] = i;
  collisionT[7] = collision_height - i;

}

/**
 * Ship destroyed by a projectile: send gem, money or malus
 * @param blast a pointer to a prjectile sprite
 */
void
sprite_ship::destroy (sprite_projectile * blast)
{
  if (enable_counter > 0)
    {
      return;
    }
  explode ();
  Sint32 h = bonus_or_penalty[random_counter & 0xF];
  //h = CODE_GEMME; //test only
  switch (h)
    {
    case CODE_GEMME:
    {
      controller_gems* gems = controller_gems::get_instance ();
      gems->send (blast);
    }
    break;
    case CODE_MALUS:
    {
      controller_capsules* capsules = controller_capsules::get_instance ();
      capsules->send_penalty_or_bonus (blast);
    }
    break;
    case CODE_MONEY:
    {
      controller_moneys* moneys = controller_moneys::get_instance ();
      moneys->send_money (blast);
    }
    break;
    }
}

/**
 * Ship destroyed by a ball: send gem, money or malus
 * @param ball a pointer to a balll sprite
 */
void
sprite_ship::destroy (sprite_ball * ball)
{
  if (enable_counter > 0)
    {
      return;
    }
  explode ();
  Uint32 h = bonus_or_penalty[random_counter & 0xF];
  //h = CODE_GEMME; //test only
  switch (h)
    {
    case CODE_GEMME:
    {
      controller_gems* gems = controller_gems::get_instance ();
      gems->send (ball);
    }
    break;
    case CODE_MALUS:
    {
      controller_capsules* capsules = controller_capsules::get_instance ();
      capsules->send_penalty_or_bonus (ball);
    }
    break;
    case CODE_MONEY:
    {
      controller_moneys* moneys = controller_moneys::get_instance ();
      moneys->send_money (ball);
    }
    break;
    }
}

/**
 * Explode the ship
 */
void
sprite_ship::explode ()
{
  sprite_has_shadow = false;
  random_counter = random_counter + frame_index;
  is_exploding = true;
  strength = strength_initial;
  /* time before activation */
  enable_counter = apparition + (random_counter & 63);
  frame_index = atom_oexpl;
  frame_index_min = atom_oexpl;
  frame_index_max = atom_oexpl + ATOM_ANIMA - 1;
  set_image (frame_index);
  frame_delay = frame_period;
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::SHIP_EXPLODE);
#endif
}

/** Values of possible time during which a BouiBoui keep the same 
 *  trajectory */
Sint32 sprite_ship::ghostWait1[16] =
  { 50, 50, 30, 50, 20, 12, 8, 50,
    45, 32, 12, 56, 33, 22, 22, 20
  };
Sint32 sprite_ship::ghostWait2[16] =
  { 40, 20, 30, 47, 22, 33, 50, 50,
    55, 52, 42, 32, 42, 32, 42, 32
  };
Sint32 sprite_ship::ghostWait3[16] =
  { 10, 10, 20, 27, 12, 13, 20, 10,
    10, 20, 10, 25, 26, 12, 32, 33
  };
Sint32 sprite_ship::ghostWait4[16] =
  { 60, 20, 50, 37, 42, 33, 33, 20,
    20, 22, 33, 70, 20, 22, 32, 32
  };
Sint32 * sprite_ship::ghost_wait[4] =
  { ghostWait1, ghostWait2, ghostWait3, ghostWait4
  };

//-----------------------------------------------------------------------------
// offsets x, y
//-----------------------------------------------------------------------------
Sint32
sprite_ship::ghost_traj[32] = { 0, 1,
                                0, 0,
                                -1, 0,
                                0, 0,
                                0, -1,
                                -1, 1,
                                -1, -1,
                                -1, 0,
                                -1, 1,
                                1, -1,
                                1, -1,
                                1, 1,
                                1, 1,
                                1, 0,
                                0, 1,
                                0, 1
                              };

//-----------------------------------------------------------------------------
// index animations images of sprites (20 BouiBouis differents)
//-----------------------------------------------------------------------------
Sint32
sprite_ship::ghost_bobs[32] =
  { 0 * ATOM_ANIMA, 1 * ATOM_ANIMA, 2 * ATOM_ANIMA, 3 * ATOM_ANIMA,
    4 * ATOM_ANIMA, 5 * ATOM_ANIMA, 6 * ATOM_ANIMA, 7 * ATOM_ANIMA,
    8 * ATOM_ANIMA, 9 * ATOM_ANIMA, 10 * ATOM_ANIMA, 11 * ATOM_ANIMA,
    12 * ATOM_ANIMA, 13 * ATOM_ANIMA, 14 * ATOM_ANIMA, 15 * ATOM_ANIMA,
    16 * ATOM_ANIMA, 17 * ATOM_ANIMA, 18 * ATOM_ANIMA, 19 * ATOM_ANIMA,
    0 * ATOM_ANIMA, 1 * ATOM_ANIMA, 2 * ATOM_ANIMA, 3 * ATOM_ANIMA,
    4 * ATOM_ANIMA, 5 * ATOM_ANIMA, 6 * ATOM_ANIMA, 7 * ATOM_ANIMA,
    8 * ATOM_ANIMA, 9 * ATOM_ANIMA, 10 * ATOM_ANIMA, 11 * ATOM_ANIMA
  };

/**
 * Bonus or penalty code when a ship is destroyed
 */
Sint32
sprite_ship::bonus_or_penalty[16] = { CODE_GEMME,
                                      CODE_MONEY,
                                      CODE_MALUS,
                                      CODE_MONEY,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MONEY,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MONEY,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MALUS,
                                      CODE_MONEY
                                    };
