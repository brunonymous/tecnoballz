/** 
 * @file controller_guardians.cc 
 * @brief Guardians controller 
 * @created 2003-01-10 
 * @date 2007-10-21
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.13 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: controller_guardians.cc,v 1.13 2007-10-29 13:18:53 gurumeditation Exp $
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
#include "../include/controller_guardians.h"
#include "../include/handler_resources.h"
#include "../include/handler_levels.h"

/**
 * Create the guardians controller 
 */
controller_guardians::controller_guardians ()
{
  littleInit ();
  max_of_sprites = 2;
  sprites_have_shades = true;
  sprite_type_id = sprite_object::MEDIUM_GUARDIAN_AREA_1A;
  offset_ptr = 0;
  scrollTemp = 0;
}

/**
 * Release the guardians controller 
 */
controller_guardians::~controller_guardians ()
{
  if (NULL != displacement_curve)
    {
      delete[](char *) displacement_curve;
      displacement_curve = NULL;
    }
  if (NULL != life_gauges_list)
    {
      for (Uint32 i = 0; i < max_of_sprites; i++)
        {
          sprite_object *sprite = life_gauges_list[i];
          if (NULL != sprite)
            {
              delete sprite;
              life_gauges_list[i] = NULL;
            }
        }
      delete[]life_gauges_list;
      life_gauges_list = NULL;
    }
  release_sprites_list ();
}

/**
 * Create and initialize the guardians and life gauges sprites
 * @param grdPt
 */
void
controller_guardians::create_guardians_list (Sint32 grdPt)
{

  /* count the number of guardians, 1 or 2 guardians */
  max_of_sprites = -1;
  Sint32 i;
  Sint32 j = grdPt;
  Uint32 free_sprites = sprites->get_sprites_remaining ();
  do
    {
      i = level_list[j++];
      max_of_sprites++;
      if (max_of_sprites > free_sprites )
        {
          std::cerr << "(!)controller_guardians::" << 
            "create_guardians_list() maximum number of sprites "
             << "has been reached!";
          throw std::runtime_error ("(!)controller_guardians::"
                                   "create_guardians_list() maximum "
                                   "number of sprites has been "
                                   "reached!"); 
        }
    }
  while (i >= 0);

  /* loading curves file of all guards (23304 bytes) */
  displacement_curve =
    (unsigned char *) resources->load_data (handler_resources::RESGCURVES);

  /* allocate memory for the "sprite_guardian" object(s) */
  alloc_sprites_list ();

  /* initialize the guardian(s) sprite(s) */
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      Sint32 p = level_list[grdPt++];
      sprite_guardian *guard = new sprite_guardian ();
      guard->set_object_pos (i);
      guard->create_sprite (guard_list[p].para_nsbob, sprites_bitmap, true);
      sprites_list[i] = guard;
      sprites->add (guard);
      guard->enable ();
      guard->initialize (&guard_list[p], getLissaPt (guard_list[p].para_lissa));
    }

  /*
   * Create the life gauges sprites
   */
  try
  {
    life_gauges_list = new sprite_object *[max_of_sprites];
  }
  catch (std::bad_alloc &)
  {
    std::
      cerr << "(!)controller_guardians::create_guardians_list() "
      "not enough memory to allocate " <<
      max_of_sprites << " list elements!" << std::endl;
    throw;
  }
 for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = new sprite_object ();
      life_gauges_list[i] = sprite;
      sprite->set_object_pos (i);
      sprite->create_sprite (sprite_object::LIFE_GAUGE,
                             sprites_bitmap, false);
      sprites->add (sprite);
      sprite->set_coordinates (i * 16 * resolution, 0);
      sprite->enable ();
      sprite->enable_vertical_repeat (20);
    }
  set_gauge_height ();
}

/**
 * Calcul the position and number of repetition of the life gauges 
 */
void
controller_guardians::set_gauge_height ()
{
  Uint32 y_base = 238 * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      sprite_object *gauge_sprite = life_gauges_list[i];
      if (!guard_sprite->energy_level)
        {
          gauge_sprite->disable ();
          break;
        }
      Uint32 h = guard_sprite->energy_level / 4;
      gauge_sprite->enable_vertical_repeat (h);
      if (0 == h)
        {
          h = 1;
        }
      Sint32 y = y_base - h * gauge_sprite->get_sprite_height ();
      gauge_sprite->set_y_coord (y);
    }
}

/**
 * Move the guardians, and fire bullets and gigablitz
 */
void
controller_guardians::run ()
{
  offset_ptr++;
  offset_ptr = offset_ptr & 255;
  Uint32 voffset = offzetbyte[offset_ptr] * resolution;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      guard_sprite->run (voffset);
    }
  set_gauge_height ();
}

/**
 * check if the guardian's level is finished
 * @return true if all guardians was destroyed
 */
bool
controller_guardians::is_guardians_destroyed ()
{
  bool is_finished = true;
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_guardian *guard_sprite = sprites_list[i];
      if (guard_sprite->energy_level > 0)
        {
          is_finished = false;
        }
    }
  return is_finished;
}

void
controller_guardians::killguards (Sint32 numGa)
{
  switch (numGa)
    {
    case 2:
      if (max_of_sprites > 1)
        {
          if (sprites_list[1]->energy_level)
            {
              sprites_list[1]->energy_level = 0;
              sprites_list[1]->explode_delay_counter = 500;
            }
        }
      break;

    case 1:
      if (sprites_list[0]->energy_level)
        {
          sprites_list[0]->energy_level = 0;
          sprites_list[0]->explode_delay_counter = 500;
        }
      break;

    default:
      if (max_of_sprites > 1)
        {
          if (sprites_list[1]->energy_level)
            {
              sprites_list[1]->energy_level = 0;
              sprites_list[1]->explode_delay_counter = 500;
            }
        }

      if (sprites_list[0]->energy_level)
        {
          sprites_list[0]->energy_level = 0;
          sprites_list[0]->explode_delay_counter = 500;
        }
      break;
    }

}

/**
 * Return speed and direction of the vertical scrolling
 * @param ntype
 * @param speed the current speed of the scrolling
 * @param ball the first found ball 
 * @param weapo the first found bullet 
 * @return the speed of the scrolling
 */
Sint32
controller_guardians::get_scrolling_speed (Uint32 ntype, Sint32 speed,
                                  sprite_ball * ball, sprite_bullet * weapo)
{
  sprite_guardian *guard1, *guard2;
  Sint32 res = resolution;
  if (max_of_sprites < 1)
    {
      return speed;
    }
  guard1 = sprites_list[0];
  if (max_of_sprites > 1)
    {
      guard2 = sprites_list[1];
    }
  else
    {
      guard2 = guard1;
    }

  switch (ntype)
    {
    /* medium area 1 (SCROLL DOWN) */
    case 1:
      if (0 == guard1->energy_level && 0 == guard2->energy_level)
        {
          speed = 2;
        }
      else
        {
          if (0 == guard1->energy_level || 0 == guard2->energy_level)
            {
              speed = 1;
            }
          else
            {
              speed = 0;
            }
        }
      break;

    /* final area 1 (SCROLL GUARD 1) */
    case 2:
      if (guard1->energy_level != 0)
        {
          Sint32 guard_ycoord = guard1->y_coord;
          if (scrollTemp)
            {
              guard_ycoord = scrollTemp - guard_ycoord;
              guard_ycoord = guard_ycoord >> 1;
              if (guard_ycoord != 0)
                {
                  speed = guard_ycoord;
                }
            }
          scrollTemp = guard1->y_coord;
        }
      break;

     /* medium area 2 (SCROLL COLLISION 1) */
    case 3:
      if (guard1->recently_touched > 0 && guard1->energy_level > 0)
        {
          speed = -1;
        }
      else
        {
          if (guard2->recently_touched > 0 && guard2->energy_level > 0)
            {
              speed = 1;
            }
        }
      break;

     /* final area 2 (SCROLL BALL 1) */
    case 4:
      if (guard1->energy_level)
        {
          if (ball->sticky_paddle_num > 0 || !ball->is_enabled)
            {
              speed = -1;
              scrollTemp = 0;
            }
          else
            {
              Sint32 guard_ycoord = ball->y_coord;
              if (scrollTemp)
                {
                  guard_ycoord = scrollTemp - guard_ycoord;
                  guard_ycoord = guard_ycoord >> 1;
                  if (guard_ycoord)
                    speed = guard_ycoord;
                }
              scrollTemp = ball->y_coord;
            }
        }
      break;

      /* medium area 3 (SCROLL COLLISION 2) */
    case 5:
      if (guard1->recently_touched > 0 && guard1->energy_level)
        {
          if (--speed < (-4 * res))
            {
              speed = (-4 * res);
            }
        }
      else
        {
          if (guard2->recently_touched > 0 && guard2->energy_level)
            {
              if (++speed > (4 * res))
                {
                  speed = (4 * res);
                }
            }
        }
      break;

     /* final area 3 (SCROLL GUARD 2) */
    case 6:
      if (guard1->energy_level > 0)
        {
          Sint32 guard_ycoord = guard1->y_coord;
          if (scrollTemp != 0)
            {
              guard_ycoord = guard_ycoord - scrollTemp;
              if (guard_ycoord != 0)
                {
                  speed = guard_ycoord * 2;
                }
            }
          scrollTemp = guard1->y_coord;
        }
      break;

      /* medium area 4 (SCROLL FIRE 1) */
    case 7:
      if (weapo->is_enabled)
        {
          if (scrollTemp > 0)
            {
              Sint32 guard_ycoord = weapo->y_coord;
              guard_ycoord = guard_ycoord - scrollTemp;
              speed += guard_ycoord;
              if (speed > 15 * res)
                {
                  speed = 15 * res;
                }
              if (speed < (-15 * res))
                {
                  speed = -15 * res;
                }
            }
          scrollTemp = weapo->y_coord;
        }
      else
        {
          scrollTemp = 0;
        }
      break;

      /* final area 4 (SCROLL FIRE 2) */
    case 8:
      if (weapo->is_enabled)
        {
          if (scrollTemp != 0)
            {
              Sint32 guard_ycoord = weapo->y_coord;
              guard_ycoord = scrollTemp - guard_ycoord;
              speed += guard_ycoord;
              if (speed > 15 * res)
                {
                  speed = 15 * res;
                }
              if (speed < (-15 * res))
                {
                  speed = -15 * res;
                }
            }
          scrollTemp = weapo->y_coord;
        }
      else
        {
          scrollTemp = 0;
        }
      break;

      /* medium area 5 (SCROLL COLLISION 3) */
    case 9:
      if (guard1->recently_touched > 0 && guard1->energy_level > 0)
        {
          if (--speed < (-16 * res))
            {
              speed = (-16 * res);
            }
        }
      else
        {
          if (guard2->recently_touched > 0 && guard2->energy_level)
            {
              if (++speed > (16 * res))
                {
                  speed = (16 * res);
                }
            }
        }
      break;

      /* final area 5 (SIMPLE SCROLL) */
    case 10:
      speed = 8 * res;
      break;

     /* final-final area 5 (SIMPLE SCROLL) */
    case 11:
      speed = -8 * res;
      break;
    }
  return speed;
}

//-------------------------------------------------------------------------------
// select curve's gard
//-------------------------------------------------------------------------------
unsigned char *
controller_guardians::getLissaPt (Sint32 lissa)
{
  // calcul le pointeur sur la courbe
  Uint32 p = 0;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  lissa = lissa * 4;
  p = (unsigned char) displacement_curve[lissa + 0];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 1];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 2];
  p = p << 8;
  p = p | (unsigned char) displacement_curve[lissa + 3];
#else
  Uint32 *ptr32 = (Uint32 *) displacement_curve;
  p = ptr32[lissa];
#endif
  return displacement_curve + p;
}

//-------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------
const char
  controller_guardians::offzetbyte[256] = {
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3e, 0x3e, 0x3e, 0x3e,
  0x3d, 0x3e, 0x3d, 0x3d, 0x3c, 0x3d, 0x3c, 0x3c, 0x3b, 0x3b, 0x3a, 0x3a,
  0x39, 0x3a, 0x38, 0x39, 0x37, 0x38, 0x36, 0x37, 0x35, 0x36, 0x34, 0x35,
  0x33, 0x33, 0x32, 0x32, 0x30, 0x31, 0x2f, 0x30, 0x2e, 0x2e, 0x2c, 0x2d,
  0x2b, 0x2b, 0x29, 0x2a, 0x28, 0x28, 0x26, 0x27, 0x25, 0x25, 0x23, 0x24,
  0x22, 0x22, 0x20, 0x21, 0x1f, 0x1f, 0x1d, 0x1e, 0x1b, 0x1c, 0x1a, 0x1b,
  0x18, 0x19, 0x17, 0x18, 0x15, 0x16, 0x14, 0x15, 0x13, 0x13, 0x11, 0x12,
  0x10, 0x10, 0x0e, 0x0f, 0x0d, 0x0e, 0x0c, 0x0d, 0x0b, 0x0b, 0x0a, 0x0a,
  0x08, 0x09, 0x07, 0x08, 0x06, 0x07, 0x06, 0x06, 0x05, 0x05, 0x04, 0x04,
  0x03, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x02, 0x01, 0x01, 0x01, 0x01,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02,
  0x03, 0x03, 0x04, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x06,
  0x08, 0x07, 0x09, 0x08, 0x0a, 0x0a, 0x0b, 0x0b, 0x0d, 0x0c, 0x0e, 0x0d,
  0x0f, 0x0e, 0x10, 0x10, 0x12, 0x11, 0x13, 0x13, 0x15, 0x14, 0x16, 0x15,
  0x18, 0x17, 0x19, 0x18, 0x1b, 0x1a, 0x1c, 0x1b, 0x1e, 0x1d, 0x1f, 0x1f,
  0x21, 0x20, 0x22, 0x22, 0x24, 0x23, 0x25, 0x25, 0x27, 0x26, 0x28, 0x28,
  0x2a, 0x29, 0x2b, 0x2b, 0x2d, 0x2c, 0x2e, 0x2e, 0x30, 0x2f, 0x31, 0x30,
  0x32, 0x32, 0x33, 0x33, 0x35, 0x34, 0x36, 0x35, 0x37, 0x36, 0x38, 0x37,
  0x39, 0x38, 0x3a, 0x39, 0x3a, 0x3a, 0x3b, 0x3b, 0x3c, 0x3c, 0x3d, 0x3c,
  0x3d, 0x3d, 0x3e, 0x3d, 0x3e, 0x3e, 0x3e, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f
};

/**
 * Describe guardian levels
 */
gardlevel
  controller_guardians::guard_list[] = {
  /* AREA 1 : intermediaires */
  {25,                          // resistance
   16,                          // centre X depart tir
   555,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   7,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   25,                          // fenetre de collision x-maxi
   17,                          // fenetre de collision y-maxi
   11,                          // centre Y depart tir
   400,                         // frequence tir
   sprite_object::MEDIUM_GUARDIAN_AREA_1A,                  // numero du BOB
   LISSA_NB01,                  // numero de la courbe
   {0, -1}                      // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   16,
   666,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   8,                           // xmin zone sensible
   16,                          // ymin zone sensible
   24,                          // xmax zone sensible
   42,                          // ymax zone sensible
   24,
   240,                         //delai entre 2 tirs
   sprite_object::MEDIUM_GUARDIAN_AREA_1B,
   LISSA_NB02,
   {4, -1}
   },

  /* AERA 1 : Final */
  {50,                          // resistance du gardien
   32,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   34,                          // ymin zone sensible
   52,                          // xmax zone sensible
   77,                          // ymax zone sensible
   52,
   200,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_1,
   LISSA_NB03,
   {0, 4, 8, -1}
   },


  /* AERA 2 : intermediaires */
  {30,                          // resistance
   16,                          // centre X depart tir
   444,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   9,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   23,                          // fenetre de collision x-maxi
   30,                          // fenetre de collision y-maxi
   21,                          // centre Y depart tir
   300,                         // frequence tir
   sprite_object::MEDIUM_GUARDIAN_AREA_2A,                  // numero du BOB
   LISSA_NB02,                  // numero de la courbe
   {12, -1}                     // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   15,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   8,                           // xmin zone sensible
   10,                          // ymin zone sensible
   24,                          // xmax zone sensible
   49,                          // ymax zone sensible
   28,
   280,                         //delai entre 2 tirs
   sprite_object::MEDIUM_GUARDIAN_AREA_2B,
   LISSA_NB04,
   {8, -1}
   },

  /* AERA 2 : final */
  {100,                         // resistance du gardien
   32,
   555,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   14,                          // xmin zone sensible
   17,                          // ymin zone sensible
   50,                          // xmax zone sensible
   66,                          // ymax zone sensible
   45,
   200,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_2,
   LISSA_NB15,
   {16, 0, 8, 4, 12, -1}
   },



  /* AERA 3 : intermediaires */
  {35,                          // resistance
   16,                          // centre X depart tir
   333,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   6,                           // fenetre de collision x-mini
   5,                           // fenetre de collision y-mini
   26,                          // fenetre de collision x-maxi
   30,                          // fenetre de collision y-maxi
   19,                          // centre Y depart tir
   300,                         // frequence tir
   sprite_object::MEDIUM_GUARDIAN_AREA_3A,                  // numero du BOB
   LISSA_NB11,                  // numero de la courbe
   {20, 8, -1}                  // table pointeur des tirs
   },
  {30,                          // resistance du gardien
   15,
   555,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   10,                          // xmin zone sensible
   7,                           // ymin zone sensible
   22,                          // xmax zone sensible
   78,                          // ymax zone sensible
   46,
   200,                         //delai entre 2 tirs
   sprite_object::MEDIUM_GUARDIAN_AREA_3B,
   LISSA_NB13,
   {24, 4, -1}
   },
  /* AERA 3 : final */
  {70,                          // resistance du gardien
   32,
   444,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   22,                          // ymin zone sensible
   52,                          // xmax zone sensible
   61,                          // ymax zone sensible
   41,
   250,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_3,
   LISSA_NB07,
   {24, 0, 8, 12, 4, 16, -1}
   },


  /* AERA 4 : intermediaires */
  {30,                          // resistance
   16,                          // centre X depart tir
   222,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   10,                          // fenetre de collision x-mini
   6,                           // fenetre de collision y-mini
   22,                          // fenetre de collision x-maxi
   42,                          // fenetre de collision y-maxi
   23,                          // centre Y depart tir
   300,                         // frequence tir
   sprite_object::MEDIUM_GUARDIAN_AREA_4A,                  // numero du BOB
   LISSA_NB08,                  // numero de la courbe
   {16, 28, 8, -1}              // table pointeur des tirs
   },
  {40,                          // resistance du gardien
   16,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   7,                           // xmin zone sensible
   12,                          // ymin zone sensible
   25,                          // xmax zone sensible
   68,                          // ymax zone sensible
   42,
   400,                         //delai entre 2 tirs
   sprite_object::MEDIUM_GUARDIAN_AREA_4B,
   LISSA_NB04,
   {12, 32, 8, -1}
   },
  /* AERA 4 : final */
  {60,                          // resistance du gardien
   32,
   222,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   13,                          // xmin zone sensible
   30,                          // ymin zone sensible
   52,                          // xmax zone sensible
   73,                          // ymax zone sensible
   49,
   160,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_4,
   LISSA_NB05,
   {0, 4, 32, 16, 20, 8, 28, 12, 24, -1}
   },

  /* AERA 5 : intermediaires */
  {40,                          // resistance
   16,                          // centre X depart tir
   222,                         // frequence tir gigaBlitz
   2,                           // vitesse de deplacement
   7,                           // fenetre de collision x-mini
   14,                          // fenetre de collision y-mini
   25,                          // fenetre de collision x-maxi
   55,                          // fenetre de collision y-maxi
   31,                          // centre Y depart tir
   360,                         // frequence tir
   sprite_object::MEDIUM_GUARDIAN_AREA_5A,                  // numero du BOB
   LISSA_NB08,                  // numero de la courbe
   {36, 20, -1}                 // table pointeur des tirs
   },
  {40,                          // resistance du gardien
   16,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   6,                           // xmin zone sensible
   14,                          // ymin zone sensible
   26,                          // xmax zone sensible
   77,                          // ymax zone sensible
   41,
   180,                         //delai entre 2 tirs
   sprite_object::MEDIUM_GUARDIAN_AREA_5B,
   LISSA_NB12,
   {40, 0, -1}

   },

  /* AERA 5 : final */
  {70,                          // resistance du gardien
   31,
   333,                         // delai entre 2 tirs
   2,                           // vit. déplacement
   22,                          // xmin zone sensible
   82,                          // ymin zone sensible
   42,                          // xmax zone sensible
   102,                         // ymax zone sensible
   55,
   200,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_5,
   LISSA_NB09,
   {24, 8, 40, 12, 4, 0, 16, 20, 28, 36, 32, -1}
   },

  /* Final TecnoballZ  */
  {150,                         //resistance du gardien
   32,
   222,                         //delai entre 2 tirs
   2,                           //vit. déplacement
   11,                          //xmin zone sensible
   68,                          //ymin zone sensible
   54,                          //xmax zone sensible
   128,                         //ymax zone sensible
   83,
   150,                         //delai entre 2 tirs
   sprite_object::FINAL_GUARDIAN_AREA_6,
   LISSA_NB12,
   {20, 4, 36, 8, 32, 12, 24, 16, 0, 28, 40, -1}
   },

};

//-------------------------------------------------------------------------------
// list of all levels gardians
//-------------------------------------------------------------------------------
Sint32
  controller_guardians::level_list[] = {
  0, 1, -1,                     //area 1
  2, -1,

  3, 4, -1,                     //area 2
  5, -1,

  6, 7, -1,                     //area 3
  8, -1,

  9, 10, -1,                    //area 4
  11, -1,

  12, 13, -1,                   //area 5
  14, -1,
  15, -1, -1
};

/**
 * Convert area and level numbers to a guardian level pointer
 * @param area_num area number from 1 to 5
 * @param level_num level number from 1 to 13 
 */
Sint32
controller_guardians::level2gdpt (Uint32 area_num, Uint32 level_num)
{
  if (area_num < 1)
    { 
      area_num = 1;
    }
  if (level_num < 1)
    {
      level_num = 1;
    }
  if (area_num > handler_levels::MAX_OF_AREAS)
    {
      area_num = handler_levels::MAX_OF_AREAS;
    }
  Uint32 vloop = (level_num / 6) - 1 + (area_num - 1) * 2;
  if (level_num > handler_levels::NUM_OF_LEVELS_PER_AREA)
    {
      vloop++;
    }
  Uint32 index = 0;
  Sint32 gdptr = 0;
  for (;;)
    {
      if (index++ == vloop)
        break;                  //pointer is found :-)
      while (level_list[++gdptr] != -1);
      if (level_list[gdptr + 1] == -1)
        {
          fprintf (stderr,
                   "controller_guardians::level2gdpt(%i, %i) end of table\n",
                   area_num, level_num);
          gdptr = 0;
          break;
        }
      gdptr++;
    }
  return gdptr;
}
