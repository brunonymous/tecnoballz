/**
 * @file supervisor.h
 * @brief Virtual class for all supervisors 
 * @date 2007-10-02
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
#ifndef __SUPERVISOR__
#define __SUPERVISOR__
#include "../include/tecnoballz.h"
class supervisor:public virtual tecnoballz
  {
  protected:
    Uint32 next_phase;

  public:
    supervisor ();
    virtual ~ supervisor ();
    void initialize ();
    void release ();
    virtual void first_init ();
    virtual Uint32 main_loop ();
  };
#endif
