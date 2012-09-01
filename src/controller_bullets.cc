/** 
 * @file controller_bullets.cc 
 * @brief Bullets controller 
 * @date 2007-10-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.12 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_bullets.cc,v 1.12 2007-10-29 13:18:53 gurumeditation Exp $
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
#include "../include/controller_bullets.h"
#include "../include/handler_resources.h"

/**
 * Create the bullets controller
 */
controller_bullets::controller_bullets ()
{
  littleInit ();
  max_of_sprites = 48;
  sprites_have_shades = false;
  sprite_type_id = sprite_object::BULLET;
}

/**
 * Release the bullets controller
 */
controller_bullets::~controller_bullets ()
{
  release_sprites_list ();
}

/**
 * Move all bullets 
 */
void
controller_bullets::move ()
{
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_bullet *bullet_sprite = sprites_list[i];
      bullet_sprite->move ();
    }
}

/**
 * The animations of the bullets are played in loop-mode
 */
void
controller_bullets::play_animation_loop ()
{
  sprite_bullet *bullet_sprite = sprites_list[0];
  bullet_sprite->play_animation_loop ();
  Uint32 index = bullet_sprite->get_frame_index ();
  Uint32 cycle = index & 0X1;
  if (cycle == 0)
    {
      cycle = sprite_object::DRAW_WITH_TABLES;
    }
  else
    {
      cycle = sprite_object::DRAW_COLOR_CYCLING_MASK;
    }
  for (Uint32 i = 1; i < max_of_sprites; i++)
    {
      bullet_sprite = sprites_list[i];
      bullet_sprite->set_image (index);
      bullet_sprite->draw_method = cycle;
    }
}

/**
 * Check collisions between bullets and the paddle
 */ 
void
controller_bullets::check_paddle_collisions ()
{
  controller_paddles* paddles = controller_paddles::get_instance ();
  sprite_paddle *paddle =
    paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  controller_explosions* explosions = controller_explosions::get_instance ();

  Sint32 y1_paddle = paddle->get_y_coord ();
  Sint32 x1_paddle = paddle->get_x_coord ();
  Sint32 x2_paddle = x1_paddle + paddle->get_length ();
  Sint32 y2_paddle = y1_paddle + paddle->get_sprite_height ();
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_bullet *bullet = sprites_list[i];
      if (!bullet->is_enabled)
        {
          continue;
        }
      Sint32 y_bullet = bullet->y_coord;
      if (y_bullet >= y2_paddle)
        {
          continue;
        }
      Sint32 x_bullet = bullet->x_coord;
      if (x_bullet >= x2_paddle)
        {
          continue;
        }
      y_bullet += bullet->sprite_height;
      if (y_bullet <= y1_paddle)
        {
          continue;
        }
      x_bullet += bullet->sprite_width;
      if (x_bullet <= x1_paddle)
        {
          continue;
        }
      bullet->is_enabled = false;
      if (paddle->is_invincible ())
        {
          continue;
        }
      paddle->set_invincibility (100);
      current_player->remove_life (1);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::PADDLE_EXPLOSION);
      audio->play_sound (handler_audio::LOST_LIFE);
#endif
      explosions->add
        (x1_paddle + paddle->get_length () / 2,
         y1_paddle + paddle->get_sprite_height () / 2);
    }
}

/**
 * Return the last bullet object of the list
 */ 
sprite_bullet *
controller_bullets::get_last_bullet ()
{
  return sprites_list[max_of_sprites - 1];
}

/**
 * Fire some bullets
 */
void
controller_bullets::fire (Uint32 fire_id, sprite_guardian * guardian)
{
  switch (fire_id)
    {
    case 0:
      init_fire_01 (guardian);
      break;
    case 4:
      init_fire_02 (guardian);
      break;
    case 8:
      init_fire_03 (guardian);
      break;
    case 12:
      init_fire_04 (guardian);
      break;
    case 16:
      init_fire_05 (guardian);
      break;
    case 20:
      init_fire_06 (guardian);
      break;
    case 24:
      init_fire_07 (guardian);
      break;
    case 28:
      init_fire_08 (guardian);
      break;
    case 32:
      init_fire_09 (guardian);
      break;
    case 36:
      init_fire_10 (guardian);
      break;
    case 40:
      init_fire_11 (guardian);
      break;
    }
}

/**
 * Initialize the fire 1 composed of 12 bullets
 * @param guardian a guardian sprite
 */
void
controller_bullets::init_fire_01 (sprite_guardian * guardian)
{
  Uint32 index = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord - (16 * resolution);
  Sint32 gardy = guardian->y_coord;
  Uint32 count = 12;
  Sint32 s = 0;                 //pointeur sur la table sinus
  do
    {
      sprite_bullet *bullet = sprites_list[index];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 1;
          bullet->x_coord = tir01_posi[s] * resolution + gardx;
          bullet->tablesinus = tir01_posi;
          bullet->flagDepla1 = s;        //pointeur table sinus x
          bullet->flagDepla2 = gardx;    //sauve position x
          bullet->y_coord = gardy;       // position y du tir
          gardy = gardy + 5 * resolution;
          s = s + 5;
          if (count-- == 7)
            {
              gardy = guardian->y_coord;
            }
        }
    }
  while (count > 0 && --index >= 0);
}

/**
 * Initialize the "buzz saw" fire composed of 10 bullets
 * @param guardian a guardian sprite
 */
void
controller_bullets::init_fire_02 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                //10 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          if (n > 3)
            {
              bullet->displacement = 2;
              bullet->flagDepla3 = s;
              s = s + 2;
              bullet->flagDepla1 = gardx;
              bullet->flagDepla2 = gardy;
              bullet->tablesinus = fire_sinus;
              if (--n == 3)
                {
                  gardx = gardx - 10 * resolution;
                }
            }
          else
            {
              bullet->displacement = 3;
              bullet->x_coord = gardx;
              bullet->y_coord = gardy;
              gardx = gardx + 10 * resolution;
              n--;
            }
        }
    }
  while (n > 0 && --t >= 0);

}

/**
 * Initialize the "xevious" fire composed of 4 bullets
 * @param guardian A guardian sprite
 */
void
controller_bullets::init_fire_03 (sprite_guardian * guardian)
{
  controller_paddles* paddles = controller_paddles::get_instance ();
  sprite_paddle *paddle =
    paddles->get_paddle (controller_paddles::BOTTOM_PADDLE);
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  /* fire composed of 5 objects */
  Sint32 n = 5;
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->paddle_target = paddle;
          bullet->is_enabled = true;
          if (n == 1)
            {
              bullet->displacement = 7;
              bullet->x_coord = gardx;
              bullet->y_coord = gardy;
            }
          else
            {
              bullet->displacement = 6;
              bullet->flagDepla1 = gardx;
              bullet->flagDepla2 = gardy;
              switch (n)
                {
                case 5:
                  bullet->tablesinus = fire_sinus;
                  bullet->flagDepla3 = 0;
                  break;
                case 4:
                  bullet->tablesinus = fire_sinus;
                  bullet->flagDepla3 = 28;
                  break;
                case 3:
                  bullet->tablesinus = tir02_posi;
                  bullet->flagDepla3 = 0;
                  break;
                case 2:
                  bullet->tablesinus = tir02_posi;
                  bullet->flagDepla3 = 28;
                  break;
                }
            }
          n--;
        }
    }
  while (n > 0 && --t >= 0);
}

// *-----------------*TIR 04, scie circulaire II, 7 bobs*----------------------*
void
controller_bullets::init_fire_04 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 7;                 //7 objets pour ce tir
  const Sint16 *ptir = tir04_posi;
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 4;
          bullet->flagDepla1 = gardx;
          bullet->flagDepla2 = gardy;
          bullet->flagDepla3 = s;
          bullet->flagDepla4 = 1;
          bullet->flagDepla5 = 10;
          bullet->flagDepla6 = *(ptir++);
          bullet->flagDepla7 = *(ptir++);
          bullet->tablesinus = fire_sinus;
          s = s + 8;
          n--;
        }
    }
  while (n > 0 && --t >= 0);
}

// **--------------------* TIR 05, automatique 9 coups *---------------------**
void
controller_bullets::init_fire_05 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 tempo = 10;
  Sint32 incrx = -5 * resolution;
  Sint32 n = 9;                 //9 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 5;
          bullet->x_coord = gardx;
          bullet->y_coord = gardy;
          bullet->flagDepla1 = tempo;
          tempo += 15;
          bullet->flagDepla2 = incrx;
          incrx += resolution;
          bullet->flagDepla3 = 5 * resolution;
          n--;
        }
    }
  while (n > 0 && --t >= 0);

}

// **---------------------* TIR 06, scie circulaire III *---------------------**
void
controller_bullets::init_fire_06 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 s = 0;
  Sint32 n = 30;                //30 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 8;
          bullet->ptguardian = guardian;
          bullet->flagDepla3 = s;
          bullet->flagDepla4 = 2 * resolution;
          bullet->tablesinus = handler_resources::cosinus360;
          bullet->tablecosin = handler_resources::zesinus360;
          s = s + 12;
          n--;
        }
    }
  while (n > 0 && --t >= 0);

}

// **---------------------* TIR 07, scie circulaire IV *---------------------**
void
controller_bullets::init_fire_07 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                //10 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 9;
          bullet->flagDepla1 = gardx;
          bullet->flagDepla2 = gardy;
          bullet->flagDepla3 = s;
          bullet->flagDepla4 = 1;
          bullet->tablesinus = handler_resources::cosinus360;
          bullet->tablecosin = handler_resources::zesinus360;
          s = s + 36;
          n--;
        }
    }
  while (n > 0 && --t >= 0);

}

// **----------------------* TIR 08, scie circulaire V *----------------------**
void
controller_bullets::init_fire_08 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 10;                //10 objets pour ce tir
  Sint32 tempo = 20;
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 10;
          bullet->flagDepla1 = gardx;
          bullet->flagDepla2 = gardy;
          bullet->flagDepla3 = s;
          s += 72;
          bullet->flagDepla4 = 0;
          bullet->flagDepla5 = tempo;
          bullet->tablesinus = handler_resources::cosinus360;
          bullet->tablecosin = handler_resources::zesinus360;
          n--;
          if (n == 5)
            {
              gardy = gardy + 25 * resolution;
              tempo = 1;
            }
        }
    }
  while (n > 0 && --t >= 0);
}

// **--------------------------* TIR 09, triangle *----------------------------*
void
controller_bullets::init_fire_09 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 s = 0;
  Sint32 n = 9;                 //10 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 11;
          bullet->flagDepla1 = gardx;
          bullet->flagDepla2 = gardy;
          bullet->tablesinus = handler_resources::cosinus360;
          bullet->tablecosin = handler_resources::zesinus360;
          bullet->flagDepla3 = s;
          s += 120;
          n--;
          if (n == 6)
            {
              s = 0;
              gardx += (16 * resolution);
            }
          else
            {
              if (n == 3)
                {
                  s = 0;
                  gardx -= (8 * resolution);
                }
            }
        }
    }
  while (n > 0 && --t >= 0);
}


// **---------------------** tir 10, epee de DAMOCLES **----------------------**
void
controller_bullets::init_fire_10 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord - (17 * resolution);
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent - (10 * resolution);
  const Sint16 *ptir = tir10_posi;
  Sint32 n = 6;                 //6 objets pour ce tir
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 3;
          bullet->x_coord = gardx + (*(ptir++) * bullet->sprite_width);
          bullet->y_coord = gardy + (*(ptir++) * bullet->sprite_height);
          n--;
        }
    }
  while (n > 0 && --t >= 0);
}

// *=-----------------------=* TIR 11, triangle II *=-------------------------=*
void
controller_bullets::init_fire_11 (sprite_guardian * guardian)
{
  Uint32 t = max_of_sprites - 1;
  Sint32 gardx = guardian->x_coord + guardian->canon_xcoord;
  Sint32 gardy = guardian->y_coord + guardian->gard_ycent;
  Sint32 n = 15;                //15 objets pour ce tir
  Sint32 inc_x = 0;
  Sint32 inc_y = 5 * resolution;
  Sint32 s = 0;
  do
    {
      sprite_bullet *bullet = sprites_list[t];
      if (!bullet->is_enabled)
        {
          bullet->is_enabled = true;
          bullet->displacement = 12;
          bullet->flagDepla1 = gardx;
          bullet->flagDepla2 = gardy;
          bullet->flagDepla3 = s;
          bullet->flagDepla4 = inc_x;
          bullet->flagDepla5 = inc_y;
          bullet->tablesinus = handler_resources::cosinus360;
          bullet->tablecosin = handler_resources::zesinus360;
          s += 72;
          n--;
          if (n == 10)
            {
              s = 0;
              inc_x = -1 * resolution;
              inc_y = 4 * resolution;
            }
          else
            {
              if (n == 5)
                {
                  s = 0;
                  inc_x = 1 * resolution;
                  inc_y = 4 * resolution;
                }
            }
        }
    }
  while (n > 0 && --t >= 0);
}


// tir gardien : table sinus ---------------------------------------------------
const Sint16
  controller_bullets::tir01_posi[62] =
  { 32, 32, 32, 31, 31, 30, 29, 28, 27, 25,
  24, 22, 21, 19, 17, 16, 14, 12, 11, 9,
  8, 6, 5, 4, 3, 2, 1, 1, 0, 0,
  0, 0, 1, 1, 2, 3, 4, 5, 6, 8,
  9, 11, 12, 14, 16, 17, 19, 21, 22, 24,
  25, 27, 28, 29, 30, 31, 31, 32, 32, 32,
  99, 99
};

// tir gardien : table de points d'un cercle de 8 pixels de rayon --------------
const
  Sint16
  controller_bullets::tir02_posi[60] =
  { 8, 0, 8, -1, 6, -3, 4, -4, 3, -6, 1, -8, 0, -8,
  0, -8, -1, -8, -3, -6, -4, -4, -6, -3, -8, -1, -8, 0,
  -8, 0, -8, 1, -6, 3, -4, 4, -3, 6, -1, 8, 0, 8,
  0, 8, 1, 8, 3, 6, 4, 4, 6, 3, 8, 1, 8, 0,
  99, 99, 99, 99
};

// tir gardien : table des directions de la roue accelerante -------------------
const
  Sint16
controller_bullets::tir04_posi[14] =
  { -1, 5, 3, 5, 1, 6, -2, 6, 0, 6, -1, 4, 2, 6
};

//  tir gardien : position des points, epee d'DAMOCLES -------------------------
const
  Sint16
controller_bullets::tir10_posi[12] = { 1, 0, 0, 1, 1, 1, 2, 1, 1, 2, 1, 3
};

// anciennement appelle sinus -------------------------------------------------
const
  Sint16
  controller_bullets::fire_sinus[60] =
  { 15, -3, 13, -7, 11, -10, 9, -12, 7, -13, 3, -15, 0, -15,
  -3, -15, -6, -14, -9, -12, -11, -10, -13, -7, -14, -4, -15, -1,
  -15, 3, -13, 7, -11, 10, -9, 12, -7, 13, -3, 15, 0, 15,
  3, 15, 6, 14, 9, 12, 11, 10, 13, 7, 14, 4, 15, 1,
  99, 99, 99, 99
};
