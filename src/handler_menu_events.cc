/**
 * @file handler_menu_events.cc
 * @brief Handler the events of the menu
 * @created 2007-1O-29
 * @date 2014-08-16
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
#include "../include/handler_menu_events.h"
#include "../include/handler_keyboard.h"

/**
 * Create menu events object
 */
handler_menu_events::handler_menu_events ()
{
  stop ();
}

/**
 * Release menu events object
 */
handler_menu_events::~handler_menu_events ()
{
  stop ();
}

/**
 * Stop menu events
 */
void
handler_menu_events::stop ()
{
  is_enabled = false;
  y_coord_left_down = handler_keyboard::NULL_YCOORD;
  y_coord_right_down = handler_keyboard::NULL_YCOORD;
  line_spacing = 0;
  x_center = 0;
  top_y_coord = 0;
  line_min = 0;
  line_max = 0;
  current_line = 0;
  key_delay = 0;
  previous_key_code_down = 0;
  row_spacing = 0;
  row_min = 0;
  row_max = 0;
  current_row = 0;
  left_x_coord = 0;
}

/**
 * Start a new horizontal/vertical menu handler
 * @param xspace Space between lines in pixels
 * @param yspace Space between rows in pixels
 * @param xmin Minimum row number
 * @param ymin Minimum line number
 * @param xmax Maximum row number
 * @param ymax Maximum line number
 * @param xleft X-coordinate left of the menu
 * @param ytop Y-coordinate top of the menu
 */
void
handler_menu_events::begin (Sint32 xspace, Sint32 yspace,
                            Sint32 xmin, Sint32 ymin, Sint32 xmax,
                            Sint32 ymax, Sint32 xleft, Sint32 ytop)
{
  row_spacing = xspace;
  row_min = xmin;
  row_max = xmax;
  current_row = 0;
  left_x_coord = xleft;
  start (yspace, ymin, ymax, 0, ytop);
}

/**
 * Start a new vertical menu handler
 * @param spacing Space between lines in pixels
 * @param min Minimum line number
 * @param max Maximum line number
 * @param xcenter X-coordinate center of the menu
 * @param ytop Y-coordinate top of the menu
 */
void
handler_menu_events::start (Sint32 spacing, Sint32 min, Sint32 max,
                            Sint32 xcenter, Sint32 ytop)
{
  if (is_verbose)
    {
      std::cout << " handler_menu_events::start() " <<
                " line_spacing: " << spacing << " line_min: " << min <<
                " line_max: " << max << " x_center: " << xcenter <<
                " top_y_coord: " << ytop << std::endl;
    }
  is_enabled = true;
  y_coord_left_down = handler_keyboard::NULL_YCOORD;
  y_coord_right_down = handler_keyboard::NULL_YCOORD;
  line_spacing = spacing;
  line_min = min;
  line_max = max;
  x_center = xcenter;
  current_line = 0;
  top_y_coord = ytop;
  previous_key_code_down = 0;
  Sint32 xmouse, ymouse;
  SDL_GetMouseState (&xmouse, &ymouse);
  if (ymouse < ytop + min * spacing)
    {
      current_line = min;
    }
  else if (ymouse > ytop + max * spacing)
    {
      current_line = max;
    }
  else
    {
      current_line = (ymouse - ytop) / spacing;
    }
}

/**
 * Code keys recognized in a menu
 */
Uint32
handler_menu_events::keys[MAX_OF_KEYS] =
{
  handler_keyboard::K_UP,
  handler_keyboard::K_DOWN,
  handler_keyboard::K_LEFT,
  handler_keyboard::K_RIGHT,
  handler_keyboard::K_FIRE,
  handler_keyboard::K_RELEASE_BALL,
  handler_keyboard::K_GIGABLITZ
};

/**
 * Check key handles
 * @param kcode Pointer the current key code down
 * @param prev_kcode Pointer the previous key code down
 */
bool handler_menu_events::check_keys (Uint32 * kcode, Uint32 * prev_kcode)
{
  bool
  is_pressed = false;
  *prev_kcode = 0;
  *kcode = 0;
  if (keyboard->get_input_cursor_pos () >= 0)
    {
      return false;
    }
  /* check keyboards events */
  if (previous_key_code_down > 0 &&
      !keyboard->control_is_pressed (previous_key_code_down))
    {
      *prev_kcode = previous_key_code_down;
      previous_key_code_down = 0;
      key_delay = 0;
    }
  if (key_delay < 1)
    {
      for (Uint32 i = 0; i < MAX_OF_KEYS; i++)
        {
          if (!keyboard->control_is_pressed (keys[i]))
            {
              continue;
            }
          is_pressed = true;
          *kcode = keys[i];
          if (previous_key_code_down != keys[i])
            {
              previous_key_code_down = keys[i];
              key_delay = 30;
            }
          else
            {
              key_delay = 10;
            }
          break;
        }
    }
  else
    {
      *kcode = 0;
      key_delay--;
    }
  return is_pressed;
}

/**
 * Check keys events for the shop
 */
bool handler_menu_events::check ()
{
  if (!is_enabled)
    {
      return false;
    }
  Uint32
  kcode = 0;
  Uint32
  prev_kcode = 0;
  bool
  is_pressed = check_keys (&kcode, &prev_kcode);
  bool
  is_selected = false;
  switch (prev_kcode)
    {
    case handler_keyboard::K_FIRE:
      is_selected = true;
      break;
    case handler_keyboard::K_RELEASE_BALL:
      is_selected = true;
      break;
    }
  /* check if right or left button are pressed */
  bool
  is_warp = false;
  if (is_pressed)
    {
      switch (kcode)
        {
        case handler_keyboard::K_LEFT:
          if (current_row == row_min)
            {
              current_row = row_max;
            }
          else
            {
              current_row--;
            }
          is_warp = true;
          break;
        case handler_keyboard::K_RIGHT:
          if (current_row == row_max)
            {
              current_row = row_min;
            }
          else
            {
              current_row++;
            }
          is_warp = true;
          break;
        case handler_keyboard::K_UP:
          if (current_line == line_min)
            {
              current_line = line_max;
            }
          else
            {
              current_line--;
            }
          is_warp = true;
          break;
        case handler_keyboard::K_DOWN:
          if (current_line == line_max)
            {
              current_line = line_min;
            }
          else
            {
              current_line++;
            }
          is_warp = true;
          break;
        }
      if (is_warp)
        {
          SDL_WarpMouse (left_x_coord + current_row * row_spacing,
                         top_y_coord + current_line * line_spacing);
        }
    }
  return is_selected;
}

/**
 * Check mouses events for the main menu and popup menu
 * @param pos_y Pointer to a integer which will contain y-coordinate
 *              where the mouse clicked
 * @param inc Pointer to a integer which will contain -1 if left mouse
 *            button clicked, 1 if right button clicked, otherwise 0
 * @return true if left mouse button clicked, otherwise false
 */
bool handler_menu_events::check (Sint32 * pos_y, Sint32 * inc)
{
  if (!is_enabled)
    {
      return false;
    }
  Uint32
  kcode = 0;
  Uint32
  prev_kcode = 0;
  check_keys (&kcode, &prev_kcode);
  bool
  is_selected = false;
  switch (prev_kcode)
    {
    case handler_keyboard::K_FIRE:
      *inc = 1;
      is_selected = true;
      break;
    case handler_keyboard::K_RELEASE_BALL:
      *inc = -1;
      is_selected = true;
      break;
    }

  /* check if right or left button are pressed */
  Sint32
  mposx;
  switch (kcode)
    {
    case handler_keyboard::K_FIRE:
      break;
    case handler_keyboard::K_RELEASE_BALL:
      break;
    case handler_keyboard::K_UP:
      if (current_line == line_min)
        {
          current_line = line_max;
        }
      else
        {
          current_line--;
        }
      SDL_WarpMouse (x_center, top_y_coord + current_line * line_spacing);
      break;
    case handler_keyboard::K_DOWN:
      if (current_line == line_max)
        {
          current_line = line_min;
        }
      else
        {
          current_line++;
        }
      SDL_WarpMouse (x_center, top_y_coord + current_line * line_spacing);
      break;
    }


  /*
   * check mouse events
   */
  bool
  is_left_down = keyboard->is_left_button ();
  bool
  is_right_down = keyboard->is_right_button ();
  /* read y where is pressed */
  if (is_left_down && y_coord_left_down == handler_keyboard::NULL_YCOORD)
    {
      y_coord_left_down = keyboard->get_mouse_y ();
    }
  else
    {
      if (is_right_down
          && y_coord_right_down == handler_keyboard::NULL_YCOORD)
        {
          y_coord_right_down = keyboard->get_mouse_y ();
        }
    }

  bool
  is_right_up = false;
  bool
  is_left_up = keyboard->is_left_button_up (&mposx, pos_y);
  if (!is_left_up)
    {
      is_right_up = keyboard->is_right_button_up (&mposx, pos_y);
    }
  if ((is_left_up && *pos_y == y_coord_left_down) ||
      (is_right_up && *pos_y == y_coord_right_down))
    {
      if (is_left_up)
        {
          *inc = 1;
          y_coord_left_down = handler_keyboard::NULL_YCOORD;
        }
      if (is_right_up)
        {
          *inc = -1;
          y_coord_right_down = handler_keyboard::NULL_YCOORD;
        }
      is_selected = true;
    }
  if (!is_left_down)
    {
      y_coord_left_down = handler_keyboard::NULL_YCOORD;
    }
  if (!is_right_down)
    {
      y_coord_right_down = handler_keyboard::NULL_YCOORD;
    }
  return is_selected;
}
