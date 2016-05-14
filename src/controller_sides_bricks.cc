/**
 * @file controller_sides_bricks.cc 
 * @brief Sides bricks controller. The small bricks on the side, the walls top
 *        left and right
 * @date 2012-09-06 
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
#include "../include/controller_sides_bricks.h"
#include "../include/handler_players.h"
#include "../include/handler_display.h"

/**
 * Create the sides bricks controller
 */
controller_sides_bricks::controller_sides_bricks ()
{
  littleInit ();
  xcoord_left_wall = XCOORD_LEFT_WALL * resolution;
  ycoord_left_wall = YCOORD_LEFT_WALL * resolution;
  xcoord_right_wall = XCOORD_RIGHT_WALL * resolution;
  ycoord_right_wall = YCOORD_RIGHT_WALL * resolution;
  xcoord_top_wall = XCOORD_TOP_WALL * resolution;
  ycoord_top_wall = YCOORD_TOP_WALL * resolution;

  if (!has_background)
    {
      max_of_sprites = (MAX_OF_SIDES_BRICKS + 4) * 3;
    }
  else
    {
      max_of_sprites = 2;
    }
  sprites_have_shades = true;

  horizontal_brick = (sprite_object *) NULL;
  vertical_brick = (sprite_object *) NULL;
  is_top_wall_breakable = false;
  is_right_wall_breakable = false;
  is_left_wall_breakable = false;
  background_top_side = NULL;
  background_right_side = NULL;
  background_left_side = NULL;
  restore_background = (restore_struct *) NULL;
  restore_save_index = 0;
  restore_index = 0;
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      map_left_wall[i] = map_right_wall[i] = map_top_wall[i] = false;
      sprites_left[i] = NULL;
      sprites_top[i] = NULL;
      sprites_right[i] = NULL;
    }
}

/**
 * Release the sides bricks controller
 */
controller_sides_bricks::~controller_sides_bricks ()
{
  if (NULL != restore_background)
    {
      delete[]restore_background;
      restore_background = NULL;
    }
  if (NULL != horizontal_brick)
    {
      delete horizontal_brick;
      horizontal_brick = (sprite_object *) NULL;
      sprites_top[0] = NULL;
    }
  if (NULL != vertical_brick)
    {
      delete vertical_brick;
      vertical_brick = (sprite_object *) NULL;
      sprites_right[0] = NULL;
    }
  if (NULL != background_top_side)
    {
      delete background_top_side;
      background_top_side = NULL;
    }
  if (NULL != background_right_side)
    {
      delete background_right_side;
      background_right_side = NULL;
    }
  if (NULL != background_left_side)
    {
      delete background_left_side;
      background_left_side = NULL;
    }

  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (sprites_left[i] != NULL)
        {
          delete sprites_left[i];
          sprites_left[i] = NULL;
        }
      if (sprites_top[i] != NULL)
        {
          delete sprites_top[i];
          sprites_top[i] = NULL;
        }
      if (sprites_right[i] != NULL)
        {
          delete sprites_right[i];
          sprites_right[i] = NULL;
        }
    }
  object_free ();
  release_sprites_list ();
}

/**
 * Perform some initializations 
 * params true if rebuilt the walls, false otherwise
 */
void
controller_sides_bricks::initialize ()
{
  if (is_verbose)
    {
      std::cout << ">controller_sides_bricks::initialize() start!" << std::
      endl;
    }
  /* create a sprite object for small horizontal bricks */
  horizontal_brick = new sprite_object ();
  horizontal_brick->create_sprite (sprite_object::HORIZONTAL_SIDE_BRICK,
				 sprites_bitmap, true);
  horizontal_brick_width = horizontal_brick->get_sprite_width ();
  horizontal_brick_height = horizontal_brick->get_sprite_height ();

  /* create a sprite object for small vertical bricks */
  vertical_brick = new sprite_object ();
  vertical_brick->create_sprite (sprite_object::VERTICAL_SIDE_BRICK,
				   sprites_bitmap, true);
  vertical_brick_width = vertical_brick->get_sprite_width ();
  vertical_brick_height = vertical_brick->get_sprite_height ();

  /* set collisions coordinates of the 3 walls */
  left_collision_xcoord = xcoord_left_wall + vertical_brick_width;
  right_collision_xcoord = xcoord_right_wall;
  top_collision_ycoord = ycoord_top_wall + horizontal_brick_height;

  /* area 2 to 4; rebuild wall the three walls automatically */
  Uint32 area = current_player->get_area_number ();
  bool is_rebuild = current_player->is_rebuild_walls ();
  current_player->set_rebuild_walls (false);
  if (area < 5 || is_rebuild)
    {
      for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
        {
          map_left_wall[i] = true;
          map_top_wall[i] = true;
          map_right_wall[i] = true;
        }
    }

  /* area 5: recopy the state of the walls of the preceding level */
  else
    {
      bool *left = current_player->get_map_left ();
      bool *right = current_player->get_map_right ();
      bool *top = current_player->get_map_top ();
      for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
        {
          map_left_wall[i] = left[i];
          map_top_wall[i] = top[i];
          map_right_wall[i] = right[i];
        }
    }

  /* determine the behavior of the three walls, according to the area */
  //area = 4; /* test only */
  switch (area)
    {
    case 2:
      is_top_wall_breakable = true;
      is_right_wall_breakable = false;
      is_left_wall_breakable = false;
      break;

    case 3:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = false;
      break;

    case 4:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = true;
      break;

    case 5:
      is_top_wall_breakable = true;
      is_right_wall_breakable = true;
      is_left_wall_breakable = true;
      break;

    default:
      is_top_wall_breakable = false;
      is_right_wall_breakable = false;
      is_left_wall_breakable = false;
      break;
    }
  /* wall of the top is breakable (test only) */
  //is_top_wall_breakable = true;
  /* wall of the right is breakable (test only) */
  //is_right_wall_breakable = true;
  /* wall of the left is breakable (test only) */
  //is_left_wall_breakable = true;

  /* create the sprites of the walls if necessary */
  create_bricks_sprites ();

  /* allocate memory for background restoration under sides bricks */
  try
    {
      restore_background = new restore_struct[MAX_OF_RESTORED_BRICKS];
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!) controller_sides_bricks::initialize() "
      "not enough memory to allocate restore_background structure "
      << std::endl;
      throw;
    }
  for (Uint32 i = 0; i < MAX_OF_RESTORED_BRICKS; i++)
    {
      restore_background[i].wall_id = 0;
      restore_background[i].side_brick_index = 0;

    }
  if (is_verbose)
    {
      std::cout << "/controller_sides_bricks::initialize() end!" << std::endl;
    }
}

/**
 * Create the sprites of the walls
 */
void
controller_sides_bricks::create_bricks_sprites ()
{
  if (has_background)
    {
      return;
    }

  Sint32 xcoord_top = xcoord_top_wall;
  Sint32 ycoord_left = ycoord_left_wall;
  Sint32 ycoord_right = ycoord_right_wall;
  Uint32 width = horizontal_brick->get_sprite_width ();
  Uint32 height = vertical_brick->get_sprite_height ();
  sprites_top[0] = horizontal_brick;
  sprites_right[0] = vertical_brick;
  /* 12 bricks per wall */
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      /* top wall */
      if (i > 0)
        {
          sprites_top[i] = new sprite_object ();
          horizontal_brick->duplicate_to (sprites_top[i]);
        }
      sprites->add (sprites_top[i]);
      sprites_top[i]->set_coordinates (xcoord_top, ycoord_top_wall);
      if (map_left_wall[i])
        {
          sprites_top[i]->enable ();
        }
      /* right wall */
      if (i > 0)
        {
          sprites_right[i] = new sprite_object ();
          vertical_brick->duplicate_to (sprites_right[i]);
        }
      sprites->add (sprites_right[i]);
      sprites_right[i]->set_coordinates (xcoord_right_wall, ycoord_right);
      if (map_right_wall[i])
        {
          sprites_right[i]->enable ();
        }
      /* left wall */
      sprites_left[i] = new sprite_object ();
      vertical_brick->duplicate_to (sprites_left[i]);
      sprites->add (sprites_left[i]);
      sprites_left[i]->set_coordinates (xcoord_left_wall, ycoord_left);
      if (map_top_wall[i])
        {
          sprites_left[i]->enable ();
        }
      xcoord_top += width;
      ycoord_left += height;
      ycoord_right += height;
    }
}

/**
 * Recopy the state of walls after a level 
 */
void
controller_sides_bricks::save_state_of_walls ()
{
  bool *left = current_player->get_map_left ();
  bool *right = current_player->get_map_right ();
  bool *top = current_player->get_map_top ();
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      left[i] = map_left_wall[i];
      right[i] = map_right_wall[i];
      top[i] = map_top_wall[i];
    }
}

/**
 * Save background under small brick
 */
void
controller_sides_bricks::save_background ()
{

  background_top_side =
    background_screen->cut_to_surface (xcoord_top_wall, ycoord_top_wall,
                                       horizontal_brick_width *
                                       MAX_OF_SIDES_BRICKS,
                                       horizontal_brick_height);
  background_right_side =
    background_screen->cut_to_surface (xcoord_right_wall, ycoord_right_wall,
                                       vertical_brick_width,
                                       vertical_brick_height *
                                       MAX_OF_SIDES_BRICKS);
  background_left_side =
    background_screen->cut_to_surface (xcoord_left_wall, ycoord_left_wall,
                                       vertical_brick_width,
                                       vertical_brick_height *
                                       MAX_OF_SIDES_BRICKS);
}

/**
 * Disable sides bricks if necessary
 */
void
controller_sides_bricks::run ()
{
  if (has_background)
    {
      /* sides bricks are drawing to background */
      restore ();
    }
  else
    {
      /* sides bricks are displayed as sprites */
      disable_sprites ();
    }
}

/**
 * Restore the background under the brick
 */
void
controller_sides_bricks::restore ()
{

  if (0 == restore_background[restore_index].wall_id)
    {
      return;
    }
  Uint32 index = restore_background[restore_index].side_brick_index;
  SDL_Rect source;
  SDL_Rect dest;
  SDL_Surface *source_surface = NULL;
  Uint32 voffset = game_screen->get_vertical_offset ();

  switch (restore_background[restore_index].wall_id)
    {
    case TOP_WALL:
      source.w = dest.w = horizontal_brick_width;
      source.h = dest.h = horizontal_brick_height;
      source.x = index * horizontal_brick_width;
      source.y = 0;
      dest.x = xcoord_top_wall + index * horizontal_brick_width;
      dest.y = voffset + ycoord_top_wall;
      source_surface = background_top_side->get_surface ();
      break;
    case LEFT_WALL:
    case RIGHT_WALL:
      source.w = dest.w = vertical_brick_width;
      source.h = dest.h = vertical_brick_height;
      source.x = 0;
      source.y = index * vertical_brick_height;
      if (LEFT_WALL == restore_background[restore_index].wall_id)
        {
          dest.x = xcoord_left_wall;
          dest.y = voffset + ycoord_left_wall + index * vertical_brick_height;
          source_surface = background_left_side->get_surface ();
        }
      else
        {
          dest.x = xcoord_right_wall;
          dest.y =
            voffset + ycoord_right_wall + index * vertical_brick_height;
          source_surface = background_right_side->get_surface ();
        }
      break;
    }
  if (SDL_BlitSurface
      (source_surface, &source, game_screen->get_surface (), &dest) < 0)
    {
      std::cerr << "(!)controller_sides_bricks::restore_background() " <<
      "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }
  if (SDL_BlitSurface
      (source_surface, &source, background_screen->get_surface (), &dest) < 0)
    {
      std::cerr << "(!)controller_sides_bricks::restore_background() " <<
      "SDL_BlitSurface() return " << SDL_GetError () << std::endl;
    }

  /* clear shadow */
  dest.x += handler_display::SHADOWOFFX * resolution;
  dest.y = dest.y - voffset + handler_display::SHADOWOFFY * resolution;
  switch (restore_background[restore_index].wall_id)
    {
    case TOP_WALL:
      if (0 == index)
        {
          dest.w -= resolution * 4;
          dest.x += resolution * 4;
        }
      break;
    case RIGHT_WALL:
      if (index == MAX_OF_SIDES_BRICKS - 1)
        {
          dest.h -= resolution * 4;
        }
      break;
    }
  display->clr_shadow (dest.x, dest.y, dest.w, dest.h);
  restore_index = (restore_index + 1) & (MAX_OF_RESTORED_BRICKS - 1);
}

/**
* Disable sides bricks sprites if necessary
*/
void
controller_sides_bricks::disable_sprites ()
{
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (sprites_left[i]->is_enable () && !map_left_wall[i])
        {
          sprites_left[i]->disable ();
        }
      if (sprites_top[i]->is_enable () && !map_top_wall[i])
        {
          sprites_top[i]->disable ();
        }
      if (sprites_right[i]->is_enable () && !map_right_wall[i])
        {
          sprites_right[i]->disable ();
        }
    }
}

/**
 * Draw shadow of the three walls (before a bricks level) 
 */
void
controller_sides_bricks::draw_shadows_to_brackground ()
{
  if (!has_background)
    {
      return;
    }
  Sint32 xcoord_top = xcoord_top_wall;
  Sint32 ycoord_left = ycoord_left_wall;
  Sint32 ycoord_right = ycoord_right_wall;
  Uint32 width = horizontal_brick->get_sprite_width ();
  Uint32 height = vertical_brick->get_sprite_height ();
  /* 12 bricks per wall */
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (map_top_wall[i])
        {
          horizontal_brick->set_coordinates (xcoord_top, ycoord_top_wall);
          horizontal_brick->draw_shadow_to_brackground ();
        }
      if (map_left_wall[i])
        {
          vertical_brick->set_coordinates (xcoord_left_wall, ycoord_left);
          vertical_brick->draw_shadow_to_brackground ();
        }
      if (map_right_wall[i])
        {
          vertical_brick->set_coordinates (xcoord_right_wall, ycoord_right);
          vertical_brick->draw_shadow_to_brackground ();
        }
      xcoord_top += width;
      ycoord_left += height;
      ycoord_right += height;
    }
}

/**
 * Draw bricks of the three walls (before a bricks level) 
 */
void
controller_sides_bricks::draw_to_brackground ()
{
  if (!has_background)
    {
      return;
    }
  Sint32 xcoord_top = xcoord_top_wall;
  Sint32 ycoord_left = ycoord_left_wall;
  Sint32 ycoord_right = ycoord_right_wall;
  Uint32 width = horizontal_brick->get_sprite_width ();
  Uint32 height = vertical_brick->get_sprite_height ();
  /* 12 bricks per wall */
  for (Uint32 i = 0; i < MAX_OF_SIDES_BRICKS; i++)
    {
      if (map_top_wall[i])
        {
          horizontal_brick->set_coordinates (xcoord_top, ycoord_top_wall);
          horizontal_brick->draw_to_brackground ();
        }
      if (map_left_wall[i])
        {
          vertical_brick->set_coordinates (xcoord_left_wall, ycoord_left);
          vertical_brick->draw_to_brackground ();
        }
      if (map_right_wall[i])
        {
          vertical_brick->set_coordinates (xcoord_right_wall, ycoord_right);
          vertical_brick->draw_to_brackground ();
        }
      xcoord_top += width;
      ycoord_left += height;
      ycoord_right += height;
    }
}

/**
 * Return collision y-coordinate of the top wall
 * @return y-coordinate of the top wall
 */
Sint32 controller_sides_bricks::get_top_ycoord ()
{
  return top_collision_ycoord;
}

/**
 * Return collision x-coordinate of the right wall
 * @return x-coordinate of the left wall
 */
Sint32 controller_sides_bricks::get_right_xcoord ()
{
  return right_collision_xcoord;
}

/**
 * Return collision x-coordinate of the left wall
 * @return x-coordinate of the left wall
 */
Sint32 controller_sides_bricks::get_left_xcoord ()
{
  return left_collision_xcoord;
}

/**
 * Collision with the wall of the left
 * @param ycoord y coordinate of the ball
 * @return true if collision, otherwise false
 */
bool controller_sides_bricks::collision_with_left_wall (Sint32 ycoord)
{
  if (!is_left_wall_breakable)
    {
      /* because the left wall is unbreakable, there is always collision */
      return true;
    }
  Sint32
  index = (ycoord - (Sint32) ycoord_left_wall) / vertical_brick_height;
  if (index < 0 || index >= (Sint32) MAX_OF_SIDES_BRICKS)
    {
      return true;
    }
  if (!map_left_wall[index])
    {
      /* there is no more brick on side which protects */
      return false;
    }
  map_left_wall[index] = false;
  restore_background[restore_save_index].wall_id = LEFT_WALL;
  restore_background[restore_save_index].side_brick_index = (Uint32) index;
  restore_save_index =
    (restore_save_index + 1) & (MAX_OF_RESTORED_BRICKS - 1);
  return true;
}

/**
 * Collision with the wall of the right 
 * @param ycoord y coordinate of the ball
 * @return true if collision, otherwise false
 */
bool controller_sides_bricks::collision_with_right_wall (Sint32 ycoord)
{
  if (!is_right_wall_breakable)
    {
      /* because the right wall is unbreakable, there is always collision */
      return true;
    }
  Sint32
  index =
    (ycoord - (Sint32) ycoord_right_wall) / (Sint32) vertical_brick_height;
  if (index < 0 || index >= (Sint32) MAX_OF_SIDES_BRICKS)
    {
      return true;
    }
  if (!map_right_wall[index])
    {
      /* there is no more brick on side which protects */
      return false;
    }
  map_right_wall[index] = false;
  restore_background[restore_save_index].wall_id = RIGHT_WALL;
  restore_background[restore_save_index].side_brick_index = (Uint32) index;
  restore_save_index =
    (restore_save_index + 1) & (MAX_OF_RESTORED_BRICKS - 1);
  return true;
}

/**
 * Collision with the wall of the top 
 * @param ycoord y coordinate of the ball
 * @return true if collision, otherwise false
 */
bool controller_sides_bricks::collision_with_top_wall (Sint32 xcoord)
{
  if (!is_top_wall_breakable)
    {
      /* because the top is unbreakable, there is always collision */
      return true;
    }
  Sint32
  index =
    (Sint32) ((Sint32) (xcoord - (Sint32) xcoord_top_wall)) /
    (Sint32) horizontal_brick_width;
  if (index < 0 || index >= (Sint32) MAX_OF_SIDES_BRICKS)
    {
      return true;
    }
  if (!map_top_wall[index])
    {
      /* there is no more brick on side which protects */
      return false;
    }
  map_top_wall[index] = false;
  restore_background[restore_save_index].wall_id = TOP_WALL;
  restore_background[restore_save_index].side_brick_index = (Uint32) index;
  restore_save_index =
    (restore_save_index + 1) & (MAX_OF_RESTORED_BRICKS - 1);
  return true;
}
