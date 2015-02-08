/**
 * @file right_panel_score.cc
 * @brief The right panel score in the bricks levels
 * @date 2015-02-08
 * @copyright 1991-2015 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision$
 */
/*
 * copyright (c) 1991-2015 TLK Games all rights reserved
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
#include "../include/right_panel_score.h"
#include "../include/handler_resources.h"
#include "../include/handler_high_score.h"

right_panel_score *
  right_panel_score::panel_score_singleton = NULL;

/**
 * Create the right panel score
 */
right_panel_score::right_panel_score ()
{
  initial_me ();
  gauge_height = GAUGE_HEIGHT * resolution;
  gigablitz_countdown = gauge_height;
  delay_gigablitz_countdown = 0;
  flip_white = false;
  panel_width = 0;
}

/**
 * Release the right panel score
 */
right_panel_score::~right_panel_score ()
{
  destroy_me ();
  panel_score_singleton = NULL;
}

/**
 * Get the object instance
 * right_panel_score is a singleton
 * @return the right_panel_score object
 */
right_panel_score *
right_panel_score::get_instance ()
{
  if (NULL == panel_score_singleton)
    {
      panel_score_singleton = new right_panel_score ();
    }
  return panel_score_singleton;
}

/**
 * First initialization
 * @param balls
 */
void
right_panel_score::first_init ()
{
  initialize ();
  gauge_pixel = game_screen->get_pixel_data
    (GAUGE_XCOORD * resolution, GAUGE_YCOORD * resolution);
  draw_background ();
}

/**
 * Draw the background of the panel score
 */
void
right_panel_score::draw_background ()
{
  bitmap_data *bmp = new bitmap_data ();
  bmp->load (handler_resources::BITMAP_RIGHT_PANEL);

  /* drawn panel score */
  offscreen_surface *screen;
  if (has_background)
    {
      screen = background_screen;
    }
  else
    {
      screen = game_screen;
    }
  panel_width = bmp->get_width ();
  screen->blit_surface (bmp, 0, 0, 256 * resolution, 0,
                        bmp->get_width (), 240 * resolution);
  delete bmp;
  bmp = (bitmap_data *) NULL;
  draw (screen, AREA_NUM_XCOORD * resolution,
        AREA_NUM_YCOORD * resolution, current_player->area_number, 2);
  draw (screen, LEVEL_NUM_XCOORD * resolution,
        LEVEL_NUM_YCOORD * resolution, current_player->level_number, 2);
  draw (screen, PLAYERNAME_XCOORD * resolution,
        PLAYERNAME_YCOORD * resolution, current_player->player_name);
  draw (screen, BEST_SCORE_XCOORD * resolution,
        BEST_SCORE_YCOORD * resolution, high_score->get_best_score (), 6);
  draw (screen, BEST_PLAYER_XCOORD * resolution,
        BEST_PLAYER_YCOORD * resolution, high_score->get_best_playername (),
        0);
}

/**
 * Display score, number of lifes and number of bricks
 */
void
right_panel_score::text_refresh ()
{
  draw (game_screen, SCORE_XCOORD * resolution,
        SCORE_YCOORD * resolution, current_player->score_value, 6);
  draw (game_screen, LIFES_XCOORD * resolution,
        LIFES_YCOORD * resolution, bricks_counter, 3);
  draw (game_screen, BRICKS_XCOORD * resolution,
        BRICKS_YCOORD * resolution, current_player->number_of_lifes, 2);
}

/**
 * Decrease the number of bricks
 * @param dec number to substract
 */
void
right_panel_score::decrease_bricks_counter (Uint32 dec)
{
  if (dec >= bricks_counter)
    {
      bricks_counter = 0;
    }
  else
    {
      bricks_counter -= dec;
    }
}

/**
 * Return the number of bricks
 * @return the number of bricks
 */
Uint32 right_panel_score::get_bricks_counter ()
{
  return bricks_counter;
}

/**
 * Intialize the bricks counter
 * @param counter the number of bricks
 */
void
right_panel_score::set_bricks_counter (Uint32 counter)
{
  bricks_counter = counter;
}

/**
 * Draw the gauge of the Gigablitz
 */
void
right_panel_score::draw_gigablizt_gauge ()
{
  controller_gigablitz *gigablitz = controller_gigablitz::get_instance ();
  controller_balls *balls = controller_balls::get_instance ();
  if (keyboard->is_gigablitz_or_tilt ()
      && !gigablitz->is_enable () && !balls->is_sticky_balls_remains ())
    {
      if (++delay_gigablitz_countdown >= DELAY_GIGABLITZ_COUNTDOWN)
        {
          if (gigablitz_countdown > 0)
            {
              delay_gigablitz_countdown = 0;
              gigablitz_countdown = gigablitz_countdown - resolution;
              /* gauge maximum height reached? */
              if (gigablitz_countdown == 0)
                {
                  /* yes, launch the gigablitz! */
                  gigablitz->shoot_paddle ();
                }
            }
        }
    }
  else
    {
      if (gigablitz_countdown < gauge_height)
        {
          delay_gigablitz_countdown = 0;
          gigablitz_countdown = gigablitz_countdown + resolution;
        }
    }

  /* draw the vertical gauge */
  unsigned char *dest = (unsigned char *) gauge_pixel;
  Sint32 next = game_screen->get_width ();
  unsigned char pixel = 0;
  if (resolution == 1)
    {
      for (Uint32 i = 0; i < gigablitz_countdown; i++)
        {
          dest[0] = pixel;
          dest[1] = pixel;
          dest[2] = pixel;
          dest[3] = pixel;
          dest[4] = pixel;
          dest += next;
        }
    }
  else
    {
      for (Uint32 i = 0; i < gigablitz_countdown; i++)
        {
          dest[0] = pixel;
          dest[1] = pixel;
          dest[2] = pixel;
          dest[3] = pixel;
          dest[4] = pixel;
          dest[5] = pixel;
          dest[6] = pixel;
          dest[7] = pixel;
          dest[8] = pixel;
          dest[9] = pixel;
          dest += next;
        }
    }

  Sint32 h = (gauge_height) - gigablitz_countdown;
  flip_white = flip_white ? false : true;
  if (flip_white)
    {
      unsigned char pixel = 130;
      if (resolution == 1)
        {
          for (Sint32 i = 0; i < h; i++)
            {
              dest[0] = pixel;
              dest[1] = pixel;
              dest[2] = pixel;
              dest[3] = pixel;
              dest[4] = pixel;
              dest += next;
            }
        }
      else
        {
          for (Sint32 i = 0; i < h; i++)
            {
              dest[0] = pixel;
              dest[1] = pixel;
              dest[2] = pixel;
              dest[3] = pixel;
              dest[4] = pixel;
              dest[5] = pixel;
              dest[6] = pixel;
              dest[7] = pixel;
              dest[8] = pixel;
              dest[9] = pixel;
              dest += next;
            }
        }
    }
  else
    {
      if (resolution == 1)
        {
          unsigned char *e = temoinCol1;
          for (Sint32 i = 0; i < h; i++)
            {
              char pixel = (char) e[i];
              dest[0] = pixel;
              dest[1] = pixel;
              dest[2] = pixel;
              dest[3] = pixel;
              dest[4] = pixel;
              dest += next;
            }
        }
      else
        {
          unsigned char *e = temoinCol2;
          for (Sint32 i = 0; i < h; i++)
            {
              unsigned char pixel = e[i];
              dest[0] = pixel;
              dest[1] = pixel;
              dest[2] = pixel;
              dest[3] = pixel;
              dest[4] = pixel;
              dest[5] = pixel;
              dest[6] = pixel;
              dest[7] = pixel;
              dest[8] = pixel;
              dest[9] = pixel;
              dest += next;
            }
        }
    }
}

/**
 * Reset the countdown of the Gigablitz
 */
void
right_panel_score::reset_gigablitz_countdown ()
{
  gigablitz_countdown = gauge_height;
}

/**
 * Return the width of the panel
 * @return Width panel scores in pixels
 */
Uint32 right_panel_score::get_width ()
{
  return panel_width;
}

/** List of 54 couleurs from gigablitz */
unsigned char
  right_panel_score::temoinCol1[GAUGE_HEIGHT] = {
  255,
  255,
  254,
  254,
  253,
  253,
  252,
  252,
  251,
  250,
  250,
  249,
  248,
  248,
  247,
  246,
  246,
  245,
  244,
  244,
  243,
  242,
  242,
  241,
  240,
  240,
  239
};

unsigned char
  right_panel_score::temoinCol2[GAUGE_HEIGHT * 2] = {
  255,
  255,
  255,
  255,
  254,
  254,
  254,
  254,
  253,
  253,
  253,
  253,
  252,
  252,
  252,
  251,
  251,
  251,
  250,
  250,
  250,
  249,
  249,
  249,
  248,
  248,
  248,
  247,
  247,
  247,
  246,
  246,
  246,
  245,
  245,
  245,
  244,
  244,
  244,
  243,
  243,
  243,
  242,
  242,
  242,
  241,
  241,
  241,
  240,
  240,
  240,
  239,
  239,
  239
};
