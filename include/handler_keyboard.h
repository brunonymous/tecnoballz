/**
 * @file handler_keyboard.h
 * @brief Handler of the keyboard and mouse
 * @date 2012-10-13 
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
#ifndef __HANDLER_KEYBOARD__
#define __HANDLER_KEYBOARD__

#include "../include/tecnoballz.h"

class handler_keyboard:public virtual tecnoballz
  {
  public:
    static const Sint32 NULL_YCOORD = -10240;

  private:
#ifdef TECNOBALLZ_GP2X
    /* GP2X button codes, as received through SDL joystick events */
    typedef enum
    {
      GP2X_BUTTON_UP,
      GP2X_BUTTON_UPLEFT,
      GP2X_BUTTON_LEFT,
      GP2X_BUTTON_DOWNLEFT,
      GP2X_BUTTON_DOWN,
      GP2X_BUTTON_DOWNRIGHT,
      GP2X_BUTTON_RIGHT,
      GP2X_BUTTON_UPRIGHT,
      GP2X_BUTTON_START,
      GP2X_BUTTON_SELECT,
      GP2X_BUTTON_R,
      GP2X_BUTTON_L,
      GP2X_BUTTON_A,
      GP2X_BUTTON_B,
      GP2X_BUTTON_Y,
      GP2X_BUTTON_X,
      GP2X_BUTTON_VOLUP,
      GP2X_BUTTON_VOLDOWN,
      GP2X_BUTTON_CLICK,
      GP2X_NUM_BUTTONS
    } GP2X_BUTTONS_CODE;
    /* The current state of all the GP2X buttons is stored in
     * this array - used to handle multi-key actions */
    static bool gp2x_buttons[GP2X_NUM_BUTTONS];
#endif
#ifdef TECNOBALLZ_PSP
    /* PSP button codes, as received through SDL joystick events */
    typedef enum
    {
      PSP_BUTTON_Y,
      PSP_BUTTON_B,
      PSP_BUTTON_A,
      PSP_BUTTON_X,
      PSP_BUTTON_L,
      PSP_BUTTON_R,
      PSP_BUTTON_DOWN,
      PSP_BUTTON_LEFT,
      PSP_BUTTON_UP,
      PSP_BUTTON_RIGHT,
      PSP_BUTTON_SELECT,
      PSP_BUTTON_START,
      PSP_NUM_BUTTONS
    } PSP_BUTTONS_CODE;
    static bool psp_buttons[PSP_NUM_BUTTONS];
#endif

    typedef enum
    {
      IJOY_LEFT = 1,
      IJOY_RIGHT,
      IJOY_TOP,
      IJOY_DOWN,
      IJOY_FIRE,
      IJOY_RELEASE,
      IJOY_GIGABLITZ,
      IJOY_OPTION
    }
    JOYCODE_ENUM;

  public:
    typedef enum
    {
      COMMAND_KEY_PAUSE,
      QUIT_TECNOBALLZ,
      QUIT_TO_MAIN_MENU,
      CAUSE_GAME_OVER,
      TOGGLE_AUDIO,
      TOGGLE_SOUND,
      TOGGLE_MUSIC,
      TOGGLE_FULLSCREEN,
      TOGGLE_POPUP_MENU,
      DISABLE_TIMER,
      VOLUME_DOWN,
      VOLUME_UP,
      NUMOF_COMMAND_KEYS
    }
    COMAND_KEYS_ENUM;

    typedef enum
    {
      K_LEFT,
      K_RIGHT,
      K_UP,
      K_DOWN,
      K_RELEASE_BALL,
      K_FIRE,
      K_GIGABLITZ,
      K_VOLUME_UP,
      K_VOLUME_DOWN,
      K_MAXOF
    } KEY_CODES_ENUM;

  private:
    static bool last_command_keys[NUMOF_COMMAND_KEYS];
    static bool command_keys[NUMOF_COMMAND_KEYS];
    /** Predefinded keys to control the paddle */
    static Uint32 key_codes[K_MAXOF];
    static handler_keyboard *keyboard_singleton;

  private:
    /** True if grabs mouse and keyboard input */
    bool is_grab_input;
    /** Number of available joysticks */
    Uint32 numof_joysticks;
    SDL_Joystick **sdl_joysticks;
    bool joy_fire;
    bool joy_release;
    bool joy_gigablitz;
    bool joy_option;
    bool joy_left;
    bool joy_right;
    bool joy_top;
    bool joy_down;

    Uint32 joy_code_down;
    Uint32 joy_code_down_prev;
    Uint32 joy_up;
    Sint32 input_joy_tempo;

    Sint32 mouse_x_offset;
    Sint32 mouse_y_offset;

    /* x and y coordinates of the mouse pointer */
    Sint32 mouse_x_coord;
    Sint32 mouse_y_coord;

    bool is_left_button_down;
    bool is_right_button_down;
    bool is_middle_button_down;
    bool is_left_button_released;
    bool is_right_button_released;

    /* keyboard input delay */
    Sint32 key_delay;
    Uint32 key_code_down;
    Uint32 previous_key_code_down;
    Uint32 code_keyup;
    /* cursor position into input string */
    Sint32 string_cursor_pos;
    /* input string size */
    Sint32 string_input_size;
    /* pointer to the current input string */
    char *current_input_string;

    bool is_key_waiting;
    Sint32 wait_key_pressed;
  private:
    handler_keyboard ();
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
    void handle_console_buttons (SDL_Event * event);
#endif
  public:
    ~handler_keyboard ();
    static handler_keyboard *get_instance ();

    void set_grab_input (bool mode);
    void read_events ();
    void clear_command_keys ();
    bool command_is_pressed (Uint32 code, bool clear = false);
    bool is_left_button ();
    bool is_right_button ();
    bool is_gigablitz_or_tilt ();
    //bool is_right_left_buttons ();
    bool is_left_button_up (Sint32 * off_x, Sint32 * off_y);
    bool is_right_button_up (Sint32 * off_x, Sint32 * off_y);
    Sint32 get_mouse_x_offset ();
    Sint32 get_mouse_y_offset ();
    Sint32 get_mouse_x ();
    Sint32 get_mouse_y ();
    bool key_is_pressed (Sint32 code);
    bool key_is_released (Sint32 code);
    bool control_is_pressed (Uint32 code);
    void set_input_string (char *str, Uint32 size);
    Sint32 get_input_cursor_pos ();
    void stop_string_input ();
    Uint32 get_key_down_code ();
    bool wait_key ();

  private:
    void toggle_popup_menu ();
    void toggle_pause ();
    void init_joysticks ();
    void set_joy (Uint32 code);
    void clr_joy (Uint32 code);
    void input_string ();
    void input_string (Uint32 code);
    void set_key_code_down (Uint32 code);
    void set_keycode_up (Uint32 code);
  };
#endif
