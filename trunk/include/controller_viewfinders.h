/** 
 * @file controller_viewfinders.h
 * @brief Paddles viewfinders controller 
 * @date 2007-02-18
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
#ifndef __CONTROLLER_VIEWFINDERS__
#define __CONTROLLER_VIEWFINDERS__

class controller_viewfinders;

#include "../include/objects_list.h"
#include "../include/sprite_paddle.h"
#include "../include/controller_paddles.h"

class controller_viewfinders:public objects_list < sprite_object, controller_viewfinders >
{
private:
  /** List of the paddles */
  sprite_paddle ** paddles_list;
  /** Number of paddles 1 to 4 */
  Uint32 num_of_paddles;

public:
    controller_viewfinders ();
   ~controller_viewfinders ();
  void initialize ();
  void run ();
};

#endif
