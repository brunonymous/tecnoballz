/**
 * @file controller_sides_bricks.h
 * @brief Sides bricks controller. The small bricks on the side, the walls top
 *        left and right
 * @date 2007-09-30
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
#ifndef __CONTROLLER_SIDES_BRICKS__
#define __CONTROLLER_SIDES_BRICKS__

class controller_sides_bricks;

#include "../include/objects_list.h"
#include "../include/sprite_object.h"
#include "../include/tecnoballz.h"

class controller_sides_bricks:public objects_list < sprite_object,
      controller_sides_bricks >
  {
  public:
    /** Maximum number of bricks per wall */
    static const Uint32 MAX_OF_SIDES_BRICKS = 12;

    typedef enum
    {
      TOP_WALL = 1,
      RIGHT_WALL,
      LEFT_WALL
    } WALLS_ENUM;

  private:
    /**
     * Structure for redraw side bricks
     */
    static const Uint32 MAX_OF_RESTORED_BRICKS = 64;
    /** X-coordinate of the left wall */
    static const Uint32 XCOORD_LEFT_WALL = 12;
    /** Y-coordinate of the left wall */
    static const Uint32 YCOORD_LEFT_WALL = 24;
    /** X-coordinate of the right wall */
    static const Uint32 XCOORD_RIGHT_WALL = 240;
    /** Y-coordinate of the right wall */
    static const Uint32 YCOORD_RIGHT_WALL = 24;
    /** X-coordinate of the top wall */
    static const Uint32 XCOORD_TOP_WALL = 32;
    /** Y-coordinate of the top wall */
    static const Uint32 YCOORD_TOP_WALL = 4;
    /** X-coordinate of the left wall */
    Uint32 xcoord_left_wall;
    /** Y-coordinate of the left wall */
    Uint32 ycoord_left_wall;
    /** X-coordinate of the right wall */
    Uint32 xcoord_right_wall;
    /** Y-coordinate of the right wall */
    Uint32 ycoord_right_wall;
    /** X-coordinate of the top wall */
    Uint32 xcoord_top_wall;
    /** Y-coordinate of the top wall */
    Uint32 ycoord_top_wall;
    /** Small horizontal side brick */
    sprite_object *horizontal_brick;
    /** Small vertical side brick */
    sprite_object *vertical_brick;
    /** Bitmap surface to save background under top bricks */
    surface_sdl *background_top_side;
    /** Bitmap surface to save background under right bricks */
    surface_sdl *background_right_side;
    /** Bitmap surface to save background under left bricks */
    surface_sdl *background_left_side;
    /** True if the wall of the top is breakable */
    bool is_top_wall_breakable;
    /** True if the wall of the right is breakable */
    bool is_right_wall_breakable;
    /** True if the wall of the left is breakable */
    bool is_left_wall_breakable;
    /** Collision x-coordinate of the left wall */
    Sint32 left_collision_xcoord;
    /** Collision x-coordinate of the right wall */
    Sint32 right_collision_xcoord;
    /** Collision y-coordinate of the top wall */
    Sint32 top_collision_ycoord;
    /* wall of sprites (bob_ground = 1) */
    sprite_object *sprites_top[MAX_OF_SIDES_BRICKS];
    sprite_object *sprites_right[MAX_OF_SIDES_BRICKS];
    sprite_object *sprites_left[MAX_OF_SIDES_BRICKS];
    /** State of the top wall, used for collision */
    bool map_top_wall[MAX_OF_SIDES_BRICKS];
    /** State of the right wall, used for collision */
    bool map_right_wall[MAX_OF_SIDES_BRICKS];
    /** State of the left wall, used for collision */
    bool map_left_wall[MAX_OF_SIDES_BRICKS];
    /** Width of a horizontal side brick in pixels */
    Uint32 horizontal_brick_width;
    /** Height of a horizontal side brick in pixels */
    Uint32 horizontal_brick_height;
    /** Height of a vertical side brick in pixels */
    Uint32 vertical_brick_width;
    /** Height of a vertical side brick in pixels */
    Uint32 vertical_brick_height;
    /** Structure used for restore  */
    typedef struct
      {
        /** Wall identifier TOP_WALL, RIGHT_WALL or LEFT_WALL */
        Uint32 wall_id;
        /** Index of the side brick from 0 to 11 */
        Uint32 side_brick_index;
      }
    restaure_struct;
    /** Structures used for restore background under sides bricks */
    restaure_struct *restore_background;
    /** Index to save the next brick */
    Uint32 restore_save_index;
    /** Index to save the current brick */
    Uint32 restore_index;
  public:
    controller_sides_bricks ();
    ~controller_sides_bricks ();
    void initialize ();
    void save_state_of_walls ();
    void save_background ();
    void run ();
    void draw_shadows_to_brackground ();
    void draw_to_brackground ();
    Sint32 get_top_ycoord ();
    Sint32 get_right_xcoord ();
    Sint32 get_left_xcoord ();
    bool collision_with_left_wall (Sint32 ycoord);
    bool collision_with_right_wall (Sint32 ycoord);
    bool collision_with_top_wall (Sint32 xcoord);

  private:
    void create_bricks_sprites ();
    void restore ();
    void disable_sprites ();
  };

#endif
