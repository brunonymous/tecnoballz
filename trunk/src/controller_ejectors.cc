/** 
 * @file controller_ejectors.cc 
 * @brief Ejectors corners controller 
 * @date 2007-10-21
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
#include "../include/controller_ejectors.h"

/**
 * Create the ejectors controller
 */
controller_ejectors::controller_ejectors ()
{
  littleInit ();
  max_of_sprites = MAX_OF_EJECTORS;
  sprites_have_shades = true;

}

/**
 * Release the ejectors controller
 */
controller_ejectors::~controller_ejectors ()
{
  release_sprites_list ();
}

/**
 * Create the ejectors sprites
 */
void
controller_ejectors::create_ejectors_sprites ()
{

  alloc_sprites_list ();

  sprites_list[TOP_LEFT_EJECTOR] = new sprite_object ();
  sprites_list[TOP_RIGHT_EJECTOR] = new sprite_object ();
  sprites_list[BOTTOM_LEFT_EJECTOR] = new sprite_object ();
  sprites_list[BOTTOM_RIGHT_EJECTOR] = new sprite_object ();

  sprites_list[TOP_LEFT_EJECTOR]->create_sprite (sprite_object::EJECTOR_1,
                                                 sprites_bitmap, true);
  sprites_list[TOP_LEFT_EJECTOR]->set_coordinates (COORD_EJECTOR_1 *
                                                   resolution,
                                                   (COORD_EJECTOR_1 *
                                                    resolution) -
                                                   sprites_list
                                                   [TOP_LEFT_EJECTOR]->
                                                   get_sprite_height () / 2);

  sprites_list[TOP_RIGHT_EJECTOR]->create_sprite (sprite_object::EJECTOR_4,
                                                  sprites_bitmap, true);
  sprites_list[TOP_RIGHT_EJECTOR]->set_coordinates (COORD_EJECTOR_2 *
                                                    resolution,
                                                    (COORD_EJECTOR_1 *
                                                     resolution) -
                                                    sprites_list
                                                    [TOP_RIGHT_EJECTOR]->
                                                    get_sprite_height () / 2);

  sprites_list[BOTTOM_LEFT_EJECTOR]->create_sprite (sprite_object::EJECTOR_2,
                                                    sprites_bitmap, true);
  sprites_list[BOTTOM_LEFT_EJECTOR]->set_coordinates (COORD_EJECTOR_1 *
                                                      resolution,
                                                      (COORD_EJECTOR_2 *
                                                       resolution) -
                                                      sprites_list
                                                      [BOTTOM_LEFT_EJECTOR]->
                                                      get_sprite_height () /
                                                      2);

  sprites_list[BOTTOM_RIGHT_EJECTOR]->create_sprite (sprite_object::EJECTOR_3,
                                                     sprites_bitmap, true);
  sprites_list[BOTTOM_RIGHT_EJECTOR]->set_coordinates (COORD_EJECTOR_2 *
                                                       resolution,
                                                       (COORD_EJECTOR_2 *
                                                        resolution) -
                                                       sprites_list
                                                       [BOTTOM_RIGHT_EJECTOR]->
                                                       get_sprite_height () /
                                                       2);

  /* has_background = false: ejectors are managed like sprites */  
  if (!has_background)
    {
      sprites->add (sprites_list[TOP_LEFT_EJECTOR]);
      sprites->add (sprites_list[TOP_RIGHT_EJECTOR]);
      sprites->add (sprites_list[BOTTOM_LEFT_EJECTOR]);
      sprites->add (sprites_list[BOTTOM_RIGHT_EJECTOR]);
      sprites_list[TOP_LEFT_EJECTOR]->enable ();
      sprites_list[TOP_RIGHT_EJECTOR]->enable ();
      sprites_list[BOTTOM_LEFT_EJECTOR]->enable ();
      sprites_list[BOTTOM_RIGHT_EJECTOR]->enable ();
    }
}

/**
 * Draw ejectors shadows
 */
void
controller_ejectors::draw_shadow ()
{
  if (!has_background)
    {
      return;
    }
  sprites_list[TOP_LEFT_EJECTOR]->draw_shadow_to_brackground ();
  sprites_list[TOP_RIGHT_EJECTOR]->draw_shadow_to_brackground ();
  sprites_list[BOTTOM_LEFT_EJECTOR]->draw_shadow_to_brackground ();
  sprites_list[BOTTOM_RIGHT_EJECTOR]->draw_shadow_to_brackground ();
}

/**
 * Draw ejectors shadows
 */
void
controller_ejectors::draw ()
{
  if (!has_background)
    {
      return;
    }
  sprites_list[TOP_LEFT_EJECTOR]->draw_to_brackground ();
  sprites_list[TOP_RIGHT_EJECTOR]->draw_to_brackground ();
  sprites_list[BOTTOM_LEFT_EJECTOR]->draw_to_brackground ();
  sprites_list[BOTTOM_RIGHT_EJECTOR]->draw_to_brackground ();
}

/**
 * Return a ejector sprite
 * @param id identifier of the ejector
 * @return a pointer to the ejector sprite
 */
sprite_object *
controller_ejectors::get_ejector (Uint32 id)
{
  switch (id)
    {
    case TOP_LEFT_EJECTOR:
      return sprites_list[TOP_LEFT_EJECTOR];
    case TOP_RIGHT_EJECTOR:
      return sprites_list[TOP_RIGHT_EJECTOR];
    case BOTTOM_LEFT_EJECTOR:
      return sprites_list[BOTTOM_LEFT_EJECTOR];
    case BOTTOM_RIGHT_EJECTOR:
      return sprites_list[BOTTOM_RIGHT_EJECTOR];
    }
  return NULL;
}

/**
 * Initialize the table of the positions of the balls on the ejectors. 
 * @param table Pointer to the structure of the ball ejectors
 */
bool controller_ejectors::is_pos_ball_initialized = false;
void
controller_ejectors::initialize_ball_positions (ball_ejector_coords * table)
{
  /* is the position of the balls already initialized? */
  if (is_pos_ball_initialized)
    {
      return;
    }
  is_pos_ball_initialized = true;

  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      table->x_coord =
        (table->x_coord * resolution) + sprites_list[i]->get_x_coord ();
      table->y_coord =
        (table->y_coord * resolution) + sprites_list[i]->get_y_coord ();
      table++;
    }
}
