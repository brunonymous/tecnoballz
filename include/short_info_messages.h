/**
 * @file short_info_messages.h 
 * @brief Display short info messages in bricks levels 
 * @created 2002-12-30
 * @date 2007-09-28
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
#ifndef __SHORT_INFO_MESSAGES__
#define __SHORT_INFO_MESSAGES__
class short_info_messages;
#include "../include/handler_display.h"
#include "../include/tecnoballz.h"
#include "../include/bitmap_data.h"

class short_info_messages:public virtual tecnoballz
  {
  public:
    typedef enum
    {
      ARE_YOU_READY,
      YEAH_YOU_WHERE,
      SIMPLE_FIRE,
      COOL_FIRE,
      COOL_GUY,
      LOST_FILE,
      WIN_LIFE,
      TOO_LATE,
      BUDGET_PRICES,
      GLUE_OPTION,
      NEXT_LEVEL,
      SMALL_PADDLE,
      BIG_PADDLE,
      EXTRA_BALLS,
      MULTI_BALLS,
      POWERBALLS,
      MEGA_POWERBALLS,
      INVERSE_COMMANDS,
      MAXIMUM_ACCELERATION,
      RIGHT_PADDLE,
      TOP_PADDLE,
      LEFT_PADDLE,
      BIG_BALLS,
      HUGE_BALLS,
      MAXIMUM_OPTIONS,
      WALL_ENABLE,
      ROBOT_ENABLE,
      CONTROL_BALLS,
      MAX_OF_MESSAGES
    } MESSAGES_ENUM;

  private:
    static short_info_messages *info_messages_singleton;
    /** All message strings loaded from a text file */
    char **messages;
    /** Bitmap used to restore the background under message,
     * if has_background == true */
    bitmap_data *restore_surface;
    /** Pointer to the pixels from the restore surface */
    char *restore_pixels;
    static const Uint32 XCOORD_MESSAGE = 96;
    static const Uint32 YCOORD_MESSAGE = 37;
    /** Maximum number of chars of a message */
    static const Uint32 MAX_OF_CHARS = 16;
    /** Time delay counter, if < 5 then clear the message */
    Uint32 delay_counter;
    /** Current char to draw from 0 to 15 */
    Sint32 current_char;
    /** Current required message string */
    const char *required_message;
    /** Modulo destination in the game offscreen */
    Sint32 dest_offset;
    /** Modulo source in the font bitmap */
    Sint32 src_offset;
    /** Font bitmap used to draw a message */
    bitmap_data *bitmap_small_fonts;
    /** Pointer to the pixels of the font bitmap */
    char *font_pixels;
    /** Pointer to the message in the game offscreen */
    char *screen;
    /** Pointer to the message in the background offscreen */
    char *background;
    /** Height of a character in pixels */
    Uint32 height_font;
    /** Width of a character in pixels, 4 or 8 */
    Uint32 width_font;
    /** Width of message's surface 64 or 128 pixels */
    Uint32 width_surface;
    bool messages_request[MAX_OF_MESSAGES];

  private:
    short_info_messages ();
    void init_with_background ();
    void draw ();
    void display_in_background ();
    void display (Uint32 height);
    void clear_message ();
  public:
    ~short_info_messages ();
    static short_info_messages *get_instance ();
    void clear_messages_request ();
    void initialize ();
    void send_message_request (Uint32 id);
    void run ();
  };
#endif
