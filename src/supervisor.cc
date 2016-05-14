/** 
 * @file supervisor.cc 
 * @brief Virtual class for all supervisors 
 * @date 2007-10-02
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
#include "../include/supervisor.h"

/**
 * Create supervisor object
 */
supervisor::supervisor ()
{
  initialize ();
}

/**
 * Release supervisor object
 */
supervisor::~supervisor ()
{
  release ();
}

void
supervisor::initialize ()
{
  object_init ();
  next_phase = 0;
}

void
supervisor::release ()
{
  object_free ();
}

void
supervisor::first_init ()
{
}

Uint32
supervisor::main_loop ()
{
  return MAIN_MENU;
}
