/**
 * @file supervisor_map_editor.cc
 * @brief The tile map editor for the menu and guardians levels
 * @created 2004-09-13
 * @date 2012-08-19
 * @copyright 1991-2016 TLK Games
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
#include "../include/supervisor_map_editor.h"
#include "../include/handler_resources.h"
#include "../include/list_sprites.h"
#include "../include/handler_keyboard.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

/**
 * Create the tile map editor
 */
supervisor_map_editor::supervisor_map_editor ()
{
  initialize ();
  /*  vertical background scrolling */
  tiles_map = new tilesmap_scrolling ();
  mouse_pointer = new sprite_mouse_pointer ();
  view_mode = SHOW_MAP;
  is_space_key_down = false;
  tiles_ycoord = 0;
  map_width = 0;
  is_right_button_down = false;
  cycled_colors_index = 0;
  tiles_brush = NULL;
  brush_bitmap = (bitmap_data *) NULL;
  is_left_button_down = false;
  brush_posx = 0;
  brush_posy = 0;
  brush_width = 0;
  brush_height = 0;
  is_s_key_down = false;
  first_init ();
}

/**
 * Release the tile map editor
 */
supervisor_map_editor::~supervisor_map_editor ()
{
  if (NULL != mouse_pointer)
    {
      delete mouse_pointer;
      mouse_pointer = NULL;
    }
  if (NULL != tiles_map)
    {
      delete tiles_map;
      tiles_map = NULL;
    }
  if (NULL != map_selection)
    {
      delete map_selection;
    }
  if (NULL != tiles_selection)
    {
      delete tiles_selection;
    }
  if (NULL != tiles_brush)
    {
      delete[]tiles_brush;
      tiles_brush = NULL;
    }
  if (NULL != brush_bitmap)
    {
      delete brush_bitmap;
      brush_bitmap = NULL;
    }
  release ();
}

/**
 * Perform some initializations
 */
void
supervisor_map_editor::first_init ()
{

  try
    {
      map_selection = new selected_region;
      tiles_selection = new selected_region;
    }
  catch (std::bad_alloc &)
    {
      std::
      cerr << "(!)supervisor_map_editor::first_init() "
      "not enough memory to allocate " <<
      " 'selected_region' structure!" << std::endl;
      throw;
    }
  current_selection = map_selection;
  map_selection->x1 = map_selection->x2 = 0;
  map_selection->y1 = map_selection->y2 = 0;
  map_selection->number_of_cols = map_selection->number_of_raws = 0;
  tiles_selection->x1 = tiles_selection->x2 = 0;
  tiles_selection->y1 = tiles_selection->y2 = 0;
  tiles_selection->number_of_cols = tiles_selection->number_of_raws = 0;

  screen_height = display->get_height ();
  screen_width = display->get_width ();

  sprites->reset ();
  resources->load_sprites_bitmap ();
  mouse_pointer->create_pointer_sprite (sprites_bitmap);
  resources->release_sprites_bitmap ();


  Sint32 edmap = tilesmap_scrolling::MAP_MENU;
  //Sint32        edmap = tilesmap_scrolling::MAP_GUARDIANS;
  //Sint32 edmap = tilesmap_scrolling::MAP_CONGRATULATIONS;

  tiles_map->initialize (tilesmap_scrolling::TILES_COLOR_MENU, edmap);
  tiles_bitmap = tiles_map->get_bitmap ();

  tile_width = tiles_map->get_tiles_width ();
  tile_mask1 = 0xffffffff ^ (tile_width - 1);
  tile_mask2 = ~tile_mask1;
  map_width = tiles_map->get_map_width ();
  display->set_color_gradation ();
}

/**
 * Main loop
 */
Uint32 supervisor_map_editor::main_loop ()
{
  display->wait_frame ();

  map_selection->y_offset = tiles_map->get_y_coord ();
  tiles_selection->y_offset = tiles_ycoord;

  switch (view_mode)
    {
    case SHOW_TILES:
      view_tiles ();
      break;
    case SHOW_MAP:
    default:
      view_map_editor ();
      break;
    }

  display->lock_surfaces ();
  mouse_pointer->move ();
  sprites->draw ();

  /* copy whole game surface into screen surface */
  display->unlock_surfaces ();
  display->window_update ();

  /* [ctrl] + escape key to leave! */
  if (keyboard->command_is_pressed (handler_keyboard::QUIT_TECNOBALLZ))
    {
      next_phase = LEAVE_TECNOBALLZ;
    }

  check_keys ();
  /* back to menu */
  if (keyboard->key_is_pressed (SDLK_F10))
    {
      next_phase = MAIN_MENU;
    }

  if (keyboard->key_is_pressed (SDLK_F1))
    {
      tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_MENU,
                             tilesmap_scrolling::MAP_MENU);
    }
  else if (keyboard->key_is_pressed (SDLK_F2))
    {
      tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_GUARDIANS,
                             tilesmap_scrolling::MAP_GUARDIANS);
    }
  else if (keyboard->key_is_pressed (SDLK_F3))
    {
      tiles_map->switch_map (tilesmap_scrolling::TILES_COLOR_CONGRATULATIONS,
                             tilesmap_scrolling::MAP_CONGRATULATIONS);
    }

  /* save the map */
  if (keyboard->key_is_pressed (SDLK_s) && !is_s_key_down)
    {
      is_s_key_down = true;
    }
  if (keyboard->key_is_released (SDLK_s) && is_s_key_down)
    {
      is_s_key_down = false;
      save_tilesmap ();
    }
  return next_phase;
}

/**
 * Display map editor view
 */
void
supervisor_map_editor::view_map_editor ()
{
  current_selection = map_selection;
  Sint32 speed = get_speed ();
  /* draw tiles map on the screen */
  tiles_map->scroll (speed);
  select_rectangle ();
  highlight_selection ();
  draw_brush ();
}

/**
 * Create a brush from map editor
 */
void
supervisor_map_editor::map_to_brush ()
{
  if (is_verbose)
    {
      std::cout << "supervisor_map_editor::map_to_brush() (" <<
      current_selection->x1 << ", " << current_selection->y1 << "," <<
      current_selection->x2 << ", " << current_selection->y2 << std::endl;
    }

  /* allocate memory for tiles brush */
  alloc_tilesmap_brush (current_selection->number_of_raws,
                        current_selection->number_of_cols);
  Sint32 ycoord = current_selection->y1;
  ycoord = (ycoord / tiles_map->tile_height) + 0;
  ycoord *= map_width;
  ycoord += (current_selection->x1 / tiles_map->tile_width);
  Uint16 *map = tiles_map->map_tiles + ycoord;
  Uint16 *brush = tiles_brush;
  for (Uint32 y = 0; y < current_selection->number_of_raws; y++)
    {
      for (Uint32 x = 0; x < current_selection->number_of_cols; x++)
        {
          *(brush++) = map[x];
        }
      map += map_width;
    }
  alloc_brush ();
}

/**
 * View tiles mode
 */
void
supervisor_map_editor::view_tiles ()
{
  current_selection = tiles_selection;
  Sint32 speed = get_speed ();
  Sint32 y_max = tiles_bitmap->get_height () - screen_height;
  tiles_ycoord = tiles_ycoord + speed;
  if (tiles_ycoord < 0)
    {
      tiles_ycoord = 0;
    }
  else if (tiles_ycoord > y_max)
    {
      tiles_ycoord = y_max;
    }
  game_screen->clear ();
  game_screen->blit_surface (tiles_bitmap, 0, tiles_ycoord, 0, 0,
                             screen_width, screen_height);
  select_rectangle ();
  highlight_selection ();
}

/**
 * Create a brush from tiles bitmap
 */
void
supervisor_map_editor::tiles_to_brush ()
{
  if (is_verbose)
    {
      std::cout << "supervisor_map_editor::map_to_brush() (" <<
      current_selection->x1 << ", " << current_selection->y1 << "," <<
      current_selection->x2 << ", " << current_selection->y2 << std::endl;
    }

  /*
   * allocate tilesmap for the brush
   */
  alloc_tilesmap_brush (current_selection->number_of_raws,
                        current_selection->number_of_cols);

  /*
   * copy tiles offsets to brush map
   */
  Uint32 width = tiles_bitmap->get_width () / tile_width;
  Sint32 offset =
    (current_selection->y1 / tile_width) * width +
    (current_selection->x1 / tile_width);
  Uint16 *brush = tiles_brush;
  for (Uint32 y = 0; y < current_selection->number_of_raws; y++)
    {
      Sint32 index = offset;
      for (Uint32 x = 0; x < current_selection->number_of_cols; x++)
        {
          *(brush++) = index;
          index++;
        }
      offset += width;
    }
  alloc_brush ();
}

/**
 * Check keyboard keys
 */
void
supervisor_map_editor::check_keys ()
{
  if (keyboard->key_is_pressed (SDLK_SPACE))
    {
      is_space_key_down = true;
    }
  if (keyboard->key_is_released (SDLK_SPACE) && is_space_key_down)
    {
      switch (view_mode)
        {
        case SHOW_MAP:
          view_mode = SHOW_TILES;
          break;
        case SHOW_TILES:
        default:
          view_mode = SHOW_MAP;
          break;
        }
      is_space_key_down = false;
    }
}

/**
 * Determine vertical scrolling speed
 * @return srolling speed
 */
Sint32 supervisor_map_editor::get_speed ()
{
  Sint32
  speed = 0;
  Uint32
  mousY = keyboard->get_mouse_y ();
  if (mousY > 0 && mousY < 8 * resolution)
    {
      speed = -16 * resolution;
    }
  if (mousY >= 8 * resolution && mousY < 16 * resolution)
    {
      speed = -8 * resolution;
    }
  if (mousY >= 16 * resolution && mousY < 24 * resolution)
    {
      speed = -4 * resolution;
    }
  if (mousY <= screen_height - 16 * resolution
      && mousY > screen_height - 24 * resolution)
    {
      speed = 4 * resolution;
    }
  if (mousY <= screen_height - 8 * resolution
      && mousY > screen_height - 16 * resolution)
    {
      speed = 8 * resolution;
    }
  if (mousY < screen_height && mousY > screen_height - 8 * resolution)
    {
      speed = 16 * resolution;
    }
  return speed;
}

/**
 * Handle rectangle selection with the right mouse button
 */
void
supervisor_map_editor::select_rectangle ()
{
  /* reads coordinates if the right mouse button has just been pressed */
  bool is_right_down = keyboard->is_right_button ();
  if (is_right_down && !is_right_button_down)
    {
      is_right_button_down = true;
      current_selection->x1 = keyboard->get_mouse_x ();
      current_selection->y1 =
        current_selection->y_offset + keyboard->get_mouse_y ();
      current_selection->x1 &= tile_mask1;
      current_selection->y1 &= tile_mask1;
      if (NULL != brush_bitmap)
        {
          delete brush_bitmap;
          brush_bitmap = (bitmap_data *) NULL;
        }
    }

  if (is_right_button_down)
    {
      current_selection->x2 = keyboard->get_mouse_x ();
      current_selection->y2 =
        keyboard->get_mouse_y () + current_selection->y_offset;
      if (current_selection->x2 & tile_mask2)
        {
          current_selection->x2 += tile_width;
        }
      if (current_selection->y2 & tile_mask2)
        {
          current_selection->y2 += tile_width;
        }
      current_selection->x2 &= tile_mask1;
      current_selection->y2 &= tile_mask1;
      if (current_selection->x2 >= current_selection->x1 &&
          current_selection->x2 - current_selection->x1 < tile_width)
        {
          current_selection->x2 = current_selection->x1 + tile_width;
        }
      if (current_selection->x2 < current_selection->x1 &&
          current_selection->x1 - current_selection->x2 < tile_width)
        {
          current_selection->x2 = current_selection->x1 - tile_width;
        }
      if (current_selection->y2 >= current_selection->y1 &&
          current_selection->y2 - current_selection->y1 < tile_width)
        {
          current_selection->y2 = current_selection->y1 + tile_width;
        }
      if (current_selection->y2 < current_selection->y1 &&
          current_selection->y1 - current_selection->y2 < tile_width)
        {
          current_selection->x2 = current_selection->x1 - tile_width;
        }
      if (current_selection->y1 < current_selection->y2
          && current_selection->y2 - current_selection->y1 >
          (screen_height / 2))
        {
          current_selection->y2 = current_selection->y1 + (screen_height / 2);
        }
      if (current_selection->y1 > current_selection->y2
          && current_selection->y1 - current_selection->y2 >
          (screen_height / 2))
        {
          current_selection->y2 = current_selection->y1 - (screen_height / 2);
        }
    }

  if (is_right_button_down && is_right_down)
    {
      return;
    }
  if (is_right_down || !is_right_button_down)
    {
      return;
    }

  /* right mouse button released */
  is_right_button_down = false;
  if (current_selection->x1 > current_selection->x2)
    {
      Sint32 x = current_selection->x1;
      current_selection->x1 = current_selection->x2;
      current_selection->x2 = x;
    }
  if (current_selection->y1 > current_selection->y2)
    {
      Sint32 y = current_selection->y1;
      current_selection->y1 = current_selection->y2;
      current_selection->y2 = y;
    }
  current_selection->number_of_cols =
    (current_selection->x2 - current_selection->x1) / tile_width;
  current_selection->number_of_raws =
    (current_selection->y2 - current_selection->y1) / tile_width;
  switch (view_mode)
    {
    case SHOW_TILES:
      tiles_to_brush ();
      break;
    case SHOW_MAP:
    default:
      map_to_brush ();
      break;
    }

}

/**
 * Draw highlighting selection
 */
void
supervisor_map_editor::highlight_selection ()
{
  char *screen;
  Uint32 cycle_delay = 0;

  if (current_selection->x2 == current_selection->x1 ||
      current_selection->y2 == current_selection->y1)
    {
      return;
    }
  Uint32 x1 = current_selection->x1;
  Uint32 x2 = current_selection->x2;
  if (current_selection->y_offset > (Sint32) current_selection->y1 ||
      current_selection->y_offset > (Sint32) current_selection->y2)
    {
      return;
    }
  Sint32 y1 = current_selection->y1 - current_selection->y_offset;
  Sint32 y2 = current_selection->y2 - current_selection->y_offset;

  if (x1 > x2)
    {
      Uint32 x = x1;
      x1 = x2;
      x2 = x;
    }
  Uint32 width = x2 - x1;
  if (y1 > y2)
    {
      Uint32 y = y1;
      y1 = y2;
      y2 = y;
    }
  Uint32 height = y2 - y1;

  if (cycled_colors_index++ > MAX_OF_CYCLED_COLORS)
    {
      cycled_colors_index = 0;
    }
  Uint32 color = cycled_colors_index;


  /* top border */
  if (y1 >= 0 && y1 < (Sint32)screen_height)
    {
      screen = game_screen->get_pixel_data (x1, y1);
      cycle_delay = 0;
      for (Uint32 i = 0; i < width; i++)
        {
          unsigned char pixel = cycled_colors_list[color];
          screen[i] = pixel;
          if (++cycle_delay >= 5)
            {
              cycle_delay = 0;
              if (color++ >= MAX_OF_CYCLED_COLORS)
                {
                  color = 0;
                }
            }
        }
    }

  /* right border */
  Uint32 rowsize = game_screen->get_row_size ();
  screen = game_screen->get_pixel_data (x2 - 1, y1 + 1);
  for (Sint32 i = 1; i < (Sint32)height; i++)
    {
      unsigned char pixel = cycled_colors_list[color];
      if ((y1 + i) >= 0 && (y1 + i) <  (Sint32) screen_height)
        {
          *screen = pixel;
        }
      if (++cycle_delay >= 5)
        {
          cycle_delay = 0;
          if (color++ >= MAX_OF_CYCLED_COLORS)
            {
              color = 0;
            }
        }
      screen += rowsize;
    }

  /* bottom border */
  if (y2 >= 0 && y2 < (Sint32)screen_height)
    {
      screen = game_screen->get_pixel_data (x1, y2);
      for (Sint32 i = width - 1; i >= 0; i--)
        {
          unsigned char pixel = cycled_colors_list[color];
          screen[i] = pixel;
          if (++cycle_delay >= 5)
            {
              cycle_delay = 0;
              if (color++ >= MAX_OF_CYCLED_COLORS)
                {
                  color = 0;
                }
            }
        }
    }

  /* left border */
  screen = game_screen->get_pixel_data (x1, y2 - 1);
  for (Sint32 i = 1; i < (Sint32)height; i++)
    {
      unsigned char pixel = cycled_colors_list[color];
      if ((y2 - i) >= 0 && (y2 - i) < (Sint32)screen_height)
        {
          *screen = pixel;
        }
      if (++cycle_delay >= 5)
        {
          cycle_delay = 0;
          if (color++ > MAX_OF_CYCLED_COLORS)
            {
              color = 0;
            }
        }
      screen -= rowsize;
    }
}


/**
 * Allocate a tilesmap brush of the required size
 * @param number_of_raws number of raws of tiles
 * @param number_of_cols number of cols of tiles
 */
void
supervisor_map_editor::alloc_tilesmap_brush (Uint32 number_of_raws,
    Uint32 number_of_cols)
{
  if (NULL != tiles_brush)
    {
      delete[]tiles_brush;
    }
  Uint32 size = number_of_raws * number_of_cols;
  try
    {
      tiles_brush = new Uint16[size];
    }
  catch (std::bad_alloc &)
    {
      std::
      cerr << "(!)supervisor_map_editor::alloc_tilesmap_brush() "
      "not enough memory to allocate " << size << " Uint16!" << std::endl;
      throw;
    }
}

/**
 * Allocate a bitmap brush of the required size
 */
void
supervisor_map_editor::alloc_brush ()
{
  if (NULL != brush_bitmap)
    {
      delete brush_bitmap;
    }
  brush_bitmap =
    tiles_map->alloc_brush (tiles_brush, current_selection->number_of_cols,
                            current_selection->number_of_raws);
  brush_width = current_selection->number_of_cols;
  brush_height = current_selection->number_of_raws;
  return;
}

/**
 * Draw the current brush
 */
void
supervisor_map_editor::draw_brush ()
{
  if (NULL == brush_bitmap)
    {
      return;
    }
  Uint32 xcoord = keyboard->get_mouse_x ();
  Uint32 ycoord = keyboard->get_mouse_y ();
  xcoord &= tile_mask1;
  ycoord &= tile_mask1;
  if (xcoord > screen_width - brush_bitmap->get_width ())
    {
      xcoord = screen_width - brush_bitmap->get_width ();
    }
  if (ycoord > screen_height - brush_bitmap->get_height ())
    {
      ycoord = screen_height - brush_bitmap->get_height ();
    }
  Uint32 map_ycoord = tiles_map->get_y_coord ();
  bool is_left_down = keyboard->is_left_button ();
  if (is_left_down && !is_left_button_down)
    {
      is_left_button_down = true;
      brush_posx = xcoord;
      brush_posy = ycoord;
    }
  if (is_left_down && is_left_button_down)
    {
      xcoord = brush_posx;
      ycoord = brush_posy;
    }
  else
    {
      /*
       * left mouse button released, copy tiles brush to the map
       */
      if (!is_left_down && is_left_button_down)
        {
          is_left_button_down = false;
          Uint32 i = map_ycoord + brush_posy;
          i = (i / tiles_map->tile_height);
          i *= map_width;
          Uint16 *brush = tiles_brush;
          Uint16 *table = tiles_map->map_tiles + i;
          Uint16 *t_end =
            tiles_map->map_tiles +
            (tilesmap_scrolling::MAP_HEIGHT * map_width);
          table += (brush_posx / tiles_map->tile_width);
          for (i = 0; i < brush_height; i++)
            {
              if (table > t_end)
                {
                  table -= (tilesmap_scrolling::MAP_HEIGHT * map_width);
                }

              for (Uint32 j = 0; j < brush_width; j++)
                {
                  table[j] = *(brush++);
                }
              table += map_width;
            }

          /* copy a height of the screen (for scrolling rotation) */
          table = tiles_map->map_tiles;
          i = (tilesmap_scrolling::MAP_HEIGHT * map_width);
          Sint32 tsupp =
            (display->get_height () / tiles_map->tile_height) * 2;
          for (Uint32 j = 0; j < (tsupp * map_width); j++)
            {
              tiles_map->map_tiles[i++] = table[j];
            }
        }
    }
  game_screen->blit_surface (brush_bitmap, 0, 0, xcoord,
                             ycoord - (map_ycoord & tile_mask2),
                             brush_bitmap->get_width (),
                             brush_bitmap->get_height ());
}

/**
 * Save tiles map file
 */
bool supervisor_map_editor::save_tilesmap ()
{
  Uint32
  map_size = tilesmap_scrolling::MAP_HEIGHT * map_width;
//map_size = map_size * 2;
  Uint32
  bytes_size = map_size * sizeof (Uint16);
  size_t left;
  ssize_t bytes_written;

  /*
    Uint16 *map2 = new Uint16[map_size];
    for (Uint32 v = 0; v < map_size; v++)
      {
        map2[v] = 0;
      }
    Uint16 *map = (Uint16 *) tiles_map->map_tiles;
    Uint16 *x2 = map2;
    for (Uint32 v = 0; v < tilesmap_scrolling::MAP_HEIGHT; v++)
      {
        for (Uint32 w = 0; w < map_width; w++)
          {
            x2[w] = map[w];
            x2[w + map_width] = map[w];
          }
          map = map + map_width;
          x2 = x2 + map_width + map_width ;
      }
  */

  Uint16 *
  filedata;
  try
    {
      filedata = new Uint16[map_size];
    }
  catch (std::bad_alloc &)
    {
      std::
      cerr << "(!)supervisor_map_editor::save_tilesmap() "
      "not enough memory to allocate " << map_size << " bytes!" << std::endl;
      throw;
    }

  Uint16 *
  map = (Uint16 *) tiles_map->map_tiles;
//map = map2;
  unsigned char *
  buffer = (unsigned char *) filedata;
  for (Uint32 i = 0; i < map_size; i++)
    {
      Uint16
      code = *map;
      code = code << 2;
      /* convert short int to big endian */
      buffer[1] = code;
      code = code >> 8;
      buffer[0] = code;
      map++;
      buffer += 2;
    }
  /* set umask so that files are group-writable */
  umask (0002);
  const char *
  filename = "edmap.data";
  Sint32
  handle = open (filename, O_WRONLY | O_CREAT, 00666);
  if (-1 == handle)
    {
      std::cerr << "supervisor_map_editor::save_tilesmap() file " <<
      filename << "; error " << strerror (errno) << std::endl;
      delete[]filedata;
      return false;
    }
  left = bytes_size;
  while (left > 0)
    {
      bytes_written = write (handle, filedata + bytes_size - left, left);
      if (bytes_written == -1)
       {
         std::cerr << "(!)supervisor_map_editor::save_tilesmap():" <<
          " filename: " << filename << "; error: " << strerror (errno);  
         close (handle);
         delete[]filedata;
         return false;
       }
      left -= bytes_written;
    }
  if (close (handle) == -1)
    {
      std::cerr << "supervisor_map_editor::save_tilesmap() file " <<
      filename << "; error " << strerror (errno) << std::endl;
      delete[]filedata;
      return false;
    }
  delete[]filedata;
  if (is_verbose)
    {
      std::cout << "supervisor_map_editor::save_tilesmap() " <<
      filename << "file was saved" << std::endl;
    }
  return true;
}

const unsigned char
supervisor_map_editor::cycled_colors_list[MAX_OF_CYCLED_COLORS] =
{
  239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
  253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
  243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
  249, 250, 251, 252, 253, 254, 255
};
