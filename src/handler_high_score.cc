/**
 * @file handler_high_score.cc 
 * @brief high score handler 
 * @created 2004-04-30 
 * @date 2007-10-08
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.6 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_high_score.cc,v 1.6 2007-10-08 05:44:03 gurumeditation Exp $
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
#include "../include/handler_high_score.h"
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_players.h"

handler_high_score *
handler_high_score::high_score_singleton = NULL;

/**
 * Create a high score handler 
 */
handler_high_score::handler_high_score ()
{
  scores_tables = (player_score **) NULL;
  scores_table_size =
    (sizeof (Uint32) * 3 +
     8) * MAX_OF_HIGH_SCORES * MAX_OF_DIFFICULTY_LEVELS + sizeof (Uint32);
  first_init ();
}

/**
 * Get the object instance
 * handler_high_score is a singleton
 * @return the handler_high_score object 
 */
handler_high_score *
handler_high_score::get_instance ()
{
  if (NULL == high_score_singleton)
    {
      high_score_singleton = new handler_high_score ();
    }
  return high_score_singleton;
}

/**
 * Release high score handler 
 */
handler_high_score::~handler_high_score ()
{
  if (NULL == scores_tables)
    {
      return;
    }
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      if (NULL != scores_tables[i])
        {
          delete[](char *) scores_tables[i];
          scores_tables[i] = NULL;
        }
    }
  delete[](char *) scores_tables;
  scores_tables = NULL;
}

/**
 * Perform some initializations
 */
void
handler_high_score::first_init ()
{
  try
    {
      scores_tables = new player_score *[MAX_OF_DIFFICULTY_LEVELS];

      /* allocate and fill scores tables */
      for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
        {
          scores_tables[i] = new player_score[MAX_OF_HIGH_SCORES];
          player_score *score = scores_tables[i];
          for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
            {
              score[j].area_number = 1;
              score[j].level_number = 1;
              score[j].value = 1000 - j * 100;
              score[j].player_name[0] = 'T';
              score[j].player_name[1] = ' ';
              score[j].player_name[2] = 'L';
              score[j].player_name[3] = ' ';
              score[j].player_name[4] = 'K';
              score[j].player_name[5] = ' ';
              score[j].player_name[6] = 0;
              /* SPARC unaligned memory access */
              score[j].player_name[7] = 0;
            }
        }
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!)handler_high_score::first_init() " <<
      "not enough memory to allocate high score table!" << std::endl;
      throw;
    }
  if (!load_high_score ())
    {
      save_high_score ();
    }
}

/**
 * Load high score file and copy data in high score structure
 * @return true if the high score file is successfully loaded or false otherwise
 */
bool handler_high_score::load_high_score ()
{
  char *
  file_data = resources->load_high_score_file ();
  if (NULL == file_data)
    {
      return false;
    }
  Uint32 file_size = resources->get_filesize_loaded ();
  if (file_size != scores_table_size)
    {
      std::cerr << "(!)handler_high_score::load_high_score() " <<
      " bad file size, " << file_size << " byte(s) instead" <<
      scores_table_size << "bytes!" << std::endl;
      delete[]file_data;
      return false;
    }

  /* verify the checksum */
  Uint32 read_sum;
  big_endian_to_int ((Uint32 *) file_data, &read_sum);
  Uint32 * ptr32 = (Uint32 *) file_data;
  Uint32 calculated_sum = calculate_checksum (ptr32 + 1,
                                       (scores_table_size -
                                        sizeof (Uint32)) / sizeof (Uint32));
  if (calculated_sum != read_sum)
    {
      std::cerr << "(!)handler_high_score::load_high_score() " <<
      " bad checksum, " << read_sum << " instead " <<
      calculated_sum << "!" << std::endl;
      delete[]file_data;
      return false;
    }

  /* copy data into the structures */
  char * addr = file_data + sizeof (Uint32);
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      player_score *
      score = scores_tables[i];
      for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
        {
          ptr32 = (Uint32 *) addr;
          big_endian_to_int (ptr32++, &score[j].area_number);
          big_endian_to_int (ptr32++, &score[j].level_number);
          big_endian_to_int (ptr32++, &score[j].value);
          addr = (char *) ptr32;
          for (Uint32 k = 0; k < 8; k++)
            {
              score[j].player_name[k] = *(addr++);
            }
        }
    }
  delete[]file_data;
  return true;
}

/**
 * Save the high score
 */
void
handler_high_score::save_high_score ()
{
  char *file_data;
  try
    {
      file_data = new char[scores_table_size];
    }
  catch (std::bad_alloc &)
    {
      std::cerr << "(!)handler_high_score::save_high_score() " <<
      "not enough memory to allocate " << scores_table_size <<
      "bytes!" << std::endl;
      throw;
    }


  Uint32 *ptr32;
  char *addr = file_data + sizeof (Uint32);
  for (Uint32 i = 0; i < MAX_OF_DIFFICULTY_LEVELS; i++)
    {
      player_score *score = scores_tables[i];
      for (Uint32 j = 0; j < MAX_OF_HIGH_SCORES; j++)
        {
          ptr32 = (Uint32 *) addr;
          int_to_big_endian (&score[j].area_number, ptr32++);
          int_to_big_endian (&score[j].level_number, ptr32++);
          int_to_big_endian (&score[j].value, ptr32++);
          addr = (char *) ptr32;
          for (Uint32 k = 0; k < 8; k++)
            {
              *(addr++) = score[j].player_name[k];
            }
        }
    }

  /* calculate checksum value */
  ptr32 = (Uint32 *) file_data;
  Uint32 ckVal = calculate_checksum (ptr32 + 1,
                                     (scores_table_size -
                                      sizeof (Uint32)) / sizeof (Uint32));
  int_to_big_endian (&ckVal, ptr32);
  resources->save_high_score_file (file_data, scores_table_size);
  delete[]file_data;
}

/**
 * Calculate checksum value
 * @param addr pointer to the data
 * @param data_size size of the data in long words
 * @return checksum value
 */
Uint32
handler_high_score::calculate_checksum (Uint32 * addr, Uint32 data_size)
{
  Uint32 value = 0;
  for (Uint32 i = 0; i < data_size; i++)
    {
      Uint32 long_word;
      big_endian_to_int (addr++, &long_word);
      value |= long_word;
    }
  return value;
}

/**
 * Check if the current player can be inserted in the scores table
 * @return true if the player score is ranked, otherwise false
 */
bool handler_high_score::is_player_ranked ()
{
  bool
  is_ranked = is_player_ranked (&current_player->player_name[0],
                                current_player->score_value,
                                current_player->level_number,
                                current_player->area_number);
  if (is_ranked)
    {
      sort_scores ();
      save_high_score ();
    }
  return is_ranked;
}

/**
 * Insert a player in the score table if this player get a good score
 * @param playername a name of player
 * @param score_value a score
 * @param level_num level number
 * @param area_num area number
 * @return true if the player score is ranked, otherwise false
 */
bool
handler_high_score::is_player_ranked (char *playername, Uint32 score_value,
                                      Uint32 level_num, Uint32 area_num)
{
  /* exit if cheat mode! */
  if (is_enabled_cheat_mode || birth_flag)
    {
      //return false;
    }

  /* verify if the name exists */
  player_score *score = scores_tables[difficulty_level - 1];
  for (Uint32 i = 0; i < MAX_OF_HIGH_SCORES; i++)
    {
      if (strcmp (playername, score[i].player_name) == 0)
        {
          if (score_value > score[i].value)
            {
              score[i].value = score_value;
              score[i].level_number = level_num;
              score[i].area_number = area_num;
              return true;
            }
          else
            {
              return false;
            }
        }
    }

  for (Uint32 i = 0; i < MAX_OF_HIGH_SCORES; i++)
    {
      if (score_value > score[i].value)
        {
          i = MAX_OF_HIGH_SCORES - 1;
          score[i].value = score_value;
          score[i].level_number = level_num;
          score[i].area_number = area_num;
          char *p = score[i].player_name;
          for (Sint32 j = 0; j < 6; j++)
            {
              p[j] = playername[j];
            }
          if (is_verbose)
            {
              std::cout << "handler_high_score::is_player_ranked() " <<
              "rank:" << i << " player name:" << score[i].player_name
              << std::endl;
            }
          return true;
        }
    }
  return false;
}

/**
 * Sort scores table
 */
void
handler_high_score::sort_scores ()
{
  if (is_verbose)
    {
      std::cout << "handler_high_score::sort_scores() " <<
      "difficulty_level:" << difficulty_level << std::endl;
    }
  bool is_sorted;
  player_score *score = scores_tables[difficulty_level - 1];
  do
    {
      is_sorted = false;
      for (Uint32 i = 0; i < (MAX_OF_HIGH_SCORES - 1); i++)
        {
          if (score[i].value >= score[i + 1].value)
            {
              continue;
            }
          Uint32 tmp = score[i].value;
          score[i].value = score[i + 1].value;
          score[i + 1].value = tmp;

          tmp = score[i].level_number;
          score[i].level_number = score[i + 1].level_number;
          score[i + 1].level_number = tmp;

          tmp = score[i].area_number;
          score[i].area_number = score[i + 1].area_number;
          score[i + 1].area_number = tmp;

          for (Uint32 j = 0; j < 6; j++)
            {
              tmp = score[i].player_name[j];
              score[i].player_name[j] = score[i + 1].player_name[j];
              score[i + 1].player_name[j] = tmp;
            }
          is_sorted = true;
        }
    }
  while (is_sorted);
}

/**
 * Return pointer to the scores list structure
 * @return Pointer to a 'player_score' structure
 */
player_score *
handler_high_score::get_high_score_table ()
{
  return scores_tables[difficulty_level - 1];
}

/**
 * Return the player name
 * @return poiner to the best player name
 */
char *
handler_high_score::get_best_playername ()
{
  player_score *score = scores_tables[difficulty_level - 1];
  return &score[0].player_name[0];
}

/**
 * Return the best player score
 * @return the best player score value
 */
Uint32 handler_high_score::get_best_score ()
{
  player_score *
  score = scores_tables[difficulty_level - 1];
  return score[0].value;
}
