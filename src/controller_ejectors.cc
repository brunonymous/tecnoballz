/**
 * @file controller_ejectors.cc
 * @brief Ejectors corners controller
 * @date 2015-06-14
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
  sprite_ejector *top_l =
    new sprite_ejector (EJECTOR0_BALL_OFFSETX, EJECTOR0_BALL_OFFSETY);
  sprite_ejector *bot_l =
    new sprite_ejector (EJECTOR1_BALL_OFFSETX, EJECTOR1_BALL_OFFSETY);
  sprite_ejector *bot_r =
    new sprite_ejector (EJECTOR2_BALL_OFFSETX, EJECTOR2_BALL_OFFSETY);
  sprite_ejector *top_r =
    new sprite_ejector (EJECTOR3_BALL_OFFSETX, EJECTOR3_BALL_OFFSETY);

  sprites_list[TOP_LEFT_EJECTOR] = top_l;
  sprites_list[BOTTOM_LEFT_EJECTOR] = bot_l;
  sprites_list[BOTTOM_RIGHT_EJECTOR] = bot_r;
  sprites_list[TOP_RIGHT_EJECTOR] = top_r;

  top_l->create_sprite (sprite_object::EJECTOR_1, sprites_bitmap, true);
  top_l->set_coordinates (COORD_EJECTOR_1 * resolution,
                          (COORD_EJECTOR_1 * resolution) -
                          top_l->get_sprite_height () / 2);

  bot_l->create_sprite (sprite_object::EJECTOR_2, sprites_bitmap, true);
  bot_l->set_coordinates (COORD_EJECTOR_1 * resolution,
                          (COORD_EJECTOR_2 * resolution) -
                          bot_l->get_sprite_height () / 2);

  bot_r->create_sprite (sprite_object::EJECTOR_3, sprites_bitmap, true);
  bot_r->set_coordinates (COORD_EJECTOR_2 * resolution,
                          (COORD_EJECTOR_2 * resolution) -
                          bot_r->get_sprite_height () / 2);

  top_r->create_sprite (sprite_object::EJECTOR_4, sprites_bitmap, true);
  top_r->set_coordinates (COORD_EJECTOR_2 * resolution,
                          (COORD_EJECTOR_1 * resolution) -
                          top_r->get_sprite_height () / 2);

  /* has_background = false: ejectors are managed like sprites */
  if (!has_background)
    {
      sprites->add (top_l);
      sprites->add (top_r);
      sprites->add (bot_l);
      sprites->add (bot_r);
      top_l->enable ();
      top_r->enable ();
      bot_l->enable ();
      bot_r->enable ();
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
sprite_ejector *
controller_ejectors::get_ejector (Uint32 id)
{
  id &= 3;
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
