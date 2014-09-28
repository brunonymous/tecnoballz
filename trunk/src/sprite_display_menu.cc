/**
 * @file sprite_display_menu.cc 
 * @brief Sprite wich display text of the menu in the menu principal 
 * @date 2012-09-02
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
#include "../include/sprite_display_menu.h"
#include "../include/handler_resources.h"
#include "../include/handler_keyboard.h"
#include "../include/handler_players.h"
#include "../include/handler_high_score.h"
#include "../include/supervisor_main_menu.h"

/**
 * Create the sprite displaying text
 */
sprite_display_menu::sprite_display_menu ()
{
  clear_sprite_members ();
  text_offscreen = NULL;
  y_coord_left_down =  handler_keyboard::NULL_YCOORD;
  y_coord_right_down = handler_keyboard::NULL_YCOORD;
  font_width = 8 * resolution;
  font_height = 8 * resolution;
  if (resolution == 2)
    {
      line_spacing = 17;
    }
  else
    {
      line_spacing = 8;
    }
  menu_first_color_index = 0;

  is_clean_zone = false;
  clear_zone_height = 0;
  clear_zone_width = 0;
  clear_zone_xcoord = 0;
  clear_zone_ycoord = 0;
  blink_cursor_delay = 0;
  texts_of_menus = NULL;
  menu_events = new handler_menu_events();
}

/**
 * Release the sprite displaying text
 */
sprite_display_menu::~sprite_display_menu ()
{
  if (NULL != text_offscreen)
    {
      delete text_offscreen;
      text_offscreen = NULL;
    }
  if (NULL != bitmap_fonts)
    {
      delete bitmap_fonts;
      bitmap_fonts = NULL;
    }
  if (texts_of_menus != NULL)
    {
      delete[](char *) texts_of_menus;
      texts_of_menus = NULL;
    }
  if (menu_events != NULL)
    {
      delete menu_events;
      menu_events = NULL;
    }
}

/**
 * Read the texts file 
 */
void
sprite_display_menu::load_text_file ()
{
  if (texts_of_menus != NULL)
    {
      return;
    }
  texts_of_menus =
    resources->load_texts (handler_resources::TEXTS_MAIN_MENU,
                           MAX_OF_LINES, NUM_OF_COLUMNS, 0, false);
}

/**
 * Create offscreen and perform some initializations
 */
void
sprite_display_menu::first_init ()
{
  load_text_file ();
  load_bitmap_fonts (handler_resources::BITMAP_MENU_FONTS);

  /* allocate 512 * 323 pixels buffer for text menu  */
  text_offscreen = new bitmap_data ();
  text_offscreen->create_surface (NUM_OF_COLUMNS * font_width,
                                  NUM_OF_ROWS * line_spacing);

  /* initialize sprite object */
  make_sprite (text_offscreen);
  enable ();
  set_coordinates (32 * resolution, 80 * resolution);
  set_current_menu_section(MAIN_SECTION);

  /* initialize palette color chars */
  SDL_Color *palette = display->get_palette ();
  Sint32 i = random_counter & 0x0F;
  if (i >= 10)
    {
      i = i - 10;
    }
  const Uint32 *degrade = (handler_resources::color_gradations + i * 18);
  SDL_Color *pal = palette + 239;
  for (i = 0; i < 17; i++)
    {
      Uint32 color = degrade[i];
      Uint32 blue = color & 0x000000ff;
      Uint32 green = color & 0x0000ff00;
      green = green >> 8;
      Uint32 red = color & 0x00ff0000;
      red = red >> 16;
      pal->r = red;
      pal->g = green;
      pal->b = blue;
      pal++;
    }
  display->enable_palette (palette);
}

/**
 * Check events and draw menu text
 * @return exit code DO_NO_EXIT, PROGRAM_EXIT, or START_GAME
 */
Uint32 sprite_display_menu::check_and_display ()
{
  clear_input_zone ();
  update_strings ();
  Sint32 y = (keyboard->get_mouse_y () - y_coord) / line_spacing;
  Uint32 exit_code = check_events ();

  /* read color table offset (color line over the mouse ) */
  if (menu_first_color_index++ > 32)
    {
      menu_first_color_index = 0;
    }
  Uint32 color = menu_first_color_index;

  /* display menu text */
  char * dest = pixel_data;
  Sint32 s_offset = off_source;
  Sint32 d_offset1 = row_size;
  Sint32 d_offset2 = row_size * (line_spacing - 1);
  Sint32 * font = (Sint32 *) caract_adr;
  char * c = ascii_to_index;
  Sint32 a, b, j;

  /*
   * mode low-res (320 x 200)
   */
  if (resolution == 1)
    {
      for (Sint32 k = 0; k < NUM_OF_ROWS; k++, dest += d_offset2)
        {
          char *p = texts_of_menus[current_menu_section + k];
          if (y != k)
            {
              /* display normal line of 32 characters */
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  a = *(p++) - 32;
                  if (a != 0)
                    {
                      b = c[a];
                      b = b << 3;
#ifndef BYTES_COPY
                      Sint32 * s = (Sint32 *) font;
                      Sint32 * d = (Sint32 *) dest;
                      s = (Sint32 *) ((char *) s + b);
                      for (b = 0; b < 8; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          s = (Sint32 *) ((char *) s + s_offset);
                          d = (Sint32 *) ((char *) d + d_offset1);
                        }
#else
                      char * s = (char *) font;
                      char * d = dest;
                      s += b;
                      for (b = 0; b < 8; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          d[2] = s[2];
                          d[3] = s[3];
                          d[4] = s[4];
                          d[5] = s[5];
                          d[6] = s[6];
                          d[7] = s[7];
                          s += s_offset;
                          d += d_offset1;
                        }
#endif
                    }
                  dest = dest + 8;
                }
            }
          else
            {
              /* display selected line of 32 characters */
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  unsigned char pixel = color_cycling[color];
                  a = *(p++) - 32;
                  if (a != 0)
                    {
                      b = c[a];
                      unsigned char * s = (unsigned char *) font;
                      unsigned char * d = (unsigned char *) dest;
                      b = b << 3;
                      s = s + b;
                      for (b = 0; b < 8; b++)
                        {
                          for (Sint32 z = 0; z < 8; z++)
                            {
                              a = s[z];
                              if (a != 0)
                                {
                                  a = pixel;
                                  d[z] = pixel;
                                }
                            }
                          s = s + s_offset;
                          d = d + d_offset1;
                        }
                    }
                  dest = dest + 8;
                  if (color++ > 32)
                    {
                      color = 0;
                    }
                }
            }
        }
    }

  /*
   * mode hi-res (640 x 400)
   */
  else
    {

      for (Sint32 k = 0; k < NUM_OF_ROWS; k++, dest += d_offset2)
        {
          char* p = texts_of_menus[current_menu_section + k];
          if (y != k)
            {
              /* display normal line of 32 characters */
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  a = *(p++) - 32;
                  if (a != 0)
                    {
                      b = c[a];
                      b = b << 4;
#ifndef BYTES_COPY
                      Sint32 * s = (Sint32 *) font;
                      Sint32 * d = (Sint32 *) dest;
                      s = (Sint32 *) ((char *) s + b);
                      for (b = 0; b < 16; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          d[2] = s[2];
                          d[3] = s[3];
                          s = (Sint32 *) ((char *) s + s_offset);
                          d = (Sint32 *) ((char *) d + d_offset1);
                        }
#else
                      char * s = (char *) font;
                      char * d = dest;
                      s += b;
                      for (b = 0; b < 16; b++)
                        {
                          d[0] = s[0];
                          d[1] = s[1];
                          d[2] = s[2];
                          d[3] = s[3];
                          d[4] = s[4];
                          d[5] = s[5];
                          d[6] = s[6];
                          d[7] = s[7];
                          d[8] = s[8];
                          d[9] = s[9];
                          d[10] = s[10];
                          d[11] = s[11];
                          d[12] = s[12];
                          d[13] = s[13];
                          d[14] = s[14];
                          d[15] = s[15];
                          s += s_offset;
                          d += d_offset1;
                        }
#endif
                    }
                  dest = dest + 16;
                }
            }
          else
            {
              /* display selected line of 32 characters */
              for (j = 0; j < NUM_OF_COLUMNS; j++)
                {
                  unsigned char pixel = color_cycling[color];
                  a = *(p++) - 32;
                  if (a != 0)
                    {
                      b = c[a];
                      unsigned char * s = (unsigned char *) font;
                      unsigned char * d = (unsigned char *) dest;
                      b = b << 4;
                      s = s + b;
                      for (b = 0; b < 16; b++)
                        {
                          for (Sint32 z = 0; z < 16; z++)
                            {
                              a = s[z];
                              if (a)
                                {
                                  a = pixel;
                                  d[z] = pixel;
                                }
                            }
                          s = s + s_offset;
                          d = d + d_offset1;
                        }
                    }
                  dest = dest + 16;
                  if (color++ > 32)
                    {
                      color = 0;
                    }
                }
            }
        }

    }
  draw_input_cursor ();
  return exit_code;
}

/**
 * Set a new menu
 */
void
sprite_display_menu::set_current_menu_section(Uint32 current)
{
  current_menu_section = current;
  Sint32 y = y_coord;
  Uint32 min = 0;
  Uint32 max = NUM_OF_ROWS;
  for(Sint32 i = 0; i < NUM_OF_ROWS; i++)
    {
      bool found = false;
      char *p = texts_of_menus[current + i];
      for(Sint32 j = 0; j < NUM_OF_COLUMNS; j++)
        {
          if(p[j] == ' ' )
            {
              continue;
            }
          found = true;
          break;
        }
      if (found)
        {
          break;
        }
      min++;
    }

  for(Uint32 i = NUM_OF_ROWS - 1; i > 0; i--)
    {
      bool found = false;
      char *p = texts_of_menus[current + i];
      for(Sint32 j = 0; j < NUM_OF_COLUMNS; j++)
        {
          if(p[j] == ' ' )
            {
              continue;
            }
          found = true;
          break;
        }
      if (found)
        {
          break;
        }
      max--;
    }
  menu_events->start(line_spacing, min, max - 1, display->get_width() >> 1, y);
}

/**
 * Check mouse events 
 * @return exit code DO_NO_EXIT, PROGRAM_EXIT, or START_GAME
 */
Uint32
sprite_display_menu::check_events ()
{
  Uint32 exit_code = DO_NO_EXIT;
  Sint32 pos_y = 0;
  Sint32 incre = 0;
  if (menu_events->check(&pos_y, &incre))
    {
      pos_y = (pos_y - y_coord) / line_spacing;
      switch (current_menu_section)
        {
          /* main menu */
        case MAIN_SECTION:
          switch (pos_y)
            {
            case LINE_START:
              menu_events->stop();
              clear_text_offscreen ();
              clear_zone_stop ();
              exit_code = START_GAME;
              break;
            case LINE_PARAM:
              clear_text_offscreen ();
              clear_zone_stop ();
              set_current_menu_section(OPTIONS_SECTION);
              break;
            case LINE_ABOUT:
#ifndef WIN32
              audio->play_music (handler_audio::FRIDGE_IN_SPACE_MUSIC);
#endif
              clear_text_offscreen ();
              clear_zone_stop ();
              set_current_menu_section(ABOUT_SECTION);
              break;
            case LINE_SALUT:
              audio->play_music (handler_audio::MON_LAPIN_MUSIC);
              clear_text_offscreen ();
              clear_zone_stop ();
              set_current_menu_section(GREETINGS_SECTION);
              break;
            case LINE_INFOS:
              audio->play_music (handler_audio::IN_GAME_MUSIC);
              clear_text_offscreen ();
              clear_zone_stop ();
              set_current_menu_section(INFOS_SECTION);
              break;
            case LINE_SCORE:
              audio->play_music (handler_audio::TERMIGATOR_MUSIC);
              clear_text_offscreen ();
              clear_zone_stop ();
              copy_high_score_in_menu ();
              set_current_menu_section(SCORE_SECTIONS);
              break;

              /* input the code area */
            case LINE_CODE1:
            case LINE_CODE2:
            {
              char *
              area_code = supervisor_main_menu::get_current_area_code ();
              start_input_string (10, LINE_CODE2,
                                  supervisor_main_menu::AREA_CODE_LENGTH,
                                  area_code);
            }
            break;
            case LINE_SORTI:
              exit_code = PROGRAM_EXIT;
              break;
            }
          break;

          /*
           * options
           */
        case OPTIONS_SECTION:
          switch (pos_y)
            {
            case 5:
              clear_zone_start (24, 5, 1, 1);
              if (++number_of_players >
                  (Sint32) handler_players::MAX_OF_PLAYERS)
                {
                  number_of_players = 1;
                }
              break;

              // input players names
            case 6:
              start_input_string (24, 6, 6,
                                  handler_players::players_list[0]->get_name ());
              break;
            case 7:
              start_input_string (24, 7, 6,
                                  handler_players::players_list[1]->get_name ());
              break;
            case 8:
              start_input_string (24, 8, 6,
                                  handler_players::players_list[2]->get_name ());
              break;
            case 9:
              start_input_string (24, 9, 6,
                                  handler_players::players_list[3]->get_name ());
              break;
            case 10:
              start_input_string (24, 10, 6,
                                  handler_players::players_list[4]->get_name ());
              break;
            case 11:
              start_input_string (24, 11, 6,
                                  handler_players::players_list[5]->get_name ());
              break;

              //
            case 12:
              clear_zone_start (24, 12, 4, 1);
              difficulty_level += incre;
              if (difficulty_level > DIFFICULTY_HARD)
                {
                  difficulty_level = DIFFICULTY_EASY;
                }
              if (difficulty_level < DIFFICULTY_EASY)
                {
                  difficulty_level = DIFFICULTY_HARD;
                }
              update_strings ();
              break;

            case 13:
              clear_zone_start (24, 13, 2, 1);
              initial_num_of_lifes += incre;
              if (initial_num_of_lifes > 9)
                {
                  initial_num_of_lifes = 1;
                }
              if (initial_num_of_lifes < 1)
                {
                  initial_num_of_lifes = 9;
                }
              update_strings ();
              break;

              /* return to main menu */
            case 14:
              clear_text_offscreen ();
              set_current_menu_section(MAIN_SECTION);
              clear_zone_stop ();
              break;
            }
          break;

        case ABOUT_SECTION:
          clear_text_offscreen ();
          set_current_menu_section(MAIN_SECTION);
          break;

        case INFOS_SECTION:
          clear_text_offscreen ();
          set_current_menu_section(MAIN_SECTION);
          break;

        case GREETINGS_SECTION:
          clear_text_offscreen ();
          set_current_menu_section(MAIN_SECTION);
          break;

        case SCORE_SECTIONS:
          clear_text_offscreen ();
          set_current_menu_section(MAIN_SECTION);
          break;
        }
    }
  return exit_code;
}

/**
 * Update strings menu: area code, player names, current difficulty, and
 * the number of lifes
 */
void
sprite_display_menu::update_strings ()
{
  const char *source;
  char *dest;

  /* copy current area code */
  dest = texts_of_menus[MAIN_SECTION + LINE_CODE2] + 10;
  supervisor_main_menu::copy_current_area_code (dest);

  /* copy number of players */
  dest = texts_of_menus[OPTIONS_SECTION + 5] + 24;
  integer_to_ascii (number_of_players, 1, dest);

  /* copy player names */
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      dest = texts_of_menus[OPTIONS_SECTION + 6 + i] + 24;
      source = handler_players::players_list[i]->get_name ();
      for (Uint32 j = 0; j < handler_players::PLAYER_NAME_LENGTH; j++)
        {
          dest[j] = source[j];
        }
    }

  /* copy current difficulty level */
  source = &difficulte[(difficulty_level - 1) * 4];
  dest = texts_of_menus[OPTIONS_SECTION + 12] + 24;
  for (Sint32 i = 0; i < 4; i++)
    {
      dest[i] = source[i];
    }
  dest = texts_of_menus[OPTIONS_SECTION + 13] + 24;
  integer_to_ascii (initial_num_of_lifes, 2, dest);

  /* check if the first part of the cheat code is enabled */
  birth_flag = true;
  for (Uint32 i = 0; i < handler_players::MAX_OF_PLAYERS; i++)
    {
      source = handler_players::players_list[i]->get_name ();
      if (source[0] != '0' || source[1] != '4' || source[2] != '0' ||
          source[3] != '6' || source[4] != '7' || source[5] != '0')
        {
          birth_flag = false;
        }
    }
}

/**
 * Erase the entirety of the offscreen before drawing a new menu section 
 */
void
sprite_display_menu::clear_text_offscreen ()
{
  text_offscreen->clear ();
}

/**
 * Clear a zone of the menu (used to modify a string of the menu)
 */
void
sprite_display_menu::clear_input_zone ()
{
  if (!is_clean_zone)
    {
      return;
    }
  text_offscreen->clear (0, clear_zone_xcoord, clear_zone_ycoord,
                         clear_zone_width, clear_zone_height);
}

/**
 * Draw the input string cursor
 */
void
sprite_display_menu::draw_input_cursor ()
{
  if (!is_clean_zone)
    {
      return;
    }
  Sint32 xcurs = keyboard->get_input_cursor_pos ();
  if (xcurs < 0)
    {
      return;
    }
  if (--blink_cursor_delay == 0)
    {
      blink_cursor_delay = 50;
    }
  if (blink_cursor_delay > 30)
    {
      return;
    }
  unsigned char z = 0xEE;
  unsigned char *d = (unsigned char *)
    text_offscreen->get_pixel_data (clear_zone_xcoord + xcurs * font_width,
                                    clear_zone_ycoord);
  Uint32 n = row_size;
  for (Uint32 h = 0; h < font_height; h++)
    {
      for (Uint32 w = 0; w < font_width; w++)
        {
          if (0 == d[w])
            {
              d[w] = z;
            }
        }
      d += n;
    }
}

/**
 * Initialize a string to input
 * @param xcoord X-coordinate in the offscreen menu
 * @param ycoord Y-coordinate in the offscreen menu
 * @param width Number of columns  
 * @parm str Pointer to the string to input
 */
void
sprite_display_menu::start_input_string (Uint32 xcoord, Uint32 ycoord, Uint32 width,
    char *str)
{
  clear_zone_start (xcoord, ycoord, width, 1);
  if (str == NULL)
    {
      return;
    }
  blink_cursor_delay = 50;
  keyboard->set_input_string (str, width);
}

/**
 * Initialize a zone to clear; used to modify a string of the menu
 * @param xcoord X-coordinate in the offscreen menu
 * @param ycoord Y-coordinate in the offscreen menu
 * @param width Number of columns  
 * @param height Number of lines 
 */
void
sprite_display_menu::clear_zone_start (Uint32 xcoord, Uint32 ycoord, Uint32 width,
                                       Uint32 height)
{
  clear_zone_stop ();
  is_clean_zone = true;
  clear_zone_width = (width * font_width);
  clear_zone_height = height * font_height;
  clear_zone_xcoord = xcoord * font_width;
  clear_zone_ycoord = ycoord * line_spacing;
}

/**
 * Disable the cleaning of a zone"
 */
void
sprite_display_menu::clear_zone_stop ()
{
  is_clean_zone = false;
  keyboard->stop_string_input ();
}

/**
 * Copy high score table into the menu
 */
void
sprite_display_menu::copy_high_score_in_menu ()
{
  player_score *score = high_score->get_high_score_table ();
  if (NULL == score)
    {
      return;
    }
  for (Uint32 i = 0; i < handler_high_score::MAX_OF_HIGH_SCORES; i++)
    {
      char *dest = texts_of_menus[SCORE_SECTIONS + 6 + i];
      char *name = score[i].player_name;
      for (Uint32 j = 0; j < handler_players::PLAYER_NAME_LENGTH; j++)
        {
          dest[6 + j] = name[j];
        }
      integer_to_ascii (score[i].value, 6, &dest[24]);
      integer_to_ascii (score[i].area_number, 1, &dest[19]);
      integer_to_ascii (score[i].level_number, 2, &dest[13]);
    }
}

//------------------------------------------------------------------------------
// strings of the main menu
//------------------------------------------------------------------------------
char const
sprite_display_menu::difficulte[] = "EASY" "HARD" "MAD " "DEAD";


/**
 * Offsets of the first 128 ASCII characters 
 */
char sprite_display_menu::ascii_to_index[128] =
  { 26,  // 32 ' '
    37,  // 33 '!'
    40,  // 34 '"'
    26,  // 35 '#' space
    26,  // 36 '$' space
    26,  // 37 '%' space
    26,  // 38 '&' space
    40,  // 39 "'"
    48,  // 40 '('
    49,  // 41 ')'
    47,  // 42 '*'
    26,  // 43 '+' space
    26,  // 44 ','
    41,  // 45 '-'
    42,  // 46 '.'
    26,  // 47 '/' space
    27,  // 48 '0'
    28,  // 49 '1'
    29,  // 50 '2'
    30,  // 51 '3'
    31,  // 52 '4'
    32,  // 53 '5'
    33,  // 54 '6'
    34,  // 55 '7'
    35,  // 56 '8'
    36,  // 57 '9'
    39,  // 58 ':'
    38,  // 59 ';'
    44,  // 60 '<'
    26,  // 61 '=' space
    45,  // 62 '>'
    43,  // 63 '?'
    26,  // 64 '@' space
    0,   // 65 'A'
    1,   // 66 'B'
    2,   // 67 'C'
    3,   // 68 'D'
    4,   // 69 'E'
    5,   // 70 'F'
    6,   // 71 'G'
    7,   // 72 'H'
    8,   // 73 'I'
    9,   // 74 'J'
    10,  // 75 'K'
    11,  // 76 'L'
    12,  // 77 'M'
    13,  // 78 'N'
    14,  // 79 'O'
    15,  // 80 'P'
    16,  // 81 'Q'
    17,  // 82 'R'
    18,  // 83 'S'
    19,  // 84 'T'
    20,  // 85 'U'
    21,  // 86 'V'
    22,  // 87 'W'
    23,  // 88 'X'
    24,  // 89 'Y'
    25,  // 90 'Z'
    48,  // 91 '['
    26,  // 92 '\' space
    49,  // 93 ']'
    26,  // 94 '^' space
    26,  // 95 '_' space
    26,  // 96 '`' space
    26,  // 97 'a' space
    47,  // 98 'b' gray star
    52,  // 99 'c' horizontal lines
    26,  // 100 'd' space
    50,  // 101 'e' full square
    26,  // 102 'f' space
    46,  // 103 'g' gray heart
    26,  // 104 'h' space
    26,  // 105 'i' space
    26,  // 106 'j' space
    26,  // 107 'k' space
    26,  // 108 'l' space
    26,  // 109 'm' space
    26,  // 110 'n' space
    26,  // 111 'o' space
    26,  // 112 'p' space
    55,  // 113 'q' ! white
    26,  // 114 'r' space
    26,  // 115 's' space
    53,  // 116 't' < white
    54,  // 117 'u' > white
    26,  // 118 'v' space
    51,  // 119 'w' pink ellipsis
    26,  // 120 'x' space
    26,  // 121 'y' space
    26,  // 122 'z' space
    26,  // 123 '{' space
    26,  // 124 '|' space
    26,  // 125 '}' space
    26,  // 126 '~' space
    26   // 127 ' ' space
  };

const unsigned char
sprite_display_menu::color_cycling[] =
  {
    239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
    253, 254, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244,
    243, 242, 241, 240, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248,
    249, 250, 251, 252, 253, 254, 255
  };
