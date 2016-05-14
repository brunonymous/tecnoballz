/** 
 * @file handler_players.h
 * @brief players handler 
 * @date 2012-09-05 
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
#ifndef __HANDLER_PLAYERS__
#define __HANDLER_PLAYERS__
class handler_players;


#include "../include/tecnoballz.h"
#include "../include/controller_capsules.h"
#include "../include/supervisor_shop.h"

class handler_players:public virtual tecnoballz
{
  friend class right_panel_score;
  friend class supervisor_guards_level;
  friend class supervisor_bricks_level;
  friend class controller_indicators;
  friend class handler_high_score;
public:
  static const Uint32 PLAYER_NAME_LENGTH = 6;
  static const Uint32 MAX_OF_PLAYERS = 6;

public:
  /** List of all players object */
  static handler_players **players_list;

private:
  /** Maximum number of players */
  static Uint32 max_of_players;
  /** Pointer to the first player alive */
  static handler_players *first_player;
  /** Pointer to the next player object */
  handler_players *next_player;
  /** Pointer to the previsous player object */
  handler_players *previous_player;
  /** Name of the player */
  char player_name[PLAYER_NAME_LENGTH + 1];
  /** Player number from 1 to 6 */
  Uint32 player_num;
  /** Player score value */
  Uint32 score_value;
  /** Bonus life granted every 25,000 points */
  Uint32 bonus_life_counter;
  /** Area number from 1 to 5 */
  Uint32 area_number;
  /** Level number in the current area, from 1 to 13 */
  Uint32 level_number;
  /** Number of lifes remaining */
  Sint32 number_of_lifes;
  /** List ot the items bought in the shop */
  //Sint32 shopping_cart[supervisor_shop::MAX_OF_CAPSULES_BOUGHT + 1];
  Sint32 shopping_cart[20];
  /** The number of items in the shopping cart */
  Uint32 shopping_cart_items;
  /** Amount of money available to be spend in the shop */
  Uint32 amount_of_money;
  /** State of the left wall of bricks */
  bool map_left_wall[12];
  /** State of the right  wall of bricks */
  bool map_right_wall[12];
  /** State of the top wall of bricks */
  bool map_top_wall[12];
  /** Alive counter of the right paddle, 0 = paddle disabled */ 
  Uint32 right_paddle_alive_counter;
  /** Alive counter of the top paddle, 0 = paddle disabled */ 
  Uint32 top_paddle_alive_counter;
  /** Alive counter of the left paddle, 0 = paddle disabled */ 
  Uint32 left_paddle_alive_counter;
  /** If true, then the wall must be rebuilt on the next level
   * ihis option is only available in area 5 */
  bool must_rebuild_walls;
  /** Less bricks counter */
  Uint32 less_bricks_count;
  /** Width of the horizontal paddles
   * and height of the vertical paddles */
  Uint32 paddle_length;
  /** If true all options prices will be to 1 in the next shop */
  bool budget_prices;
  /* states of the gems, collected or not */
  bool gems_state_list[6];
  Sint32 guardianPt;            //pt / level_list of the guards

private:
    handler_players ();
   ~handler_players ();
public:
  void initialize (Uint32 lifes, Uint32 areaN,
                   Uint32 level, Uint32 monay, Uint32 grdPt);
  void set_name (const char *playername);
  char *get_name ();
  Uint32 get_area_number ();
  Uint32 get_level_number ();
  Sint32 get_num_of_lifes ();
  Uint32 get_paddle_length ();
  void set_paddle_length (Uint32 length);
  Uint32 get_money_amount ();
  bool decrease_money_amount (Uint32 value);
  void increase_money_amount (Uint32 value);

  void add_score (Uint32 value);
  void clear_shopping_cart ();
  Sint32 *get_shopping_cart ();
  Uint32 get_numof_items_in_shopping_cart ();
  void set_numof_items_in_shopping_cart (Uint32 count);

  void clear_collected_gems ();
  bool are_collected_all_gems (Uint32 gemNu);
  bool is_collected_gem (Uint32 gemNu);

  Uint32 get_paddle_alive_counter (Uint32 paddle_num);
  void set_paddle_alive_counter (Uint32 paddle_num, Uint32 count);
  void set_less_bricks (Uint32 count);
  Uint32 get_less_bricks ();
  void set_rebuild_walls (bool enable);
  bool is_rebuild_walls ();
  void set_budget_prices (bool enbale);
  bool is_budget_prices ();
  bool *get_map_left ();
  bool *get_map_right ();
  bool *get_map_top ();

  Sint32 zlastlevel ();
  Sint32 next_level (Sint32 grdNx = 0);
  Uint32 get_next_phase ();

  void add_life (Uint32 add);
  void remove_life (Uint32 remove);
  void remove_all_lifes ();

  Sint32 getGuardPt ();
  void setGuardPt (Sint32 grdPt);

  static void release_all_players ();
  static handler_players *create_all_players (Uint32 numof);
  static handler_players *get_next_player (handler_players *, Uint32 *,
                                      Sint32 grdNx = 0);


private:
  handler_players *get_previous_player ();
  void set_next_player (handler_players *player);
  void set_previous_player (handler_players *player);
  void reset_members ();


};
#endif
