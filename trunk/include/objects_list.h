/**
 * @file objects_list.h
 * @brief Template of management of objects list
 * @date 2007-10-21
 * @copyright 1998-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1998-2007 TLK Games all rights reserved
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
#ifndef __OBJECTS_LIST__
#define __OBJECTS_LIST__

template < class X, class T > class objects_list;
#include "../include/list_sprites.h"
#include "../include/handler_display.h"
#include "../include/tecnoballz.h"

template < class X, class T > class objects_list:public virtual tecnoballz
  {
  protected:
    static T *singleton;
    X ** sprites_list;
    Uint32 max_of_sprites;
    Uint32 num_of_sprites_allocated;
    Uint32 num_of_sprites;
    bool sprites_have_shades;
    Uint32 sprite_type_id;
    bool is_draw_pixel_by_pixel;

  public:
    objects_list ();
    ~objects_list ();
    static T * get_instance ();
    void littleInit ();
    void release_sprites_list ();
    void alloc_sprites_list ();
    void create_sprites_list (Uint32 maxof, bool have_shades = true);
    void create_sprites_list ();
    void disable_sprites ();
    void enable_sprites ();
    X **get_sprites_list ();
    X *get_first_sprite ();
    Uint32 get_max_of_sprites ();
    void set_max_of_sprites (Uint32 maxof);
  };

template < class X, class T > T* objects_list < X, T >::singleton = NULL;



/**
 * Create the list of objects
 */
template < class X, class T > objects_list < X, T >::objects_list ()
{
}

/**
 * Release the lits objects
 */
template < class X, class T > objects_list < X, T >::~objects_list ()
{
  singleton = NULL;
}

template < class X, class T > T* objects_list < X, T >::get_instance ()
{
  if (NULL == singleton)
    {
      singleton = new T ();
    }
  return (static_cast< T*> (singleton));
}

/**
 * Clear some members
 */
template < class X, class T > void objects_list < X, T >::littleInit ()
{
  object_init ();
  max_of_sprites = 0;
  num_of_sprites_allocated = 0;
  num_of_sprites = 0;
  sprites_list = NULL;
  sprites_have_shades = false;
  sprite_type_id = 0;
  is_draw_pixel_by_pixel = false;
}

/**
 * Release the list of sprite object
 */
template < class X, class T > void objects_list < X, T >::release_sprites_list ()
{
  if (NULL == sprites_list)
    {
      return;
    }
  for (Uint32 i = 0; i < num_of_sprites_allocated; i++)
    {
      X *sprite = sprites_list[i];
      if (NULL != sprite)
        {
          delete sprite;
        }
      sprites_list[i] = (X *) NULL;
    }
  delete[]sprites_list;
  sprites_list = NULL;
  max_of_sprites = 0;
  num_of_sprites_allocated = 0;
  object_free ();
}

/**
 * Return list of the sprites objects
 * @return pointer to the list of sprites objects
 */
template < class X, class T > X ** objects_list < X, T >::get_sprites_list ()
{
  return sprites_list;
}

/**
 * Return first sprite of the list
 * @return pointer to the first sprite object of the list
 */
template < class X, class T > X * objects_list < X, T >::get_first_sprite ()
{
  if (NULL == sprites_list)
    {
      return NULL;
    }
  return sprites_list[0];
}

/**
 * Return the maxium number of sprites objects
 * @return the maxium number of sprites objects
 */
template < class X, class T > Uint32 objects_list < X, T >::get_max_of_sprites ()
{
  return max_of_sprites;
}

/**
 * Initialize the maximum number of sprites
 * @param maxof maximum number of sprites
 */
template < class X, class T > void objects_list < X, T >::set_max_of_sprites (Uint32 maxof)
{
  max_of_sprites = maxof;
}

/**
 * Allocate memory for the list of sprites
 */
template < class X, class T > void objects_list < X, T >::alloc_sprites_list ()
{
  if (0 == max_of_sprites)
    {
      std::cerr << "(!)objects_list::alloc_sprites_list() " <<
      "Our array should always have at least one element in it!" <<
      std::endl;
      throw ("(!)objects_list::alloc_sprites_list() failed! "
             "At least one element is required");
    }

  release_sprites_list ();
  try
    {
      sprites_list = new X*[max_of_sprites];
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!)objects_list::alloc_sprites_list() " <<
      "not enough memory to allocate " <<
      max_of_sprites << " elements!" << std::endl;
      throw;
    }
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprites_list[i] = NULL;
    }
  num_of_sprites_allocated = max_of_sprites;
}

/**
 * Initialize the list of sprites objects
 * @param maxof maximum number of sprites
 * @param have_shadow true if the sprite has shadow, true by default
 */
template < class X, class T > void objects_list < X, T >::create_sprites_list (Uint32 maxof, bool have_shades)
{
  max_of_sprites = maxof;
  sprites_have_shades = have_shades;
  create_sprites_list ();
}

/**
 * Initialize the list of sprites objects
 */
template < class X, class T > void objects_list < X, T >::create_sprites_list ()
{
  alloc_sprites_list ();

  X *sprite_template = new X ();
  sprite_template->set_object_pos (0);
  sprites_list[0] = sprite_template;

  /* reserves only once the memory required for the
   * graphic data of the sprite */
  sprite_template->create_sprite (sprite_type_id, sprites_bitmap,
                                  sprites_have_shades, is_draw_pixel_by_pixel);
  sprite_template->set_draw_method (sprite_object::DRAW_WITH_TABLES);
  sprites->add (sprite_template);
  for (Uint32 i = 1; i < max_of_sprites; i++)
    {
      X *sprite = new X ();
      sprite->set_object_pos (i);
      sprite_template->duplicate_to (sprite);
      sprites_list[i] = sprite;
      sprites->add (sprite);
    }
}

/**
 * Enable all sprites objects
 */
template < class X, class T > void objects_list < X, T >::enable_sprites ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      X *sprite = sprites_list[i];
      sprite->enable ();
    }
}

/**
 * Disable all sprites objects
 */
template < class X, class T > void objects_list < X, T >::disable_sprites ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      X *sprite = sprites_list[i];
      sprite->disable ();
    }
}

#endif
