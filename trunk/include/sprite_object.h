/**
 * @file sprite_object.h
 * @brief Draw sprites on the screen 
 * @date 2014-07-27 
 * @copyright 1991-2014 TLK Games
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
#ifndef __SPRITE_OBJECT__
#define __SPRITE_OBJECT__
class sprite_object;
#include "../include/handler_display.h"
#include "../include/bitmap_data.h"
#include "../include/tecnoballz.h"
#include "../include/display_text_bitmap.h"

/*
 * sprite's ID
 */
/*
typedef enum
{
  BOB_ATOMES,
  BOB_EJECT1,
  BOB_EJECT2,
  BOB_EJECT3,
  BOB_EJECT4,
  BOB_BRICKV,
  BOB_BRICKH,
  BOB_BRICKS,
  BOB_BUMPHR,
  BOB_BUMPVT,
  BOB_BALLES,
  BOB_FIREBP,
  BOB_MONEYS,
  BOB_GADGET,
  BOB_LETTRE,
  BOB_LEDSHP,
  BOB_GAMEOV,
  BOB_DEFILE,
  BOB_LOGOTB,
  BOB_MOUSE1,
  BOB_MOUSE2,
  BOB_GIGAB1,
  BOB_GIGAB2,
  BOB_GIGAB3,
  BOB_GIGAB4,
  BOB_GIGAB5,
  BOB_GIGAB6,
  BOB_GIGAB7,
  BOB_MONEY0,
  BOB_ROBOT0,
  FINAL_GUARDIAN_AREA_1,
  FINAL_GUARDIAN_AREA_2,
  FINAL_GUARDIAN_AREA_3,
  FINAL_GUARDIAN_AREA_4,
  FINAL_GUARDIAN_AREA_5,
  FINAL_GUARDIAN_AREA_6,
  MEDIUM_GUARDIAN_AREA_1A,
  MEDIUM_GUARDIAN_AREA_1B,
  MEDIUM_GUARDIAN_AREA_2A,
  MEDIUM_GUARDIAN_AREA_2B,
  MEDIUM_GUARDIAN_AREA_3A,
  MEDIUM_GUARDIAN_AREA_3B,
  MEDIUM_GUARDIAN_AREA_4A,
  MEDIUM_GUARDIAN_AREA_4B,
  MEDIUM_GUARDIAN_AREA_5A,
  MEDIUM_GUARDIAN_AREA_5B,
  PADDLE_IN_GUARDIANS_LEVEL,
  BOB_MISSIL,
  BOB_EXPLO1,
  BOB_EXPLO2,
  BOB_ARGENT,
  BOB_GRDNRJ,
  BOB_GEMSTO,
  BOB_WALLBO,
  BOB_DIRECT,
  BOB_ESCMEN,
  BOB_MAGEYE,
  BOB_LEDSH2,
  BOB_BRICK1
}
BOB_ENUM;
*/


/**
 * Structure used to store x and y coordinates in the png image source
 */ 
typedef struct
  {
    /** X-coordinate: real x = xcoord * 16 */
    Sint16 xcoord;
    /** Y-coordinate: real y = ycoord * 2 */
    Sint16 ycoord;
  }
sprite_coordinates;

/**
 * Structure used to store caracteristics of the sprite in the png image
 * source width, height, coordinates in the png image source
 */
typedef struct
  {
    /* real width = width * resolution */
    Uint16 width;
    /* real height = height * resolution */
    Uint16 height;
    /* number of animation(s) */
    Uint16 number_of_images;
    sprite_coordinates *coordinates;
  }
sprite_description;

typedef struct
  {
    Sint32 COUNTERTAB;            // size of table offsets / loops
    Sint32 OFFSETLEFT;
    Sint16 *TABAFFICH1;           // table of offsets and loops counters
    char *TABAFFICH2;             // table of pixels
  }
bb_afligne;

class sprite_object:public virtual tecnoballz
  {
    friend class controller_game_over;
    friend class controller_explosions;
    friend class controller_guardians;
    friend class controller_gems;
    friend class controller_bullets;
    friend class controller_spheres;
    friend class display_text_bitmap;

  public:

    typedef enum
    {
      SHIPS,
      EJECTOR_1,
      EJECTOR_2,
      EJECTOR_3,
      EJECTOR_4,
      VERTICAL_SIDE_BRICK,
      HORIZONTAL_SIDE_BRICK,
      BOB_BRICKS,
      HORIZONTAL_PADDLE,
      VERTICAL_PADLLE,
      BALLS,
      PROJECTILES,
      MONEY,
      CAPSULES,
      FONT_GAME,
      SHOP_LED_INDICATOR_320,
      GAME_OVER_LETTERS,
      FONT_MENU,
      TECNOBALLZ_LOGO,
      MOUSE_POINTER_1,
      MOUSE_POINTER_2,
      GIGABLITZ_1,
      GIGABLITZ_2,
      GIGABLITZ_3,
      GIGABLITZ_4,
      GIGABLITZ_5,
      GIGABLITZ_6,
      GIGABLITZ_7,
      FONT_MONEY,
      PADDLE_ROBOT,
      FINAL_GUARDIAN_AREA_1,
      FINAL_GUARDIAN_AREA_2,
      FINAL_GUARDIAN_AREA_3,
      FINAL_GUARDIAN_AREA_4,
      FINAL_GUARDIAN_AREA_5,
      FINAL_GUARDIAN_AREA_6,
      MEDIUM_GUARDIAN_AREA_1A,
      MEDIUM_GUARDIAN_AREA_1B,
      MEDIUM_GUARDIAN_AREA_2A,
      MEDIUM_GUARDIAN_AREA_2B,
      MEDIUM_GUARDIAN_AREA_3A,
      MEDIUM_GUARDIAN_AREA_3B,
      MEDIUM_GUARDIAN_AREA_4A,
      MEDIUM_GUARDIAN_AREA_4B,
      MEDIUM_GUARDIAN_AREA_5A,
      MEDIUM_GUARDIAN_AREA_5B,
      PADDLE_IN_GUARDIANS_LEVEL,
      BULLET,
      EXPLOSION_1,
      EXPLOSION_2,
      METALLIC_SPHERE,
      LIFE_GAUGE,
      GEM,
      BOTTOM_WALL,
      VIEWFINDER,
      POPUP_MENU,
      MAGNETIC_EYE,
      SHOP_LED_INDICATOR_640,
      BRICKS,
      NUMOF_SPRITE_TYPES
    }
    SPRITES_ENUM;

    typedef enum {
      DRAW_WITH_TABLES,
      COPY_FROM_BITMAP,
      DRAW_LINE_BY_LINE,
      DRAW_REPEAT_SPRITE,
      DRAW_COLOR_CYCLING_MASK,
      DRAW_CAPSULE,
      DRAW_BRICK
    }
    DRAW_METHOD_ENUM;

  private:
    /** List of tables of the drawing pixels data. Used for drawing sprite
     * pixel by pixel, with color cycling.
     * Only used for the chance capsule sprite */
    Sint16 **drawing_pixels;
    /** List of pointers of all images of the sprite
     * in the source bitmap page */
    char **images_pixel_data;
    /** Pointer to sprite shadow in game offscreen for sprite */
    char *shadow_screen_ptr;
    /** Pointer to the background in restore offscreen for shadow */
    char *shadow_restore_ptr;
    Sint32 offsetSrce;            // offset source
    Sint32 offsetDest;            // offset destination

    /*
     * Drawing tables
     */
    /** List of tables of the drawing offsets and repeats values */
    Sint16 **drawing_values;
    /** Table of drawing offsets and repeats values for
     * current sprite image frame */
    Sint16 *current_drawing_values;
    /** List of tables of the drawing pixels data */
    char **drawing_data;
    /** Data structure of drawing sprite line by line.
     * Only used for the Gigablitz sprite */
    bb_afligne **drawing_peer_line;

    static Sint32 ombredecax;
    static Sint32 ombredecay;
    static Sint32 ombrepixe4;
    static char ombrepixel;


  protected:
    /** Table of drawing pixels data for current sprite image frame */
    char *current_drawing_data;
    /** Table of drawing data for current sprite image frame */
    Sint16 *current_drawing_pixels;
    /** Pointer to sprite in game offscreen */
    char *screen_ptr;
    /** Pointer to the background in restore offscreen */
    char *restore_ptr;
    /** The pixel data of the sprite */
    char *pixel_data;
    /** Width in bytes of bitmap in which is the sprite */
    Uint32 row_size;
    /** Width in bytes of offscreen */
    Uint32 offscreen_pitch;
    /** True if the sprite is enabled and drawn */
    bool is_enabled;
    /** True if the sprite has a shadow */
    bool sprite_has_shadow;
    /** X coordinate */
    Sint32 x_coord;
    /** Y coordinate */
    Sint32 y_coord;
    /** Width of the sprite in pixels */
    Uint32 sprite_width;
    /** Height of the sprite in pixels */
    Uint32 sprite_height;
    /** Time delay before next image of the animation */
    Sint32 frame_period;
    /** Counter time delay before next image */
    Sint32 frame_delay;
    /** Maximum number of images in the animation */
    Sint32 max_of_images;
    /** Index of the current image */
    Sint32 frame_index;
    Sint32 frame_index_max;
    Sint32 frame_index_min;
    /** Position of sprite in global list of all sprites */
    Sint32 display_pos;
    /** Width of the game screen in pixels */
    Sint32 screen_width;
    /** Height of the game screen in pixels */
    Sint32 screen_height;
    /** X coordinate minimum */
    Sint32 x_minimum;
    /** Y coordinate minimum */
    Sint32 y_minimum;
    /** X coordinate maximum */
    Sint32 x_maximum;
    /** Y coordinate maximum */
    Sint32 y_maximum;
    /** Type identifier of the sprite, integer from 0 to n */
    Uint32 sprite_type_id;
    Sint32 affligFrSv;            // premiere ligne a afficher (si afflignesF=1)
    Sint32 affligLaSv;            // derniere ligne a afficher (si afflignesF=1)
    /** If true, object has allocated memory for pixels of sprite */
    bool has_allocated_memory;
    /** Position of sprite in its list from 0 to n */
    Sint32 object_pos;
    /** true if the object must release
     * the pixel data memory at its destruction */
    bool is_release_pixel_data;
    /** If true generate additional table to drawing pixel by pixel.
     * Used with for color cycling */
    bool is_draw_pixel_by_pixel;

  public:
    /** Width used for the collisions */
    Uint32 collision_width;
    /** Height used for the collisions */
    Uint32 collision_height;
    Sint32 affligFrst;            // premiere ligne a afficher (si afflignesF=1)
    Sint32 affligLast;            // derniere ligne a afficher (si afflignesF=1)
    /** If true then mirror lines vertically in the initialization
     * of the sprite */
    bool is_mirrored_vertically;
    Sint32 num_of_repeats;            // > 1 repeat
    /** Current draw method used to display sprite */
    Uint32 draw_method;
    /** Current cycling color table index */
    Sint32 cycling_index;
    /** Current cycling color table used for projectiles */
    const Sint32 *current_cycling;

  public:
    sprite_object ();
    virtual ~sprite_object ();
    void clear_sprite_members ();
    void release_sprite ();
    void enable ();
    void disable ();
    bool is_enable ();
    Uint32 get_sprite_type_id ();
    void set_display_pos (Sint32 num);
    void set_object_pos (Sint32 num);
    void duplicate_to (sprite_object * sprite_dest);
    void set_coordinates (Sint32 xcoord, Sint32 ycoord);
    void create_sprite (Sint32 type_id, surface_sdl * bitmap, bool shadow,
                        bool by_pixel = false);
    void make_sprite (surface_sdl * image, bool shadow = false);
    void init_coords_max_min(Uint32 width_less = 0);
    void set_x_coord (Sint32 xcoord);
    void set_y_coord (Sint32 ycoord);
    void move_x (Sint32 x_offset);
    void move_y (Sint32 y_offset);
    bool has_shadow ();
    void set_shadow (bool shadow = true);
    Sint32 get_x_coord ();
    Sint32 get_y_coord ();
    void set_image ();
    void set_image (Sint32 index);
    Sint32 get_frame_index ();
    void restore_background_under_sprite ();
    void restore_line_by_line ();
    virtual void draw ();
    void restore_background_under_shadow ();
    void draw_shadow ();
    void draw_to_brackground ();
    void draw_capsule ();
    void draw_shadow_to_brackground ();
    void draw_copy_from_bitmap ();
    void copy_to_game_screen ();
    void restore_rectangle_background ();
    void pull (sprite_object * sprite, Sint32 xoffset = 0, Sint32 yoffset = 0);
    void attract (sprite_object * sprite, Sint32 xoffset = 0, Sint32 yoffset = 0);
    virtual bool collision (sprite_object * sprite);
    virtual bool collision (Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);
    void set_frame_delay (Sint32 delay);
    void set_frame_period (Sint32 period);
    bool play_animation_once ();
    void play_animation_loop ();
    void new_offset (Sint32 nume);
    void clip_coordinates ();
    Uint32 get_sprite_width ();
    Uint32 get_sprite_height ();
    Uint32 get_collision_width ();
    void enable_vertical_repeat (Uint32 numof_repeats);
    void set_draw_method (Uint32 method);
    void set_pixel_data (char *pixel, bool is_release = false);

  public:
    static const Sint32 cycling_01[8];
    static const Sint32 cycling_02[8];
    static const sprite_description *zelistBOB[];

    void draw_with_tables ();
  private:
    void init_common (surface_sdl * image, bool shadow);
    void alloc_drawing_tables (Sint32 num_images);
    void draw_line_by_line ();
    void draw_vertically_repeated ();
    void draw_cycling_color ();
    void draw_brick ();

  };
#endif
