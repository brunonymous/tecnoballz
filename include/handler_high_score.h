/**
 * @file handler_high_score.h
 * @brief high score handler
 * @created 2004-04-30
 * @date 2007-11-18
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.4 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_high_score.h,v 1.4 2007-11-18 16:13:19 gurumeditation Exp $
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
#ifndef __HANDLER_HIGH_SCORE__
#define __HANDLER_HIGH_SCORE__

class handler_high_score;

#include "../include/tecnoballz.h"

typedef struct
  {
    char player_name[8];
    Uint32 value;
    Uint32 area_number;
    Uint32 level_number;
  } player_score;

class handler_high_score:public virtual tecnoballz
  {
  public:
    static const Uint32 MAX_OF_HIGH_SCORES = 10;

  private:
    static handler_high_score *high_score_singleton;
    player_score ** scores_tables;

  private:
    Uint32 scores_table_size;
    bool is_player_ranked (char *playename, Uint32 score_value,
                           Uint32 level_num, Uint32 area_num);
    void sort_scores ();
    bool load_high_score ();
    void save_high_score ();
    Uint32 calculate_checksum (Uint32 * addr, Uint32 data_size);
    void first_init ();
  private:
    handler_high_score ();
  public:
    ~handler_high_score ();
    static handler_high_score *get_instance ();
    bool is_player_ranked ();
    player_score *get_high_score_table ();
    char *get_best_playername ();
    Uint32 get_best_score ();
  };
#endif
