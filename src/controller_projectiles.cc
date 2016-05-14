/** 
 * @file controller_projectiles.cc 
 * @brief Projectiles controller for a single paddle! 
 * @date 2007-10-24
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
#include "../include/controller_projectiles.h"

/**
 * Create the projectiles controller
 */
controller_projectiles::controller_projectiles ()
{
  littleInit ();
  countTempo = 0;
  max_of_sprites = MAX_OF_PROJECTILES;
  sprites_have_shades = false;
  sprite_type_id = sprite_object::PROJECTILES;
  if (resolution == 1)
    {
      circular_sin = &circular_sin_320[0];
    }
  else
    {
      circular_sin = &circular_sin_640[0];
    }
}

/**
 * Release the projectiles controller
 */
controller_projectiles::~controller_projectiles ()
{
  release_sprites_list ();
}

/**
 * Create and initialize the list of projectiles sprites
 * @param paddle a pointer to a paddle sprite
 */
void
controller_projectiles::create_projectiles_list (sprite_paddle * paddle)
{
  create_sprites_list ();
  gun_paddle = paddle;
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      projectile->init_members (paddle);
    }
  set_fire_1 ();
}

/**
 * Check if fire is available
 */
void
controller_projectiles::disponible ()
{

  if (gun_paddle->fire_state == sprite_paddle::NOT_OWN_GUN)
    {
      /* not fire available */
      return;
    }

  Uint32 t = max_of_sprites;
  sprite_projectile **projectiles = sprites_list;

  /* special fire 7 (circular fire) */
  if (gun_paddle->length == gun_paddle->width_maxi)
    {
      for (Uint32 i = 0; i < t; i++)
        {
          sprite_projectile *projectile = *(projectiles++);
          if (!projectile->on_paddle)
            {
              return;
            }
        }
    }

  /* other fires */
  else
    {
      for (Uint32 i = 0; i < t; i++)
        {
          sprite_projectile *projectile = *(projectiles++);
          if (projectile->is_enabled)
            {
              return;
            }
        }
    }
  countTempo = 0;
  /** fire is requested */
  gun_paddle->fire_state = sprite_paddle::FIRE;
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::PADDLE_FIRE);
#endif
}

/**
 * New fire start
 */
void
controller_projectiles::fire ()
{
  if (gun_paddle->fire_state != sprite_paddle::NOT_OWN_GUN)
    {
      Sint32 i = gun_paddle->length;
      paddle_length = i;
      /* smallest paddle is of 16/32 pixels width */
      i -= gun_paddle->width_mini;
      /* size of paddle step by 8/16 pixels */
      i >>= gun_paddle->shift_width;
      switch (i)
        {
        case 0:
          init_type1 ();
          break;
        case 1:
          init_type2 ();
          break;
        case 2:
          init_type3 ();
          break;
        case 3:
          init_type4 ();
          break;
        case 4:
          init_type5 ();
          break;
        case 5:
          init_type6 ();
          break;
        case 6:
          init_type7 ();
          break;
        }
    }
}

/**
 * Fire 1: 16/32 pixels paddle's whidth
 */
void
controller_projectiles::init_type1 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  if (paddle->is_vertical)
    {
      y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  else
    {
      x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  sprite_projectile *projectile = sprites_list[0];
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
}

/**
 * Fire 2: 24/48 pixels paddle's whidth
 */
void
controller_projectiles::init_type2 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  if (paddle->is_vertical)
    {
      y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  else
    {
      x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  sprite_projectile **projectiles = sprites_list;
  sprite_projectile *projectile;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
}

/**
 * Fire 3: 24/48 pixels paddle's whidth
 */
void
controller_projectiles::init_type3 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  if (paddle->is_vertical)
    {
      y += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  else
    {
      x += (paddle_length / 2) - (SIZE_OF_PROJECTILE / 2);
    }
  sprite_projectile **projectiles = sprites_list;
  sprite_projectile *projectile;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  projectile = *projectiles;
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
}

/**
 * Fire 4: 40/80 pixels paddle's whidth
 */
void
controller_projectiles::init_type4 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  sprite_projectile **projectiles = sprites_list;
  sprite_projectile *projectile;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  if (paddle->is_vertical)
    {
      y += 18 * resolution;
    }
  else
    {
      x += 18 * resolution;
    }
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  if (paddle->is_vertical)
    {
      y = paddle->y_coord + paddle->length - 4;
    }
  else
    {
      x = paddle->x_coord + paddle->length - 4;
    }
  projectile = *projectiles;
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
}

/** 
 * Fire 5: 48/96 pixels paddle's whidth
 */
void
controller_projectiles::init_type5 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  sprite_projectile **projectiles = sprites_list;
  sprite_projectile *projectile;
  projectile = *(projectiles++);
  projectile->is_enabled = true;
  projectile->x_coord = x;
  projectile->y_coord = y;
  Sint32 quarter = paddle_length / 4;
  if (paddle->is_vertical)
    {
      Sint32 i = paddle->projectile_xinc1;
      x += i;
      y += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      x += i;
      y += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      x -= i;
      y += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      x -= i;
      y += quarter - 2 * resolution;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
    }
  else
    {
      Sint32 i = paddle->projectile_yinc1;
      y += i;
      x += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      y += i;
      x += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      y -= i;
      x += quarter;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      y -= i;
      x += quarter - 2 * resolution;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
    }
}

/**
 * Fire 6: 56/112 pixels paddle's whidth
 */
void
controller_projectiles::init_type6 ()
{
  sprite_paddle *paddle = gun_paddle;
  if (paddle->fire_state != sprite_paddle::FIRE)
    {
      return;
    }
  paddle->fire_state = sprite_paddle::OWN_GUN;
  sprite_projectile **projectiles = sprites_list;
  Sint32 x = paddle->x_coord;
  Sint32 y = paddle->y_coord;
  Sint32 offst = 22 * resolution;

  sprite_projectile *projectile;
  if (paddle->is_vertical)
    {
      Sint32 a = x + paddle->projectile_xoffset;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = a;
      projectile->y_coord = y;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      y += offst;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      y += offst;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = a;
      projectile->y_coord = y;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
    }
  else
    {
      Sint32 o = y + paddle->projectile_yoffset;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = o;
      x += offst;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
      x += offst;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = o;
      projectile = *(projectiles++);
      projectile->is_enabled = true;
      projectile->x_coord = x;
      projectile->y_coord = y;
    }
}

/**
 * Fire 7: 64/128 pixels paddle's width
 */
void
controller_projectiles::init_type7 ()
{
  sprite_paddle *paddle = gun_paddle;

  /* paddle is firing? */
  if (paddle->fire_state == sprite_paddle::FIRE)
    {
      paddle->fire_state = sprite_paddle::OWN_GUN;
      sprite_projectile **projectiles = sprites_list;
      Sint32 x = paddle->x_coord + paddle->projectile_xcenter;
      Sint32 y = paddle->y_coord + paddle->projectile_ycenter;
      Sint32 j = 0;
      for (Sint32 i = 0; i < 7; i++, j += 8)
        {
          sprite_projectile *projectile = *(projectiles++);
          projectile->on_paddle = false;
          projectile->fire_Xscie = x;
          projectile->fire_Yscie = y;
          projectile->indexSinus = j;
        }
    }

  /* fire on the paddle */
  else
    {
      sprite_projectile **projectiles = sprites_list;
      paddle->fire_state = sprite_paddle::OWN_GUN;
      for (Sint32 i = 0; i < 7; i++)
        {
          sprite_projectile *projectile = *(projectiles++);
          if (projectile->is_enabled)
            {
              return;
            }
        }
      projectiles = sprites_list;
      Sint32 x = paddle->x_coord + paddle->projectile_xcenter;
      Sint32 y = paddle->y_coord + paddle->projectile_ycenter;
      Sint32 j = 0;
      for (Sint32 i = 0; i < 7; i++, j += 8)
        {
          sprite_projectile *projectile = *(projectiles++);
          projectile->is_enabled = true;
          projectile->on_paddle = true;
          projectile->fire_Xscie = x;
          projectile->fire_Yscie = y;
          projectile->indexSinus = j;
        }

    }
}

/**
 * Move paddle's projectiles
 */
void
controller_projectiles::move ()
{
  Sint32 i = gun_paddle->length;
  paddle_length = i;
  /* smallest paddle width is of 16/32 pixels */
  i -= gun_paddle->width_mini;
  /* size of paddle step by 8/16 pixels */
  i >>= gun_paddle->shift_width;
  switch (i)
    {
    case 0:
      move_type1 ();
      break;
    case 1:
      move_type2 ();
      break;
    case 2:
      move_type3 ();
      break;
    case 3:
      move_type4 ();
      break;
    case 4:
      move_type5 ();
      break;
    case 5:
      move_type6 ();
      break;
    case 6:
      move_type7 ();
      break;
    }
}

/**
 * Move linear projectile (paddle's size: 16/32 pixels)
 */
void
controller_projectiles::move_type1 ()
{
  sprite_projectile *projectile = sprites_list[0];
  sprite_paddle *paddle = gun_paddle;
  projectile->x_coord += paddle->projectile_xinc0;
  projectile->y_coord += paddle->projectile_yinc0;
}

/**
 * Move "fishtail fire" (paddle's size: 24/48 pixels)
 */
void
controller_projectiles::move_type2 ()
{
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;
  Sint32 a = countTempo;
  Sint32 b, c, d;
  a++;
  if (a > 20)
    {
      a = 0;
    }
  countTempo = a;
  if (a > 10)
    {
      a = paddle->projectile_xinc1;
      b = paddle->projectile_yinc1;
      c = paddle->projectile_xinc2;
      d = paddle->projectile_yinc2;
    }
  else
    {
      a = paddle->projectile_xinc2;
      b = paddle->projectile_yinc2;
      c = paddle->projectile_xinc1;
      d = paddle->projectile_yinc1;
    }
  sprite_projectile *projectile;
  projectile = *(projectiles++);
  projectile->x_coord += a;
  projectile->y_coord += b;
  projectile = *projectiles;
  projectile->x_coord += c;
  projectile->y_coord += d;
}

/**
 * Move "triangle fire" (paddle's size: 32/64 pixels)
 */
void
controller_projectiles::move_type3 ()
{
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;
  Sint32 i, j;
  sprite_projectile *projectile;
  /* shot leaves to the left */
  projectile = *(projectiles++);
  i = paddle->projectile_xinc1;
  j = paddle->projectile_yinc1;
  projectile->x_coord += i;
  projectile->y_coord += j;
  /* shot leaves any right */
  projectile = *(projectiles++);
  i = paddle->projectile_xinc0;
  j = paddle->projectile_yinc0;
  projectile->x_coord += i;
  projectile->y_coord += j;
  /* shot leaves to the right */
  projectile = *projectiles;
  i = paddle->projectile_xinc2;
  j = paddle->projectile_yinc2;
  projectile->x_coord += i;
  projectile->y_coord += j;
}

/**
 * Move "fishtail fire" + "linear fire" (paddle of 40/80 pixels width)
 */
void
controller_projectiles::move_type4 ()
{
  Sint32 i, j;
  i = countTempo;
  i++;
  if (i > 20)
    {
      i = 0;
    }
  countTempo = i;
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;
  sprite_projectile *projectile;
  /* [1] linar projectile */
  projectile = *(projectiles++);
  j = paddle->projectile_xinc0;
  projectile->x_coord += j;
  j = paddle->projectile_yinc0;
  projectile->y_coord += j;

  if (i > 10)
    {
      /* [2] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc1;
      j = paddle->projectile_yinc1;
      projectile->x_coord += i;
      projectile->y_coord += j;
      /* [3] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc2;
      j = paddle->projectile_yinc2;
      projectile->x_coord += i;
      projectile->y_coord += j;
    }
  else
    {
      /* [2] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc2;
      j = paddle->projectile_yinc2;
      projectile->x_coord += i;
      projectile->y_coord += j;
      /* [3] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc1;
      j = paddle->projectile_yinc1;
      projectile->x_coord += i;
      projectile->y_coord += j;
    }
  /* [1] linar projectile */
  projectile = *projectiles; 
  i = paddle->projectile_xinc0;
  j = paddle->projectile_yinc0;
  projectile->x_coord += i;
  projectile->y_coord += j;
}

/**
 * Move 5 linears projectiles (paddle of 48/96 pixels width)
 */

void
controller_projectiles::move_type5 ()
{
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;
  Sint32 x = paddle->projectile_xinc0;
  Sint32 y = paddle->projectile_yinc0;
  for (Sint32 i = 0; i < 5; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      projectile->x_coord += x;
      projectile->y_coord += y;
    }
}

/**
 * Move 4 linears projectiles + 2 fishtails projectiles
 * (paddle of 56/112 pixels)
 */
void
controller_projectiles::move_type6 ()
{
  Sint32 a, i, j;

  a = countTempo;
  a++;
  if (a > 20)
    {
      a = 0;
    }
  countTempo = a;
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;

  i = paddle->projectile_xinc0;
  j = paddle->projectile_yinc0;
  sprite_projectile *projectile = *(projectiles++);

  projectile->x_coord += i;
  projectile->y_coord += j;
  projectile = *(projectiles++);
  projectile->x_coord += i;
  projectile->y_coord += j;
  if (a > 10)
    {
      /* [3] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc1;
      j = paddle->projectile_yinc1;
      projectile->x_coord += i;
      projectile->y_coord += j;
      /* [4] fishtail prjectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc2;
      j = paddle->projectile_yinc2;
      projectile->x_coord += i;
      projectile->y_coord += j;
    }
  else
    {
      /* [3] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc2;
      j = paddle->projectile_yinc2;
      projectile->x_coord += i;
      projectile->y_coord += j;
      /* [4] fishtail projectile */
      projectile = *(projectiles++);
      i = paddle->projectile_xinc1;
      j = paddle->projectile_yinc1;
      projectile->x_coord += i;
      projectile->y_coord += j;
    }
  i = paddle->projectile_xinc0;
  j = paddle->projectile_yinc0;
  projectile = *(projectiles++);
  projectile->x_coord += i;
  projectile->y_coord += j;
  projectile = *projectiles;
  projectile->x_coord += i;
  projectile->y_coord += j;
}

/*
 * Move 7 circular projectiles (paddle of 56/128 pixels)
 */
void
controller_projectiles::move_type7 ()
{
  sprite_projectile **projectiles = sprites_list;
  sprite_paddle *paddle = gun_paddle;
  for (Sint32 i = 0; i < 7; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      if (!projectile->is_enabled)
        {
          continue;
        }
      Sint32 j = projectile->indexSinus;
      j += 2;
      const Sint16 *table = circular_sin + j;
      if (*table == 99)
        {
          j = 0;
          table = circular_sin;
        }
      projectile->indexSinus = j;
      if (projectile->on_paddle)
        {
          Sint32 k = *(table++);
          k += paddle->x_coord + paddle->projectile_xcenter;
          projectile->x_coord = k;
          k = *table;
          k += paddle->y_coord + paddle->projectile_ycenter;
          projectile->y_coord = k;
        }
      else
        {
          projectile->fire_Xscie += paddle->projectile_xinc0;
          projectile->fire_Yscie += paddle->projectile_yinc0;
          Sint32 k = *(table++);
          k += projectile->fire_Xscie;
          projectile->x_coord = k;
          k = *table;
          k += projectile->fire_Yscie;
          projectile->y_coord = k;
        }
    }
}

/**
 * Enable fire power 1
 */
void
controller_projectiles::set_fire_1 ()
{
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      projectile->set_power1 ();
    }
}

/**
 * Enable fire power 2
 */
void
controller_projectiles::set_fire_2 ()
{
  sprite_projectile **projectiles = sprites_list;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_projectile *projectile = *(projectiles++);
      projectile->set_power2 ();
    }
}

/**
 * Sinus used for he fire 7 in 640 pixels resolution
 */
const
  Sint16
  controller_projectiles::circular_sin_640[] =
  { 15 * 2, -3 * 2, 13 * 2, -7 * 2, 11 * 2, -10 * 2, 9 * 2, -12 * 2, 7 * 2,
  13 * 2, 3 * 2, -15 * 2, 0 * 2, -15 * 2,
  -3 * 2, -15 * 2, -6 * 2, -14 * 2, -9 * 2, -12 * 2, -11 * 2, -10 * 2,
  -13 * 2, -07 * 2, -14 * 2, -04 * 2, -15 * 2, -01 * 2,
  -15 * 2, 3 * 2, -13 * 2, 7 * 2, -11 * 2, 10 * 2, -9 * 2, 12 * 2, -7 * 2,
  13 * 2, -3 * 2, 15 * 2, 0 * 2, 15 * 2,
  3 * 2, 15 * 2, 6 * 2, 14 * 2, 9 * 2, 12 * 2, 11 * 2, 10 * 2, 13 * 2, 7 * 2,
  14 * 2, 4 * 2, 15 * 2, 1 * 2,
  99, 99, 99, 99
};

/**
 * Sinus used for he fire 7 in 320 pixels resolution
 */
const
  Sint16
  controller_projectiles::circular_sin_320[] =
  { 15, -3, 13, -7, 11, -10, 9, -12, 7, -13, 3, -15, 0, -15,
  -3, -15, -6, -14, -9, -12, -11, -10, -13, -7, -14, -4, -15, -1,
  -15, 3, -13, 7, -11, 10, -9, 12, -7, 13, -3, 15, 0, 15,
  3, 15, 6, 14, 9, 12, 11, 10, 13, 7, 14, 4, 15, 1,
  99, 99, 99, 99
};
