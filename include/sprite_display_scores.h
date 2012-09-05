/** 
 * @file sprite_display_scores.h 
 * @brief Sprite wich display text of the score table 
 * @created 2003-04-30 
 * @date 2007-02-18
 * @copyright 1991-2012 TLK Games
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
#ifndef __SPRITE_DISPLAY_SCORES__
#define __SPRITE_DISPLAY_SCORES__

class sprite_display_scores;

#include "../include/display_text_bitmap.h"
#include "../include/sprite_object.h"
#include "../include/bitmap_data.h"


class sprite_display_scores:public virtual display_text_bitmap,
  public virtual sprite_object
{

private:
  static const Sint32 TEXT_LARGE = 23;  //number of characters by lines
  static const Sint32 TEXT_HAUTE = 12;  //number of lines of characters
  static char scorestext[];     //list of names and score

  Sint32 width_font;            //fonte's width font 8 or 16
  Sint32 heightfont;            //fonte's height
  Sint32 space2next;            //number of lines to next fonte 9 or 18
  bitmap_data *offscreen_text;
  Sint32 run_offset;
  Sint32 max_offset;

public:
    sprite_display_scores ();
   ~sprite_display_scores ();
  void first_init (Uint32 x_offset = 0);
  void copyToText ();
  Sint32 displayTxt ();
};
#endif
