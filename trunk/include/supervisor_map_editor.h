/**
 * @file supervisor_map_editor.h
 * @brief The tile map editor for the menu and guardians levels
 * @created 2004-09-13
 * @date 2007-11-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.17 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: supervisor_map_editor.h,v 1.17 2007-11-18 16:13:19 gurumeditation Exp $
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
#ifndef __SUPERVISOR_MAP_EDITOR__
#define __SUPERVISOR_MAP_EDITOR__

#include "../include/supervisor.h"
#include "../include/tilesmap_scrolling.h"
#include "../include/sprite_mouse_pointer.h"
#include "../include/bitmap_data.h"


class supervisor_map_editor:public virtual supervisor
  {
  private:
    typedef struct
      {
        /** Left column of selection rectangle */
        Uint32 x1;
        /** Top row of selection rectangle */
        Uint32 x2;
        /** Right column of selection rectangle */
        Uint32 y1;
        /** Bottom row of selection rectangle */
        Uint32 y2;
        Uint32 number_of_cols;
        Uint32 number_of_raws;
        Sint32 y_offset;
      }
    selected_region;

    typedef enum
    {
      SHOW_MAP,
      SHOW_TILES
    } SHOW_ENUM;
    tilesmap_scrolling *tiles_map;
    sprite_mouse_pointer *mouse_pointer;
    /** Number of tiles per row in the map */
    Uint32 map_width;
    /** Height of the main window */
    Uint32 screen_height;
    /** Weight of the main window */
    Uint32 screen_width;
    /** SHOW_MAP or SHOW_TILES */
    Uint32 view_mode;
    /** True if the space key is pressed down */
    bool is_space_key_down;
    /** Y-coordinate of the tiles bitmap */
    Sint32 tiles_ycoord;
    bitmap_data *tiles_bitmap;
    /** If true, the right mouse button is currently pressed */
    bool is_right_button_down;

    /*
     * brush handle
     */
    /** If true, the left mouse button is currently pressed */
    bool is_left_button_down;
    /** X-coordinate of the current brush in pixels */
    Sint32 brush_posx;
    /** Y-coordinate of the current brush in pixels */
    Sint32 brush_posy;
    /** Width of the current brush in pixels */
    Uint32 brush_width;
    /** Height of the current brush in pixels */
    Uint32 brush_height;
    /** Brush composed of tiles copied from the main map */
    Uint16 *tiles_brush;
    /** Brush bitmap */
    bitmap_data *brush_bitmap;

    Uint32 cycled_colors_index;
    selected_region *current_selection;
    selected_region *map_selection;
    selected_region *tiles_selection;
    Uint32 tile_width;
    Uint32 tile_mask1;
    Uint32 tile_mask2;
    bool is_s_key_down;

  public:
    supervisor_map_editor ();
    ~supervisor_map_editor ();
    void first_init ();
    Uint32 main_loop ();

  private:
    void view_map_editor ();
    void view_tiles ();
    void tiles_to_brush ();
    void map_to_brush ();
    void check_keys ();
    Sint32 get_speed ();
    void select_rectangle ();
    void highlight_selection ();
    void alloc_tilesmap_brush (Uint32 number_of_raws, Uint32 number_of_cols);
    void alloc_brush ();
    void draw_brush ();
    bool save_tilesmap ();

    static const Uint32 MAX_OF_CYCLED_COLORS = 49;
    static const unsigned char cycled_colors_list[MAX_OF_CYCLED_COLORS];
  };
#endif
