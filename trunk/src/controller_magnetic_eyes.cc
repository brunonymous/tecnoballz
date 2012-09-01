/**
 * @file controller_magnetic_eyes.cc 
 * @brief Magectic eyes controller 
 * @created 2004-09-17 
 * @date 2007-10-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_magnetic_eyes.cc,v 1.6 2007-10-29 13:18:53 gurumeditation Exp $
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
#include "../include/controller_magnetic_eyes.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_resources.h"

/**
 * Create the magectic eyes controller
 */
controller_magnetic_eyes::controller_magnetic_eyes ()
{
  littleInit ();
  max_of_sprites = 3;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::MAGNETIC_EYE;
}

/**
 * Release the magectic eyes controller
 */
controller_magnetic_eyes::~controller_magnetic_eyes ()
{
  release_sprites_list ();
}

/**
 * Enable a new eye
 * @return true if an eye was activated
 */
bool controller_magnetic_eyes::create_eye ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_eye *
      eye = sprites_list[i];
      if (eye->is_enabled)
        {
          continue;
        }
      eye->is_enabled = true;
      return true;
    }
  return false;
}

/**
 * Create a initialize eyes sprites
 */
void
controller_magnetic_eyes::create_eyes_list ()
{
  Sint32 res = resolution;
  create_sprites_list ();
  Sint32 random = random_counter;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_eye *eye = sprites_list[i];
      eye->centerPosx = x_coordinates[random & 31] * res;
#if __WORDSIZE == 64
      random += (long) eye;
#else
      random += (Sint32) eye;
#endif
      eye->centerPosy = y_coordinates[random & 31] * res;
      random += keyboard->get_mouse_y ();
      eye->finishPosx = x_coordinates[random & 31] * res;
      random += keyboard->get_mouse_x ();
      eye->finishPosy = y_coordinates[random & 31] * res;
      random++;
      eye->radius_360 = 0;
    }
  /* value used for the collisions with balls */
  hypotenuse = sprites_list[0]->collision_width - res * 2;
  center_x = hypotenuse / 2;
  center_y = center_x;
  hypotenuse = hypotenuse * hypotenuse;
}

/**
 * Move all eyes
 */
void
controller_magnetic_eyes::move ()
{
  Sint32 random = random_counter;
  Sint32 res = resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_eye *eye = sprites_list[i];
      eye->play_animation_loop ();

      /* verify if center coordinates arrived at destination */
      Sint32 j = 3 * res;
      if (eye->centerPosx > eye->finishPosx - j &&
          eye->centerPosx < eye->finishPosx + j &&
          eye->centerPosy > eye->finishPosy - j &&
          eye->centerPosy < eye->finishPosy + j)
        {
          random += keyboard->get_mouse_y ();
          eye->finishPosx = x_coordinates[random & 31] * res;
          random += keyboard->get_mouse_x ();
          eye->finishPosy = y_coordinates[random & 31] * res;
        }

      /* move center */
      Sint32 inc_x = res;
      Sint32 inc_y = res;
      Sint32 delta_x = eye->finishPosx - eye->centerPosx;
      if (delta_x < 0)
        {
          delta_x = -delta_x;
          inc_x = -res;
        }
      Sint32 delta_y = eye->finishPosy - eye->centerPosy;
      if (delta_y < 0)
        {
          delta_y = -delta_y;
          inc_y = -res;
        }
      Sint32 hflag = 0;
      if (delta_y > delta_x)
        {
          j = delta_y;
          delta_y = delta_x;
          delta_x = j;
          hflag = 1;
        }
      Sint32 value = delta_y * 2 - delta_x;
      delta_x *= 2;
      j = delta_x - 1;
      do
        {
          if (hflag)
            {
              eye->centerPosy += inc_y;
            }
          else
            {
              eye->centerPosx += inc_x;
            }
          value -= delta_x;
          if (value < 0)
            {
              if (!hflag)
                {
                  eye->centerPosy += inc_y;
                }
              else
                {
                  eye->centerPosx += inc_x;
                }
              break;
            }
          j--;
        }
      while (j >= 0);


      /* move circle */
      eye->radius_360 = (eye->radius_360 + 4) % 360;
      Sint32 x =
        (handler_resources::zesinus360[eye->radius_360] * 10 * res) >> 7;
      Sint32 y =
        (handler_resources::cosinus360[eye->radius_360] * 10 * res) >> 7;
      eye->x_coord = eye->centerPosx + x + (15 * res);
      eye->y_coord = eye->centerPosy + y + (15 * res);
      random += 4;
    }
}
const Uint16
controller_magnetic_eyes::x_coordinates[32] =
  {
    40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 92, 96, 100, 105, 111, 115,
    118, 120, 122, 128, 130, 132, 135, 140, 144, 146, 150, 152, 160, 164, 166,
    170
  };
const
Uint16
controller_magnetic_eyes::y_coordinates[32] =
  {
    40, 44, 56, 58, 60, 62, 70, 78, 80, 82, 85, 90, 92, 94, 96, 100,
    101, 120, 122, 124, 130, 138, 144, 146, 148, 150, 152, 153, 154, 155, 160,
    170
  };
