/** 
 * @file sprite_guardian.cc 
 * @brief The guardian sprite 
 * @created 2003-01-09 
 * @date 2012-09-02 
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
#include "../include/sprite_guardian.h"

/**
 * Create a guardian sprite
 */
sprite_guardian::sprite_guardian ()
{
  energy_level = 0;
  canon_xcoord = 0;
  gigablitz_delay_counter = 0;
  gigablitz_frequency = 0;
  speed_of_guardian = 0;
  gard_colx1 = 0;
  gard_coly1 = 0;
  gard_colx2 = 0;
  gard_coly2 = 0;
  gard_ycent = 0;
  shot_delay_counter = 0;
  shoot_frequency = 0;
  gard_lissa = 0;
  gardptfire = 0;
  recently_touched = 0;
  is_blinking = false;
  hasardval2 = random_counter;
  if (hasardval2 < 0)
    {
      hasardval2 = -hasardval2;
    }
}

/**
 * Release a guardian sprite
 */
sprite_guardian::~sprite_guardian ()
{
}

/** 
 * perform some initializations
 */
void
sprite_guardian::initialize (gardlevel * guard, unsigned char *ptLis)
{
  energy_level = guard->para_power * difficulty_level;  //strength 
  canon_xcoord = (guard->para_xcent * resolution) - (11 * resolution / 2);        //middle x from where weapons starts 
  gigablitz_delay_counter = guard->para_waitf / difficulty_level;
  gigablitz_frequency = gigablitz_delay_counter / difficulty_level;
  speed_of_guardian = guard->para_speed;
  if (difficulty_level == 4)
    {
      speed_of_guardian *= 2;
    }
  gard_colx1 = guard->para_colx1 * resolution;
  gard_coly1 = guard->para_coly1 * resolution;
  gard_colx2 = guard->para_colx2 * resolution;
  gard_coly2 = guard->para_coly2 * resolution;
  gard_ycent = (guard->para_ycent * resolution) - (11 * resolution / 2);
  shot_delay_counter = guard->para_wait2 / difficulty_level;
  shoot_frequency = guard->para_wait2 / difficulty_level;
  gard_tfire = &guard->para_tfire[0];
  ptr_lissa1 = ptLis;
  ptr_lissa2 = ptLis;
  x_minimum = 6 * resolution;
  y_minimum = 2 * resolution;
  x_maximum = screen_width - sprite_width;
  y_maximum = screen_height - sprite_height;
  y_maximum = (232 - 8 - 1 - 16) * resolution;
  if (sprite_width == resolution * 32)
    {
      explode_frequency = 7;
    }
  else
    {
      explode_frequency = 3;
    }
}

/**
 * Move the guardian, fire the bullets and gigablitz
 * @param voffset vertical offset
 */
void
sprite_guardian::run (Uint32 /* voffset */)
{
  controller_explosions *explosions = controller_explosions::get_instance ();

  /**
   * guards is alive
   */
  if (energy_level > 0)
    {
      Sint32 x = (ptr_lissa1[0]);
      x = x * (screen_width - sprite_width) / 192;
      Sint32 y = ptr_lissa1[1] * resolution;
      x_coord = x;
      y_coord = y;
      //y_coord = y + voffset;
      ptr_lissa1 = ptr_lissa1 + speed_of_guardian;
      Sint16 *ptest = (Sint16 *) ptr_lissa1;
      if (ptest[0] == -1)
        {
          ptr_lissa1 = ptr_lissa2;
        }
      is_blinking = is_blinking ? false : true;
      if (recently_touched > 0 && is_blinking)
        {
          is_enabled = false;
          recently_touched--;
        }
      else
        {
          is_enabled = true;
        }
      fire_bullets ();
      startBlitz ();
    }
  else
    {
      /* 
       * the guard explodes 
       */
      if (explode_delay_counter > 0)
        {
          explode_delay_counter--;
          if (!(explode_delay_counter & explode_frequency))
            {
              Sint32 xcoord = x_coord;
              Sint32 ycoord = y_coord;
              random_counter += xcoord;
              Sint32 vrand = random_counter;
              if (vrand < 0)
                {
                  vrand = -vrand;
                }
              Sint32 val_1 = (vrand + explode_delay_counter - y_coord) % sprite_width;
              Sint32 val_2 = (hasardval2 - explode_delay_counter + y_coord) % sprite_height;
              xcoord += val_1;
              ycoord += val_2;
              explosions->add (xcoord, ycoord);
              hasardval2 = vrand;
            }
          is_blinking = is_blinking ? false : true;
          if (is_blinking)
            {
              is_enabled = false;
            }
          else
            {
              is_enabled = true;
            }
        }
      /*
       * the guard is dead 
       */
      else
        {
          is_enabled = false;
        }
    }
}

/**
 * Fire new bullets
 */
void
sprite_guardian::fire_bullets ()
{
  if (shot_delay_counter-- > 0)
    {
      return;
    }
  if (y_coord > Sint32(80 * resolution))
    {
      return;
    }
  shot_delay_counter = shoot_frequency;
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::GUARDIAN_FIRE);
#endif
  Sint32 nfire = gard_tfire[gardptfire];
  if (nfire < 0)
    {
      nfire = gard_tfire[0];
      gardptfire = 0;
    }
   /** Index of the next weapons */
   gardptfire++;
   controller_bullets* bullets = controller_bullets::get_instance ();
   bullets->fire (nfire, this);
}

/**
 * Fire a new GigaBlitz
 */
void
sprite_guardian::startBlitz ()
{
  if (gigablitz_delay_counter-- > 0)
  {
    return;
  }
  Uint32 v = random_counter & 7;
  if (sprite_width > 32 * resolution)
  {
    v = table_gga1[v];
  }
  else
  {
    v = table_gga2[v];
  }
  controller_gigablitz* gigablitz = controller_gigablitz::get_instance ();
  if (gigablitz->shoot_guardian (v, x_coord, y_coord, sprite_width))
  {
    gigablitz_delay_counter = gigablitz_frequency;
  } 
}

//-----------------------------------------------------------------------------
// list of differents gigablitz
//-----------------------------------------------------------------------------
// Guard of 64/128 pixels
const Sint32
sprite_guardian::table_gga1[8] = { 0, 1, 2, 3, 4, 5, 6, 5
};

// Guard of 32/64 pixels
const Sint32
sprite_guardian::table_gga2[8] = { 6, 5, 4, 6, 5, 4, 6, 5
};
