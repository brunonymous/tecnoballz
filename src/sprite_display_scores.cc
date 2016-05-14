/** 
 * @file sprite_display_scores.cc 
 * @brief Sprite wich display text of the score table 
 * @created 2003-04-30 
 * @date 2007-03-09
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
#include "../include/sprite_display_scores.h"
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"
#include "../include/handler_high_score.h"

/**
 * Create the sprite displaying scores
 */
sprite_display_scores::sprite_display_scores ()
{
  clear_sprite_members ();
  offscreen_text = (bitmap_data *) NULL;
  width_font = 8 * resolution;
  heightfont = 8 * resolution;
  if (resolution == 2)
    {
      space2next = 17;
    }
  else
    {
      space2next = 8;
    }
  run_offset = 0;
  max_offset = TEXT_LARGE * TEXT_HAUTE;
}

/**
 * Release the sprite displaying scores
 */
sprite_display_scores::~sprite_display_scores ()
{
  if (offscreen_text)
    delete offscreen_text;
  if (bitmap_fonts)
    delete bitmap_fonts;
  bitmap_fonts = (bitmap_data *) NULL;
  offscreen_text = (bitmap_data *) NULL;
}

/**
 * Perform some initializations
 */
void
sprite_display_scores::first_init (Uint32 x_offset)
{

  load_bitmap_fonts (handler_resources::BITMAP_SCORES_FONTS);

/* allocate 512 * 323 pixels buffer for scores text */ 
  offscreen_text = new bitmap_data ();
  offscreen_text->create_surface (TEXT_LARGE * width_font,
                              TEXT_HAUTE * space2next);

  /* initialize sprite object */
  make_sprite (offscreen_text);
  enable ();
  set_coordinates (32 * resolution + x_offset, 80 * resolution);
  copyToText ();
}

//------------------------------------------------------------------------------
// display the text menu into buffer memory
//------------------------------------------------------------------------------
Sint32
sprite_display_scores::displayTxt ()
{
  //###################################################################
  // display menu text    
  //###################################################################
  char *desP1 = pixel_data;
  Sint32 offSc = off_source;
  Sint32 offDs = row_size;
  Sint32 offD2 = row_size * (space2next - 1);
  //Sint32 offD2 = row_size * 8 * resolution;
  Sint32 *basPT = (Sint32 *) caract_adr;
  char *p = scorestext;
  char *c = ascii2code;
  Sint32 a, b, j;
  Sint32 offst = 0;
  Sint32 oldva = run_offset;

  if (resolution == 1)
    {
      for (Sint32 k = 0; k < TEXT_HAUTE; k++, desP1 += offD2)
        {                       //###########################################################
          // display normal line of 32 characters
          //###########################################################
          for (j = 0; j < TEXT_LARGE; j++)
            {
              a = *(p++) - 32;
              if (run_offset < max_offset && offst >= run_offset)
                run_offset++;
              if (offst < max_offset)
                offst++;
              if (a)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 3;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 8; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                  if (run_offset > oldva)
                    return 0;
                }
              desP1 = desP1 + 8;
            }
        }
    }
  else
    {

      for (Sint32 k = 0; k < TEXT_HAUTE; k++, desP1 += offD2)
        {                       //###########################################################
          // display normal line of 32 characters
          //###########################################################
          for (j = 0; j < TEXT_LARGE; j++)
            {
              a = *(p++) - 32;
              if (run_offset < max_offset && offst >= run_offset)
                run_offset++;
              if (offst < max_offset)
                offst++;
              if (a)
                {
                  b = c[a];
                  Sint32 *s = (Sint32 *) basPT;
                  Sint32 *d = (Sint32 *) desP1;
                  b = b << 4;
                  s = (Sint32 *) ((char *) s + b);
                  for (b = 0; b < 16; b++)
                    {
                      d[0] = s[0];
                      d[1] = s[1];
                      d[2] = s[2];
                      d[3] = s[3];
                      s = (Sint32 *) ((char *) s + offSc);
                      d = (Sint32 *) ((char *) d + offDs);
                    }
                  if (run_offset > oldva)
                    return 0;
                }
              desP1 = desP1 + 16;
            }
        }
    }
  return 1;
}

/**
 * Copy high score table into strings
 */
void
sprite_display_scores::copyToText ()
{
  player_score *score = high_score->get_high_score_table ();
  if (NULL == score)
    {
      return;
    }
  char *ptext = scorestext + TEXT_LARGE * 2;
  for (Uint32 i = 0; i < handler_high_score::MAX_OF_HIGH_SCORES; i++)
    {
      char *pName = score[i].player_name;
      for (Uint32 j = 0; j < handler_players::PLAYER_NAME_LENGTH; j++)
        {
          ptext[j] = pName[j];
        }
      integer_to_ascii (score[i].value, 6,  &ptext[7]);
      integer_to_ascii (score[i].area_number, 1, &ptext[16]);
      integer_to_ascii (score[i].level_number, 2, &ptext[21]);
      ptext += TEXT_LARGE;
    }
}

//------------------------------------------------------------------------------
// scores table text
//------------------------------------------------------------------------------
char
  sprite_display_scores::scorestext[] = "    TECNOBALL TOP TEN  "       //0
  "NAME  SCORE  AREA LEVEL"     //1
  "...... 000000  :1    01"     //2
  "...... 000000  :1    01"     //3
  "...... 000000  :1    01"     //4
  "...... 000000  :1    01"     //5
  "...... 000000  :1    01"     //6
  "...... 000000  :1    01"     //7
  "...... 000000  :1    01"     //8
  "...... 000000  :1    01"     //9
  "...... 000000  :1    01"     //10
  "...... 000000  :1    01";    //11
