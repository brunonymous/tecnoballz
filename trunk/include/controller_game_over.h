/**
 * @file controller_game_over.h
 * @brief Game Over controller 
 * @created 2002-12-14
 * @date 2007-09-25
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.7 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_game_over.h,v 1.7 2007-09-25 16:00:44 gurumeditation Exp $
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
#ifndef __CONTROLLER_GAME_OVER__
#define __CONTROLLER_GAME_OVER__

class controller_game_over;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
#include "../include/sprite_display_scores.h"

class controller_game_over:public objects_list < sprite_object,
   controller_game_over >
  {
  private:
    /** Phase number: 0, 1, 2 or 3 */
    Uint32 move_phase;
    /** Delay counter before next phase */
    Uint32 next_phase_counter;
    sprite_display_scores *sprite_high_score;
    /** X offset of the chars */
    Sint32 char_x_offset;
    static const Sint32 SIZETSINUS = 200;
    static const Sint32 sinus_over[SIZETSINUS];
    static const Sint32 initial_coordinates[];

  private:
    void displacement_01 ();
    void displacement_02 ();
    void displacement_03 ();
    void displacement_04 ();

  public:
    controller_game_over ();
    ~controller_game_over ();
    sprite_display_scores *get_sprite_high_score ();
    void first_init (Uint32 x_offset = 0);
    void enable_game_over (bool is_victory);
    void run (bool is_victory = false);
  };
#endif
