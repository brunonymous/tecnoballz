/** 
 * @file controller_viewfinders.cc 
 * @brief Paddles viewfinders controller 
 * @date 2007-10-21
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
#include "../include/controller_viewfinders.h"
#include "../include/sprite_ball.h"

/*
 * Create the viewfinders controller
 */
controller_viewfinders::controller_viewfinders ()
{
  littleInit ();
  max_of_sprites = 4;
  sprites_have_shades = false;
  sprite_type_id = sprite_object::VIEWFINDER;
  num_of_paddles = 0;
}

/*
 * Release the viewfinders controller
 */
controller_viewfinders::~controller_viewfinders ()
{
  if (NULL != paddles_list)
    {
      delete[]paddles_list;
      paddles_list = (sprite_paddle **) NULL;
    }
  release_sprites_list ();
}

/**
 * Perform some initializations
 */
void
controller_viewfinders::initialize ()
{
  controller_paddles *paddles = controller_paddles::get_instance ();
  num_of_paddles = paddles->get_max_of_sprites ();
  if (num_of_paddles > 1)
    {
      /* the robot paddle does not have a viewfinder */
      num_of_paddles--;
    }
  if (num_of_paddles == 0)
    {
      throw std::runtime_error ("(!)controller_viewfinders::initialize() "
          "failed! Number of paddles must be higher than 0!");
    }
  try 
  {
    paddles_list = new sprite_paddle *[num_of_paddles];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)controller_viewfinders::initialize() "
      "not enough memory to allocate " <<
      num_of_paddles << " list elements!" << std::endl;
    throw;
  }
  for (Uint32 i = 0; i < num_of_paddles; i++)
    {
      paddles_list[i] = paddles->get_paddle (i + 1);
    }
}

/**
 * Enable, move, anim and disable the wiewfinders
 */
void
controller_viewfinders::run ()
{
  for (Uint32 i = 0; i < num_of_paddles; i++)
    {
      sprite_ball *ball = paddles_list[i]->stuck_ball;
      if (NULL != ball && ball->is_enabled)
        {
          sprites_list[i]->attract (ball);
          sprites_list[i]->enable ();
          sprites_list[i]->set_image (ball->direction / 4);
        }
      else
        {
          sprites_list[i]->disable ();
        }
    }
}
