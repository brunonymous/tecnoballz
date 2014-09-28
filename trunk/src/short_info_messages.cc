/**
 * @file short_info_messages.cc 
 * @brief Display short info messages in bricks levels 
 * @created 2002-12-30
 * @date 2007-03-16
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
#include "../include/short_info_messages.h"
#include "../include/handler_resources.h"

short_info_messages *
short_info_messages::info_messages_singleton = NULL;

/**
 * Create the info messages object
 */
short_info_messages::short_info_messages ()
{
  messages = NULL;
  bitmap_small_fonts = (bitmap_data *) NULL;
  restore_surface = (bitmap_data *) NULL;
  delay_counter = 0;
  object_init ();
}

/**
 * Relesae the info messages object
 */
short_info_messages::~short_info_messages ()
{
  if (NULL != bitmap_small_fonts)
    {
      delete bitmap_small_fonts;
      bitmap_small_fonts = (bitmap_data *) NULL;
    }
  if (NULL != restore_surface)
    {
      delete restore_surface;
      restore_surface = (bitmap_data *) NULL;
    }
  if (NULL != messages)
    {
      delete[](char *) messages;
      messages = NULL;
    }
  object_free ();
  info_messages_singleton = NULL;
}

/**
 * Get the object instance
 * short_info_messages is a singleton
 * @return the short_info_messages object 
 */
short_info_messages *
short_info_messages::get_instance ()
{
  if (NULL == info_messages_singleton)
    {
      info_messages_singleton = new short_info_messages ();
    }
  return info_messages_singleton;
}

/**
 * Initialize, load fonts bitmap, create offscreen
 */
void
short_info_messages::initialize ()
{
  if (NULL == bitmap_small_fonts)
    {
      bitmap_small_fonts = new bitmap_data ();
      bitmap_small_fonts->load (handler_resources::BITMAP_SMALL_FONTS);

      dest_offset = game_screen->get_row_size ();
      src_offset = bitmap_small_fonts->get_row_size ();
      screen = game_screen->get_pixel_data (XCOORD_MESSAGE * resolution,
                                            YCOORD_MESSAGE * resolution);
      font_pixels = bitmap_small_fonts->get_pixel_data ();
      /* height of a char 6 or 12 pixels */
      height_font = 6 * resolution;
      /* width of a char 4 or 8 pixels */
      width_font = 4 * resolution;
    }

  /* more initialization if exists a static background with bricks
   * and ejectors draw inside*/
  init_with_background();

  /* load message strings from a simple text file */ 
  if (NULL == messages)
    {
      messages =
        resources->load_texts (handler_resources::TEXTS_MESSAGES,
                               MAX_OF_MESSAGES, MAX_OF_CHARS, 0);
      for (Uint32 i = 0; i < MAX_OF_MESSAGES; i++)
        {
          char *str = messages[i];
          for (Uint32 j = 0; j < MAX_OF_CHARS; j++)
            {
              char c = str[j];
              if (' ' == c)
                {
                  str[j] = '[';
                  continue;
                }
              if ('!' == c)
                {
                  str[j] = '\\';
                  continue;
                }
              if ('?' == c)
                {
                  str[j] = ']';
                  continue;
                }
              if ('.' == c)
                {
                  str[j] = '^';
                  continue;
                }
              if (c < 'A' or c > 'Z')
                {
                  str[j] = '[';
                }
            }
        }
    }
  clear_messages_request ();
}

/**
 * Save background for restore
 */
void
short_info_messages::init_with_background ()
{
  if (!has_background)
    {
      return;
    }

  background =
    background_screen->get_pixel_data (XCOORD_MESSAGE * resolution,
				       YCOORD_MESSAGE * resolution);
  /* width of message surface 16*4=64 or 16*8=128 */
  width_surface = width_font * MAX_OF_CHARS;

  if (NULL == restore_surface)
    {
      restore_surface = new bitmap_data ();
      restore_surface->create_surface (width_surface, height_font);
      restore_pixels = restore_surface->get_pixel_data ();
    }

  /* save the background under message */
  char *dest = restore_pixels;
  char *source = background;
  Sint32 mod = background_screen->get_row_size ();
  for (Uint32 y = 0; y < height_font; y++)
    {
      for (Uint32 x = 0; x < width_surface; x++)
        {
          dest[x] = source[x];
        }
      dest = dest + width_surface;
      source = source + mod;
    }
}

/**
 * Clear all message request 
 */
void
short_info_messages::clear_messages_request ()
{
  for (Uint32 i = 0; i < MAX_OF_MESSAGES; i++)
    {
      messages_request[i] = false;
    }
}

/**
 * Request for display a message
 * @param id message identifier from 0 to 34
 */
void
short_info_messages::send_message_request (Uint32 id)
{
  messages_request[id] = true;
}

/**
 * Display short info messages
 */
void
short_info_messages::run ()
{
  if (delay_counter > 0)
    {
      draw ();
      return;
    }
  Sint32 i = MAX_OF_MESSAGES;
  while (--i >= 0)
    {
      if (!messages_request[i])
        {
          continue;
        }
      messages_request[i] = false;
      current_char = 0;
      delay_counter = 66;
      required_message = messages[i];
      draw ();
      return;
    }
}

/**
 * Draw or clear a info message
 */
void
short_info_messages::draw ()
{
  if (current_char < 16)
    {
      /* draw a message */
      if (has_background)
        {
          display_in_background ();
        }
      else
        {
          display (height_font);
        }
      current_char++;
    }
  else
    {
      delay_counter--;
      if (has_background)
	{
	  if (delay_counter < height_font)
            {
              clear_message ();
            }
	}
      else
	{ 
	  Uint32 h = height_font;
	  if (delay_counter <= h )
	    {
	      h = delay_counter;
	    }
	  display (h);
        }
    }
}

/**
 * Draw the message
 * @param height of a char 
 */
void
short_info_messages::display (Uint32 height)
{
  Sint32 font_mod = src_offset;
  Sint32 dest_mod = dest_offset;
  char *dest = screen;
  for (Sint32 i = 0; i < current_char; i++, dest += width_font)
    {
      Sint32 c = required_message[i] - 'A';
      char *font = font_pixels + c * 16 * resolution;
      char *d = dest;
      for (Uint32 y = 0; y < height; y++, font += font_mod, d += dest_mod)
        {
          for (Uint32 x = 0; x < width_font; x++)
            {
              char pixel = font[x];
              if (pixel != 0)
                {
                  d[x] = pixel;
                }
            }
        }
    }
}

/**
 * Display the message character by character in the background
 */
void
short_info_messages::display_in_background ()
{
  const char *str = required_message;
  str = str + current_char;
  Sint32 c = *str - 'A';
  char *font = font_pixels + c * 16 * resolution;
  Sint32 font_mod = src_offset;
  Sint32 dest_mod = dest_offset;
  char *dest1 = screen + (current_char * width_font);
  char *dest2 = background + (current_char * width_font);
  for (Uint32 y = 0; y < height_font; y++)
    {
      for (Uint32 x = 0; x < width_font; x++)
        {
          char pixel = font[x];
          if (pixel > 0)
            {
              dest1[x] = pixel;
              dest2[x] = pixel;
            }
        }
      font = font + font_mod;
      dest1 = dest1 + dest_mod;
      dest2 = dest2 + dest_mod;
    }
}

/**
 * Clear the message line by line
 */
void
short_info_messages::clear_message ()
{
  Sint32 xcoord = XCOORD_MESSAGE * resolution;
  Sint32 ycoord = (YCOORD_MESSAGE * resolution) + delay_counter;
  char *dest1 = game_screen->get_pixel_data (xcoord, ycoord);
  char *dest2 = background_screen->get_pixel_data (xcoord, ycoord);
  char *source = restore_pixels + (delay_counter * width_surface);
  for (Uint32 i = 0; i < width_surface; i++)
    {
      char pixel = source[i];
      dest1[i] = pixel;
      dest2[i] = pixel;
    }
}
