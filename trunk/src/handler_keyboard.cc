/**
 * @file handler_keyboard.cc
 * @brief Handler of the keyboard and mouse
 * @date 2012-09-04 
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
#include "../include/handler_keyboard.h"
#include "../include/handler_display.h"

bool
handler_keyboard::command_keys[NUMOF_COMMAND_KEYS];
bool
handler_keyboard::last_command_keys[NUMOF_COMMAND_KEYS];
Uint32
handler_keyboard::key_codes[K_MAXOF] =
{
  SDLK_LEFT,
  SDLK_RIGHT,
  SDLK_UP,
  SDLK_DOWN,
  SDLK_SPACE,
  SDLK_LCTRL,
  SDLK_LALT,
  SDLK_PAGEUP,
  SDLK_PAGEDOWN
};
handler_keyboard *
handler_keyboard::keyboard_singleton = NULL;

#ifdef TECNOBALLZ_GP2X
bool
handler_keyboard::gp2x_buttons[GP2X_NUM_BUTTONS];
#endif
#ifdef TECNOBALLZ_PSP
bool
handler_keyboard::psp_buttons[PSP_NUM_BUTTONS];
#endif

/**
 * Create the handler_keyboard singleton object, and clear members
 */
handler_keyboard::handler_keyboard ()
{
  object_init ();
  is_grab_input = false;
  previous_mouse_x_coord = 0;
  previous_mouse_y_coord = 0;
  mouse_x_offset = 0;
  mouse_y_offset = 0;
  /* x and y coordinates of the mouse */
  mouse_x_coord = 0;
  mouse_y_coord = 0;
  is_left_button_down = false;
  is_right_button_down = false;
  is_middle_button_down = false;
  is_left_button_released = 0;
  is_right_button_released = 0;
  key_delay = 0;
  key_code_down = 0;
  previous_key_code_down = 0;
  string_cursor_pos = 0;
  string_input_size = 0;
  code_keyup = 0;
  current_input_string = NULL;
  for (Uint32 i = 0; i < NUMOF_COMMAND_KEYS; i++)
    {
      command_keys[i] = false;
      last_command_keys[i] = false;
    }
  init_joysticks ();
  is_key_waiting = true;
  wait_key_pressed = false;
  joy_code_down = 0;
  joy_code_down_prev = 0;
  joy_up = 0;
  input_joy_tempo = 0;
#ifdef TECNOBALLZ_GP2X
  for (Uint32 i = 0; i < GP2X_NUM_BUTTONS; i++)
    {
      gp2x_buttons[i] = false;
    }
#endif
#ifdef TECNOBALLZ_PSP
  for (Uint32 i = 0; i < PSP_NUM_BUTTONS; i++)
    {
      psp_buttons[i] = false;
    }
#endif
}

/**
 * Release the handler_keyboard singleton object
 */
handler_keyboard::~handler_keyboard ()
{
  object_free ();
  keyboard_singleton = NULL;
  if (numof_joysticks > 0)
    {
      for (Uint32 i = 0; i < numof_joysticks; i++)
        {
          if (sdl_joysticks[i] != NULL)
            {
              SDL_JoystickClose (sdl_joysticks[i]);
              sdl_joysticks[i] = NULL;
            }
        }
      delete[]sdl_joysticks;
      numof_joysticks = 0;
    }
}

/**
 * Open joystick(s) if available
 */
void
handler_keyboard::init_joysticks ()
{
  joy_fire = false;
  joy_release = false;
  joy_gigablitz = false;
  joy_option = false;
  joy_left = false;
  joy_right = false;
  joy_top = false;
  joy_down = false;
  numof_joysticks = SDL_NumJoysticks ();
  if (is_verbose)
    {
      std::cout << "handler_keyboard::init_joysticks()" <<
      " number of joysticks available: " << numof_joysticks << std::endl;
    }
  if (numof_joysticks > 0)
    {
      sdl_joysticks = new SDL_Joystick *[numof_joysticks];
      for (Uint32 i = 0; i < numof_joysticks; i++)
        {

          sdl_joysticks[i] = SDL_JoystickOpen (i);
          if (sdl_joysticks[i] == NULL)
            {
              std::cerr << "(!)handler_keyboard::init_joysticks()" <<
              " couldn't open joystick " << i << ": "
              << SDL_GetError () << std::endl;
            }
          else
            {
              if (is_verbose)
                {
                  std::
                  cout << "- joystick  : " << SDL_JoystickName (i) << std::
                  endl;
                  std::
                  cout << "- axes      : " <<
                  SDL_JoystickNumAxes (sdl_joysticks[i]) << std::endl;
                  std::
                  cout << "- buttons   : " <<
                  SDL_JoystickNumButtons (sdl_joysticks[i]) << std::endl;
                  std::
                  cout << "- trackballs: " <<
                  SDL_JoystickNumButtons (sdl_joysticks[i]) << std::endl;
                  std::
                  cout << "- hats      : " <<
                  SDL_JoystickNumHats (sdl_joysticks[i]) << std::endl;
                }
            }
        }
    }
}

/**
 * Get the object instance
 * handler_keyboard is a singleton
 * @return the handler_keyboard object
 */
handler_keyboard *
handler_keyboard::get_instance ()
{
  if (NULL == keyboard_singleton)
    {
      keyboard_singleton = new handler_keyboard ();
    }
  return keyboard_singleton;
}

/**
 * Grabs mouse and keyboard input, the mouse is confined to the
 * application window, and nearly all keyboard input is passed
 * directly to the application
 * @param mode true to enable grab input, false to disable it
 */
#ifdef UNDER_DEVELOPMENT
void
handler_keyboard::set_grab_input (bool)
{
  return;
}
#else
void
handler_keyboard::set_grab_input (bool mode)
{
  if (mode)
    {
      is_grab_input = true;
      SDL_WM_GrabInput (SDL_GRAB_ON);
      SDL_EventState (SDL_MOUSEMOTION, SDL_IGNORE);
      SDL_WarpMouse (display->get_width () >> 1, display->get_height () >> 1);
      SDL_EventState (SDL_MOUSEMOTION, SDL_ENABLE);
    }
  else
    {
      is_grab_input = false;
      SDL_WM_GrabInput (SDL_GRAB_OFF);
    }
}
#endif

/**
 * Handle buttons of handheld video game console
 * @param event Pointer to a SDL_Event structure
 */
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
void
handler_keyboard::handle_console_buttons (SDL_Event * event)
{
#ifdef TECNOBALLZ_GP2X
  if (event->jbutton.button >= GP2X_NUM_BUTTONS)
    {
      return;
    }
  if (event->type == SDL_JOYBUTTONDOWN)
    {
      gp2x_buttons[event->jbutton.button] = true;
      if (event->jbutton.button == GP2X_BUTTON_UP
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT)
        {
          set_joy (IJOY_TOP);
        }
      if (event->jbutton.button == GP2X_BUTTON_DOWN
          || event->jbutton.button == GP2X_BUTTON_DOWNLEFT
          || event->jbutton.button == GP2X_BUTTON_DOWNRIGHT)
        {
          set_joy (IJOY_DOWN);
        }

      if (event->jbutton.button == GP2X_BUTTON_LEFT
          || event->jbutton.button == GP2X_BUTTON_UPLEFT
          || event->jbutton.button == GP2X_BUTTON_DOWNLEFT
          || event->jbutton.button == GP2X_BUTTON_L)
        {
          set_joy (IJOY_LEFT);
        }

      if (event->jbutton.button == GP2X_BUTTON_RIGHT
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT
          || event->jbutton.button == GP2X_BUTTON_DOWNRIGHT
          || event->jbutton.button == GP2X_BUTTON_R)
        {
          set_joy (IJOY_RIGHT);
        }
      if (event->jbutton.button == GP2X_BUTTON_A
          || event->jbutton.button == GP2X_BUTTON_B
          || event->jbutton.button == GP2X_BUTTON_CLICK)
        {
          set_joy (IJOY_FIRE);
        }
      if (event->jbutton.button == GP2X_BUTTON_X
          || event->jbutton.button == GP2X_BUTTON_Y)
        {
          set_joy (IJOY_OPTION);
        }
      if (event->jbutton.button == GP2X_BUTTON_SELECT)
        {
          last_command_keys[TOGGLE_POPUP_MENU] = true;
        }
      if (event->jbutton.button == GP2X_BUTTON_START)
        {
          last_command_keys[COMMAND_KEY_PAUSE] = true;
        }
      if (event->jbutton.button == GP2X_BUTTON_VOLUP)
        {
          last_command_keys[VOLUME_UP] = true;
        }
      if (event->jbutton.button == GP2X_BUTTON_VOLDOWN)
        {
          last_command_keys[VOLUME_DOWN] = true;
        }
    }
  else
    {
      gp2x_buttons[event->jbutton.button] = false;
      if (event->jbutton.button == GP2X_BUTTON_UP
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT)
        {
          clr_joy (IJOY_TOP);
        }
      if (event->jbutton.button == GP2X_BUTTON_DOWN
          || event->jbutton.button == GP2X_BUTTON_DOWNLEFT
          || event->jbutton.button == GP2X_BUTTON_DOWNRIGHT)
        {
          clr_joy (IJOY_DOWN);
        }
      if (event->jbutton.button == GP2X_BUTTON_LEFT
          || event->jbutton.button == GP2X_BUTTON_UPLEFT
          || event->jbutton.button == GP2X_BUTTON_DOWNLEFT
          || event->jbutton.button == GP2X_BUTTON_L)
        {
          clr_joy (IJOY_LEFT);
        }
      if (event->jbutton.button == GP2X_BUTTON_RIGHT
          || event->jbutton.button == GP2X_BUTTON_UPRIGHT
          || event->jbutton.button == GP2X_BUTTON_DOWNRIGHT
          || event->jbutton.button == GP2X_BUTTON_R)
        {
          clr_joy (IJOY_RIGHT);
        }
      if (event->jbutton.button == GP2X_BUTTON_A
          || event->jbutton.button == GP2X_BUTTON_B
          || event->jbutton.button == GP2X_BUTTON_CLICK)
        {
          clr_joy (IJOY_FIRE);
        }
      if (event->jbutton.button == GP2X_BUTTON_X
          || event->jbutton.button == GP2X_BUTTON_Y)
        {
          clr_joy (IJOY_OPTION);
        }
      if (event->jbutton.button == GP2X_BUTTON_SELECT
          && last_command_keys[TOGGLE_POPUP_MENU])
        {
          toggle_popup_menu ();
        }
      if (event->jbutton.button == GP2X_BUTTON_START
          && last_command_keys[COMMAND_KEY_PAUSE])
        {
          toggle_pause ();
        }
      if (event->jbutton.button == GP2X_BUTTON_VOLUP
          && last_command_keys[VOLUME_UP])
        {
          last_command_keys[VOLUME_UP] = false;
          command_keys[VOLUME_UP] = true;
        }
      if (event->jbutton.button == GP2X_BUTTON_VOLDOWN
          && last_command_keys[VOLUME_DOWN])
        {
          last_command_keys[VOLUME_DOWN] = false;
          command_keys[VOLUME_DOWN] = true;
        }
    }
  /* This button mapping conforms to the GP2X Common User Interface
     Recommendations, as of 2006-07-29, available from
     http://wiki.gp2x.org/wiki/Common_User_Interface_Recommendations */

  /* Directions (UDLR) */
  joy_top = gp2x_buttons[GP2X_BUTTON_UP]
            | gp2x_buttons[GP2X_BUTTON_UPLEFT] | gp2x_buttons[GP2X_BUTTON_UPRIGHT];
  joy_down = gp2x_buttons[GP2X_BUTTON_DOWN]
             | gp2x_buttons[GP2X_BUTTON_DOWNLEFT]
             | gp2x_buttons[GP2X_BUTTON_DOWNRIGHT];
  joy_left = gp2x_buttons[GP2X_BUTTON_LEFT]
             | gp2x_buttons[GP2X_BUTTON_UPLEFT]
             | gp2x_buttons[GP2X_BUTTON_DOWNLEFT] | gp2x_buttons[GP2X_BUTTON_L];
  joy_right = gp2x_buttons[GP2X_BUTTON_RIGHT]
              | gp2x_buttons[GP2X_BUTTON_UPRIGHT]
              | gp2x_buttons[GP2X_BUTTON_DOWNRIGHT] | gp2x_buttons[GP2X_BUTTON_R];

  /* volume ctrl */
  joy_fire = gp2x_buttons[GP2X_BUTTON_A] | gp2x_buttons[GP2X_BUTTON_CLICK];
  joy_release = gp2x_buttons[GP2X_BUTTON_B];
  joy_gigablitz = gp2x_buttons[GP2X_BUTTON_X];
  joy_option = gp2x_buttons[GP2X_BUTTON_Y];

  /* Quit */
  command_keys[QUIT_TECNOBALLZ] = gp2x_buttons[GP2X_BUTTON_CLICK]
                                  & gp2x_buttons[GP2X_BUTTON_START];
#endif
#ifdef TECNOBALLZ_PSP
  if (event->jbutton.button >= PSP_NUM_BUTTONS)
    {
      return;
    }
  if (event->type == SDL_JOYBUTTONDOWN)
    {
      psp_buttons[event->jbutton.button] = true;
      if (event->jbutton.button == PSP_BUTTON_UP)
        {
          set_joy (IJOY_TOP);
        }
      if (event->jbutton.button == PSP_BUTTON_DOWN)
        {
          set_joy (IJOY_DOWN);
        }
      if (event->jbutton.button == PSP_BUTTON_LEFT)
        {
          set_joy (IJOY_LEFT);
        }
      if (event->jbutton.button == PSP_BUTTON_RIGHT)
        {
          set_joy (IJOY_RIGHT);
        }
      if (event->jbutton.button == PSP_BUTTON_A)
        {
          set_joy (IJOY_FIRE);
        }
      if (event->jbutton.button == PSP_BUTTON_X)
        {
          set_joy (IJOY_OPTION);
        }
      if (event->jbutton.button == PSP_BUTTON_SELECT)
        {
          last_command_keys[TOGGLE_POPUP_MENU] = true;
        }
      if (event->jbutton.button == PSP_BUTTON_START)
        {
          last_command_keys[COMMAND_KEY_PAUSE] = true;
        }
    }
  else
    {
      psp_buttons[event->jbutton.button] = false;
      if (event->jbutton.button == PSP_BUTTON_UP)
        {
          clr_joy (IJOY_TOP);
        }
      if (event->jbutton.button == PSP_BUTTON_DOWN)
        {
          clr_joy (IJOY_DOWN);
        }
      if (event->jbutton.button == PSP_BUTTON_LEFT)
        {
          clr_joy (IJOY_LEFT);
        }
      if (event->jbutton.button == PSP_BUTTON_RIGHT)
        {
          clr_joy (IJOY_RIGHT);
        }
      if (event->jbutton.button == PSP_BUTTON_A)
        {
          clr_joy (IJOY_FIRE);
        }
      if (event->jbutton.button == PSP_BUTTON_X)
        {
          clr_joy (IJOY_OPTION);
        }
      if (event->jbutton.button == PSP_BUTTON_SELECT
          && last_command_keys[TOGGLE_POPUP_MENU])
        {
          toggle_popup_menu ();
        }
      if (event->jbutton.button == PSP_BUTTON_START
          && last_command_keys[COMMAND_KEY_PAUSE])
        {
          toggle_pause ();
        }
    }

  /* Directions (UDLR) */
  joy_top = psp_buttons[PSP_BUTTON_UP];
  joy_down = psp_buttons[PSP_BUTTON_DOWN];
  joy_left = psp_buttons[PSP_BUTTON_LEFT];
  joy_right = psp_buttons[PSP_BUTTON_RIGHT];
  joy_fire = psp_buttons[PSP_BUTTON_A];
  joy_release = psp_buttons[PSP_BUTTON_B];
  joy_gigablitz = psp_buttons[PSP_BUTTON_X];
  joy_option = psp_buttons[PSP_BUTTON_Y];
#endif
}
#endif

/**
 * Request to enable or disable popup menu
 */
void
handler_keyboard::toggle_popup_menu ()
{
  last_command_keys[TOGGLE_POPUP_MENU] = false;
  command_keys[TOGGLE_POPUP_MENU] =
    command_keys[TOGGLE_POPUP_MENU] ? false : true;
  command_keys[COMMAND_KEY_PAUSE] = command_keys[TOGGLE_POPUP_MENU];
  if (is_grab_input && command_keys[COMMAND_KEY_PAUSE])
    {
      SDL_WM_GrabInput (SDL_GRAB_OFF);
    }
  if (is_grab_input && !command_keys[COMMAND_KEY_PAUSE])
    {
      SDL_WM_GrabInput (SDL_GRAB_ON);
    }

  if (!command_keys[TOGGLE_POPUP_MENU])
    {
      SDL_ShowCursor (SDL_DISABLE);
    }
  if (command_keys[TOGGLE_POPUP_MENU])
    {
      SDL_ShowCursor (SDL_ENABLE);
    }
}

/**
 * Request to enable or disable pause mode
 */
void
handler_keyboard::toggle_pause ()
{
  last_command_keys[COMMAND_KEY_PAUSE] = false;
  if (!command_keys[TOGGLE_POPUP_MENU])
    {
      command_keys[COMMAND_KEY_PAUSE] =
        command_keys[COMMAND_KEY_PAUSE] ? false : true;
      if (is_grab_input && command_keys[COMMAND_KEY_PAUSE])
        {
          SDL_WM_GrabInput (SDL_GRAB_OFF);
        }
      if (is_grab_input && !command_keys[COMMAND_KEY_PAUSE])
        {
          SDL_WM_GrabInput (SDL_GRAB_ON);
        }
    }
}

/**
 * Read keyboard mouse and exit events
 */
void
handler_keyboard::read_events ()
{
  command_keys[TOGGLE_FULLSCREEN] = false;
  command_keys[DISABLE_TIMER] = false;
  command_keys[VOLUME_DOWN] = false;
  command_keys[VOLUME_UP] = false;
  is_left_button_released = 0;
  is_right_button_released = 0;
  SDL_Event event;
  SDL_KeyboardEvent *kbEvt;

  while (SDL_PollEvent (&event) > 0)
    {
      switch (event.type)
        {
          /* mouse motion event */
        case SDL_MOUSEMOTION:
        {
          /* read the x and y coordinates of the mouse */
          mouse_x_coord = event.motion.x;
          mouse_y_coord = event.motion.y;
        }
        break;

        /* a key is pressed */
        case SDL_KEYDOWN:
        {
          kbEvt = (SDL_KeyboardEvent *) & event;
          Uint8 *keys = SDL_GetKeyState (NULL);

          if (keys[SDLK_RALT] == SDL_RELEASED &&
              keys[SDLK_LALT] == SDL_RELEASED &&
              keys[SDLK_LSHIFT] == SDL_RELEASED &&
              keys[SDLK_RSHIFT] == SDL_RELEASED)
            {

              if (keys[SDLK_RCTRL] == SDL_RELEASED &&
                  keys[SDLK_LCTRL] == SDL_RELEASED)
                {
                  if (keys[SDLK_p] == SDL_PRESSED)
                    {
                      last_command_keys[COMMAND_KEY_PAUSE] = true;
                    }
                  if (keys[SDLK_f] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_FULLSCREEN] = true;
                    }
                  if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_POPUP_MENU] = true;
                    }
                  if (keys[SDLK_l] == SDL_PRESSED)
                    {
                      last_command_keys[DISABLE_TIMER] = true;
                    }
                  if (keys[SDLK_PAGEDOWN] == SDL_PRESSED)
                    {
                      last_command_keys[VOLUME_DOWN] = true;
                    }
                  if (keys[SDLK_PAGEUP] == SDL_PRESSED)
                    {
                      last_command_keys[VOLUME_UP] = true;
                    }
                }
              else
                {
                  if (keys[SDLK_ESCAPE] == SDL_PRESSED)
                    {
                      last_command_keys[QUIT_TECNOBALLZ] = true;
                    }
                  if (keys[SDLK_x] == SDL_PRESSED)
                    {
                      last_command_keys[CAUSE_GAME_OVER] = true;
                    }
                  if (keys[SDLK_q] == SDL_PRESSED)
                    {
                      last_command_keys[QUIT_TO_MAIN_MENU] = true;
                    }
                  if (keys[SDLK_f] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_SOUND] = true;
                    }
                  if (keys[SDLK_s] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_AUDIO] = true;
                    }
                  if (keys[SDLK_d] == SDL_PRESSED)
                    {
                      last_command_keys[TOGGLE_MUSIC] = true;
                    }
                }
            }

          if (kbEvt->keysym.unicode > 0)
            {
              set_key_code_down (kbEvt->keysym.unicode);
            }
          else
            {
              set_key_code_down (kbEvt->keysym.sym);
            }
        }
        break;

        /* a key is released */
        case SDL_KEYUP:
        {
          kbEvt = (SDL_KeyboardEvent *) & event;
          Uint8 *keys = SDL_GetKeyState (NULL);


          /* enable pause [P] key */
          if (keys[SDLK_p] == SDL_RELEASED
              && last_command_keys[COMMAND_KEY_PAUSE])
            {
              toggle_pause ();
            }

          /* enable context menu [ESC] key */
          if (keys[TOGGLE_POPUP_MENU] == SDL_RELEASED
              && last_command_keys[TOGGLE_POPUP_MENU])
            {
              toggle_popup_menu ();
            }

          if (keys[SDLK_f] == SDL_RELEASED
              && last_command_keys[TOGGLE_FULLSCREEN])
            {
              last_command_keys[TOGGLE_FULLSCREEN] = false;
              command_keys[TOGGLE_FULLSCREEN] = true;
            }
          {
            if (keys[SDLK_ESCAPE] == SDL_RELEASED
                && last_command_keys[QUIT_TECNOBALLZ])
              {
                last_command_keys[QUIT_TECNOBALLZ] = false;
                command_keys[QUIT_TECNOBALLZ] = true;
              }
            if (keys[SDLK_x] == SDL_RELEASED
                && last_command_keys[CAUSE_GAME_OVER])
              {
                last_command_keys[CAUSE_GAME_OVER] = false;
                command_keys[CAUSE_GAME_OVER] = true;
              }
            if (keys[SDLK_q] == SDL_RELEASED
                && last_command_keys[QUIT_TO_MAIN_MENU])
              {
                last_command_keys[QUIT_TO_MAIN_MENU] = false;
                command_keys[QUIT_TO_MAIN_MENU] = true;
              }
            if (keys[SDLK_f] == SDL_RELEASED
                && last_command_keys[TOGGLE_SOUND])
              {
                last_command_keys[TOGGLE_SOUND] = false;
                command_keys[TOGGLE_SOUND] = true;
              }
            if (keys[SDLK_s] == SDL_RELEASED
                && last_command_keys[TOGGLE_AUDIO])
              {
                last_command_keys[TOGGLE_AUDIO] = false;
                command_keys[TOGGLE_AUDIO] = true;
              }
            if (keys[SDLK_d] == SDL_RELEASED
                && last_command_keys[TOGGLE_MUSIC])
              {
                last_command_keys[TOGGLE_MUSIC] = false;
                command_keys[TOGGLE_MUSIC] = true;
              }
            if (keys[SDLK_l] == SDL_RELEASED
                && last_command_keys[DISABLE_TIMER])
              {
                last_command_keys[DISABLE_TIMER] = false;
                command_keys[DISABLE_TIMER] = true;
              }
            if (keys[SDLK_PAGEDOWN] == SDL_RELEASED
                && last_command_keys[VOLUME_DOWN])
              {
                last_command_keys[VOLUME_DOWN] = false;
                command_keys[VOLUME_DOWN] = true;
              }
            if (keys[SDLK_PAGEUP] == SDL_RELEASED
                && last_command_keys[VOLUME_UP])
              {
                last_command_keys[VOLUME_UP] = false;
                command_keys[VOLUME_UP] = true;
              }

          }

          if (kbEvt->keysym.unicode > 0)
            {
              set_keycode_up (kbEvt->keysym.unicode);
            }
          else
            {
              set_keycode_up (kbEvt->keysym.sym);
            }
        }
        break;

        /* a mouse button is pressed */
        case SDL_MOUSEBUTTONDOWN:
        {
          switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
              is_left_button_down = true;
              break;
            case SDL_BUTTON_RIGHT:
              is_right_button_down = true;
              break;
            case SDL_BUTTON_MIDDLE:
              is_middle_button_down = true;
              break;
            }
        }
        break;

        /* a mouse button is relased */
        case SDL_MOUSEBUTTONUP:
        {
          switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
              if (is_left_button_down)
                {
                  is_left_button_released = true;
                }
              is_left_button_down = false;
              break;
            case SDL_BUTTON_RIGHT:
              if (is_right_button_down)
                {
                  is_right_button_released = true;
                }
              is_right_button_down = false;
              break;
            case SDL_BUTTON_MIDDLE:
              is_middle_button_down = false;
              break;
            }
        }
        break;

        case SDL_JOYAXISMOTION:
        {
          Sint32 deadzone = 4096;
          /* x axis */
          if (event.jaxis.axis == 0)
            {
              if (event.jaxis.value < -deadzone)
                {
                  joy_left = true;
                  set_joy (IJOY_LEFT);
                  joy_right = false;
                }
              else if (event.jaxis.value > deadzone)
                {
                  joy_left = false;
                  joy_right = true;
                  set_joy (IJOY_RIGHT);
                }
              else
                {
                  joy_left = false;
                  joy_right = false;
                  clr_joy (IJOY_RIGHT);
                  clr_joy (IJOY_LEFT);
                }
            }
          /* y axis */
          else if (event.jaxis.axis == 1)
            {
              if (event.jaxis.value < -deadzone)
                {
                  joy_down = false;
                  joy_top = true;
                  set_joy (IJOY_TOP);
                }
              else if (event.jaxis.value > deadzone)
                {
                  joy_down = true;
                  joy_top = false;
                  set_joy (IJOY_DOWN);
                }
              else
                {
                  joy_down = false;
                  joy_top = false;
                  clr_joy (IJOY_TOP);
                  clr_joy (IJOY_DOWN);
                }
            }
        }
        break;
        case SDL_JOYBUTTONDOWN:
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
          handle_console_buttons (&event);
#else
          if (event.jbutton.button == 2)
            {
              joy_gigablitz = true;
              set_joy (IJOY_GIGABLITZ);
            }
          else if (event.jbutton.button == 0)
            {
              joy_fire = true;
              set_joy (IJOY_FIRE);
            }
          else if (event.jbutton.button == 1)
            {
              joy_release = true;
              set_joy (IJOY_RELEASE);
            }
          else if (event.jbutton.button > 2)
            {
              joy_option = true;
              last_command_keys[TOGGLE_POPUP_MENU] = true;
              set_joy (IJOY_OPTION);
            }
          break;
#endif
        case SDL_JOYBUTTONUP:
#ifdef TECNOBALLZ_HANDHELD_CONSOLE
          handle_console_buttons (&event);
#else
          if (event.jbutton.button == 2)
            {
              joy_gigablitz = false;
              clr_joy (IJOY_GIGABLITZ);
            }
          else if (event.jbutton.button == 0)
            {
              joy_fire = false;
              clr_joy (IJOY_FIRE);
            }
          else if (event.jbutton.button == 1)
            {
              joy_release = false;
              clr_joy (IJOY_RELEASE);
            }
          else if (event.jbutton.button > 2)
            {
              joy_option = false;
              clr_joy (IJOY_OPTION);
              if (last_command_keys[TOGGLE_POPUP_MENU])
                {
                  last_command_keys[TOGGLE_POPUP_MENU] = false;
                  command_keys[TOGGLE_POPUP_MENU] =
                    command_keys[TOGGLE_POPUP_MENU] ? false : true;
                  command_keys[COMMAND_KEY_PAUSE] =
                    command_keys[TOGGLE_POPUP_MENU];
                }
            }
#endif
          break;

          /* quit the game */
        case SDL_QUIT:
        {
          command_keys[QUIT_TECNOBALLZ] = true;
        }
        break;
        }
    }
  random_counter += mouse_x_coord;
  random_counter += mouse_y_coord;
  input_string ();
}

/**
 * Check if a key is pressed
 * @param code a SDL Keysym definition
 * @return true if the key is pressed
 */
bool
handler_keyboard::key_is_pressed (Sint32 code)
{
  Uint8 *keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[code] == SDL_PRESSED)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Check if a key is released
 * @param code a SDL Keysym definition
 * @return true if the key is released
 */
bool
handler_keyboard::key_is_released (Sint32 code)
{
  Uint8 *keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[code] == SDL_RELEASED)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Check if a control key is pressed
 * @param code A code of key
 * @return true if the key is pressed
 */
bool handler_keyboard::control_is_pressed (Uint32 code)
{
  /*
     if(code >= K_MAXOF)
     {
     std::cerr << "(!)handler_keyboard::control_is_pressed() " <<
     code << " is greated or equal to " << K_MAXOF << std::endl;
     return false;
     }
   */
  Uint8 *
  keys;
  keys = SDL_GetKeyState (NULL);
  if (keys[key_codes[code]] == SDL_PRESSED)
    {
      return true;
    }
  else
    {
      switch (code)
        {
        case K_LEFT:
          return joy_left ? true : false;
          break;
        case K_RIGHT:
          return joy_right ? true : false;
          break;
        case K_UP:
          return joy_top ? true : false;
          break;
        case K_DOWN:
          return joy_down ? true : false;
          break;
        case K_FIRE:
          return joy_fire ? true : false;
          break;
        case K_RELEASE_BALL:
          return joy_release ? true : false;
          break;
        case K_GIGABLITZ:
          return joy_gigablitz ? true : false;
          break;
        }
      return false;
    }
}

/**
 * Clear some command keys and set grab input
 */
void
handler_keyboard::clear_command_keys ()
{
  command_keys[COMMAND_KEY_PAUSE] = false;
  command_keys[QUIT_TO_MAIN_MENU] = false;
  command_keys[CAUSE_GAME_OVER] = false;
  command_keys[TOGGLE_AUDIO] = false;
  command_keys[TOGGLE_SOUND] = false;
  command_keys[TOGGLE_MUSIC] = false;
  command_keys[TOGGLE_POPUP_MENU] = false;
  last_command_keys[COMMAND_KEY_PAUSE] = false;
  last_command_keys[QUIT_TO_MAIN_MENU] = false;
  last_command_keys[CAUSE_GAME_OVER] = false;
  last_command_keys[TOGGLE_AUDIO] = false;
  last_command_keys[TOGGLE_SOUND] = false;
  last_command_keys[TOGGLE_MUSIC] = false;
  last_command_keys[TOGGLE_POPUP_MENU] = false;
  if (is_grab_input)
    {
      SDL_WM_GrabInput (SDL_GRAB_ON);
    }
  previous_mouse_x_coord = mouse_x_coord;
  previous_mouse_y_coord = mouse_y_coord;
  is_key_waiting = true;
  wait_key_pressed = false;
}

/**
 * Check if a command key is pressed
 * @param code of the command key
 * @param clear true if clear flag after read
 * @return true if the command key is pressed
 */
bool
handler_keyboard::command_is_pressed (Uint32 code, bool clear)
{
  bool is_pressed = command_keys[code];
  if (clear)
    {
      command_keys[code] = false;
    }
  return is_pressed;
}

/**
 * Test if mouse left button is pressed
 * @return true if mouse left button is down
 */
bool
handler_keyboard::is_left_button ()
{
  return is_left_button_down;
}

/**
 * Test if mouse right button is pressed
 * @return true if mouse right button is down
 */
bool
handler_keyboard::is_right_button ()
{
  return is_right_button_down;
}


/**
 * Check if player want send a gigablitz or enable tilt
 * @return true if want send a gigablitz or enable tilt
 */
bool handler_keyboard::is_gigablitz_or_tilt ()
{
  if (is_middle_button_down || control_is_pressed (K_GIGABLITZ))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Test if mouse right and left button are pressed (start gigablitz)
 * @return true if mouse right and left buttons are down
 */
/*
bool handler_keyboard::is_right_left_buttons ()
{
  if (is_right_button_down && is_left_button_down)
    {
      return true;
    }
  else
    {
      return false;
    }
}
*/

/**
 * Test if the left mouse button were released
 * @param x_coord pointer to integer in which to return x mouse
 * @param y_coord pointer to integer in which to return y mouse
 * @return true if the left mouse button is released
 */
bool
handler_keyboard::is_left_button_up (Sint32 * x_coord, Sint32 * y_coord)
{
  *x_coord = mouse_x_coord;
  *y_coord = mouse_y_coord;
  return is_left_button_released;
}

/**
 * Test if the right mouse button were released
 * @param x_coord pointer to integer in which to return x mouse
 * @param y_coord pointer to integer in which to return y mouse
 * @return true if the right mouse button is released
 */
bool
handler_keyboard::is_right_button_up (Sint32 * x_coord, Sint32 * y_coord)
{
  *x_coord = mouse_x_coord;
  *y_coord = mouse_y_coord;
  return is_right_button_released;
}

/**
 * Caculate and return offset of horizontal displacement of the mouse
 * @return the mouse horizonal offset
 */
Sint32
handler_keyboard::get_mouse_x_offset ()
{
  mouse_x_offset = mouse_x_coord - previous_mouse_x_coord;
  mouse_y_offset = mouse_y_coord - previous_mouse_y_coord;
  previous_mouse_x_coord = mouse_x_coord;
  previous_mouse_y_coord = mouse_y_coord;
  random_counter += previous_mouse_x_coord;
  random_counter += previous_mouse_y_coord;
  return mouse_x_offset;
}

/**
 * Return absolute mouse y coordinate
 * @return the mouse pointer y coordinate
 */
Sint32
handler_keyboard::get_mouse_y ()
{
  return mouse_y_coord;
}

/**
 * Return absolute mouse x coordinate
 * @return the mouse pointer x coordinate
 */
Sint32
handler_keyboard::get_mouse_x ()
{
  return mouse_x_coord;
}

/**
 * Set new pressed key for handle string input
 * @param kcode key code of the pressed key
 */
void
handler_keyboard::set_key_code_down (Uint32 kcode)
{
  if (kcode != SDLK_LSHIFT && kcode != SDLK_RSHIFT && kcode != SDLK_LCTRL &&
      kcode != SDLK_RCTRL)
    key_code_down = kcode;
}

/**
 * Set new relased key for handle string input
 * @param kcode key code of the pressed key
 */
void
handler_keyboard::set_keycode_up (Uint32 kcode)
{
  if (kcode != SDLK_LSHIFT && kcode != SDLK_RSHIFT && kcode != SDLK_LCTRL &&
      kcode != SDLK_RCTRL)
    {
      code_keyup = kcode;
      if (code_keyup == key_code_down)
        {
          key_code_down = 0;
        }
      key_delay = 0;
      previous_key_code_down = 0;
    }
}


/**
 * Set a joystick code
 * @param code A joystick code, a buttom is down
 */
void
handler_keyboard::set_joy (Uint32 code)
{
  joy_code_down = code;
  input_joy_tempo = 0;
}

/**
 * Clear a joystick code, a button is up
 * @param code A joystick code
 */
void
handler_keyboard::clr_joy (Uint32 code)
{
  joy_up = code;
  if (joy_up == joy_code_down)
    {
      joy_code_down = 0;
      input_joy_tempo = 0;
      key_delay = 0;
      joy_code_down_prev = 0;
    }
}

/**
 * Handle string input
 */
void
handler_keyboard::input_string ()
{
  Sint32 kcode = 0;
  if (key_delay < 1)
    {
      kcode = key_code_down;
      if (key_code_down > 0)
        {
          /* it is key pressed for the first time? */
          if (previous_key_code_down != key_code_down)
            {
              previous_key_code_down = key_code_down;
              key_delay = 30;
            }
          else
            {
              /* repeat key press delay */
              key_delay = 10;
            }
        }
    }
  else
    {
      kcode = 0;
      key_delay--;
    }
  if (current_input_string == NULL)
    {
      return;
    }
  input_string (kcode);

  /* input string with the joystick */
  Sint32 joycode = 0;
  if (input_joy_tempo < 1)
    {
      joycode = joy_code_down;
      if (joy_code_down > 0)
        {
          /* button pressed for the first time? */
          if (joy_code_down_prev != joy_code_down)
            {
              joy_code_down_prev = joy_code_down;
              input_joy_tempo = 40;
            }
          else
            {
              input_joy_tempo = 5;
            }
        }
    }
  else
    {
      joycode = 0;
      input_joy_tempo--;
    }
  if (joycode > 0)
    {
      switch (joycode)
        {
        case IJOY_FIRE:
        case IJOY_RELEASE:
          current_input_string[string_cursor_pos] = ' ';
          break;
        case IJOY_OPTION:
        case IJOY_GIGABLITZ:
          input_string (SDLK_RETURN);
          break;
        case IJOY_LEFT:
          input_string (SDLK_LEFT);
          break;
        case IJOY_RIGHT:
          input_string (SDLK_RIGHT);
          break;
        case IJOY_TOP:
        {
          char c = current_input_string[string_cursor_pos] + 1;

          /* space (32) / ! (33)
           * , (44) /  - (45) / . (46) /
           * 0-9 (48-57) : (58)
           * A-Z (65 to 90)
           */
          if (c > 90)
            {
              c = 32;
            }
          else if (c > 58 && c < 65)
            {
              c = 65;
            }
          else if (c > 46 && c < 48)
            {
              c = 48;
            }
          else if (c > 33 && c < 44)
            {
              c = 44;
            }
          current_input_string[string_cursor_pos] = c;
        }
        break;
        case IJOY_DOWN:
          char c = current_input_string[string_cursor_pos] - 1;
          if (c < 32)
            {
              c = 90;
            }
          else if (c < 44 && c > 33)
            {
              c = 33;
            }
          else if (c < 48 && c > 46)
            {
              c = 46;
            }
          else if (c < 65 && c > 58)
            {
              c = 58;
            }
          current_input_string[string_cursor_pos] = c;
          break;
        }
    }
}

/**
 * Handle input string
 * @param kcode Key code enter
 */
void
handler_keyboard::input_string (Uint32 kcode)
{
  Sint32 j, i;
  if (0 == kcode)
    {
      return;
    }

  /* check pressed key */
  switch (kcode)
    {
    case SDLK_LEFT:
      string_cursor_pos--;
      break;
    case SDLK_RIGHT:
      string_cursor_pos++;
      break;

      /* backspace key pressed */
    case SDLK_BACKSPACE:
      if (string_cursor_pos > 0)
        {
          j = string_cursor_pos;
        }
      else
        {
          j = 1;
        }
      for (i = j; i < string_input_size; i++)
        {
          current_input_string[i - 1] = current_input_string[i];
        }
      current_input_string[string_input_size - 1] = ' ';
      string_cursor_pos--;
      break;

      /* [Suppr] key pressed? */
    case SDLK_DELETE:
      for (i = string_cursor_pos; i < string_input_size - 1; i++)
        {
          current_input_string[i] = current_input_string[i + 1];
        }
      current_input_string[string_input_size - 1] = ' ';
      break;

      /* [Return] or [Enter] pressed, stop string input */
    case SDLK_RETURN:
      stop_string_input ();
      return;
      break;

    default:
      kcode = kcode & 127;
      if (kcode >= 'a' && kcode <= 'z')
        {
          kcode = kcode - 32;
        }

      /* space (32) / ! (33)
       * , (44) /  - (45) / . (46) /
       * : (58) / 0-9 (48-57)
       * A-Z (65 to 90)
       */
      if ((kcode >= ' ' && kcode <= '!') ||
          (kcode >= '-' && kcode <= '.') ||
          (kcode >= '0' && kcode <= ':') ||
          (kcode >= 'A' && kcode <= 'Z') || kcode == '\'')
        {
          for (i = string_input_size - 1; i > string_cursor_pos; i--)
            {
              current_input_string[i] = current_input_string[i - 1];
            }
          current_input_string[string_cursor_pos] = kcode;
          string_cursor_pos++;
        }
    }

  /* verify overflow position cursor */
  if (string_cursor_pos < 0)
    {
      string_cursor_pos = 0;
    }
  if (string_cursor_pos > string_input_size - 1)
    {
      string_cursor_pos = string_input_size - 1;
    }
}

/**
 * Initializes a string input
 * @param strng pointer to a string
 * @param ssize size of the string
 */
void
handler_keyboard::set_input_string (char *strng, Uint32 ssize)
{
  string_cursor_pos = 0;
  string_input_size = ssize;
  current_input_string = strng;
}

/**
 * Return the current cursor position in the input string
 * @return current cursor position 0 to n
 */
Sint32
handler_keyboard::get_input_cursor_pos ()
{
  if (NULL == current_input_string)
    {
      /* not current input string */
      return -1;
    }
  return string_cursor_pos;
}

/**
 * Stop using string input
 */
void
handler_keyboard::stop_string_input ()
{
  string_cursor_pos = 0;
  string_input_size = 0;
  current_input_string = NULL;
}


/**
 * Set the code of the last key down
 * @return last pressed key code
 */
Uint32 handler_keyboard::get_key_down_code ()
{
  return key_code_down;
}


/**
 * Wait a key (used for press an key to continue)
 * @return true if a key is pressed and released
 */
bool handler_keyboard::wait_key ()
{
  if (is_key_waiting)
    {
      if (!control_is_pressed (K_FIRE) &&
          !control_is_pressed (K_RELEASE_BALL) &&
          !control_is_pressed (K_GIGABLITZ) && !is_left_button_down)
        {
          is_key_waiting = false;
        }
      wait_key_pressed = false;
      return false;
    }
  if (control_is_pressed (K_FIRE) ||
      control_is_pressed (K_RELEASE_BALL) ||
      control_is_pressed (K_GIGABLITZ) || is_left_button_down)
    {
      wait_key_pressed = true;
      return false;
    }
  if (wait_key_pressed)
    {
      return true;
    }
  return false;
}
