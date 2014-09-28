/** 
 * @file head_animation.cc 
 * @brief Animate the head in the right score panel 
 * @date 2007-02-21
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
#include "../include/head_animation.h"
#include "../include/handler_resources.h"

head_animation * head_animation::head_anim_singleton = NULL;

/**
 * Create the head animation object
 */
head_animation::head_animation ()
{
  object_init ();
  head_bitmap = (bitmap_data *) NULL;
  frame_delay = 0;
  requested_animation = 0;
  frame_index = 0;
  frame_step = 0;
}

/**
 * Release the head animation object
 */
head_animation::~head_animation ()
{
  object_free ();
  if (head_bitmap)
    {
      delete head_bitmap;
      head_bitmap = (bitmap_data *) NULL;
    }
  head_anim_singleton = NULL;
}

/**
 * Get the object instance
 * head_animation is a singleton
 * @return the head_animation object 
 */
head_animation *
head_animation::get_instance ()
{
  if (NULL == head_anim_singleton)
    {
      head_anim_singleton = new head_animation ();
    }
  return head_anim_singleton;
}




/** 
 * Load the bitmap of the head animation
 */
void
head_animation::load_bitmap ()
{
  head_bitmap = new bitmap_data ();
  head_bitmap->load (handler_resources::BITMAP_HEAD_ANIMATION);
  head_height = head_bitmap->get_height ();
  head_width = head_bitmap->get_width () / MAX_OF_IMAGES;
}

/**
 * Draw the current image of the hean animation
 */
void
head_animation::draw ()
{
  
  Uint32 xcoord = frame_index * head_width;
  game_screen->blit_surface (head_bitmap, xcoord, 0, 272 * resolution, 106 * resolution, head_width, head_height);
}

/**
 * Play and draw the animation of the head
 */
void
head_animation::play ()
{
  draw ();
  /* Counter time delay before next image */
  if (frame_delay > 0)
    {
      frame_delay--;
      return;
    }

  Uint32 frame_num = requested_animation;
  requested_animation = 0;
  /* animation in progress? */
  if (frame_index == 0) 
    {
      frame_index = frame_num;
      frame_step = 1;
      frame_delay = FRAME_PERIOD_1;
      return;
    }

  /* forward animation */
  if (frame_step >= 0)
    {
      switch (frame_index)
        {
        case INTERFERENCE_STOP:
          frame_step = -1;
          frame_index = frame_index + frame_step;
          break;
        case YAWN_STOP:
          frame_step = -1;
          frame_delay = FRAME_PERIOD_2;
          break;
        case LAUGH_STOP:
          frame_step = -1;
          frame_delay = FRAME_PERIOD_2;
          break;
        default:
          frame_delay = FRAME_PERIOD_1;
          frame_index = frame_index + frame_step;
        }
      return;
    }

  /* reverse animation */
  switch (frame_index)
    {
    case INTERFERENCE_START:
      frame_index = 0;
      break;
    case YAWN_START:
      frame_index = 0;
      break;
    case LAUGH_START:
      frame_index = 0;
      break;
    default:
      frame_delay = FRAME_PERIOD_1;
      frame_index = frame_index + frame_step;
    }
}

/**
 * Start laugh head animation
 */
void
head_animation::start_laugh ()
{
  requested_animation = LAUGH_START;
}

/**
 * Start yawn head animation
 */
void
head_animation::start_yawn ()
{
  requested_animation = YAWN_START;
}

/**
 * Start interference animation
 */
void
head_animation::start_interference ()
{
  requested_animation = INTERFERENCE_START;
}
