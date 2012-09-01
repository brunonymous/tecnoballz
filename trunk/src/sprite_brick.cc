/** 
 * @file sprite_brick.cc
 * @brief The sprite of the brick 
 * @created 2007-09-12
 * @date 2007-09-30
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.5 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_brick.cc,v 1.5 2007-09-30 18:59:52 gurumeditation Exp $
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
#include "../include/sprite_brick.h"

/**
 * Create the Gigablitz sprite
 */
sprite_brick::sprite_brick ()
{
  current_cycling = &sprite_object::cycling_01[0];
  set_draw_method (sprite_object::DRAW_CAPSULE);
  clear_sprite_members ();
}

/**
 * Release the Gigablitz sprite
 */
sprite_brick::~sprite_brick ()
{
}

/**
 * Set the brick color
 * @param color Palette index from 239 to 255
 */
void 
sprite_brick::set_color (Uint32 color) 
{
  original_color = current_color = color;
}

void 
sprite_brick::touch ()
{
  current_color = original_color + 1;
  if(current_color > 255)
    {
      current_color = 239;
    }
}

bool
sprite_brick::is_cycling()
{
  if (current_color == original_color)
    {
      return false;
    }
  else
    {
      return true;
    }
}

/**
 * Change image of brick
 * @param h_pos Brick vertical position in the bricks bitmap
 *              0, 1, 2, 3, 4, 5, 6, 7, or 8  
 */
void
sprite_brick::update_image (Uint32 h_pos)
{
  if (h_pos == 0)
    {
      return;
    }
  Sint32 index = frame_index - frame_index % 7 + (h_pos >> 1); 
  set_image (index);
}



void
sprite_brick::draw ()
{
  if (!is_enabled || frame_index >= max_of_images)
    {
      return;
    }
  Uint32 color = current_color; 
  if (color != original_color)
    {
      if(++current_color > 255)
	{
	  current_color = 239;
	}
    }

  char *screen = game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = screen;
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  /* pixels data of the sprite image */
  char *pixels = current_drawing_data;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_pixels;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen = screen + k;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          char p = *(pixels++);
          if (p == 29)
            {
              *(screen++) = color;
            }
          else
            {
              *(screen++) = p;
            }
        }
    }
}



