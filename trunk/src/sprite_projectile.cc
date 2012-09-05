/**
 * @file sprite_projectile.cc 
 * @brief The fire sprite of the paddle into the bricks level
 * @date 2007-11-16
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
#include "../include/sprite_projectile.h"

/**
 * Create the fire sprite object
 */
sprite_projectile::sprite_projectile ()
{
  if (total_fire < MAXI_TOTAL_OF_PROJECTILES)
    {
      projectiles_list[total_fire] = this;
      total_fire++;
    }
  set_draw_method (DRAW_COLOR_CYCLING_MASK);
  on_paddle = false;
}

/**
 * Release the fire sprite object
 */
sprite_projectile::~sprite_projectile ()
{
}

/**
 * Enable the power 1 of the projectiles
 */
void
sprite_projectile::set_power1 ()
{
  frame_index_min = 4;
  frame_index_max = 7;
  frame_index = 4;
  can_destroy_indestructible = false;
  power = 1;
  current_cycling = &sprite_object::cycling_02[0];
}

/**
 * Enable the power 2 of the projectiles
 */
void
sprite_projectile::set_power2 ()
{
  frame_index_min = 0;
  frame_index_max = 3;
  frame_index = 0;
  can_destroy_indestructible = true;
  power = 2;
  current_cycling = &sprite_object::cycling_01[0];
}

/**
 * Clear member a simple
 */
void
sprite_projectile::init_members (sprite_paddle * pad)
{
  paddle = pad;
  indexSinus = 0;
  fire_Xscie = 0;
  fire_Yscie = 0;
  frame_index = 0;
  frame_index_max = 3;
  frame_index_min = 0;
  frame_delay = 10;
  frame_period = 10;
  can_destroy_indestructible = false;
  power = 0;
}

/**
 * Static method which initialize all projectiles before a bricks level
 */
void
sprite_projectile::start_list ()
{
  total_fire = 0;
  for (Uint32 i = 0; i < MAXI_TOTAL_OF_PROJECTILES; i++)
    {
      projectiles_list[i] = NULL;
    }
}

/**
 * Static method which manage all projectiles
 */
void
sprite_projectile::gestionTir ()
{
  check_outside ();
  play_projectiles_animations ();
  check_collisions_with_bricks ();
  check_collisions_with_ships ();
}

/**
 * Static method which check if all projectiles go out of the screen of game 
 */
void
sprite_projectile::check_outside ()
{
  sprite_projectile **projectiles = projectiles_list;
  Sint32 y1 = 15 * resolution;
  Sint32 y2 = 232 * resolution;
  Sint32 x1 = 15 * resolution;
  Sint32 x2 = 228 * resolution;
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      Sint32 a = blast->y_coord;
      if (a < y1 || a > y2)
        {
          blast->is_enabled = false;
          continue;
        }
      a = blast->x_coord;
      if (a < x1 || a > x2)
        {
          blast->is_enabled = false;
        }
    }
}

/**
 * Static method which manages the animation played in loop-mode
 */
void
sprite_projectile::play_projectiles_animations ()
{
  sprite_projectile **projectiles = projectiles_list;
  sprite_projectile *blast = projectiles[0];
  blast->play_animation_loop ();
  Sint32 index = blast->get_frame_index ();
  Sint32 cycle = index & 0X1;
  if (0 == cycle)
    {
      cycle = sprite_object::DRAW_WITH_TABLES;
    }
  else
    {
      cycle = sprite_object::DRAW_COLOR_CYCLING_MASK;
    }
  blast->draw_method = cycle;
  for (Uint32 i = 1; i < total_fire; i++)
    {
      blast = projectiles[i];
      blast->set_image (index);
      blast->draw_method = cycle;
    }
}

/**
 * Static method which check collision projectiles between bricks  
 */
void
sprite_projectile::check_collisions_with_bricks ()
{
  controller_bricks *bricks = controller_bricks::get_instance ();
  /* brick's width in pixels */
  Uint32 brick_width = bricks->get_brick_width ();
  /* y-offset between 2 bricks */
  /* first indestructible brick */
  Sint32 indestructible = bricks->get_indestructible_offset ();
  sprite_projectile **projectiles = projectiles_list;
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      if (!blast->is_enabled)
        {
          continue;
        }
      Sint32 x = blast->x_coord + 2;
      Sint32 y = blast->y_coord + 2;
      brick_redraw *redraw = bricks->get_bricks_redraw ();
      redraw->xcoord_collision = x;
      redraw->ycoord_collision = y;
      brick_info *map = bricks->get_bricks_map(x, y);

      /* collision between a blast and a brick? */
      if (map->source_offset == 0)
        {
          /* no collision */
          continue;
        }
      if (!blast->on_paddle)
        {
          blast->is_enabled = false;
        }
      redraw->paddle = blast->paddle;
      redraw->is_gigablitz_destroyed = false;
      if (!has_background)
        {
          map->sprite->touch ();
        }
      if (map->source_offset >= indestructible)
        {
          /* 
           * indestructible brick touched!
           */
          /* indestructible-destructible bricks? */
          if (map->source_offset > (Sint32)(indestructible + brick_width))
            {
              /* fire destroys the indestructibles-destructibles bricks? */
              if (blast->can_destroy_indestructible)
                {
                  redraw->is_indestructible = true;
                  redraw->pixel_offset = map->pixel_offset;
                  redraw->brick_map = map;
                  map->source_offset = 0;
                  redraw->number = map->number;
                  /* restore background under brick */
                  redraw->is_background = true;
                  bricks->bricks_redraw_next ();
                }
              else
                {
#ifndef SOUNDISOFF
                  audio->
                    play_sound (handler_audio::HIT_INDESTRUCTIBLE_BRICK2);
#endif
                }
            }
          else
            {
              /* the brick is really indestructible */
#ifndef SOUNDISOFF
              audio->play_sound (handler_audio::HIT_INDESTRUCTIBLE_BRICK1);
#endif
            }
        }
      /* 
       * Normal brick touched
       */
      else
        {
          redraw->is_indestructible = false;
          redraw->pixel_offset = map->pixel_offset;
          redraw->brick_map = map;
          /* fire power: 1 or 2 */
          x = blast->power;
          map->h_pos = map->h_pos - (x * 2);
          if (map->h_pos <= 0)
            {
              map->h_pos = 0;
              map->source_offset = 0;
              redraw->number = map->number;
              /* restore background under brick */
              redraw->is_background = true;
            }
          else
            {
              map->source_offset = map->source_offset - (x * brick_width);
              redraw->number = map->source_offset;
              /* redraw a new brick */
              redraw->is_background = false;
            }
          bricks->bricks_redraw_next ();
        }
    }
}

/**
 * Static method which check collisions between projectiles and ships 
 */
void
sprite_projectile::check_collisions_with_ships ()
{
  sprite_projectile **projectiles = projectiles_list;
  controller_ships *ships = controller_ships::get_instance ();
  Sint32 t = ships->get_max_of_sprites ();
  sprite_ship **ships_list = ships->get_sprites_list ();
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      if (!blast->is_enabled)
        {
          continue;
        }
      sprite_ship **ships = ships_list;
      Sint32 y1 = blast->y_coord;
      Sint32 y2 = y1 + 3;
      y1 -= 26;
      Sint32 x1 = blast->x_coord;
      Sint32 x2 = x1 + 3;
      x1 -= 20;
      for (Sint32 j = 0; j < t; j++)
        {
          sprite_ship *ship = *(ships++);
          if (ship->enable_counter > 0)
            {
              continue;
            }
          Sint32 k = ship->y_coord;
          if (k >= y2 || k <= y1)
            {
              continue;
            }
          k = ship->x_coord;
          if (k >= x2 || k <= x1)
            {
              continue;
            }
          if (!blast->on_paddle)
            {
              blast->is_enabled = false;
            }
          current_player->add_score (100);
          k = blast->power;
          ship->strength -= k;
          if (ship->strength < 1)
            {
              ship->destroy (blast);
            }
        }
    }
}

/**
 * Static method which disables all projectiles
 */
void
sprite_projectile::disable_sprites ()
{
  sprite_projectile **projectiles = projectiles_list;
  for (Uint32 i = 0; i < total_fire; i++)
    {
      sprite_projectile *blast = *(projectiles++);
      blast->is_enabled = false;
    }
}

Uint32 sprite_projectile::total_fire = 0;
sprite_projectile *
  sprite_projectile::projectiles_list[MAXI_TOTAL_OF_PROJECTILES];
