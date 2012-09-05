/** 
 * @file sprite_guardian.h
 * @brief The guardian sprite 
 * @created 2003-01-09 
 * @date 2007-04-14
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
#ifndef __SPRITE_GUARDIAN__
#define __SPRITE_GUARDIAN__
class sprite_guardian;
#include "../include/sprite_object.h"
#include "../include/handler_display.h"
#include "../include/controller_guardians.h"
#include "../include/controller_balls.h"
#include "../include/controller_gigablitz.h"
#include "../include/controller_explosions.h"

class sprite_guardian:public sprite_object
{
  friend class controller_guardians;
  friend class controller_bullets;
  friend class sprite_bullet;
  friend class controller_balls;

private:
  const static Sint32 table_gga1[8];    //table different size of gigablitz
  const static Sint32 table_gga2[8];    //table different size of gigablitz
  /** Strength of the guardian */
  Sint32 energy_level;
  /** Duration of the explosions */
  Uint32 explode_delay_counter;
  /** Delay value before next explision */
  Uint32 explode_frequency;
  Sint32 canon_xcoord;            //middle x from where weapons starts
  /** Counter of delay between two gigablitz */
  Uint32 gigablitz_delay_counter;
  /** Delay value before next gigablitz */
  Uint32 gigablitz_frequency;
  /** Moving speed of the guardian */
  Uint32 speed_of_guardian;
  Sint32 gard_colx1;            // fenetre de collision x-mini
  Sint32 gard_coly1;            // fenetre de collision y-mini
  Sint32 gard_colx2;            // fenetre de collision x-maxi
  Sint32 gard_coly2;            // fenetre de collision y-maxi
  Sint32 gard_ycent;            //middle y from where weapons starts
  /** Counter of delay between two shots */
  Uint32 shot_delay_counter;
  /** Delay value before next shot */ 
  Uint32 shoot_frequency;
  Sint32 gard_nsbob;            //sprite's number
  Sint32 gard_lissa;            //curve's number
  Sint32 *gard_tfire;           //table des pointeurs sur la liste tirs
  Sint32 gardptfire;            // 
  /** Guardian recently touched by a ball if greater than zero */ 
  Uint32 recently_touched;
  /** Guardian blinks if true */
  bool is_blinking ;
  unsigned char *ptr_lissa1;
  unsigned char *ptr_lissa2;
  Sint32 hasardval2;

public:
    sprite_guardian ();
   ~sprite_guardian ();
  void initialize (gardlevel * guard, unsigned char *ptLis);
  void run (Uint32 voffset);

private:
  void fire_bullets ();
  void startBlitz ();
};

#endif
