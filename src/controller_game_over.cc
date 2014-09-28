/**
 * @file controller_game_over.cc 
 * @brief Game Over controller 
 * @created 2002-12-14
 * @date 2007-10-21
 * @copyright 1991-2014 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2014 TLK Games all rights reserved
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
#include "../include/controller_game_over.h"
#include "../include/handler_audio.h"
#include "../include/handler_resources.h"
#include "../include/handler_high_score.h"

/**
 * Create the Game Over controller
 */
controller_game_over::controller_game_over ()
{
  littleInit ();
  /* there are 8 letters */
  max_of_sprites = 8;
  sprites_have_shades = true;
  max_of_sprites = 8;
  sprite_type_id = sprite_object::GAME_OVER_LETTERS;
  move_phase = 0;
  sprite_high_score = (sprite_display_scores *) NULL;
}

/**
 * Release the Game Over controller
 */
controller_game_over::~controller_game_over ()
{
  if (NULL != sprite_high_score)
    {
      delete sprite_high_score;
      sprite_high_score = NULL;
    }
  release_sprites_list ();
}

/**
 * Return pointer to the high score object
 * @return A sprite_display_scores object
 */
sprite_display_scores *
controller_game_over::get_sprite_high_score ()
{
  return sprite_high_score;
}

/**
 * Perform some initializations
 */
void
controller_game_over::first_init (Uint32 x_offset)
{
  char_x_offset = x_offset;

  /* enable_game_over score table */
  sprite_high_score = new sprite_display_scores ();
  sprite_high_score->first_init (char_x_offset);
}

/**
 * Enable the Game Over
 * @param is_victory If true game is finished, play game over with
 *                   congratulations
 */
void
controller_game_over::enable_game_over (bool is_victory)
{
  Sint32 x = 100 * resolution;
  Sint32 y = 200 * resolution;
  const Sint32 *p = initial_coordinates;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      sprite->enable ();
      sprite->set_coordinates (x, y);
      sprite->set_image (i);
      Sint32 x2 = *(p++);
      Sint32 y2 = *(p++);
      sprite->x_maximum = x2;
      sprite->y_maximum = y2;
    }
  move_phase = 1;
  next_phase_counter = 50 * 10;
#ifndef SOUNDISOFF
  bool is_ranked = high_score->is_player_ranked ();
  if (is_victory)
    {
      audio->play_music (handler_audio::MUSICCONGR);
    }
  else
    {
      if (is_ranked)
        {
          audio->play_music (handler_audio::MUSICSCORE);
        }
      else
        {
          audio->play_music (handler_audio::MUSICGOVER);
        }
    }
#else
  high_score->is_player_ranked ();
#endif
  sprite_high_score->copyToText ();
}

/**
 * Move the "GAME OVER" chars and draw high score 
 * @param is_victory If true game is finished, play game over with
 *                   congratulations
 */
void
controller_game_over::run (bool is_victory)
{
  switch (move_phase)
    {
    case 0:
      enable_game_over (is_victory);
      break;

    case 1:
      displacement_01 ();
      break;

    case 2:
      displacement_02 ();
      break;

    case 3:
      displacement_03 ();
      break;

    case 4:
      displacement_04 ();
      break;
    }
  sprite_high_score->displayTxt ();
  sprite_high_score->draw_copy_from_bitmap ();
}

/**
 * Movement 1
 */
void
controller_game_over::displacement_01 ()
{
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (++sprite->x_maximum >= maxi)
        {
          sprite->x_maximum = 0;
        }
      if (++sprite->y_maximum >= maxi)
        {
          sprite->y_maximum = 0;
        }
      Sint32 x = char_x_offset + decal
        + sinus[sprite->x_maximum] * resolution;
      Sint32 y = decal + sinus[sprite->y_maximum] * resolution;
      sprite->set_coordinates (x, y);
      random_counter += y;
    }
  if (--next_phase_counter <= 0)
    {
      move_phase = 2;
    }
}

/**
 * Movement 2
 */
void
controller_game_over::displacement_02 ()
{
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (++sprite->y_maximum >= maxi)
        {
          sprite->y_maximum = 0;
        }
      Sint32 y = decal + sinus[sprite->y_maximum] * resolution;
      sprite->set_y_coord (y);
      random_counter += y;
    }

  /* move the letters "G", "A", "M", and "E" */
  Sint32 f = 0;
  Sint32 v = 32 * resolution + char_x_offset;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (sprite->x_coord > v)
        {
          sprite->x_coord -= resolution;
          f = 1;
        }
    }

  /* move the letters "O", "V", "E", and "R" */
  v = 192 * resolution + char_x_offset;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (sprite->x_coord < v)
        {
          sprite->x_coord += resolution;
          f = 1;
        }
    }

  if (f <= 0)
    {
      move_phase = 3;
    }
}

/**
 * Movement 2
 */
void
controller_game_over::displacement_03 ()
{
  /* move the letters "G", "A", "M" and "E" */
  Sint32 f = 0;
  Sint32 maxi = SIZETSINUS;
  Sint32 decal = 32 * resolution;
  const Sint32 *sinus = sinus_over;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (++sprite->y_maximum >= maxi)
        {
          sprite->y_maximum = 0;
        }
      if (sprite->y_coord <= decal)
        {
          sprite->y_coord = decal;
          f++;
        }
      else
        {
          Sint32 y = decal + sinus[sprite->y_maximum] * resolution;
          sprite->set_y_coord (y);
        }
      random_counter += sprite->y_maximum;
    }

  /* move the letters "O", "V", "E" and "R" */
  Sint32 v = 191 * resolution;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (++sprite->y_maximum >= maxi)
        sprite->y_maximum = 0;
      if (sprite->y_coord >= v)
        {
          sprite->y_coord = v;
          f++;
        }
      else
        {
          Sint32 y = decal + sinus[sprite->y_maximum] * resolution;
          sprite->set_y_coord (y);
        }
      random_counter += sprite->y_maximum;
    }
  if (f == 8)
    {
      move_phase = 4;
    }
}

/**
 * movement 4 : the letters are moving towards their final abscissas
 */
void
controller_game_over::displacement_04 ()
{
  /* move the letters "G", "A", "M", and "E" */
  Sint32 offst = 2 * resolution;
  Sint32 final = 35 * resolution + char_x_offset;
  for (Sint32 i = 0; i < 4; i++)
    {
      sprite_object *sprite = sprites_list[i];
      Sint32 o = final - sprite->x_coord;
      if (o > offst)
        {
          o = offst;
        }
      else
        {
          if (o < (-offst))
            {
              o = -offst;
            }
        }
      sprite->x_coord += o;
      final = final + 54 * resolution;
      random_counter += sprite->x_coord;
    }

  /* move the letters "O", "V", "E" and "R" */
  final = 32 * resolution + char_x_offset;
  for (Uint32 i = 4; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      Sint32 o = final - sprite->x_coord;
      if (o > offst)
        {
          o = offst;
        }
      else
        {
          if (o < (-offst))
            {
              o = -offst;
            }
        }
      sprite->x_coord += o;
      final = final + 54 * resolution;
      random_counter += sprite->x_coord;
    }
}

const Sint32 controller_game_over::sinus_over[SIZETSINUS] =
  {
    159, 159, 159, 159, 158, 158, 158, 157, 156, 156,
    155, 154, 153, 152, 151, 150, 149, 148, 147, 145,
    144, 142, 141, 139, 137, 135, 134, 132, 130, 128,
    126, 124, 122, 120, 117, 115, 113, 111, 108, 106,
    104, 101, 99, 96, 94, 91, 89, 86, 84, 81,
    79, 76, 74, 71, 69, 66, 64, 61, 59, 57,
    54, 52, 50, 47, 45, 43, 40, 38, 36, 34,
    32, 30, 28, 26, 24, 23, 21, 19, 18, 16,
    15, 13, 12, 11, 9, 8, 7, 6, 5, 4,
    4, 3, 2, 2, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 2, 2, 3, 4,
    4, 5, 6, 7, 8, 9, 11, 12, 13, 15,
    16, 18, 19, 21, 23, 24, 26, 28, 30, 32,
    34, 36, 38, 40, 43, 45, 47, 50, 52, 54,
    57, 59, 61, 64, 66, 69, 71, 74, 76, 79,
    81, 84, 86, 89, 91, 94, 96, 99, 101, 104,
    106, 108, 111, 113, 115, 117, 120, 122, 124, 126,
    128, 130, 132, 134, 135, 137, 139, 141, 142, 144,
    145, 147, 148, 149, 150, 151, 152, 153, 154, 155,
    156, 156, 157, 158, 158, 158, 159, 159, 159, 159,
  };

const Sint32 controller_game_over::initial_coordinates[] =
  {
    0, 175,
    25, 150,
    50, 125,
    75, 100,
    100, 75,
    125, 50,
    150, 25,
    175, 0
  };
