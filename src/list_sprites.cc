/** 
 * @file list_sprites.cc 
 * @brief Call the drawing methods of all sprites
 * @date 2007-10-20
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
#include "../include/list_sprites.h"

/**
 * Create the object
 */
list_sprites::list_sprites ()
{
  shapes = NULL;
  shadows = NULL;
  num_of_shapes = 0;
  num_of_shadows = 0;
  max_of_shapes = 0;
  object_init ();
}

/**
 * Release the object
 */
list_sprites::~list_sprites ()
{
  if (shapes != NULL)
    {
      delete[]shapes;
      shapes = NULL;
    }
  if (shadows != NULL)
    {
      delete[]shadows;
      shadows = NULL;
    }
  object_free ();
}

/**
 * Allocate memory for the list of shapes and shadow
 * @params numof max numbers of shapes 
 */
void
list_sprites::init (Uint32 numof)
{
  try
  {
    if (NULL == shapes)
      {
        max_of_shapes = numof;
        shapes = new sprite_object *[max_of_shapes];
      }
    if (NULL == shadows)
      {
        shadows = new sprite_object *[max_of_shapes];
      }
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)list_sprites::init() "
      "not enough memory to allocate " <<
      max_of_shapes << " list elements!" << std::endl;
    throw;
  }
  reset ();
}

/**
 * Clear the list
 */
void
list_sprites::reset ()
{
  num_of_shapes = 0;
  num_of_shadows = 0;
  for (Sint32 i = 0; i < max_of_shapes; i++)
    {
      shapes[i] = NULL;
      shadows[i] = NULL;
    }
}



/**
 * Return the number of sprites remaining
 * @return number of free sprites 
 */
Uint32
list_sprites::get_sprites_remaining ()
{
  return max_of_shapes - num_of_shapes;
}

/**
 * Add a sprite to the display list to draw the sprites
 * @param sprite pointer to a sprite object 
 */
void
list_sprites::add (sprite_object * sprite)
{
  if (num_of_shapes >= max_of_shapes - 1)
    {
      std::cerr << "(!)list_sprites::add maximum number of sprites "
        << num_of_shapes << "has been reached!";
      throw std::runtime_error ("(!)list_sprites::add maximum "
                                "number of sprites has been reached!");
    }
  shapes[num_of_shapes] = sprite;
  num_of_shapes++;
  sprite->set_display_pos (num_of_shapes);
  if (sprite->has_shadow ())
    {
      shadows[num_of_shadows] = sprite;
      num_of_shadows++;
    }
}

/**
 *  Draw all sprites into the main offscreen
 */
void
list_sprites::draw ()
{
  
  /* draw the shahows of the objects */
  for (Sint32 i = 0; i < num_of_shadows; i++)
    {
      sprite_object *sprite = shadows[i];
      sprite->draw_shadow ();
    }

  /* draw the objects */
  for (Sint32 i = 0; i < num_of_shapes; i++)
    {
      sprite_object *sprite = shapes[i];
      sprite->draw ();
    }
}

/**
 * Restore background under the sprites
 */
void
list_sprites::clear ()
{
  for (Sint32 i = 0; i < num_of_shapes; i++)
    {
      sprite_object *sprite = shapes[i];
      sprite->restore_background_under_sprite ();
    }

  for (Sint32 i = 0; i < num_of_shadows; i++)
    {
      sprite_object *sprite = shadows[i];
      sprite->restore_background_under_shadow ();
    }
}
