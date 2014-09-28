/** 
 * @file handler_players.cc 
 * @brief players handler 
 * @date 2012-09-05 
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
#include "../include/handler_players.h"
#include "../include/controller_gems.h"
#include "../include/controller_sides_bricks.h"
#include "../include/controller_gems.h"

Uint32 handler_players::max_of_players = 0;
handler_players *handler_players::first_player = NULL;
handler_players **handler_players::players_list = NULL;

/* 
 * Create a player object
 */
handler_players::handler_players ()
{
  object_init ();

  /* 
   * add a new player
   */

  /* first player */
  if (0 == max_of_players)
    {
      first_player = this;
      next_player = this;
      previous_player = this;
    }
  else
    {
      next_player = first_player;
      handler_players *prev = first_player->get_previous_player ();
      previous_player = prev;
      first_player->set_previous_player (this);
      prev->set_next_player (this);
    }
  max_of_players++;

  /* 
   * clear members members
   */
  player_num = max_of_players;
  reset_members ();
  /* clear name of the player */
  Uint32 i;
  for (i = 0; i < 6; i++)
    {
      player_name[i] = ' ';
    }
  player_name[i] = 0;
}

/* 
 * Release a player object
 */
handler_players::~handler_players ()
{
  max_of_players--;
  if (max_of_players > 0)
    {
      next_player->set_previous_player (previous_player);
      previous_player->set_next_player (next_player);
      if (first_player == this)
        {
          first_player = next_player;
        }
    }
  else
    {
      first_player = NULL;
    }
  object_free ();
}

/**
 * Initialize a player object before a new game
 * @param lifes number of lifes
 * @param area area number (1 to 5)
 * @param level level number in current area (1 to 12)
 * @param money amount of money
 * @param grdPt level_list of the guards
 */
void
handler_players::initialize (Uint32 lifes, Uint32 area, Uint32 level,
                             Uint32 money, Uint32 grdPt)
{
  reset_members ();
  number_of_lifes = lifes;
  area_number = area;
  level_number = level;
  amount_of_money = money;
  guardianPt = grdPt;
}

/**
 * Reset some members values
 */
void
handler_players::reset_members ()
{
  /* clear the score value of the player */
  score_value = 0;
  bonus_life_counter = 0;
  area_number = 1;
  //area_number = 5; /*TEST*/
  /* level number into the current area */
  level_number = 1;
  number_of_lifes = initial_num_of_lifes;
  clear_shopping_cart ();
  amount_of_money = 500;
  for (Uint32 i = 0; i < controller_sides_bricks::MAX_OF_SIDES_BRICKS; i++)
    {
      map_left_wall[i] = true;
      map_right_wall[i] = true;
      map_top_wall[i] = true;
    }
  /* disable right, top and left paddles */
  right_paddle_alive_counter = 0;
  top_paddle_alive_counter = 0;
  left_paddle_alive_counter = 0;
  must_rebuild_walls = false;
  less_bricks_count = 0;
  /* width of the horizontal paddles
   * and height of the vertical paddles */
  paddle_length = 32 * resolution;
  budget_prices = false;
  guardianPt = 0;
  clear_collected_gems ();
}

/**
 * Set the player name
 * @param name the name of the player
 */
void
handler_players::set_name (const char *name)
{
  for (Uint32 i = 0; i < PLAYER_NAME_LENGTH; i++)
    {
      player_name[i] = ' ';
    }
  for (Uint32 i = 0; i < PLAYER_NAME_LENGTH; i++)
    {
      char c = name[i];
      if (0 == c)
        {
          return;
        }
      if (c >= 'a' && c <= 'z')
        {
          c = c - ('a' - 'A');
        }
      if ((c >= ' ' && c <= '!') ||
          (c >= '-' && c <= '.') ||
          (c >= '0' && c <= ':') || (c >= 'A' && c <= 'Z') || c == '\'')
        {
          player_name[i] = c;
        }
      else
        {
          player_name[i] = ' ';
        }
    }
}

/**
 * Return the current player name
 * @return the current name the player
 */
char *
handler_players::get_name ()
{
  return &player_name[0];
}

/**
 * Return the area number
 * @raturn area number, from 1 to 5
 */
Uint32
handler_players::get_area_number ()
{
  return area_number;
}

/** 
 * Return the level number
 * @return level number, from 1 to 13
 */
Uint32
handler_players::get_level_number ()
{
  return level_number;
}

/**
 * Return the number of life(s)
 * @return the number of life(s)
 */
Sint32
handler_players::get_num_of_lifes ()
{
  return number_of_lifes;
}

/**
 * return the paddle's length
 * @return the length of the paddle in pixels
 */
Uint32
handler_players::get_paddle_length ()
{
  return paddle_length;
}

/**
 * Initialize paddle's length
 * @param length the length of the paddle in pixels
 */
void
handler_players::set_paddle_length (Uint32 length)
{
  paddle_length = length;
}

/**
 * Return the current amount of money
 * @return the amount of money
 */
Uint32
handler_players::get_money_amount ()
{
  return amount_of_money;
}

/**
 * Decrease the amount of money
 * @param value money amount
 * @return true if the money amount could be descreased 
 */
bool
handler_players::decrease_money_amount (Uint32 value)
{
  if (value > amount_of_money)
    {
      return false;
    }
  amount_of_money -= value;
  return true;
}

/**
 * Increase the amount of money
 * @param value money amount
 */
void
handler_players::increase_money_amount (Uint32 value)
{
  amount_of_money += value;
}

/**
 * Increase the score
 * @param value is the number of points to increase the score by
 */
void
handler_players::add_score (Uint32 value)
{
  score_value += value;
  bonus_life_counter += value;
  if (bonus_life_counter > 25000)
    {
      add_life (1);
      bonus_life_counter -= 25000;
    }
}

/**
 * Clear the shopping cart 
 */
void
handler_players::clear_shopping_cart ()
{
  Sint32 *cart = shopping_cart;
  for (Uint32 i = 0; i < supervisor_shop::MAX_OF_CAPSULES_BOUGHT; i++)
    {
      *(cart++) = 0;
    }
  /* clear the number of items bought */
  shopping_cart_items = 0;
  /* end of the shopping cart */
  *cart = -1;
}

/**
 * Return the list of items bought in th shop
 * @return a pointer to the list of itemps bought in th shop
 */
Sint32 *
handler_players::get_shopping_cart ()
{
  return shopping_cart;
}

/** 
 * Return the number of itemp bought in th shop
 * @return the number of items bought in th shop
 */
Uint32
handler_players::get_numof_items_in_shopping_cart ()
{
  return shopping_cart_items;
}

/**
 * Set the number of items bought
 * @param count Number of items 
 */
void
handler_players::set_numof_items_in_shopping_cart (Uint32 count)
{
  shopping_cart_items = count;
}

/**
 * Clear the list of gems collected
 */
void
handler_players::clear_collected_gems ()
{
  for (Uint32 i = 0; i < controller_gems::MAX_OF_GEMS; i++)
    {
      /* states of the 6 gems */
      gems_state_list[i] = false;
    }
}

/**
 * Verify if the 6 gemstones are collected
 * @param gem_id last gem identifier collected
 * @return true if all gems are collected, otherwise false
 */
bool
handler_players::are_collected_all_gems (Uint32 gemNu)
{
  gems_state_list[gemNu] = true;
  for (Uint32 i = 0; i < controller_gems::MAX_OF_GEMS; i++)
    {
      if (!gems_state_list[i])
        {
          return false;
        }
    }
  clear_collected_gems ();
  return true;
}

/**
 * Return state of one of six gemstones
 * @param gem_id gem identifier 0 to 5
 * @return true if the gem is enabled, otherwise false
 */
bool
handler_players::is_collected_gem (Uint32 gem_id)
{
  return gems_state_list[gem_id];
}

/**
 * Get the alive counter of a paddle
 * @param paddle_num paddle number RIGHT_PADDLE, TOP_PADDLE, or LEFT_PADDLE
 * @return alive counter value, if 0 then the paddle is disabled
 */
Uint32
handler_players::get_paddle_alive_counter (Uint32 paddle_num)
{
  switch (paddle_num)
    {
    case controller_paddles::RIGHT_PADDLE:
      return right_paddle_alive_counter;
      break;
    case controller_paddles::TOP_PADDLE:
      return top_paddle_alive_counter;
      break;
    default:
      return left_paddle_alive_counter;
      break;
    }
}

/**
 * Set the alive counter of a paddle
 * @param paddle_num paddle number RIGHT_PADDLE, TOP_PADDLE, or LEFT_PADDLE
 * @param count value of the counter, if 0 then the paddle is disabled
 */
void
handler_players::set_paddle_alive_counter (Uint32 paddle_num, Uint32 counter)
{
  switch (paddle_num)
    {
    case controller_paddles::RIGHT_PADDLE:
      right_paddle_alive_counter = counter;
      break;
    case controller_paddles::TOP_PADDLE:
      top_paddle_alive_counter = counter;
      break;
    case controller_paddles::LEFT_PADDLE:
      left_paddle_alive_counter = counter;
      break;
    }
}

/**
 * Initialize less bricks option
 * @param count number of bricks in less
 */
void
handler_players::set_less_bricks (Uint32 count)
{
  less_bricks_count = count;
}

/**
 * Return number of bricks in less
 * @return number of bricks in less
 */
Uint32
handler_players::get_less_bricks ()
{
  return less_bricks_count;
}

/**
 * Set the budget prices option
 * @param enable true if enable the budget prices option, false otherwise
 */
void
handler_players::set_budget_prices (bool enable)
{
  budget_prices = enable;
}

/**
 * Check if the budget prices option is enable
 */
bool
handler_players::is_budget_prices ()
{
  return budget_prices;
}

/**
 * Enable or disable the option which will rebuild the walls bricks
 * on the next levels
 * @param enable true if rebuilt the walls, false otherwise
 */
void
handler_players::set_rebuild_walls (bool enable)
{
  must_rebuild_walls = enable;
}

/**
 * Check if the walls must be rebuilt.
 * @return true if rebuilt the walls, false otherwise
 */
bool
handler_players::is_rebuild_walls ()
{
  return must_rebuild_walls;
}

/**
 * Return state of the left wall bricks 
 * @return a pointer to the map of the left wall 
 */
bool *
handler_players::get_map_left ()
{
  return map_left_wall;
}

/**
 * Return state of the right wall bricks 
 * @return a pointer to the map of the right wall 
 */
bool *
handler_players::get_map_right ()
{
  return map_right_wall;
}

/**
 * Return state of the top wall bricks 
 * @return a pointer to the map of the top wall 
 */
bool *
handler_players::get_map_top ()
{
  return map_top_wall;
}

//-----------------------------------------------------------------------------
// is the lastest level of tecnoballz?
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::zlastlevel ()
{
  if (area_number >= 5 && level_number >= 13)
    return 1;
  else
    return 0;
}

//-----------------------------------------------------------------------------
// next level
//      output <= 1: end of game :-)
//-----------------------------------------------------------------------------
Sint32
handler_players::next_level (Sint32 grdNx)
{
  Sint32 r = 0;
  if (is_verbose)
    printf
      ("handler_players::next_level() area_number=%i, level_number=%i grdNx=%i guardianPt =%i\n",
       area_number, level_number, grdNx, guardianPt);
  if (area_number == 5 && level_number == 13)
    {
      area_number = 1;
      level_number = 1;
      r = 1;                    //end of game
      guardianPt = 0;
    }
  else
    {
      if (area_number == 5 && level_number == 12)
        {
          level_number++;
          guardianPt += grdNx;
        }
      else
        {
          level_number++;
          if (level_number == 13)
            {
              area_number++;
              level_number = 1;
              guardianPt += grdNx;
            }
          if (level_number == 7)
            guardianPt += grdNx;
        }
    }
  if (is_verbose)
    printf
      ("handler_players::next_level() area_number=%i, level_number=%i,  guardianPt=%i\n",
       area_number, level_number, guardianPt);
  return r;
}

/**
 * Return the phase code
 * @return the next phase code GUARDS_LEVEL or SHOP
 */
Uint32
handler_players::get_next_phase ()
{
  /* MAIN_MENU is a very improbable case */
  Uint32 phase = MAIN_MENU;
  /* levels 6, 12 and the level 13 of the area 5 are guardians levels */
  if (level_number == 6 || level_number == 12 || level_number == 13)
    {
      phase = GUARDS_LEVEL;
    }
  else
    {
      /* before a level, there is always the shop,
       * except for the first level of the first area */
      if (level_number > 0 && level_number < 12)
        {
          phase = SHOP;
        }
    }
  return phase;
}

/**
 * Return previous player
 * @return a pointer to the previous player object
 */
handler_players *
handler_players::get_previous_player ()
{
  return previous_player;
}

/**
 * Set the next player 
 * @param player pointer to a object player
 */
void
handler_players::set_next_player (handler_players * player)
{
  next_player = player;
}

/**
 * Get the previous player
 * @param player pointer to a object player
 */
void
handler_players::set_previous_player (handler_players * player)
{
  previous_player = player;
}

//-----------------------------------------------------------------------------
// get pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
Sint32
handler_players::getGuardPt ()
{
  return guardianPt;
}

//-----------------------------------------------------------------------------
// set pointer to "level_list" of the guards
//-----------------------------------------------------------------------------
void
handler_players::setGuardPt (Sint32 grdPt)
{
  guardianPt = grdPt;
}

/**
 * Add one or more lifes
 * @param add number of lifes to add
 */
void
handler_players::add_life (Uint32 add)
{
  number_of_lifes += add;
}

/**
 * Remove one or more lifes
 * @param add number of lifes to remove 
 */
void
handler_players::remove_life (Uint32 remove)
{
  number_of_lifes -= remove;
  if (number_of_lifes < 0)
    {
      number_of_lifes = 0;
    }
}

/**
 * Remove all lifes, when the game over is forced 
 */
void
handler_players::remove_all_lifes ()
{
  number_of_lifes = 0;
}

/*
 * Statics methods
 */

/**
 * Static method which return next player and determine the next phase
 * @param player the current "handler_players" object
 * @param next_phase pointer to "end_return"
 * @param grdNx pointer to "level_list" of the guards (NULL by default)
 * @return the new player object
 */
handler_players *
handler_players::get_next_player (handler_players * player, Uint32 * next_phase,
                             Sint32 grdNx)
{
  Uint32 start = player->player_num;
  Uint32 index = start;
  if (current_phase != SHOP)
    {
      /* jump to the next level */
      player->next_level (grdNx);
    }

  /* process each player object */
  for (Uint32 i = 0; i < max_of_players; i++)
    {
      if (++index > max_of_players)
        {
          index = 1;
        }
      handler_players *player = players_list[index - 1];
      if (player->number_of_lifes <= 0)
        {
          continue;
        }
      /* get next phase: GUARDS_LEVEL or SHOP */
      *next_phase = player->get_next_phase ();

      /* this player already went to the shop,
       * he jump to the bricks level */
      if (player->player_num <= start && *next_phase == SHOP
          && current_phase == SHOP)
        {
          *next_phase = BRICKS_LEVEL;
        }
      /* multiplayers case: all players play the same level */
      if (player->player_num > start && current_phase != SHOP
          && *next_phase == SHOP)
        {
          *next_phase = BRICKS_LEVEL;
        }
      return player;
    }
  /* unlikely case  */
  *next_phase = MAIN_MENU;
  return players_list[0];
}

/**
 * Static method which initializes the maximum number of players
 * @param numof maximum number of players, always 6
 */
handler_players *
handler_players::create_all_players (Uint32 numof)
{

  try
  {
    players_list = new handler_players *[numof];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)handler_players::joueursADD() "
      "not enough memory to allocate " <<
      numof << " list elements!" << std::endl;
    throw;
  }

  /* create the players objects */
  for (Uint32 i = 0; i < numof; i++)
    {
      players_list[i] = new handler_players ();
    }
  return first_player;
}

/**
 * Static method which releases all objects players
 */
void
handler_players::release_all_players ()
{
  for (Uint32 i = 0; i < max_of_players; i++)
    {
      delete first_player;
    }
  if (NULL != players_list)
    {
      delete[]players_list;
      players_list = NULL;
    }
}
