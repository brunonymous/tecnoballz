/**
* @file sprite_object.cc 
* @brief Draw a sprite on the screen 
* @date 2007-10-31
* @copyright 1991-2007 TLK Games
* @author Bruno Ethvignot
* @version $Revision: 1.43 $
*/
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: sprite_object.cc,v 1.43 2007-10-31 07:35:29 gurumeditation Exp $
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
#include "../include/sprite_object.h"

/**
 * Create a sprite object
 */
sprite_object::sprite_object ()
{
  ombredecax = handler_display::SHADOWOFFX * resolution;
  ombredecay = handler_display::SHADOWOFFY * resolution;
  ombrepixel = handler_display::SHADOW_PIX;
  ombrepixe4 = handler_display::SHADOWLONG;
  clear_sprite_members ();
}

/**
 * Release a sprite object
 */
sprite_object::~sprite_object ()
{
  release_sprite ();
}

/**
 * Release memory allocated dynamically bu the sprite object
 */
void
sprite_object::release_sprite ()
{
  if (has_allocated_memory)
    {
      for (Sint32 i = 0; i < max_of_images; i++)
        {
          if (drawing_values[i] != NULL)
            {
              delete[]drawing_values[i];
              drawing_values[i] = NULL;
            }
          if (drawing_data[i] != NULL)
            {
              delete[]drawing_data[i];
              drawing_data[i] = NULL;
            }
          if (is_draw_pixel_by_pixel && drawing_pixels[i] != NULL)
            {
              delete[]drawing_pixels[i];
              drawing_pixels[i] = NULL;
            }
        }
      if (drawing_values != NULL)
        {
          delete[]drawing_values;
          drawing_values = NULL;
        }
      if (drawing_data != NULL)
        {
          delete[]drawing_data;
          drawing_data = NULL;
        }
      if (is_draw_pixel_by_pixel && drawing_pixels != NULL)
        {
          delete[]drawing_pixels;
          drawing_pixels = NULL;
        }
      if (images_pixel_data != NULL)
        {
          delete[]images_pixel_data;
          images_pixel_data = NULL;
        }
      if (drawing_peer_line != NULL)
        {
          for (Sint32 i = 0; i < max_of_images; i++)
            {
              if (drawing_peer_line[i] != NULL)
                {
                  delete[]drawing_peer_line[i];
                  drawing_peer_line[i] = NULL;
                }
            }
          delete[]drawing_peer_line;
          drawing_peer_line = NULL;
        }
    }
  if (is_release_pixel_data && pixel_data != NULL)
    {
      delete[]pixel_data;
      is_release_pixel_data = false;
      pixel_data = NULL;
    }
  object_free ();
}

/**
 * Clear some values
 */
void
sprite_object::clear_sprite_members ()
{
  object_init ();
  pixel_data = (char *) NULL;
  images_pixel_data = (char **) NULL;
  screen_ptr = (char *) NULL;
  shadow_screen_ptr = (char *) NULL;
  frame_delay = 1;
  max_of_images = 0;
  frame_index = 0;
  is_enabled = false;
  sprite_height = 0;
  sprite_width = 0;
  drawing_values = (Sint16 **) NULL;
  drawing_data = (char **) NULL;
  drawing_pixels = (Sint16 **) NULL;
  collision_height = 0;
  collision_width = 0;
  screen_height = 0;
  screen_width = 0;
  frame_period = 1;
  x_coord = 0;
  y_coord = 0;
  display_pos = 0;
  x_maximum = 0;
  y_maximum = 0;
  frame_index_max = 0;
  x_minimum = 0;
  y_minimum = 0;
  frame_index_min = 0;
  offsetDest = 0;
  offsetSrce = 0;
  current_drawing_values = (Sint16 *) NULL;
  current_drawing_data = (char *) NULL;
  current_drawing_pixels = NULL;
  sprite_has_shadow = false;
  sprite_type_id = 0;
  row_size = 0;
  affligFrst = 0;
  affligLast = 1;
  is_mirrored_vertically = false;
  drawing_peer_line = (bb_afligne **) NULL;
  has_allocated_memory = false;
  object_pos = -1;
  num_of_repeats = 0;
  cycling_index = 0;
  current_cycling = &cycling_01[0];
  draw_method = COPY_FROM_BITMAP;
  is_release_pixel_data = false;
}

/**
 * Copy sprite members to anotger sprite
 * @param bobPt destination sprite object
 */
void
sprite_object::duplicate_to (sprite_object * sprite_dest)
{
  sprite_dest->pixel_data = pixel_data;
  sprite_dest->images_pixel_data = images_pixel_data;
  sprite_dest->screen_ptr = screen_ptr;
  sprite_dest->shadow_screen_ptr = shadow_screen_ptr;
  sprite_dest->frame_delay = frame_delay;
  sprite_dest->max_of_images = max_of_images;
  sprite_dest->frame_index = frame_index;
  sprite_dest->is_enabled = is_enabled;
  sprite_dest->sprite_height = sprite_height;
  sprite_dest->sprite_width = sprite_width;
  sprite_dest->drawing_values = drawing_values;
  sprite_dest->drawing_data = drawing_data;
  sprite_dest->drawing_pixels = drawing_pixels;
  sprite_dest->collision_height = collision_height;
  sprite_dest->collision_width = collision_width;
  sprite_dest->screen_height = screen_height;
  sprite_dest->screen_width = screen_width;
  sprite_dest->frame_period = frame_period;
  sprite_dest->x_coord = x_coord;
  sprite_dest->y_coord = y_coord;
  sprite_dest->display_pos = display_pos;
  sprite_dest->x_maximum = x_maximum;
  sprite_dest->y_maximum = y_maximum;
  sprite_dest->frame_index_max = frame_index_max;
  sprite_dest->x_minimum = x_minimum;
  sprite_dest->y_minimum = y_minimum;
  sprite_dest->frame_index_min = frame_index_min;
  sprite_dest->offsetSrce = offsetSrce;
  sprite_dest->offsetDest = offsetDest;
  sprite_dest->current_drawing_values = current_drawing_values;
  sprite_dest->current_drawing_data = current_drawing_data;
  sprite_dest->current_drawing_pixels = current_drawing_pixels;
  sprite_dest->sprite_has_shadow = sprite_has_shadow;
  sprite_dest->sprite_type_id = sprite_type_id;
  sprite_dest->row_size = row_size;
  sprite_dest->offscreen_pitch = offscreen_pitch;
  sprite_dest->draw_method = draw_method;
  sprite_dest->is_draw_pixel_by_pixel = is_draw_pixel_by_pixel;
  sprite_dest->has_allocated_memory = false;
}


/**
 * Enable the sprite
 */
void
sprite_object::enable ()
{
  is_enabled = true;
}

/**
 * Disable the sprite
 */
void
sprite_object::disable ()
{
  is_enabled = false;
}

/**
 * Check if the sprite is enable
 * @return true if the sprite is enable
 */
bool sprite_object::is_enable ()
{
  return is_enabled;
}

/**
 * Return the sprite identifier
 * @return indentifier of sprite, identify the type of the sprite
 */
Uint32
sprite_object::get_sprite_type_id ()
{
  return sprite_type_id;
}

/**
 * Set a number sprite (normally the number of position in list)
 */
void
sprite_object::set_object_pos (Sint32 num)
{
  object_pos = num;
}

void
sprite_object::set_display_pos (Sint32 num)
{
  display_pos = num;
}

/**
 * Make a simple sprite
 * @param bitmap bitmap containing the images of sprite
 * @param shadow true if the sprite has shadow, false by default
 */
void
sprite_object::make_sprite (surface_sdl * bitmap, bool shadow)
{
  init_common (bitmap, shadow);
  pixel_data = bitmap->get_pixel_data ();
}

/**
 * Initialize some common values
 * @param bitmap bitmap containing the images of sprite
 * @param shadow true if the sprite has shadow
 */
void
sprite_object::init_common (surface_sdl * bitmap, bool shadow)
{
  screen_width = display->get_width ();
  screen_height = display->get_height ();
  row_size = bitmap->get_row_size ();
  offscreen_pitch = game_screen->get_row_size ();
  sprite_has_shadow = shadow;
  screen_ptr = (char *) NULL;
  shadow_screen_ptr = (char *) NULL;
  sprite_width = bitmap->get_width ();
  sprite_height = bitmap->get_height ();
  max_of_images = 1;
  collision_height = sprite_height;
  collision_width = sprite_width;
  x_maximum = screen_width - (Sint32) collision_width;
  y_maximum = screen_height - (Sint32) collision_height;
  offsetSrce = bitmap->get_line_modulo (sprite_width);
  offsetDest = game_screen->get_line_modulo (sprite_width);
}

/**
 * Allocate memory for graphics data sprite for optimized drawing routines
 * @param numof maximum mumber of images for this sprite
 */
void
sprite_object::alloc_drawing_tables (Sint32 numof)
{
  has_allocated_memory = true;
  max_of_images = numof;
  try
    {
      /* draw lines by lines */
      if (draw_method == DRAW_LINE_BY_LINE)
        {
          affligFrst = 0;
          affligLast = sprite_height;
          drawing_peer_line = new bb_afligne* [max_of_images];
          for (Sint32 i = 0; i < max_of_images; i++)
            {
              bb_afligne *p = new bb_afligne[sprite_height];
              drawing_peer_line[i] = p;
            }
        }

      /* table giving address of each images of the sprite
       * into bitmap source */
      images_pixel_data = new char* [max_of_images];
      /* drawing tables for offsets and counters values (words and bytes) */
      drawing_values = new Sint16* [max_of_images];
      /* drawing tables of drawing pixels data */
      drawing_data = new char* [max_of_images];

      for (Sint32 i = 0; i < max_of_images; i++)
        {
          images_pixel_data[i] = NULL;
          drawing_values[i] = NULL;
          drawing_data[i] = NULL;
        }

      /* table for draw pixel by pixel, used for color cycling */
      if (is_draw_pixel_by_pixel)
        {
          drawing_pixels = new Sint16 *[max_of_images];
          for (Sint32 i = 0; i < max_of_images; i++)
            {
              drawing_pixels[i] = NULL;
            }
        }
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!)sprite_object::alloc_drawing_tables " <<
      "not enough memory to allocate " <<
      "list of sprite drawing tables for "
      << max_of_images << " images! " << std::endl;
      throw;
    }
}

/**
 * Create the structure for drawing the sprite 
 * @param type_id sprite type id, number from 1 to n
 * @praam image bitmap containing the images of sprite 
 * @param shadow true if it sprite has a shadow, false otherwise 
 * @param by_pixel if true generate additional table to drawing
 *                 pixel by pixel. Used for color cyclyng.
 *                 False by default
 */
void
sprite_object::create_sprite (Sint32 type_id, surface_sdl * image, bool shadow,
                              bool by_pixel)
{
  is_draw_pixel_by_pixel = by_pixel;
  if (draw_method == COPY_FROM_BITMAP)
    {
      draw_method = DRAW_WITH_TABLES;
    }

  sprite_type_id = type_id;
  init_common (image, shadow);

  /* read sprite characteristics */
  const sprite_description *descr = zelistBOB[sprite_type_id];
  sprite_height = descr->height;
  sprite_height *= resolution;
  max_of_images = descr->number_of_images;
  frame_index_max = max_of_images - 1;
  sprite_width = descr->width;
  sprite_width *= resolution;
  collision_height = sprite_height;
  collision_width = sprite_width;
  x_maximum = screen_width - sprite_width;
  y_maximum = screen_height - sprite_height;

  offsetSrce = image->get_line_modulo (sprite_width);
  offsetDest = game_screen->get_line_modulo (sprite_width);

  /* Allocate list for the lists of the drawing tables */
  alloc_drawing_tables (max_of_images);

  /*
   * generate the drawing tables
   */
  sprite_coordinates *coord = descr->coordinates;
  /* process each image frame of the sprite animation */
  for (Sint32 i = 0; i < max_of_images; i++)
    {
      /* counter of the number of pixels of the image frame */
      Uint32 pixels_count = 0;
      /* table size counter of offsets and loops counters values */
      Uint32 values_count = 0;
      Uint32 pos_x = (Sint32) coord[i].xcoord;
      pos_x *= resolution;
      pos_x *= 16;
      Uint32 pos_y = (Sint32) coord[i].ycoord;
      pos_y *= resolution;

      /* verify page overflow */
      if (pos_y > (image->get_height () - sprite_height) ||
          pos_x > (image->get_width () - sprite_width))
        {
          std::cerr << "(!)sprite_object::create_sprite() " <<
          "sprite_type_id: " << sprite_type_id <<
          "; x2: " << pos_x + sprite_width <<
          "; width of the bitmap: " << image->get_width () <<
          "; y2: " << pos_y + sprite_height <<
          "; height of the bitmap: " << image->get_height () <<
          std::endl;
          std::cerr << "(!)sprite_object::create_sprite() " <<
          "pox_x: " <<  coord[i].xcoord <<
          "; pos_y: " << coord[i].ycoord <<
          "; image number: " << i <<
          "; resolution: " << resolution << std::endl;
          throw std::runtime_error ("(!)sprite_object::create_sprite() "
                                    "failed! Coordinates out of range");
        }

      /*
       * mirror y if request
       */
      if (is_mirrored_vertically)
        {
          char *top = image->get_pixel_data (pos_x, pos_y);
          char *bottom = image->get_pixel_data (pos_x, pos_y + sprite_height - 1);
          Uint32 next = image->get_row_size ();
          for (Uint32 j = 0; j < sprite_height / 2; j++)
            {
              for (Uint32 k = 0; k < sprite_width; k++)
                {
                  char pixel = top[k];
                  top[k] = bottom[k];
                  bottom[k] = pixel;
                }
              top += next;
              bottom -= next;
            }
        }

      /*
       * calculation size of the table
       */
      /* counter of the contiguous pixels */
      Uint32 contiguous = 0;
      /* graphic address of the sprite in png image */
      char *pixel_data = image->get_pixel_data (pos_x, pos_y);
      for (Uint32 j = 0; j < sprite_height; j++)
        {
          for (Uint32 k = 0; k < sprite_width; k++)
            {
              /* read a pixel */
              char pixel = *(pixel_data++);
              if (pixel != 0)
                {
                  contiguous++;
                  /* increase the size of pixel's table */
                  pixels_count++;
                }
              else
                {
                  /* at least one pixel? */
                  if (contiguous > 0)
                    {
                      contiguous = 0;
                      /* increase the size of the counter's table */
                      values_count++;
                    }
                }
            }
          if (contiguous > 0)
            {
              contiguous = 0;
              values_count++;
            }
          pixel_data += offsetSrce;
        }

      /*
       * generate the sprite's table for it displaying
       */
      char *pixels;
      Sint16 *counters;
      Sint16 *destW = NULL;
      try
        {
          pixels = new char[pixels_count];
          counters = new Sint16[values_count * 3 + 1];
          if (is_draw_pixel_by_pixel)
            {
              destW = new Sint16[values_count * 2 + 1];
            }
        }
      catch (std::bad_alloc &)
        {
          std::cerr << "(!)sprite_object::create_sprite() " <<
          "not enough memory to allocate " <<
          "drawing tables for "
          << pixels_count << " pixels! " << std::endl;
          throw;
        }

      /* table of offsets and loops counters */
      drawing_values[i] = counters;
      /* table of the pixels */
      drawing_data[i] = pixels;
      if (is_draw_pixel_by_pixel)
        {
          drawing_pixels[i] = destW;
        }

      /*
       * generate the sprite's table for display
       */
      Uint32 offset = 0;
      /* counter of the contiguous pixels */
      contiguous = 0;
      pixel_data = image->get_pixel_data (pos_x, pos_y); // graphic address
      images_pixel_data[i] = pixel_data;
      *(counters++) = (Sint16) values_count;      // Nombre d'occurences
      if (is_draw_pixel_by_pixel)
        {
          *(destW++) = (Sint16) values_count;    // Nombre d'occurences
        }
      values_count = 0;                // compteur nombre d'offest et de compteur
      Sint32 left_offset = 0;
      bool flagO = false;
      for (Uint32 j = 0; j < sprite_height; j++)
        {
          /* special display mode line peer line (used for gigablitz) */
          if (draw_method == DRAW_LINE_BY_LINE)
            {
              bb_afligne *p = drawing_peer_line[i];
              p[j].TABAFFICH1 = counters;
              p[j].TABAFFICH2 = pixels;
            }

          flagO = false;
          for (Uint32 k = 0; k < sprite_width; k++)
            {
              /* read the pixel */
              char pixel = *(pixel_data++);
              /* transparent? */
              if (pixel != 0)
                {
                  /* no, save pixel value */
                  *(pixels++) = (Sint16) pixel;
                  contiguous++;
                }
              else
                {
                  /* at least one pixel? */
                  if (contiguous > 0)
                    {
                      if (is_draw_pixel_by_pixel)
                        {
                          *(destW++) = (Sint16) offset;  //previous offset
                          *(destW++) = (Sint16) contiguous;  //number of pixel(s)
                        }
                      /* save the previous offset in bytes */
                      *(counters++) = (Sint16) offset;
#ifndef BYTES_COPY
                      /* save the number of 32-bit long words = x4 pixels */
                      Sint32 n = contiguous >> 2;
                      *(counters++) = (Sint16) n;
                      /* rest 0, 1, 2 or 3 bytes */
                      contiguous &= 0x003;
#else
                      *(counters++) = 0;
#endif
                      /* save number of contiguous bytes = x1 pixel */
                      *(counters++) = (Sint16) contiguous;
                      contiguous = 0;
                      offset = 0;
                      values_count++;  //COUNTERTAB++
                      flagO = true;
                    }
                  if (!flagO)
                    {
                      left_offset++;
                    }
                  offset++;
                }
            }                   //width loop
          //***
          if (contiguous > 0)
            {
              if (is_draw_pixel_by_pixel)
                {
                  *(destW++) = (Sint16) offset;  //previous offset
                  *(destW++) = (Sint16) contiguous;  //number of pixel(s)
                }
              *(counters++) = (Sint16) offset;
#ifndef BYTES_COPY
              /* save the number of 32-bit long words = x4 pixels */
              Sint32 n = contiguous >> 2;
              *(counters++) = (Sint16) n;
              /* rest 0, 1, 2 or 3 bytes */
              contiguous &= 0x003;
#else
              *(counters++) = 0;
#endif
              /* save number of contiguous bytes = x1 pixel */
              *(counters++) = (Sint16) contiguous;
              contiguous = 0;
              values_count++;          //COUNTERTAB++
              offset = 0;
            }
          pixel_data += offsetSrce;
          offset += offsetDest;

          if (draw_method == DRAW_LINE_BY_LINE)
            {
              bb_afligne *p = drawing_peer_line[i];
              p[j].COUNTERTAB = values_count;
              p[j].OFFSETLEFT = left_offset;
              values_count = 0;
              left_offset = 0;
            }
        } /* height loop */
    } /* image frame loop */
  /* current table of offsets and loops counters */
  current_drawing_values = drawing_values[0];
  /* current pixles table */
  current_drawing_data = drawing_data[0];
  if (is_draw_pixel_by_pixel)
    {
      current_drawing_pixels = drawing_pixels[0];
    }
  pixel_data = images_pixel_data[0];   //adresse GFX pour routine "draw()"
}

/**
 * Set x and y coordinates of the sprite
 * @param xcoord the x coordinate in pixels
 * @param ycoord the y coordinate in pixels
 */
void
sprite_object::set_coordinates (Sint32 xcoord, Sint32 ycoord)
{
  x_coord = xcoord;
  y_coord = ycoord;
}

/**
 * Set x coordinate of the sprite
 * @param xcoord the x coordinate in pixels
 */
void
sprite_object::set_x_coord (Sint32 xcoord)
{
  x_coord = xcoord;
}

/**
 * Set y coordinate of the sprite
 * @param ycoord the y coordinate in pixels
 */
void
sprite_object::set_y_coord (Sint32 ycoord)
{
  y_coord = ycoord;
}

/**
 * Moving the sprite horizontally 
 * @param xoffset the horizontal offset of displacement
 */
void
sprite_object::move_x (Sint32 xoffset)
{
  x_coord += xoffset;
}

/**
 * Moving the sprite vertically 
 * @param yoffset the vertical offset of displacement
 */
void
sprite_object::move_y (Sint32 yoffset)
{
  y_coord += yoffset;
}

/**
 * Check if the sprite has a shadow
 * @return true if the sprite has a shadow
 */
bool
sprite_object::has_shadow ()
{
  return sprite_has_shadow;
}

/**
 * Set or clear the flag sprite shadow
 * param shadow true if the sprite has a shadow
 */
void
sprite_object::set_shadow (bool shadow)
{
  sprite_has_shadow = shadow;
}



/**
 * Get x coordinate
 * @return x coordinate of the sprite
 */
Sint32
sprite_object::get_x_coord ()
{
  return x_coord;
}

/**
 * Get y coordinate
 * @return y coordinate of the sprite
 */
Sint32
sprite_object::get_y_coord ()
{
  return y_coord;
}

/**
 * Set the image to use for this sprite
 */
void
sprite_object::set_image ()
{
  Sint32 index = frame_index;
  pixel_data = images_pixel_data[index];
  current_drawing_values = drawing_values[index];
  current_drawing_data = drawing_data[index];
  if (is_draw_pixel_by_pixel && drawing_pixels)
    {
      current_drawing_pixels = drawing_pixels[index];
    }
}

/**
 * Set the image to use for this sprite
 * @param index frame index 
 */
void
sprite_object::set_image (Sint32 index)
{
  frame_index = index;
  pixel_data = images_pixel_data[index];
  current_drawing_values = drawing_values[index];
  current_drawing_data = drawing_data[index];
  if (is_draw_pixel_by_pixel)
    {
      current_drawing_pixels = drawing_pixels[index];
    }
}

/**
 * Restore background where sprite was displayed
 */
void
sprite_object::restore_background_under_sprite ()
{
  /* if memory address is null, then sprite is not displaying */
  if (NULL == screen_ptr)
    {
      return;
    }

  /* special sprite, restore line by line (gigablitz) */
  if (draw_method == DRAW_LINE_BY_LINE)
    {
      restore_line_by_line ();
      return;
    }

#ifndef BYTES_COPY
  Sint32 *restore32 = (Sint32 *) restore_ptr;
  Sint32 *screen32 = (Sint32 *) screen_ptr;
  screen_ptr = (char *) NULL;
  Sint16 *counters = current_drawing_values;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen32 = (Sint32 *) ((char *) screen32 + k);
      restore32 = (Sint32 *) ((char *) restore32 + k);
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen32++) = *(restore32++);
        }
      /* number of contiguous long words */
      k = *(counters++);
      char *screen8 = (char *) screen32;
      char *restore8 = (char *) restore32;
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen8++) = *(restore8++);
        }
      screen32 = (Sint32 *) screen8;
      restore32 = (Sint32 *) restore8;
    }
#else
  char *restore = restore_ptr;
  char *screen = screen_ptr;
  screen_ptr = (char *) NULL;
  Sint16 *counters = current_drawing_values;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen += k;
      restore += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen++) = *(restore++);
        }
    }
#endif
}

/**
 * Restore background where line by line (used for gigablitz) 
 */
void
sprite_object::restore_line_by_line ()
{

  bb_afligne *p = drawing_peer_line[frame_index];
  Sint32 l = affligFrSv;
  Sint16 o = p[l].OFFSETLEFT;
  Uint32 t = (Uint32) p[l].COUNTERTAB;
  Uint16 *gfxPT = (Uint16 *) p[l].TABAFFICH1;
  gfxPT++;
#ifndef BYTES_COPY
  Sint32 *srcPT = (Sint32 *) restore_ptr;
  Sint32 *adres = (Sint32 *) screen_ptr;
  screen_ptr = (char *) NULL;
  for (Uint32 i = 0; i < t; i++)
    {
      adres = (Sint32 *) ((char *) adres + o);
      srcPT = (Sint32 *) ((char *) srcPT + o);
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = *(srcPT++);
      o = *(gfxPT++);           //number of pixels contigus
      char *adreb = (char *) adres;
      char *srcPB = (char *) srcPT;
      for (Sint32 k = 0; k < o; k++)
        *(adreb++) = *(srcPB++);
      adres = (Sint32 *) adreb;
      srcPT = (Sint32 *) srcPB;
      o = *(gfxPT++);           //offset
    }
  l++;
  for (; l < affligLaSv; l++)
    {
      gfxPT = (Uint16 *) p[l].TABAFFICH1;
      t = (Uint32) p[l].COUNTERTAB;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxPT++);       //offset
          adres = (Sint32 *) ((char *) adres + o);
          srcPT = (Sint32 *) ((char *) srcPT + o);
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
          o = *(gfxPT++);       //number of pixels contigus
          char *adreb = (char *) adres;
          char *srcPB = (char *) srcPT;
          for (Sint32 k = 0; k < o; k++)
            *(adreb++) = *(srcPB++);
          adres = (Sint32 *) adreb;
          srcPT = (Sint32 *) srcPB;
        }
    }
#else
  char *srcPT = restore_ptr;
  char *adres = screen_ptr;
  screen_ptr = (char *) NULL;
  for (Uint32 i = 0; i < t; i++)
    {
      adres = adres + o;
      srcPT = srcPT + o;
      gfxPT++;
      o = *(gfxPT++);           //number of pixels contigus
      for (Sint32 k = 0; k < o; k++)
        *(adres++) = *(srcPT++);
      o = *(gfxPT++);           //offset
    }
  l++;
  for (; l < affligLaSv; l++)
    {
      gfxPT = (Uint16 *) p[l].TABAFFICH1;
      t = (Uint32) p[l].COUNTERTAB;
      for (Uint32 i = 0; i < t; i++)
        {
          o = *(gfxPT++);       //offset
          adres = adres + o;
          srcPT = srcPT + o;
          gfxPT++;
          o = *(gfxPT++);       //number of pixels contigus
          for (Sint32 k = 0; k < o; k++)
            *(adres++) = *(srcPT++);
        }
    }
#endif
}

/**
 * Return current animation offset
 * @return the current frame index
 */
Sint32
sprite_object::get_frame_index ()
{
  return frame_index;
}

/**
 * Restore background where sprite's shadow was displayed
 */
void
sprite_object::restore_background_under_shadow ()
{
  if (NULL == shadow_screen_ptr)
    {
      return;
    }
#ifndef BYTES_COPY
  Sint32 *restore32 = (Sint32 *) shadow_restore_ptr;
  Sint32 *screen32 = (Sint32 *) shadow_screen_ptr;
  shadow_screen_ptr = (char *) NULL;
  Sint16 *counters = current_drawing_values;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen32 = (Sint32 *) ((char *) screen32 + k);
      restore32 = (Sint32 *) ((char *) restore32 + k);
      /* number of contiguous 32-bit words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen32++) = *(restore32++);
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *screen8 = (char *) screen32;
      char *restore8 = (char *) restore32;
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen8++) = *(restore8++);
        }
      screen32 = (Sint32 *) screen8;
      restore32 = (Sint32 *) restore8;
    }
#else
  char *restore = shadow_restore_ptr;
  char *screen = shadow_screen_ptr;
  shadow_screen_ptr = (char *) NULL;
  Sint16 *counters = current_drawing_values;
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen += k;
      restore += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen++) = *(restore++);
        }
    }
#endif
}

/**
 * Draw a sprite into the offscreen
 */
void
sprite_object::draw ()
{
  if (!is_enabled || frame_index >= max_of_images)
    {
      return;
    }
  switch (draw_method)
    {
    case DRAW_WITH_TABLES:
      draw_with_tables ();
      break;
    case COPY_FROM_BITMAP:
      draw_copy_from_bitmap ();
      break;
    case DRAW_LINE_BY_LINE:
      draw_line_by_line ();
      break;
    case DRAW_REPEAT_SPRITE:
      draw_vertically_repeated ();
      break;
    case DRAW_COLOR_CYCLING_MASK:
      draw_cycling_color ();
      break;
    case DRAW_CAPSULE:
      draw_capsule ();
      break;
    }
}

/**
 * Draw a sprite with tables to optimize the copy
 * It is the method most frequently used
 */
void
sprite_object::draw_with_tables ()
{
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
#ifndef BYTES_COPY
  Sint32 *screen32 = (Sint32 *) game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = (char *) screen32;
  /* pixels data of the sprite image */
  Sint32 *pixels32 = (Sint32 *) current_drawing_data;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen32 = (Sint32 *) ((char *) screen32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          Sint32 p = *(pixels32++);
          *(screen32++) = p;
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *pixels8 = (char *) pixels32;
      char *screen8 = (char *) screen32;
      for (Sint32 j = 0; j < k; j++)
        {
          char p = *(pixels8++);
          *(screen8++) = p;
        }
      pixels32 = (Sint32 *) pixels8;
      screen32 = (Sint32 *) screen8;
    }
#else
  char *screen = game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = screen;
  /* pixels data of the sprite image */
  char *pixels = current_drawing_data;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 t = (Uint32) * (counters++);
  for (Uint32 i = 0; i < t; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen++) = *(pixels++);
        }
    }
#endif
}

/**
 * Draw a color cycling sprite mask (paddle's fires and guardian's fires) 
 */
void
sprite_object::draw_cycling_color ()
{
  cycling_index &= 7;
  Sint32 pixel = current_cycling[cycling_index++];
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
#ifndef BYTES_COPY
  Sint32 *screen32 = (Sint32 *) game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = (char *) screen32;
  /* pixels data of the sprite image */
  Sint32 *pixels32 = (Sint32 *) current_drawing_data;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 h = (Uint32) * (counters++);
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen32 = (Sint32 *) ((char *) screen32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen32++) = pixel;
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *pixels8 = (char *) pixels32;
      char *screen8 = (char *) screen32;
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen8++) = pixel;
        }
      pixels32 = (Sint32 *) pixels8;
      screen32 = (Sint32 *) screen8;
    }
#else
  char *screen = game_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = screen;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_values;
  Uint32 t = (Uint32) * (counters++);
  for (Uint32 i = 0; i < t; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen++) = pixel;
        }
    }
#endif
}

/**
 * Draw somes capsules with color cyling  
 */
void
sprite_object::draw_capsule ()
{
  cycling_index &= 7;
  Sint32 color = current_cycling[cycling_index++];
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

/**
 * Draw sprite. Sprite image will be repeated vertically
 * Used only for the vertical gauge of the guardian's life level
 */
void
sprite_object::draw_vertically_repeated ()
{
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = game_screen->get_pixel_data (x_coord, y_coord);
  Sint32 yoffset = 0;
  for (Sint32 r = 0; r < num_of_repeats; r++, yoffset += sprite_height)
    {
      /* offsets and counters of loops for copies */
      Uint16 *counters = (Uint16 *) current_drawing_values;
      Uint32 h = (Uint32) * (counters++);
#ifndef BYTES_COPY
      Sint32 *screen32 =
        (Sint32 *) game_screen->get_pixel_data (x_coord, y_coord + yoffset);
      /* pixels data of the sprite image */
      Sint32 *pixels32 = (Sint32 *) current_drawing_data;
      for (Uint32 i = 0; i < h; i++)
        {
          /* offset in bytes */
          Sint16 k = *(counters++);
          screen32 = (Sint32 *) ((char *) screen32 + k);
          /* number of contiguous long words */
          k = *(counters++);
          for (Sint32 j = 0; j < k; j++)
            {
              Sint32 pixels = *(pixels32++);
              *(screen32++) = pixels;
            }
          /* number of contiguous bytes */
          k = *(counters++);
          char *pixels8 = (char *) pixels32;
          char *screen8 = (char *) screen32;
          for (Sint32 j = 0; j < k; j++)
            {
              char pixel = *(pixels8++);
              *(screen8++) = pixel;
            }
          pixels32 = (Sint32 *) pixels8;
          screen32 = (Sint32 *) screen8;
        }
#else
      char *screen = game_screen->get_pixel_data (x_coord, y_coord + yoffset);
      /* pixels data of the sprite image */
      char *pixels = current_drawing_data;
      for (Uint32 i = 0; i < h; i++)
        {
          /* offset in bytes */
          Sint16 k = *(counters++);
          screen += k;
          counters++;
          /* number of contiguous bytes */
          k = *(counters++);
          for (Sint32 j = 0; j < k; j++)
            {
              char pixel = *(pixels++);
              *(screen++) = pixel;
            }
        }
#endif
    }
}

/**
 * Draw a sprite line by line, always by using tables.
 * Method used for the gigablitz vertical clipping only
 */
void
sprite_object::draw_line_by_line ()
{
  bb_afligne *p = drawing_peer_line[frame_index];
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord + affligFrst);
  affligFrSv = affligFrst;
  affligLaSv = affligLast;
  Sint32 l = affligFrst;
  Uint32 h = (Uint32) p[l].COUNTERTAB;
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) p[l].TABAFFICH1;
  Sint16 k = p[l].OFFSETLEFT;
  counters++;
#ifndef BYTES_COPY
  Sint32 *screen32 =
    (Sint32 *) game_screen->get_pixel_data (x_coord, y_coord + affligFrst);
  Sint32 *pixels32 = (Sint32 *) p[l].TABAFFICH2;
  screen_ptr = (char *) screen32;
  for (Uint32 i = 0; i < h; i++)
    {
      screen32 = (Sint32 *) ((char *) screen32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          Sint32 pixels = *(pixels32++);
          *(screen32++) = pixels;
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *pixels8 = (char *) pixels32;
      char *screen8 = (char *) screen32;
      for (Sint32 j = 0; j < k; j++)
        {
          char pixel = *(pixels8++);
          *(screen8++) = pixel;
        }
      pixels32 = (Sint32 *) pixels8;
      screen32 = (Sint32 *) screen8;
      /* offset in bytes */
      k = *(counters++);
    }
  l++;
  for (; l < affligLast; l++)
    {
      h = (Uint32) p[l].COUNTERTAB;
      counters = (Uint16 *) p[l].TABAFFICH1;
      pixels32 = (Sint32 *) p[l].TABAFFICH2;
      for (Uint32 i = 0; i < h; i++)
        {
          k = *(counters++);
          screen32 = (Sint32 *) ((char *) screen32 + k);
          /* number of contiguous long words */
          k = *(counters++);
          for (Sint32 j = 0; j < k; j++)
            {
              Sint32 pixels = *(pixels32++);
              *(screen32++) = pixels;
            }
          /* number of contiguous bytes */
          k = *(counters++);
          char *pixels8 = (char *) pixels32;
          char *screen8 = (char *) screen32;
          for (Sint32 j = 0; j < k; j++)
            {
              char pixel = *(pixels8++);
              *(screen8++) = pixel;
            }
          pixels32 = (Sint32 *) pixels8;
          screen32 = (Sint32 *) screen8;
        }
    }
#else
  char *screen = game_screen->get_pixel_data (x_coord, y_coord + affligFrst);
  char *pixels = p[l].TABAFFICH2;
  screen_ptr = screen;
  for (Uint32 i = 0; i < h; i++)
    {
      screen += k;
      counters++;
      /* number of contiguous pixels */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          char pixel = *(pixels++);
          *(screen++) = pixel;
        }
      /* offset in bytes */
      k = *(counters++);
    }
  l++;
  for (; l < affligLast; l++)
    {
      h = (Uint32) p[l].COUNTERTAB;
      counters = (Uint16 *) p[l].TABAFFICH1;
      pixels = p[l].TABAFFICH2;
      for (Uint32 i = 0; i < h; i++)
        {
          k = *(counters++);
          screen += k;
          counters++;
          /* number of contiguous pixels */
          k = *(counters++);
          for (Sint32 j = 0; j < k; j++)
            {
              char pixel = *(pixels++);
              *(screen++) = pixel;
            }
        }
    }
#endif
}

/**
 * Draw a sprite shadow in the game offscreen 
 */
void
sprite_object::draw_shadow ()
{
  if (!is_enabled || !sprite_has_shadow)
    {
      return;
    }
  shadow_restore_ptr =
    background_screen->get_pixel_data (x_coord + ombredecax,
                                       y_coord + ombredecay);
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 h = (Uint32) * (counters++);
  Uint32 mask = ombrepixe4;
#ifndef BYTES_COPY
  Sint32 *screen32 =
    (Sint32 *) game_screen->get_pixel_data (x_coord + ombredecax,
                                            y_coord + ombredecay);
  shadow_screen_ptr = (char *) screen32;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen32 = (Sint32 *) ((char *) screen32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          Sint32 value = *screen32;
          value = value | mask;
          //*(screen32++) = value;

          //*(screen32++) |= mask;
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *screen8 = (char *) screen32;
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen8++) |= mask;
        }
      screen32 = (Sint32 *) screen8;
    }
#else
  char *screen = game_screen->get_pixel_data (x_coord + ombredecax,
                                              y_coord + ombredecay);
  shadow_screen_ptr = screen;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      screen += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(screen++) |= mask;
        }
    }
#endif
}

/**
 * Draw a sprite in the brackground surface.
 * (build the background before a bricks level)
 */
void
sprite_object::draw_to_brackground ()
{
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  /* offsets and counters of loops for copies */
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 h = (Uint32) * (counters++);
#ifndef BYTES_COPY
  Sint32 *background32 = (Sint32 *) background_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = (char *) background32;
  Sint32 *pixels32 = (Sint32 *) current_drawing_data;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      background32 = (Sint32 *) ((char *) background32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          Sint32 p = *(pixels32++);
          *(background32++) = p;
        }
      /*  number of contiguous bytes */
      k = *(counters++);
      char *pixels8 = (char *) pixels32;
      char *background8 = (char *) background32;
      for (Sint32 j = 0; j < k; j++)
        {
          char p = *(pixels8++);
          *(background8++) = p;
        }
      pixels32 = (Sint32 *) pixels8;
      background32 = (Sint32 *) background8;
    }
#else
  char *background = background_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = background;
  char *pixels = current_drawing_data;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      background += k;
      counters++;
      /*  number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          char p = *(pixels++);
          *(background++) = p;
        }
    }
#endif
}

/**
 * Draw a shadow in the brackground offscreen
 */
void
sprite_object::draw_shadow_to_brackground ()
{
  shadow_restore_ptr = background_screen->get_pixel_data (x_coord + ombredecax,
                       y_coord + ombredecay);
  Uint16 *counters = (Uint16 *) current_drawing_values;
  /* height of the sprite in pixels */
  Uint32 h = (Uint32) * (counters++);
#ifndef BYTES_COPY
  Sint32 *background32 = (Sint32 *) background_screen->get_pixel_data (x_coord + ombredecax,
                         y_coord + ombredecay);
  shadow_screen_ptr = (char *) background32;
  Sint32 p = ombrepixe4;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      background32 = (Sint32 *) ((char *) background32 + k);
      /* number of contiguous long words */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(background32++) |= p;
        }
      /* number of contiguous bytes */
      k = *(counters++);
      char *background8 = (char *) background32;
      for (Sint32 j = 0; j < k; j++)
        {
          *(background8++) |= p;
        }
      background32 = (Sint32 *) background8;
    }
#else
  char p = ombrepixel;
  char *background = background_screen->get_pixel_data (x_coord + ombredecax,
                     y_coord + ombredecay);
  shadow_screen_ptr = background;
  for (Uint32 i = 0; i < h; i++)
    {
      /* offset */
      Sint16 k = *(counters++);
      background += k;
      counters++;
      /* number of contiguous bytes */
      k = *(counters++);
      for (Sint32 j = 0; j < k; j++)
        {
          *(background++) |= p;
        }
    }
#endif
}

/**
 * Draw a sprite into the game offscreen (copy byte to byte, no table)
 */
void
sprite_object::draw_copy_from_bitmap ()
{
  if (!is_enabled)
    {
      return;
    }
  char *s = pixel_data;
  char *d = game_screen->get_pixel_data (x_coord, y_coord);
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = d;
  Sint32 m = row_size;
  Sint32 n = offscreen_pitch;
  Sint32 h = sprite_height;
  Sint32 l = sprite_width;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < l; j++)
        {
          /* read the pixel */
          char p = s[j];
          /* black color? */
          if (p != 0)
            {
              /* no, put the pixel */
              d[j] = p;
            }
        }
      s += m;
      d += n;
    }
}

/**
 * Simple byte to byte copy of the sprite in the game offscreen 
 */
void
sprite_object::copy_to_game_screen ()
{
  if (!is_enabled)
    {
      return;
    }
  char *s = pixel_data;
  char *d = game_screen->get_pixel_data (x_coord, y_coord);
  restore_ptr = background_screen->get_pixel_data (x_coord, y_coord);
  screen_ptr = d;
  Uint32 row = row_size;
  Uint32 pitch = offscreen_pitch;
  Uint32 h = sprite_height;
  Uint32 w = sprite_width;
  for (Uint32 i = 0; i < h; i++)
    {
      for (Uint32 j = 0; j < w; j++)
        {
          d[j] = s[j];
        }
      s += row;
      d += pitch;
    }
}

/**
 * Restore rectanble background where sprite was displayed
 */
void
sprite_object::restore_rectangle_background ()
{
  if (NULL == screen_ptr)
    {
      return;
    }
  char *s = restore_ptr;
  char *d = screen_ptr;
  screen_ptr = (char *) NULL;
  Uint32 pitch = offscreen_pitch;
  Uint32 h = sprite_height;
  Uint32 w = sprite_width;
  for (Uint32 i = 0; i < h; i++)
    {
      for (Uint32 j = 0; j < w; j++)
        {
          /* restore the pixel */
          d[j] = s[j];
        }
      s += pitch;
      d += pitch;
    }
}

/**
 * Position sprite on the other one
 * @param sprite Pointer to a sprite object
 * @param xoffset
 * @param yoffset
 */
void
sprite_object::pull (sprite_object * sprite, Sint32 xoffset, Sint32 yoffset)
{
  x_coord = (sprite->x_coord) + xoffset - (collision_width >> 1);
  y_coord = (sprite->y_coord) + yoffset - (collision_height >> 1);
}

/**
 * Position sprite on the other one
 * @param sprite Pointer to a sprite object
 * @param xoffset
 * @param yoffset
 */
void
sprite_object::attract (sprite_object * sprite, Sint32 xoffset, Sint32 yoffset)
{
  x_coord = (sprite->x_coord) + xoffset
    - ((collision_width - sprite->collision_width) >> 1);
  y_coord = (sprite->y_coord) + yoffset
    - ((collision_height - sprite->collision_height) >> 1);
}


/**
 * Check collision beetween two sprites
 * @param sprite Pointer to a sprite object
 * @return true if collision occurs, otherwise false
 */
bool
sprite_object::collision (sprite_object * sprite)
{
  Sint32 x1 = x_coord;
  Sint32 y1 = y_coord;
  Sint32 x2 = sprite->x_coord;
  Sint32 y2 = sprite->y_coord;
  return (x2 + (Sint32)sprite->collision_width > x1 &&
          x2 - (Sint32)collision_width < x1 &&
          y2 + (Sint32)sprite->collision_height > y1 &&
          y2 - (Sint32)collision_height < y1);
}

/**
 * Set the frame delay and period, the speed of animation
 * @param delay time delay before next image
 */
void
sprite_object::set_frame_delay (Sint32 delay)
{
  frame_delay = delay;
  frame_period = delay;
}

/**
 * Set the frame period, the speed of animation
 * @param period time delay before next image
 */
void
sprite_object::set_frame_period (Sint32 period)
{
  frame_period = period;
}

/**
 * The animation is played once 
 * @return true if animation is finished
 */
bool
sprite_object::play_animation_once ()
{
  if (--frame_delay > 0)
    {
      return is_enabled;
    }
  frame_delay = frame_period;
  if (frame_index == frame_index_max)
    {
      frame_index = frame_index_min;
      is_enabled = false;
    }
  else
    {
      frame_index++;
      set_image (frame_index);
    }
  return is_enabled;
}

/**
 * The animation is played in loop-mode
 */
void
sprite_object::play_animation_loop ()
{
  if (--frame_delay > 0)
    {
      return;
    }
  frame_delay = frame_period;
  if (frame_index == frame_index_max)
    {
      frame_index = frame_index_min;
      set_image (frame_index);
    }
  else
    {
      frame_index++;
      set_image (frame_index);
    }
}

/**
 * Change the current image of the sprite
 * @param index Index of the current image 
 */
void
sprite_object::new_offset (Sint32 index)
{
  frame_index = index;
  set_image (index);
}

/**
 * Clip coordinates of the sprite into offscreen coordinates
 */
void
sprite_object::clip_coordinates ()
{
  if (x_coord < 0)
    {
      x_coord = 0;
    }
  else if (x_coord > (Sint32)(screen_width - sprite_width))
    {
      x_coord = screen_width - sprite_width;
    }
  if (y_coord < 0)
    {
      y_coord = 0;
    }
  else if (y_coord > (Sint32)(screen_height - sprite_height))
    {
      y_coord = screen_height - sprite_height;
    }
}

/**
 * Get sprite's width 
 * @return the width of the sprite in pixels
 */
Uint32
sprite_object::get_sprite_width ()
{
  return sprite_width;
}

/**
 * Get sprite's height 
 * @return the height of the sprite in pixels
 */
Uint32
sprite_object::get_sprite_height ()
{
  return sprite_height;
}

/**
 * Get sprite's width for collision
 * @return the width of sprite for the collisions
 */
Uint32
sprite_object::get_collision_width ()
{
  return collision_width;
}

/**
 * Enable the repeat vetical of drawing of the sprite 
 * used only for the vertical gauge of the guadian's energy level 
 * @param numof_repeats 2 to n
 */
void
sprite_object::enable_vertical_repeat (Uint32 numof_repeats)
{
  if (numof_repeats < 2)
    {
      return;
    }
  num_of_repeats = numof_repeats;
  draw_method = DRAW_REPEAT_SPRITE;
}

/**
 * Set the method of displaying used to draw the sprite 
 * @param method draw method
 */
void
sprite_object::set_draw_method (Uint32 method)
{
  draw_method = method;
}

/**
 * Set pixel data of the sprite
 * @param pixel pointer to the pixel data
 * @param is_release true if the object must release the
 *        pixel data memory at its destruction 
 */
void
sprite_object::set_pixel_data (char *pixel, bool is_release)
{
  pixel_data = pixel;
  is_release_pixel_data = is_release;
}

/*
 * Statics members
 */

/** Color cylcing from paddle's projectiles of fire 1 */
const Sint32
sprite_object::cycling_01[] =
{
  0x7e7e7e7e, 0x7e7e7e7e, 0x4b4b4b4b, 0x4b4b4b4b,
  0x7a7a7a7a, 0x7a7a7a7a, 0x18181818, 0x5a5a5a5a
};
/** Color cylcing from paddle's projectiles of fire 2 */
const Sint32
sprite_object::cycling_02[] =
{
  0x3f3f3f3f, 0x3f3f3f3f, 0x17171717, 0x17171717, 0x35353535,
  0x35353535, 0x18181818, 0x22222222
};

/** Ship ennemies */
sprite_coordinates
BOB_POS000[] =
{
  /* ship 1 */
  {0, 70}, {1, 70}, {2, 70}, {3, 70},
  {4, 70}, {5, 70}, {6, 70}, {7, 70},
  /* ship 2 */
  {0, 86}, {1, 86}, {2, 86}, {3, 86},
  {4, 86}, {5, 86}, {6, 86}, {7, 86},
  /* ship 3 */
  {0, 102}, {1, 102}, {2, 102}, {3, 102},
  {4, 102}, {5, 102}, {6, 102}, {7, 102},
  /* ship 4 */
  {0, 118}, {1, 118}, {2, 118}, {3, 118},
  {4, 118}, {5, 118}, {6, 118}, {7, 118},
  /* ship 5 */
  {0, 134}, {1, 134}, {2, 134}, {3, 134},
  {4, 134}, {5, 134}, {6, 134}, {7, 134},
  /* ship 6 */
  {0, 150}, {1, 150}, {2, 150}, {3, 150},
  {4, 150}, {5, 150}, {6, 150}, {7, 150},
  /* ship 7 */
  {0, 166}, {1, 166}, {2, 166}, {3, 166},
  {4, 166}, {5, 166}, {6, 166}, {7, 166},
  /* ship 8 */
  {0, 182}, {1, 182}, {2, 182}, {3, 182},
  {4, 182}, {5, 182}, {6, 182}, {7, 182},
  /* ship 9 */
  {0, 198}, {1, 198}, {2, 198}, {3, 198},
  {4, 198}, {5, 198}, {6, 198}, {7, 198},
  /* ship 10 */
  {0, 214}, {1, 214}, {2, 214}, {3, 214},
  {4, 214}, {5, 214}, {6, 214}, {7, 214},
  /* ship 11 */
  {8, 70}, {9, 70}, {10, 70}, {11, 70},
  {12, 70}, {13, 70}, {14, 70}, {15, 70},
  /* ship 12 */
  {8, 86}, {9, 86}, {10, 86}, {11, 86},
  {12, 86}, {13, 86}, {14, 86}, {15, 86},
  /* ship 13 */
  {8, 102}, {9, 102}, {10, 102}, {11, 102},
  {12, 102}, {13, 102}, {14, 102}, {15, 102},
  /* ship 14 */
  {8, 118}, {9, 118}, {10, 118}, {11, 118},
  {12, 118}, {13, 118}, {14, 118}, {14, 118},
  /* ship 15 */
  {8, 134}, {9, 134}, {10, 134}, {11, 134},
  {12, 134}, {13, 134}, {14, 134}, {15, 134},
  /* ship 16 */
  {8, 150}, {9, 150}, {10, 150}, {11, 150},
  {12, 150}, {13, 150}, {14, 150}, {15, 150},
  /* ship 17 */
  {8, 166}, {9, 166}, {10, 166}, {11, 166},
  {12, 166}, {13, 166}, {14, 166}, {15, 166},
  /* ship 18 */
  {8, 182}, {9, 182}, {10, 182}, {11, 182},
  {12, 182}, {13, 182}, {14, 182}, {15, 182},
  /* ship 19 */
  {8, 198}, {9, 198}, {10, 198}, {11, 198},
  {12, 198}, {13, 198}, {14, 198}, {15, 198},
  /* ship 20 */
  {8, 214}, {9, 214}, {10, 214}, {11, 214},
  {12, 214}, {13, 214}, {14, 214}, {15, 214},
  /* ship's explosion 1 */
  {0, 54}, {1, 54}, {2, 54}, {3, 54},
  {4, 54}, {5, 54}, {6, 54}, {7, 54},
  /* ship's explosion 2 */
  {8, 54}, {9, 54}, {10, 54}, {11, 54},
  {12, 54}, {13, 54}, {14, 54}, {15, 54}
};
sprite_description
BOB_NUM000 = { 16, 16, 22 * 8, BOB_POS000 };

/** Ejectors */
sprite_coordinates
BOB_POS001[] = { {14, 5} };
sprite_description
BOB_NUM001 = { 16, 16, 1, BOB_POS001 };
sprite_coordinates
BOB_POS002[] = { {15, 5} };
sprite_description
BOB_NUM002 = { 16, 16, 1, BOB_POS002 };
sprite_coordinates
BOB_POS003[] = { {16, 5} };
sprite_description
BOB_NUM003 = { 16, 16, 1, BOB_POS003 };
sprite_coordinates
BOB_POS004[] = { {17, 5} };
sprite_description
BOB_NUM004 = { 16, 16, 1, BOB_POS004 };

/* Bricks */
/** Vertical side brick */
sprite_coordinates
BOB_POS005[] = { {18, 5} };
sprite_description
BOB_NUM005 = { 4, 16, 1, BOB_POS005 };
/** Horizontal side brick */
sprite_coordinates
BOB_POS006[] = { {11, 0} };
sprite_description
BOB_NUM006 = { 16, 4, 1, BOB_POS006 };
sprite_coordinates
/** Standard brick */
BOB_POS007[] = { {14, 21} };
sprite_description
BOB_NUM007 = { 16, 7, 1, BOB_POS007 };

/** Paddles */
sprite_coordinates
BOB_POS008[28] =
{
  {0, 0}, {4, 0}, {8, 0}, {12, 0}, {16, 0}, {20, 0}, {24, 0},
  {0, 8}, {4, 8}, {8, 8}, {12, 8}, {16, 8}, {20, 8}, {24, 8},
  {0, 16}, {4, 16}, {8, 16}, {12, 16}, {16, 16}, {20, 16}, {24, 16},
  {0, 24}, {4, 24}, {8, 24}, {12, 24}, {16, 24}, {20, 24}, {24, 24}
};
sprite_description
BOB_NUM008 = { 64, 8, 28, BOB_POS008 };
sprite_coordinates
BOB_POS009[28] =
{ {0, 32}, {1, 32}, {2, 32}, {3, 32}, {4, 32}, {5, 32}, {6, 32},
  {7, 32}, {8, 32}, {9, 32}, {10, 32}, {11, 32}, {12, 32}, {13, 32},
  {14, 32}, {15, 32}, {16, 32}, {17, 32}, {18, 32}, {19, 32}, {20, 32},
  {21, 32}, {22, 32}, {23, 32}, {24, 32}, {25, 32}, {26, 32}, {27, 32}
};
sprite_description
BOB_NUM009 = { 8, 64, 28, BOB_POS009 };

/** The seven gigablitz */
sprite_coordinates
BOB_POS021[1] = { {0, 0} };
sprite_description
BOB_NUM021 = { 16, 32, 1, BOB_POS021 };

sprite_coordinates
BOB_POS022[1] = { {0, 32} };
sprite_description
BOB_NUM022 = { 24, 48, 1, BOB_POS022 };

sprite_coordinates
BOB_POS023[1] = { {0, 80} };
sprite_description
BOB_NUM023 = { 32, 64, 1, BOB_POS023 };

sprite_coordinates
BOB_POS024[1] = { {0, 144} };
sprite_description
BOB_NUM024 = { 40, 80, 1, BOB_POS024 };

sprite_coordinates
BOB_POS025[1] = { {0, 224} };
sprite_description
BOB_NUM025 = { 48, 96, 1, BOB_POS025 };

sprite_coordinates
BOB_POS026[1] = { {0, 320} };
sprite_description
BOB_NUM026 = { 56, 112, 1, BOB_POS026 };

sprite_coordinates
BOB_POS027[1] = { {0, 432} };
sprite_description
BOB_NUM027 = { 64, 128, 1, BOB_POS027 };

/** Balls */
sprite_coordinates
BOB_POS010[] =
{
  /* size 1 green ball */
  {14, 46},
  /* size 1 grey ball */
  {1, 497},
  /* size 1 yellow ball */
  {14, 28},
  /* size 2 green ball */
  {15, 37},
  /* size 2 grey ball */
  {2, 497},
  /* size 2 yellow ball */
  {15, 28},
  /* size 3 green ball */
  {3, 488},
  /* size 3 grey ball */
  {3, 497},
  /* size 3 yellow ball */
  {14, 37}
};
sprite_description
BOB_NUM010 = { 9, 9, 9, BOB_POS010 };

/** Projectiles fired by a paddle in bricks level */
sprite_coordinates
BOB_POS011[] =
{
  /* projectile used for the fire power 1 */
  {3, 0}, 
  {4, 0},
  {5, 0},
  {6, 0},
  /* projectile used for the fire power 2 */
  {7, 0},
  {8, 0},
  {9, 0},
  {10, 0}
};
sprite_description
BOB_NUM011 = { 5, 5, 8, BOB_POS011 };

/** The money capsule */
sprite_coordinates
BOB_POS012[] =
  { {7, 33}, {8, 33}, {9, 33}, {10, 33}, {11, 33}, {12, 33}, {13, 33} };
sprite_description
BOB_NUM012 = { 16, 7, 7, BOB_POS012 };

/** Capsule bonuses or a penalties */
sprite_coordinates
BOB_POS013[] =
{
  /* expand paddles */
  {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5},
  /* shrink paddles */
  {0, 12}, {1, 12}, {2, 12}, {3, 12}, {4, 12}, {5, 12}, {6, 12},
  /* extra life */
  {0, 19}, {1, 19}, {2, 19}, {3, 19}, {4, 19}, {5, 19}, {6, 19},
  /* lose a life */
  {0, 26}, {1, 26}, {2, 26}, {3, 26}, {4, 26}, {5, 26}, {6, 26},
  /* power ball 1 */
  {0, 33}, {1, 33}, {2, 33}, {3, 33}, {4, 33}, {5, 33}, {6, 33},
  /* power ball 2 */
  {0, 40}, {1, 40}, {2, 40}, {3, 40}, {4, 40}, {5, 40}, {6, 40},
  /* extra balls */
  {0, 47}, {1, 47}, {2, 47}, {3, 47}, {4, 47}, {5, 47}, {6, 47},
  /* multi balls */
  {7, 5}, {8, 5}, {9, 5}, {10, 5}, {11, 5}, {12, 5}, {13, 5},
  /* fire power 1 */
  {7, 12}, {8, 12}, {9, 12}, {10, 12}, {11, 12}, {12, 12}, {13, 12},
  /* fire power 2 */
  {7, 19}, {8, 19}, {9, 19}, {10, 19}, {11, 19}, {12, 19}, {13, 19},
  /* glue */
  {7, 26}, {8, 26}, {9, 26}, {10, 26}, {11, 26}, {12, 26}, {13, 26},
  /* inverse control */
  {7, 40}, {8, 40}, {9, 40}, {10, 40}, {11, 40}, {12, 40}, {13, 40},
  /* balls size 2 */
  {17, 219}, {18, 219}, {18, 246}, {19, 246}, {19, 261}, {19, 239}, {16, 268},
  /* balls size 3 */
  {17, 268}, {18, 268}, {19, 268}, {10, 481}, {11, 481}, {12, 481}, {13, 481},
  /* chance capsule */
  {14, 392}, {14, 399}, {14, 406}, {14, 413}, {14, 420}, {14, 427}, {14, 434},
  /* enable hugely options */
  {15, 392}, {15, 399}, {15, 406}, {15, 413}, {15, 420}, {15, 427}, {15, 434},
  /* enable bottom wall */
  {0, 635}, {1, 635}, {2, 635}, {3, 635}, {4, 635}, {5, 635}, {6, 635},
  /* enable bottom robot paddle */
  {7, 635}, {8, 635}, {9, 635}, {10, 635}, {11, 635}, {12, 635}, {13, 635},
  /* enable the balls control */
  {14, 635}, {15, 635}, {16, 635}, {17, 635}, {18, 635}, {19, 635}, {2, 490},
  /* paddle invincibility (only used in guardians level */
  {16, 205}, {17, 205}, {18, 205}, {16, 212}, {17, 212}, {18, 212}, {16, 219}
};
sprite_description
BOB_NUM013 = { 16, 7, 7 * 20, BOB_POS013 };

/** font of 43 chars used to display "LEVEL n COMPLETED */ 
sprite_coordinates
BOB_POS014[] =
  { {0, 758}, {1, 758}, {2, 758}, {3, 758}, {4, 758}, {5, 758}, {6, 758}, {7,
      758},
    {8, 758}, {9, 758},
    {10, 758}, {11, 758}, {12, 758}, {13, 758}, {14, 758}, {15, 758}, {16, 758},
    {17, 758}, {18, 758}, {19, 758},
    {0, 774}, {1, 774}, {2, 774}, {3, 774}, {4, 774}, {5, 774}, {6, 774}, {7,
        774},
    {8, 774}, {9, 774},
    {10, 774}, {11, 774}, {12, 774}, {13, 774}, {14, 774}, {15, 774}, {16, 774},
    {17, 774}, {18, 774}, {19, 774},
    {0, 790}, {1, 790}, {2, 790}
  };
sprite_description
BOB_NUM014 = { 16, 16, 42, BOB_POS014 };

// LED du magasin --------------------------------------------------------------
sprite_coordinates
BOB_POS015[] = { {13, 0} };
sprite_description
BOB_NUM015 = { 16, 3, 1, BOB_POS015 };
sprite_coordinates
BOB_POS057[] = { {15, 0} };
sprite_description
BOB_NUM057 = { 16, 5, 1, BOB_POS057 };

/** GAME OVER letters */
sprite_coordinates
BOB_POS016[] =
{
  /* G */
  {0, 246},
  /* A */
  {2, 246},
  /* M */
  {4, 246},
  /* E */
  {6, 246},
  /* O */
  {8, 246},
  /* V */
  {10, 246},
  /* E */
  {12, 246},
  /* R */
  {14, 246}
};
sprite_description
BOB_NUM016 = { 32, 32, 8, BOB_POS016 };

/** Font used in the main menu scrolltext */
sprite_coordinates
BOB_POS017[43] =
{
  {0, 587}, {1, 587}, {2, 587}, {3, 587}, {4, 587},
  {5, 587}, {6, 587}, {7, 587}, {8, 587}, {9, 587},
  {10, 587}, {11, 587}, {12, 587}, {13, 587}, {14, 587},
  {15, 587}, {16, 587}, {17, 587}, {18, 587}, {19, 587},
  {0, 607}, {1, 607}, {2, 607}, {3, 607}, {4, 607},
  {5, 607}, {6, 607}, {7, 607}, {8, 607}, {9, 607},
  {10, 607}, {11, 607}, {12, 607}, {13, 607}, {14, 607},
  {15, 607}, {16, 607}, {17, 607}, {18, 607}, {19, 607},
  {17, 520}, {18, 520}, {16, 520}
};
sprite_description
BOB_NUM017 = { 16, 20, 43, BOB_POS017 };

/** TecnoballZ logo */
sprite_coordinates
BOB_POS018[1] = { {0, 520} };
sprite_description
BOB_NUM018 = { 256, 67, 1, BOB_POS018 };

/** Mouse cursor pointer 1 */
sprite_coordinates
BOB_POS019[8] =
{  
  {0, 230}, {1, 230}, {2, 230}, {3, 230},
  {4, 230}, {5, 230}, {6, 230}, {7, 230}
};
sprite_description
BOB_NUM019 = { 16, 16, 8, BOB_POS019 };
/** Mouse cursor pointer 2 */
sprite_coordinates
BOB_POS020[8] =
{
  {8, 230}, {9, 230}, {10, 230}, {11, 230},
  {12, 230}, {13, 230}, {14, 230}, {15, 230}
};
sprite_description
BOB_NUM020 = { 16, 16, 8, BOB_POS020 };

// lettre pour afficher la somme d'argent --------------------------------------
sprite_coordinates
BOB_POS028[10] = { {3, 798}, {4, 798}, {5, 798}, {6, 798}, {7, 798},
                   {8, 798}, {9, 798}, {10, 798}, {11, 798}, {12, 798}
                 };
sprite_description
BOB_NUM028 = { 8, 8, 10, BOB_POS028 };

/** Bottom paddle robot used in bricks level */
sprite_coordinates
BOB_POS029[1] = { {16, 448} };
sprite_description
BOB_NUM029 = { 64, 8, 1, BOB_POS029 };

/*
 * GUARDIANS
 */
sprite_coordinates
BOB_POS030[1] = { {0, 278} };
sprite_description
BOB_NUM030 = { 64, 104, 1, BOB_POS030 };
sprite_coordinates
BOB_POS031[1] = { {4, 278} };
sprite_description
BOB_NUM031 = { 64, 99, 1, BOB_POS031 };
sprite_coordinates
BOB_POS032[1] = { {8, 278} };
sprite_description
BOB_NUM032 = { 64, 90, 1, BOB_POS032 };
sprite_coordinates
BOB_POS033[1] = { {8, 368} };
sprite_description
BOB_NUM033 = { 64, 92, 1, BOB_POS033 };
sprite_coordinates
BOB_POS034[1] = { {12, 278} };
sprite_description
BOB_NUM034 = { 64, 114, 1, BOB_POS034 };
sprite_coordinates
BOB_POS035[1] = { {16, 278} };
sprite_description
BOB_NUM035 = { 64, 163, 1, BOB_POS035 };
sprite_coordinates
BOB_POS036[1] = { {0, 382} };
sprite_description
BOB_NUM036 = { 32, 16, 1, BOB_POS036 };
sprite_coordinates
BOB_POS037[1] = { {0, 398} };
sprite_description
BOB_NUM037 = { 32, 55, 1, BOB_POS037 };
sprite_coordinates
BOB_POS038[1] = { {0, 453} };
sprite_description
BOB_NUM038 = { 32, 36, 1, BOB_POS038 };
sprite_coordinates
BOB_POS039[1] = { {2, 382} };
sprite_description
BOB_NUM039 = { 32, 52, 1, BOB_POS039 };
sprite_coordinates
BOB_POS040[1] = { {2, 434} };
sprite_description
BOB_NUM040 = { 32, 36, 1, BOB_POS040 };
sprite_coordinates
BOB_POS041[1] = { {4, 377} };
sprite_description
BOB_NUM041 = { 32, 82, 1, BOB_POS041 };
sprite_coordinates
BOB_POS042[1] = { {4, 459} };
sprite_description
BOB_NUM042 = { 32, 47, 1, BOB_POS042 };
sprite_coordinates
BOB_POS043[1] = { {6, 377} };
sprite_description
BOB_NUM043 = { 32, 74, 1, BOB_POS043 };
sprite_coordinates
BOB_POS044[1] = { {6, 451} };
sprite_description
BOB_NUM044 = { 32, 65, 1, BOB_POS044 };
sprite_coordinates
BOB_POS045[1] = { {12, 392} };
sprite_description
BOB_NUM045 = { 32, 85, 1, BOB_POS045 };
/* paddle */
sprite_coordinates
BOB_POS046[1] = { {16, 246} };
sprite_description
BOB_NUM046 = { 32, 8, 1, BOB_POS046 };
/* bullet fired by a guardian */
sprite_coordinates
BOB_POS047[16] =
  { {19, 141}, {8, 460}, {19, 152}, {9, 460}, {19, 163}, {10, 460}, {19, 174},
    {11, 460},
    {19, 185}, {8, 471}, {19, 196}, {9, 471}, {19, 207}, {16, 573}, {19, 218},
    {17, 573}
  };
sprite_description
BOB_NUM047 = { 11, 11, 16, BOB_POS047 };


// explosion 1 ****
sprite_coordinates
BOB_POS048[8] = { {0, 54}, {1, 54}, {2, 54}, {3, 54},   //EXPLOSION 1
                  {4, 54}, {5, 54}, {6, 54}, {7, 54}
                };
sprite_description
BOB_NUM048 = { 16, 16, 8, BOB_POS048 };

// explosion 2 ****
sprite_coordinates
BOB_POS049[8] = { {8, 54}, {9, 54}, {10, 54}, {11, 54}, //EXPLOSION 2
                  {12, 54}, {13, 54}, {14, 54}, {15, 54}
                };
sprite_description
BOB_NUM049 = { 16, 16, 8, BOB_POS049 };

// boule argentee (congratulation)
sprite_coordinates
BOB_POS050[8] = { {14, 462} };
sprite_description
BOB_NUM050 = { 25, 26, 1, BOB_POS050 };

// niveau d'energie des gadiens --------------------------------------//
sprite_coordinates
BOB_POS051[] = { {14, 0} };
sprite_description
BOB_NUM051 = { 16, 1, 1, BOB_POS051 };

// gem ---------------------------------------------------------------//
sprite_coordinates
BOB_POS052[] = { {10, 472},     // grey square
                 {11, 472},                      // green sphere
                 {18, 574},                      // yellow ring
                 {19, 574},                      // blue triangle
                 {3, 480},                       // rhombus
                 {3, 472}                        // pentagon
               };
sprite_description
BOB_NUM052 = { 8, 8, 6, BOB_POS052 };

// bottom wall -------------------------------------------------------//
sprite_coordinates
BOB_POS053[] = { {0, 627} };
sprite_description
BOB_NUM053 = { 192, 8, 1, BOB_POS053 };

sprite_coordinates
BOB_POS054[] = { {4, 730}, {5, 730}, {6, 730}, {7, 730},
                 {8, 730}, {9, 730}, {10, 730}, {11, 730},
                 {12, 730}, {13, 730}, {14, 730}, {15, 730},
                 {16, 730}, {17, 730}, {18, 730}, {19, 730},
               };
sprite_description
BOB_NUM054 = { 9, 9, 16, BOB_POS054 };

sprite_coordinates
BOB_POS055[] = { {16, 21}, {17, 21}, {18, 21}, {19, 21}, {16, 25}, {17, 25},
                 {18, 25}, {19, 25}
               };
sprite_description
BOB_NUM055 = { 4, 4, 8, BOB_POS055 };

sprite_coordinates
BOB_POS056[] =
  { {0, 642}, {2, 642}, {4, 642}, {6, 642}, {8, 642}, {10, 642}, {12, 642},
    {14, 642}, {16, 642}, {18, 642},
    {0, 671}, {2, 671}, {4, 671}, {6, 671}, {8, 671}, {10, 671}, {12, 671}, {14,
        671},
    {16, 671}, {18, 671},
    {0, 700}, {2, 700}, {4, 700}, {6, 700}, {8, 700}, {10, 700}, {12, 700}, {14,
        700},
    {16, 700}, {18, 700},
    {0, 729}, {2, 729}
  };
sprite_description
BOB_NUM056 = { 32, 29, 32, BOB_POS056 };

// bricks
sprite_coordinates
BOB_POS058[] =
  {
    {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0},
    {0, 7}, {1, 7}, {2, 7}, {3, 7}, {4, 7}, {5, 7}, {6, 7},
    {0, 14}, {1, 14}, {2, 14}, {3, 14}, {4, 14}, {5, 14}, {6, 14},
    {0, 21}, {1, 21}, {2, 21}, {3, 21}, {4, 21}, {5, 21}, {6, 21},
    {0, 28}, {1, 28}, {2, 28}, {3, 28}, {4, 28}, {5, 28}, {6, 28},
    {0, 35}, {1, 35}, {2, 35}, {3, 35}, {4, 35}, {5, 35}, {6, 35},
    {0, 42}, {1, 42}, {2, 42}, {3, 42}, {4, 42}, {5, 42}, {6, 42},
    {0, 49}, {1, 49}, {2, 49}, {3, 49}, {4, 49}, {5, 49}, {6, 49},
    {0, 56}, {1, 56}, {2, 56}, {3, 56}, {4, 56}, {5, 56}, {6, 56}
  };
sprite_description
BOB_NUM058 = { 16, 7, 63, BOB_POS058 };

const sprite_description *
sprite_object::zelistBOB[NUMOF_SPRITE_TYPES] =
  {
    &BOB_NUM000,   // BouisBouis
    &BOB_NUM001,                  // Ejector
    &BOB_NUM002,                  //
    &BOB_NUM003,                  //
    &BOB_NUM004,                  //
    &BOB_NUM005,                  // Brique verticale
    &BOB_NUM006,                  // Brique horizontale
    &BOB_NUM007,                  // Brique jeu
    &BOB_NUM008,                  // Raquette horizontale
    &BOB_NUM009,                  // Raquette verticale
    &BOB_NUM010,                  // Balls
    &BOB_NUM011,                  // Bumper's fires
    &BOB_NUM012,                  // Capsule of money
    &BOB_NUM013,                  // Gadgets (bonuses or maluses)
    &BOB_NUM014,                  // Lettres (annonce)
    &BOB_NUM015,                  // shop's led (low-res)
    &BOB_NUM016,                  // Lettres GameOver
    &BOB_NUM017,                  // Lettres defilement menu
    &BOB_NUM018,                  // Logo TecnoballZ
    &BOB_NUM019,                  // Pointeur souris 1
    &BOB_NUM020,                  // Pointeur souris 2
    &BOB_NUM021,                  // GigaBlitz 1
    &BOB_NUM022,                  // GigaBlitz 2
    &BOB_NUM023,                  // GigaBlitz 3
    &BOB_NUM024,                  // GigaBlitz 4
    &BOB_NUM025,                  // GigaBlitz 5
    &BOB_NUM026,                  // GigaBlitz 6
    &BOB_NUM027,                  // GigaBlitz 7
    &BOB_NUM028,                  // Chiffres de 0 a 10 pour afficher le credit
    &BOB_NUM029,                  // Robot bottom bumper (bricks levels)
    &BOB_NUM030,                  // Guard final 1
    &BOB_NUM031,                  // Guard final 2
    &BOB_NUM032,                  // Guard final 3
    &BOB_NUM033,                  // Guard final 4
    &BOB_NUM034,                  // Guard final 5
    &BOB_NUM035,                  // Guard final 6
    &BOB_NUM036,                  // Guard intermediary 1
    &BOB_NUM037,                  // Guard intermediary 1
    &BOB_NUM038,                  // Guard intermediary 2
    &BOB_NUM039,                  // Guard intermediary 2
    &BOB_NUM040,                  // Guard intermediary 3
    &BOB_NUM041,                  // Guard intermediary 3
    &BOB_NUM042,                  // Guard intermediary 4
    &BOB_NUM043,                  // Guard intermediary 4
    &BOB_NUM044,                  // Guard intermediary 5
    &BOB_NUM045,                  // Guard intermediary 5
    &BOB_NUM046,                  // bumper into guards level
    &BOB_NUM047,                  // Guards's weapeon
    &BOB_NUM048,                  // explosion's guard (type 1)
    &BOB_NUM049,                  // explosion's guard (type 2)
    &BOB_NUM050,                  // boule argentee
    &BOB_NUM051,                  // guards's energy bar
    &BOB_NUM052,                  // Gem
    &BOB_NUM053,
    &BOB_NUM054,                  //directeur
    &BOB_NUM055,                  //echape menu
    &BOB_NUM056,                  //maget eye
    &BOB_NUM057,                  //shop's led (high-res)
    &BOB_NUM058                  //brick
  };


Sint32 sprite_object::ombredecax = handler_display::SHADOWOFFX;
Sint32 sprite_object::ombredecay = handler_display::SHADOWOFFY;
char sprite_object::ombrepixel = handler_display::SHADOW_PIX;
Sint32 sprite_object::ombrepixe4 = handler_display::SHADOWLONG;
