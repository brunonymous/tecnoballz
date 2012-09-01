/**
 * @file handler_popup_menu.cc
 * @brief popup menu handler (When the [Esc] key is pressed)
 * @created 2004-08-08
 * @date 2012-08-19
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.19 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_popup_menu.cc,v 1.19 2012-08-19 19:14:25 gurumeditation Exp $
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
#include "../include/handler_resources.h"
#include "../include/handler_popup_menu.h"
#include "../include/sprite_object.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_menu_events.h"

/**
 * Create the popup menu handler
 */
handler_popup_menu::handler_popup_menu ()
{
  menu_first_color_index = 0;
  menu_number = 0;
  texts_of_menus = NULL;
  menu_events = new handler_menu_events();
}

/**
 * Release the popup menu handler
 */
handler_popup_menu::~handler_popup_menu ()
{
  if (screen_menu != NULL)
    {
      delete screen_menu;
      screen_menu = (bitmap_data *) NULL;
    }
  if (bitmap_fonts != NULL)
    {
      delete bitmap_fonts;
      bitmap_fonts = (bitmap_data *) NULL;
    }
  if (texts_of_menus != NULL)
    {
      delete[](char *) texts_of_menus;
      texts_of_menus = NULL;
    }
  if (menu_events != NULL)
    {
      delete menu_events;
      menu_events = NULL;
    }
}

/**
 * Read the texts file
 */
void
handler_popup_menu::load_text_file ()
{
  if (texts_of_menus != NULL)
    {
      return;
    }
  texts_of_menus =
    resources->load_texts (handler_resources::TEXTS_POPUP_MENU,
                           MAX_OF_LINES, MAX_OF_CHARS, 0);
  menu_texts_pt[0] = &texts_of_menus[MENU_00];
  menu_texts_pt[1] = &texts_of_menus[MENU_01];
}

/**
 * Initialize palette color chars, if necessary (shop only)
 */
void
handler_popup_menu::initialize_palette ()
{
  SDL_Color *palette = display->get_palette ();
  SDL_Color *font_pal = palette + 239;
  Uint32 i = random_counter & 0x0F;
  if (i >= 10)
    {
      i = i - 10;
    }
  const Uint32 *color_scale = (handler_resources::color_gradations + i * 18);
  for (i = 0; i < 17; i++)
    {
      Uint32 color = color_scale[i];
      Uint32 blue = color & 0x000000ff;
      Uint32 green = color & 0x0000ff00;
      green = green >> 8;
      Uint32 red = color & 0x00ff0000;
      red = red >> 16;
      font_pal->r = red;
      font_pal->g = green;
      font_pal->b = blue;
      font_pal++;
    }
  display->enable_palette (palette);
}

/**
 * Initialize the popup menu
 * @param bmp Bitmap where are graphic elements of the menu
 */
void
handler_popup_menu::first_init (bitmap_data * bmp)
{
  load_text_file ();
  Uint32 w;
  switch (current_phase)
    {
    case BRICKS_LEVEL:
      /* determine if restore background (bricks levels and shop only) */
      if (has_background)
        {
          is_restore_background = true;
        }
      else
        {
          is_restore_background = false;
        }
      menu_number = 0;
      w = 256 * resolution;
      break;

    case SHOP:
      is_restore_background = true;
      menu_number = 1;
      w = 320 * resolution;
      initialize_palette ();
      break;

    case GUARDS_LEVEL:
    default:
      is_restore_background = false;
      menu_number = 0;
      w = 320 * resolution;
      initialize_palette ();
      break;
    }
  menu_xcenter = w / 2;

  /* determine line height into menu box */
  if (resolution == 2)
    {
      vertical_space = 17;
    }
  else
    {
      vertical_space = 8;
    }

  /* load font bitmap file and perform some initializations */
  load_bitmap_fonts (handler_resources::BITMAP_MENU_FONTS);
  build_menu_box (bmp, w);

}

/**
 * Build menu box where will be drawed the strings of menu
 */
void
handler_popup_menu::build_menu_box (bitmap_data * bmp, Uint32 w)
{
  /* determine height of the menu box */
  if (menu_number == 1)
    {
      num_of_lines = 3;
    }
  else
    {
      num_of_lines = 4;
    }
  num_of_columns = MAX_OF_CHARS;
  Uint32 numof_lines = num_of_lines + 2;

  /* allocate graphic buffer of menu box */
  screen_menu = new bitmap_data ();
  screen_menu->create_surface ((num_of_columns + 2) * char_height,
                               numof_lines * vertical_space);
  screen_menu->clear ();

  /* save coordinates of the sprites */
  char *sprite_pixels[8];
  const sprite_description *sprite_desc = sprite_object::zelistBOB[sprite_object::POPUP_MENU];
  /* height in pixels */
  Uint32 height = sprite_desc->height;
  height *= resolution;
  /* number of animation */
  Uint32 nanim = sprite_desc->number_of_images;
  /* width in pixels */
  Uint32 width = sprite_desc->width;
  width *= resolution;
  sprite_coordinates *coord = sprite_desc->coordinates;
  for (Uint32 i = 0; i < nanim; i++)
    {
      Sint32 pos_x = (Sint32) coord[i].xcoord;
      pos_x *= resolution;
      pos_x *= 16;
      Sint32 pos_y = (Sint32) coord[i].ycoord;
      pos_y *= resolution;
      sprite_pixels[i] = bmp->get_pixel_data (pos_x, pos_y);
    }
  Uint32 raw_src = bmp->get_row_size ();

  /* initialize sprite object */
  make_sprite (screen_menu);
  Sint32 ycoord = (240 * resolution - numof_lines * vertical_space) / 2;
  ycoord = (ycoord / vertical_space) * vertical_space;
  set_coordinates ((w - sprite_width) / 2, ycoord);

  /*
   * build the frame of menu box (with sprites)
   */
  Uint32 count = screen_menu->get_width () / width - 2;
  Sint32 xcoord = width;
  ycoord = screen_menu->get_height () - height;
  draw (sprite_pixels[0], 0, 0, raw_src, width, height);
  draw (sprite_pixels[3], 0, ycoord, raw_src, width, height);
  for (Uint32 i = 0; i < count; i++, xcoord += width)
    {
      draw (sprite_pixels[4], xcoord, 0, raw_src, width, height);
      draw (sprite_pixels[6], xcoord, ycoord, raw_src, width, height);
    }
  draw (sprite_pixels[1], xcoord, 0, raw_src, width, height);
  draw (sprite_pixels[2], xcoord, ycoord, raw_src, width, height);

  count = screen_menu->get_height () / height - 2;
  if (screen_menu->get_height () % height)
    {
      count++;
    }

  xcoord = screen_menu->get_width () - width;
  ycoord = height;
  for (Uint32 i = 0; i < count; i++, ycoord += height)
    {
      draw (sprite_pixels[5], 0, ycoord, raw_src, width, height);
      draw (sprite_pixels[7], xcoord, ycoord, raw_src, width, height);
    }
}

/**
 * Display a sprite into the "buffer" (copy byte to byte)
 * @param source Pointer to the pixels of the sprite
 * @param xcoord X-coordinate where draw the sprite
 * @param ycoord Y_coordinate where draw the sprite
 * @param raw_src Row size of the source bitmap
 * @param width Width of sprite
 * @param height Hieght of sprite
 */
void
handler_popup_menu::draw (char *source, Sint32 xcoord, Sint32 ycoord,
                          Sint32 raw_src, Sint32 width, Sint32 height)
{
  char *src = source;
  char *dest = screen_menu->get_pixel_data (xcoord, ycoord);
  Sint32 s_offset = raw_src;
  Sint32 d_offset = screen_menu->get_row_size ();
  Sint32 w = width;
  Sint32 h = height;
  for (Sint32 i = 0; i < h; i++)
    {
      for (Sint32 j = 0; j < w; j++)
        {
          /* read the pixel */
          char pixel = src[j];
          /* color 0? */
          if (pixel != 0)
            {
              /* no, put the pixel */
              dest[j] = pixel;
            }
        }
      src += s_offset;
      dest += d_offset;
    }
}

/**
 * Display and handle menu
 * @return Return code CAUSE_GAME_OVER, QUIT_TO_MAIN_MENU,
 *         or QUIT_TECNOBALLZ
 */
Sint32 handler_popup_menu::run ()
{
  Sint32 event = -1;

  /* [ESC] key: enable / disable menu box */
  if (keyboard->command_is_pressed (handler_keyboard::TOGGLE_POPUP_MENU))
    {
      if (!is_enabled)
        {
          menu_events->start (vertical_space, 1, num_of_lines,
                              menu_xcenter,
                              get_y_coord () + char_height);
        }
      is_enabled = true;
    }
  else
    {
      if (is_enabled && is_restore_background)
        {
          restore_rectangle_background ();
        }
      is_enabled = false;
      menu_events->stop ();
      return event;
    }

  Sint32 pos_y = 0;
  Sint32 incre = 0;
  if (menu_events->check (&pos_y, &incre))
    {
      event = (pos_y - y_coord) / vertical_space;
    }

  /* read color table offset (color line hover by mouse ) */
  if (menu_first_color_index++ > 32)
    {
      menu_first_color_index = 0;
    }
  if (resolution == 1)
    {
      display_320 ();
    }
  else
    {
      display_640 ();
    }

  /* copy menu box into screen */
  copy_to_game_screen ();

  if (event >= CONTINUE_PLAY_CURRENT && event <= QUIT_TECNOBALLZ)
    {
      if (is_enabled && is_restore_background)
        {
          restore_rectangle_background ();
        }
      is_enabled = false;
      keyboard->clear_command_keys ();
    }

  if (menu_number == 1 && event >= CAUSE_GAME_OVER)
    {
      event++;
    }
  if (event > 0)
    {
      SDL_ShowCursor (SDL_DISABLE);
    }
  return event;
}

/**
 * Display text in 640x480 mode
 */
void
handler_popup_menu::display_640 ()
{
  Sint32 color = menu_first_color_index;
  char *desP1 = pixel_data + char_height + row_size * vertical_space;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (vertical_space - 1) + (char_height * 2);
  Sint32 *basPT = (Sint32 *) caract_adr;
  char **lines = menu_texts_pt[menu_number];
  char *c = ascii2code;
  Uint32 a, b, j;
  Uint32 y = (keyboard->get_mouse_y () - y_coord) / vertical_space;
  y--;
  for (Uint32 k = 0; k < num_of_lines; k++, desP1 += offD2)
    {
      char *p = lines[k];
      if (y != k)
        {
          /* display normal line of n characters */
          for (j = 0; j < num_of_columns; j++)
            {
              a = *(p++) - 32;
              if (a != 0)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 4;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 16; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      d[2] = s[2];
                      d[3] = s[3];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                }
              desP1 = desP1 + 16;
            }
        }
      else
        {
          /* display selected line of 32 characters */
          for (j = 0; j < num_of_columns; j++)
            {
              unsigned char pixel = cycling_table[color];
              a = *(p++) - 32;
              if (a != 0)
                {
                  b = c[a];
                  unsigned char *s = (unsigned char *) basPT;
                  unsigned char *d = (unsigned char *) desP1;
                  b = b << 4;
                  s = s + b;
                  for (b = 0; b < 16; b++)
                    {
                      for (Sint32 z = 0; z < 16; z++)
                        {
                          a = s[z];
                          if (a)
                            {
                              a = pixel;
                              d[z] = pixel;
                            }
                        }
                      s = s + offSc;
                      d = d + offDs;
                    }
                }
              desP1 = desP1 + 16;
              if (color++ > 32)
                {
                  color = 0;
                }
            }
        }
    }
}

/**
 * Display text in 320x240 mode
 */
void
handler_popup_menu::display_320 ()
{
  Sint32 color = menu_first_color_index;
  char *desP1 = pixel_data + char_height + row_size * vertical_space;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (vertical_space - 1) + (char_height * 2);
  Sint32 *basPT = (Sint32 *) caract_adr;
  char **lines = menu_texts_pt[menu_number];
  char *c = ascii2code;


  Uint32 a, b, j;
  Uint32 y = (keyboard->get_mouse_y () - y_coord) / vertical_space;
  y--;
  for (Uint32 k = 0; k < num_of_lines; k++, desP1 += offD2)
    {
      char *p = lines[k];
      if (y != k)
        {
          /* display normal line of n characters */
          for (j = 0; j < num_of_columns; j++)
            {
              a = *(p++) - 32;
              if (a != 0)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 3;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 8; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                }
              desP1 = desP1 + 8;
            }
        }
      else
        {
          /* display selected line of 32 characters */
          for (j = 0; j < num_of_columns; j++)
            {
              unsigned char pixel = cycling_table[color];
              a = *(p++) - 32;
              if (a != 0)
                {
                  b = c[a];
                  unsigned char *s = (unsigned char *) basPT;
                  unsigned char *d = (unsigned char *) desP1;
                  b = b << 3;
                  s = s + b;
                  for (b = 0; b < 8; b++)
                    {
                      for (Sint32 z = 0; z < 8; z++)
                        {
                          a = s[z];
                          if (a != 0)
                            {
                              a = pixel;
                              d[z] = pixel;
                            }
                        }
                      s = s + offSc;
                      d = d + offDs;
                    }
                }
              desP1 = desP1 + 8;
              if (color++ > 32)
                {
                  color = 0;
                }
            }
        }
    }
}

const unsigned char
handler_popup_menu::cycling_table[] =
{
  239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};

char **
handler_popup_menu::menu_texts_pt[2] =
{
  NULL, NULL
};
