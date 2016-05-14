/** 
 * @file controller_spheres.h
 * @brief Metallic spheres controller used in congratulations 
 * @created 2004-08-05 
 * @date 2007-04-09
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
#ifndef __CONTROLLER_SPHERES__
#define __CONTROLLER_SPHERES__

class controller_spheres;

#include "../include/sprite_object.h"
#include "../include/objects_list.h"
#include "../include/sprite_display_scores.h"

class controller_spheres:public objects_list < sprite_object, controller_spheres >
{
private:
  Uint32 radius_horizontal_variation;
  Uint32 radius_vertical_variation;
  Uint32 radius_hinc_variation;
  Uint32 radius_vinc_variation;
  Uint32 radius_sphere_speed;
public:
    controller_spheres ();
   ~controller_spheres ();
  void initialize ();
  void run ();
};
#endif
