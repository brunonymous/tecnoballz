/**
 * @file tiles_background.cc 
 * @brief Draw tiles background in bricks levels 
 * @date 2007-09-24
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.21 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: tiles_background.cc,v 1.21 2007-09-25 05:43:20 gurumeditation Exp $
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
#include "../include/bitmap_data.h"
#include "../include/handler_display.h"
#include "../include/tiles_background.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_resources.h"

tiles_background * tiles_background::tiles_background_singleton = NULL;

/**
 * Create the tiles background object
 */
tiles_background::tiles_background ()
{
  object_init ();
  type_of_tiles = TILES_64x64_WITH_16_COLORS;
  palette_index = 0;
  if (resolution == 1 || force_4_colors_tiles)
    {
      type_of_tiles = TILES_32x32_WITH_4_COLORS;
    }
  current_tiles = NULL;
  map_tiles = NULL;
  map_height = 0;
  map_width = 0;
  map_xcoord = 0;
  map_ycoord = 0;
  map_xmax = 0;
  map_ymax = 0;
  tiles_width = 0;
  tiles_height = 0;
  map_scroll_num = 1;
  map_angle_direction = 0;
  map_velocity = 0.0;
  map_angle_speed = 0.0;
  map_scroll_delay = 0;
}

/**
 * Release the titles background object
 */
tiles_background::~tiles_background ()
{
  if (NULL != current_tiles)
    {
      delete current_tiles;
      current_tiles = NULL;
    }
  if (NULL != map_tiles)
    {
      delete[] map_tiles;
      map_tiles = NULL;
    }
  object_free ();
  tiles_background_singleton = NULL;
}

/**
 * Get the object instance
 * tiles_background is a singleton
 * @return the tiles_background object 
 */
tiles_background *
tiles_background::get_instance ()
{
  if (NULL == tiles_background_singleton)
    {
      tiles_background_singleton = new tiles_background ();
    }
  return tiles_background_singleton;
}

/**
 * Load and draw a tiles background
 * @param tiles_num 
 */
void
tiles_background::setup (Uint32 tiles_num)
{
  if (is_verbose)
    {
      std::
      cout << "tiles_background::setup() tiles_num:" << tiles_num << std::
      endl;
    }
  if (type_of_tiles > TILES_64x64_WITH_16_COLORS)
    {
      type_of_tiles = TILES_64x64_WITH_16_COLORS;
    }

  switch (type_of_tiles)
    {
    case TILES_32x32_WITH_4_COLORS:
    {
      bitmap_data *bmp = new bitmap_data ();
      if (is_verbose)
        {
          std::cout << "tiles_background::setup() "
          "load 32x32 tiles with 4 colors" << std::endl;
        }
      bmp->load (handler_resources::RES60BACKG);
      tiles_width = TILES_32_WIDTH;
      tiles_height = TILES_32_HEIGHT;

      /* select one of the 60 backgrounds */
      /* value from 0 to 63 */
      Uint32 x;
      Uint32 y = random_counter & 0x3F;
      if (y >= 60)
        {
          y -= 60;
        }
      if (y & 0x1)
        {
          /* right side */
          x = bmp->get_width () / 2;
        }
      else
        {
          x = 0;
        }
      y >>= 1;
      y = y * tiles_height;
      current_tiles = (bitmap_data *)
                      bmp->cut_to_bitmap (x, y, 5 * tiles_width, tiles_height);
      delete bmp;
    }
    break;

    case TILES_64x64_WITH_16_COLORS:
    default:
    {
      if (tiles_num < 1)
        {
          tiles_num = (random_counter & 127) + 1;
        }
      if (tiles_num > 77)
        {
          tiles_num = tiles_num - 77;
        }
      char *pathname = resources->get_tilemaps_filename (tiles_num);
      if (is_verbose)
        {
          std::cout << "tiles_background::setup() " <<
          "try to initialize" << pathname << std::endl;
        }
      current_tiles = new bitmap_data ();
      current_tiles->load (pathname);
      tiles_width = TILES_64_WIDTH;
      tiles_height = TILES_64_HEIGHT;
    }
    break;
    }

  map_width = (game_screen->get_width () - 64 * resolution) / tiles_width;
  Sint32 h = (240 * resolution) % tiles_height - 1;
  map_height = (240 * resolution) / tiles_height;
  map_xmax = tiles_width * map_width;
  if (h > 0)
    {
      map_height++;
    }
  map_ymax = tiles_height * map_height;
  map_row_size = tiles_height * map_width;

  set_palette ();
  /* generating random map */
  generate_map ();

  /* draw the tiles in background offscreen */
  map_xcoord = random_counter % map_xmax;
  map_ycoord = random_counter * frame_counter % map_ymax;
  draw (background_screen);
  draw_shadows ();
}


/**
 * Draw shadows on the top and the right of screen
 */
void
tiles_background::draw_shadows ()
{
  offscreen_surface* screen;
  if (has_background)
    {
      screen = background_screen;
    }
  else
    {
      screen = game_screen;
    }
  Uint32 size = screen->get_row_size();
  unsigned char mask = handler_display::SHADOW_PIX;

  /* draw top shadow */
  char *dest = screen->get_pixel_data();
  Uint32 hscreen = display->get_width () - (64 * resolution);
  Uint32 k = handler_display::SHADOWOFFY * resolution;
  for (Uint32 i = 0; i < k; i++, dest+=size)
    {
      for (Uint32 j = 0; j < hscreen; j++)
        {
          dest[j] |= mask;
        }
    }

  /* draw right shadow */
  dest = screen->get_pixel_data(252 * resolution, k);
  Uint32 vscreen = display->get_height () - k;
  k = 4 * resolution;
  for (Uint32 i = 0; i < vscreen;  i++, dest+=size)
    {
      for (Uint32 j = 0; j < k ; j++)
        {
          dest[j] |= mask;
        }
    }
}

/**
 * Generate tiles map
 */
void
tiles_background::generate_map ()
{

  /* allocate memory of the map */
  if (NULL == map_tiles)
    {
      try
        {
          map_tiles = new Uint32[map_width * map_height * 4];
        }
      catch (std::bad_alloc &)
        {
          std::
          cerr << "(!)tiles_background::generate_map() "
          "not enough memory to allocate " <<
          map_width * map_height << " bytes!" << std::endl;
          throw;
        }
    }


  Sint32 *positions;
  switch (type_of_tiles)
    {
    case TILES_32x32_WITH_4_COLORS:
      positions = table_pos1;
      break;
    case TILES_64x64_WITH_16_COLORS:
    default:
      positions = table_pos1;
      if (current_tiles->get_width () / TILES_64_WIDTH > 5)
        {
          positions = table_pos2;
        }
      break;
    }

#if __WORDSIZE == 64
  Sint32 rand1 = (long) display;
  Sint32 rand2 = (long) current_tiles;
#else
  /* use pointer address as random value */
  Sint32 rand1 = (Sint32) display;
  /* use pointer address as random value */
  Sint32 rand2 = (Sint32) current_tiles;
#endif
  for (Uint32 v = 0; v <  map_height; v++)
    {
      for (Uint32 h = 0; h < map_width; h++)
        {
          random_counter = random_counter + rand1 + rand2 + 1 + keyboard->get_mouse_x ();
          rand1 = rand1 + frame_counter + v;
          rand2 = rand2 + display->get_frames_per_second ();

          Uint32 x = random_counter;
          /* table index, from 0 to  15 */
          x &= 0x0f;
          /* position source, from 0 to 4 */
          x = positions[x];
          x *= tiles_width;
          Uint32 map_index = v * map_width * 2 + h;
          map_tiles[map_index] = x;
          map_tiles[map_index + map_width] = x;
          map_tiles[map_index + map_width * 2 * map_height] = x;
          map_tiles[map_index + map_width * 2 * map_height + map_width] =
            x;
        }
    }
}


/**
 * Set a type of displacement for the tilesmap scrolling
 */
void
tiles_background::set_scroll_type(Uint32 type)
{
  map_scroll_num = type;
  map_scroll_delay = 0;
  if(type != TILES_NO_SCROLL)
    {
      map_velocity = 0.0;
    } 
}

/**
 * Draw the tiles background
 */
void
tiles_background::draw ()
{

  switch (map_scroll_num)
    {
    case TILES_NO_SCROLL:
      if (map_velocity >= -0.001
          &&  map_velocity <= 0.001)
        {
          map_velocity = 0;
        }
      else if (map_velocity > -0.01)
        {
          map_velocity -= 0.1;
        }
      else
        {
          map_velocity += 0.1;
        }
      break;

    case TILES_SCROLL_WIN:
      map_angle_direction = 3.14;
      map_velocity = 5.0;
      break;

    case TILES_SCROLL_GAMEOVER:
      if (map_velocity > -0.01 && map_velocity < 0.01)
        {
          double pi = 4 * atan (1.0);
          Uint32 i = random_counter & 64;
          map_angle_direction = (pi * 2 / 64) * i;
        }
      map_velocity = cos(map_angle_speed) * 5;
      if (map_scroll_delay < 1)
        {
          map_angle_speed -= 0.01;
          if ((map_velocity > 1 - 0.1 && map_velocity < 1 + 0.1)
              || (map_velocity > -3 - 0.1 && map_velocity < -3 + 0.1)
             )
            {
              map_scroll_delay = 100;
            }
        }
      else
        {
          map_scroll_delay--;
        }
      break;

    case TILES_SCROLL_LOST:
      if (map_velocity > -0.01 && map_velocity < 0.01)
        {
          double pi = 4 * atan (1.0);
          Uint32 i = random_counter & 31;
          map_angle_direction = (pi * 2 / 32) * i;
        }
      map_velocity = cos(map_angle_speed) * 5;
      if (map_scroll_delay < 1)
        {
          map_angle_speed -= 0.01;
          if ((map_velocity > 2 - 0.1 && map_velocity < 2 + 0.1)
              || (map_velocity > -2 - 0.1 && map_velocity < -2 + 0.1)
             )
            {
              map_scroll_delay = 10;
            }
        }
      else
        {
          map_scroll_delay--;
        }
      break;

    case TILES_SCROLL_BEGIN:
      if (map_velocity > -0.01 && map_velocity < 0.01)
        {
          double pi = 4 * atan (1.0);
          Uint32 i = random_counter & 31;
          map_angle_direction = (pi * 2 / 32) * i;
        }
      map_velocity = cos(map_angle_speed) * 6;
      if (map_scroll_delay < 1)
        {
          map_angle_speed += 0.01;
          if ((map_velocity > 3 - 0.1 && map_velocity < 3 + 0.1)
              || (map_velocity > -3 - 0.1 && map_velocity < -3 + 0.1)
             )
            {
              map_scroll_delay = 50;
            }
        }
      else
        {
          map_scroll_delay--;
        }
    }
  map_ycoord = map_ycoord + (Uint32)(map_velocity * cos(map_angle_direction));
  map_xcoord = map_xcoord + (Uint32)(map_velocity * sin(map_angle_direction));
  draw (game_screen);
  draw_shadows ();
}

/**
 * Draw the tiles background
 * @param offscreen pointer to a offscreen_surface object
 */
void
tiles_background::draw (offscreen_surface *offscreen)
{
  map_xcoord = map_xcoord % map_xmax;
  map_ycoord = map_ycoord % map_ymax;

  SDL_Surface *screen_surface = offscreen->get_surface ();
  SDL_Surface *tiles_surface = current_tiles->get_surface ();
  Uint32 voffset = offscreen->get_vertical_offset ();

  /* width and height of the visible window */
  Uint32 width_box = offscreen->get_width () - 64 * resolution;
  Uint32 height_box = offscreen->get_height () - voffset * 2;
  /* index on the first  tile map */
  Uint32 map_index =
    (map_ycoord / tiles_height % map_height) * map_height * 2 +
    (map_xcoord / tiles_width % map_width);
  /* calculate the width of the tiles of the first column */
  Uint32 modulo_x = map_xcoord % tiles_width;
  Uint32 first_width = tiles_width - modulo_x;
  /* calculate the height of the tiles of the first line */
  Uint32 modulo_y = map_ycoord % tiles_height;
  Uint32 first_height = tiles_height - modulo_y;
  /* calculate the width of the tiles of the last column,
   * zero value is * possible */
  Uint32 last_width = (width_box - first_width) % tiles_width;
  /* calculate the height of the tiles of the last line,
   * zero value is * possible */
  Uint32 last_height = (height_box - first_height) % tiles_height;


  SDL_Rect rect_src;
  SDL_Rect rect_dst;

  rect_dst.y = voffset;

  Uint32 vcount = (height_box - first_height) / tiles_height + 1;
  Uint32 hcount = (width_box - first_width) / tiles_width + 1;
  for (Uint32 v = 0; v <= vcount; v++)
    {
      if (v == 0)
        {
          rect_src.h = rect_dst.h = first_height;
          rect_src.y = modulo_y;
        }
      else
        {
          rect_src.y = 0;
          if (v < vcount)
            {
              rect_src.h = rect_dst.h = tiles_height;
            }
          else
            {
              if (last_height > 0)
                {
                  rect_src.h = rect_dst.h = last_height;
                }
              else
                {
                  continue;
                }
            }
        }
      rect_dst.x = 0;
      Uint32 *map_line = &map_tiles[map_index];
      for (Uint32 h = 0; h <= hcount; h++)
        {
          if (h == 0)
            {
              rect_src.w = rect_dst.w = first_width;
              rect_src.x = *(map_line++) + modulo_x;
            }
          else
            {
              rect_src.x = *(map_line++);
              if (h < hcount)
                {
                  rect_src.w = rect_dst.w = tiles_width;
                }
              else
                {
                  if (last_width > 0)
                    {
                      rect_src.w = rect_dst.w = last_width;
                    }
                  else
                    {
                      continue;
                    }
                }
            }
          if (SDL_BlitSurface
              (tiles_surface, &rect_src, screen_surface, &rect_dst) < 0)
            {
              std::cerr << "(!)tiles_background::draw() " <<
              "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
            }
          rect_dst.x += rect_dst.w;
        }
      map_index += map_width * 2;
      rect_dst.y += rect_dst.h;
    }
}

/**
 * x-coordinates of sources in the tiles bitmap
 */
Sint32 tiles_background::table_pos1[16] =
  {
    3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 0, 0, 2, 4
  };

Sint32 tiles_background::table_pos2[16] =
  {
    3, 0, 0, 3, 4, 2, 1, 4, 3, 2, 1, 1, 5, 0, 5, 4
  };

void
tiles_background::set_palette ()
{
  /* initialize color palette */
  switch (type_of_tiles)
    {
    case TILES_32x32_WITH_4_COLORS:
      set_4_color_palette ();
      break;

    case TILES_64x64_WITH_16_COLORS:
    default:
      unsigned char *colPT = current_tiles->get_palette ();
      SDL_Color *palPT = display->get_palette ();
      SDL_Color *palP1 = palPT;
      SDL_Color *palP2 = palP1 + 128;
      unsigned char pixel;
      for (Sint32 i = 0; i < 16; i++)
        {
          /* red */
          pixel = *(colPT++);
          palP1->r = pixel;
          pixel >>= 1;
          palP2->r = pixel;
          /* green */
          pixel = *(colPT++);
          palP1->g = pixel;
          pixel >>= 1;
          palP2->g = pixel;
          /* blue */
          pixel = *(colPT++);
          palP1->b = pixel;
          pixel >>= 1;
          palP2->b = pixel;
          palP1++;
          palP2++;
        }
      display->enable_palette (palPT);
      break;
    }
}


/**
 * Randomly select one of 112 4-color palettes
 */
void
tiles_background::set_4_color_palette ()
{
  Sint32 j = random_counter & 0x1ff;
  if (j >= 448)
    {
      /* 112 preset 4 color palettes */
      j -= 448;
    }
  j = j & 0xfff0;
  set_4_color_palette (j);
}

/**
 * Set next background palette of 4 colors
 */
void
tiles_background::next_4_color_palette ()
{
  palette_index += 16;
  if (palette_index >= 432)
    {
      palette_index = 0;
    }
  set_4_color_palette (palette_index);
}

/**
 * Set previous background palette of 4 colors
 */
void
tiles_background::prev_4_color_palette ()
{
  palette_index -= 16;
  if (palette_index < 0)
    {
      palette_index = 432;
    }
  set_4_color_palette (palette_index);
}

/**
 * Initialize a background palette color of 4 colors
 * @param pal_index 4 color palette number
 */
void
tiles_background::set_4_color_palette (Uint32 pal_index)
{
  if (is_verbose)
    {
      std::cout << "tiles_background::set_4_color_palette() " <<
      "pal_index:" << pal_index << std::endl;

    }
  palette_index = pal_index;
  char *color = &couleurs[0];
  unsigned char *colPT = (unsigned char *) (color) + pal_index;
  SDL_Color *pal = display->get_palette ();
  SDL_Color *lighted = pal + 1;
  SDL_Color *dark = lighted + 128;
  for (Uint32 i = 0; i < 4; i++)
    {
      unsigned char pixel = *(colPT++);
      /* red */
      pixel = *(colPT++);
      lighted->r = pixel;
      pixel >>= 1;
      dark->r = pixel;
      /* green */
      pixel = *(colPT++);
      lighted->g = pixel;
      pixel >>= 1;
      dark->g = pixel;
      /* blue */
      pixel = *(colPT++);
      lighted->b = pixel;
      pixel >>= 1;
      dark->b = pixel;
      lighted++;
      dark++;
    }
  display->enable_palette (pal);
  if (NULL != current_tiles)
    {
      current_tiles->set_palette (pal);
    }
}

/**
 * Preset palettes: 4 colors original background (mode 320x200)
 * 448 / 16 : 28 preset palettes
 * 4 * 4    : 16 composantes by palette
 * 112 * 4  : 448 composantes
*/
char
tiles_background::couleurs[448] = {
                                    0x00, 0x40, 0x20, 0x40, 0x00, 0x60, 0x40, 0x60, 0x00, 0x80, 0x60, 0x80,
                                    0x00, 0xA0, 0x80, 0xA0, 0x00, 0x00, 0x20, 0x40, 0x00, 0x20,
                                    0x40, 0x60, 0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x00,
                                    0x20, 0x20, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
                                    0x00, 0x60, 0x80, 0x80, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60,
                                    0x00, 0x20, 0x60, 0x80, 0x00, 0x40, 0x80, 0xA0, 0x00, 0x30,
                                    0x40, 0x30, 0x00, 0x50, 0x60, 0x50, 0x00, 0x70, 0x80, 0x70, 0x00, 0x90,
                                    0xA0, 0x90, 0x00, 0x20, 0x20, 0x40, 0x00, 0x40, 0x40, 0x60,
                                    0x00, 0x60, 0x60, 0x80, 0x00, 0x80, 0x80, 0xA0, 0x00, 0x00, 0x40, 0x40,
                                    0x00, 0x20, 0x60, 0x60, 0x00, 0x40, 0x80, 0x80, 0x00, 0x60,
                                    0xA0, 0xA0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x40, 0x20, 0x40, 0x00, 0x60,
                                    0x40, 0x60, 0x00, 0x80, 0x60, 0x80, 0x00, 0x00, 0x40, 0x20,
                                    0x00, 0x00, 0x60, 0x40, 0x00, 0x20, 0x80, 0x60, 0x00, 0x40, 0xA0, 0x80,
                                    0x00, 0x40, 0x20, 0x00, 0x00, 0x60, 0x40, 0x20, 0x00, 0x80,
                                    0x60, 0x40, 0x00, 0xA0, 0x80, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00, 0x60,
                                    0x20, 0x20, 0x00, 0x80, 0x40, 0x40, 0x00, 0xA0, 0x60, 0x60,
                                    0x00, 0x40, 0x00, 0x40, 0x00, 0x60, 0x20, 0x60, 0x00, 0x80, 0x40, 0x80,
                                    0x00, 0xA0, 0x60, 0xA0, 0x00, 0x00, 0x20, 0x00, 0x00, 0x20,
                                    0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60, 0x00, 0x20,
                                    0x40, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x60, 0x80, 0x60,
                                    0x00, 0x80, 0xA0, 0x80, 0x00, 0x40, 0x40, 0x00, 0x00, 0x60, 0x60, 0x20,
                                    0x00, 0x80, 0x80, 0x40, 0x00, 0xA0, 0xA0, 0x60, 0x00, 0x00,
                                    0x40, 0x00, 0x00, 0x20, 0x60, 0x20, 0x00, 0x40, 0x80, 0x40, 0x00, 0x60,
                                    0xA0, 0x60, 0x00, 0x20, 0x20, 0x20, 0x00, 0x40, 0x40, 0x40,
                                    0x00, 0x60, 0x60, 0x60, 0x00, 0x80, 0x80, 0x80, 0x00, 0x40, 0x20, 0x60,
                                    0x00, 0x60, 0x40, 0x80, 0x00, 0x80, 0x60, 0xA0, 0x00, 0xA0,
                                    0x80, 0xC0, 0x00, 0x20, 0x20, 0x00, 0x00, 0x40, 0x40, 0x20, 0x00, 0x60,
                                    0x60, 0x40, 0x00, 0x80, 0x80, 0x60, 0x00, 0x20, 0x40, 0x60,
                                    0x00, 0x40, 0x60, 0x80, 0x00, 0x60, 0x80, 0xA0, 0x00, 0x80, 0xA0, 0xC0,
                                    0x00, 0x60, 0x40, 0x20, 0x00, 0x80, 0x60, 0x40, 0x00, 0xA0,
                                    0x80, 0x60, 0x00, 0xC0, 0xA0, 0x80, 0x00, 0x40, 0x00, 0x60, 0x00, 0x60,
                                    0x20, 0x80, 0x00, 0x80, 0x40, 0xA0, 0x00, 0xA0, 0x60, 0xC0,
                                    0x00, 0x40, 0x00, 0x20, 0x00, 0x60, 0x20, 0x40, 0x00, 0x80, 0x40, 0x60,
                                    0x00, 0xA0, 0x60, 0x80, 0x00, 0x20, 0x20, 0x60, 0x00, 0x40,
                                    0x40, 0x80, 0x00, 0x60, 0x60, 0xA0, 0x00, 0x80, 0x80, 0xC0, 0x00, 0x60,
                                    0x40, 0x00, 0x00, 0x80, 0x60, 0x20, 0x00, 0xA0, 0x80, 0x40,
                                    0x00, 0xC0, 0x80, 0x60, 0x00, 0x20, 0x40, 0x00, 0x00, 0x40, 0x60, 0x20,
                                    0x00, 0x60, 0x80, 0x40, 0x00, 0x80, 0xA0, 0x60, 0x00, 0x40,
                                    0x20, 0x20, 0x00, 0x60, 0x40, 0x40, 0x00, 0x80, 0x60, 0x60, 0x00, 0xA0,
                                    0x80, 0x80, 0x00, 0x20, 0x40, 0x40, 0x00, 0x40, 0x60, 0x60,
                                    0x00, 0x60, 0x80, 0x80, 0x00, 0x80, 0xA0, 0xA0
                                  };
