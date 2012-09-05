/**
 * @file tiles_background.h
 * @brief Draw tiles background in bricks levels 
 * @date 2012-09-02 
 * @copyright 1991-2012 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2012 TLK Games all rights reserved
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
#ifndef __TILES_BACKGROUND__
#define __TILES_BACKGROUND__
class tiles_background;
#include "../include/tecnoballz.h"

class tiles_background:public virtual tecnoballz
  {
  public:
    typedef enum
    {
      TILES_NO_SCROLL,
      TILES_SCROLL_BEGIN,
      TILES_SCROLL_LOST,
      TILES_SCROLL_WIN,
      TILES_SCROLL_GAMEOVER
    }
    TYPES_OF_SCROLL;

  private:
    static tiles_background* tiles_background_singleton;
    static Sint32 table_pos1[16];
    static Sint32 table_pos2[16];
    static unsigned char couleurs[448];
    static const Sint32 TILES_32_WIDTH = 32;
    static const Sint32 TILES_32_HEIGHT = 32;
    static const Sint32 TILES_64_WIDTH = 64;
    static const Sint32 TILES_64_HEIGHT = 64;

    typedef enum
    {
      TILES_32x32_WITH_4_COLORS,
      TILES_64x64_WITH_16_COLORS
    }
    TYPES_OF_TILES;
    /** Type of tiles TILES_64x64_WITH_16_COLORS
     * or TILES_32x32_WITH_4_COLORS */
    Uint32 type_of_tiles;
    /** 4 color palette index */
    Sint32 palette_index;

    bitmap_data *current_tiles;
    Uint32 *map_tiles;
    Uint32 map_height;
    Uint32 map_width;
    Uint32 map_row_size;
    Uint32 map_xcoord;
    Uint32 map_ycoord;
    Uint32 map_xmax;
    Uint32 map_ymax;
    Uint32 tiles_width;
    Uint32 tiles_height;
    /** Type of scrolling TILES_NO_SCROLL, TILES_SCROLL_BEGIN, ... */
    Uint32 map_scroll_num;
    double map_angle_direction;
    double map_angle_speed;
    /** Velocity of the tiles background */
    double map_velocity;
    Sint32 map_scroll_delay;


  private:
    tiles_background ();
    void draw_shadows ();
    void generate_map ();
    void set_palette ();
    void set_4_color_palette (Uint32 pal_index);

  public:
    ~tiles_background ();
    static tiles_background *get_instance ();
    void setup (Uint32 titles_num);
    void set_scroll_type(Uint32 type);
    void draw ();
    void draw (offscreen_surface * offscreen);
    void set_4_color_palette ();
    void next_4_color_palette ();
    void prev_4_color_palette ();
  };
#endif
