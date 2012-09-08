/**
 * @file controller_bricks.cc
 * @brief Control the bricks in bricks levels
 * @created 1996-11-13
 * @date 2012-09-08
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
#include "../include/controller_bricks.h"
#include "../include/handler_resources.h"
#include "../include/handler_display.h"

/**
 * create the bricks controller
 */
controller_bricks::controller_bricks ()
{
  littleInit ();
  if (!has_background)
    {
      max_of_sprites = MAX_OF_BRICKS_HORIZONTALLY * MAX_OF_BRICKS_VERTICALLY;
    }
  else
    {
      max_of_sprites = 0;
    }
  sprites_have_shades = true;
  sprite_type_id = sprite_object::BRICKS;
  bricks_redraw = (brick_redraw *) NULL;
  background_under_bricks = (char *) NULL;
  bricks_map = (brick_info *) NULL;
  bitmap_bricks = (bitmap_data *) NULL;
  briqueSave = 0;
  brique_clr = 0;
  less_bricks_count = 0;
  less_bricks_delay = 10;
  bricks_height = 112 * resolution;
  bricks_width = 63 * resolution;
  brick_width = BRICK_WIDTH * resolution;
  brick_height = BRICK_HEIGHT * resolution;
  brick_size = brick_width * brick_height;
  brkyoffset = BRKYOFFSET * resolution;
  indestructible_offset = 8 * brick_height * bricks_height;
  shadow_offset = 3 * resolution;
  shadow_left = (BRICK_HEIGHT * resolution) - shadow_offset;
  shadow_yoff = (BRKYOFFSET - BRICK_HEIGHT) * resolution;
  shadow_top1 = shadow_offset - shadow_yoff;
  cycling_count = 0;
  is_cycling = true;
  were_sprites_added = false;
}

/**
 * Release the bricks controller
 */
controller_bricks::~controller_bricks ()
{
  if (bricks_redraw != NULL)
    {
      delete[]bricks_redraw;
      bricks_redraw = NULL;
    }
  if (bricks_map != NULL)
    {
      delete[]bricks_map;
      bricks_map = NULL;
    }
  if (background_under_bricks != NULL)
    {
      delete[]background_under_bricks;
      background_under_bricks = NULL;
    }
  if (bitmap_bricks != NULL)
    {
      delete bitmap_bricks;
      bitmap_bricks = NULL;
    }
  release_sprites_list ();
}

/**
 * First initialization
 */
void
controller_bricks::first_init ()
{
  /* allocate memory for the redraw bricks table */
  if (NULL == bricks_redraw)
    {
      bricks_redraw = new brick_redraw[MAXBRIKCLR];
    }

  /* allocate memory to save background under bricks */
  if (NULL == background_under_bricks)
    {
      background_under_bricks = new char[brick_size * MAX_OF_BRICKS];
    }

  /* allocate memory for current brick level */
  if (NULL == bricks_map)
    {
      bricks_map = new brick_info[MAX_OF_BRICKS];
    }
}

/**
 * Initialize new level and draw bricks
 */
void
controller_bricks::initialize ()
{
  if (is_verbose)
    {
      std::cout << ">controller_bricks::initialize() start!" << std::endl;
    }
  if (!has_background)
    {
      alloc_sprites_list ();
    }


  if (NULL == bitmap_bricks)
    {
      less_bricks_count = current_player->get_less_bricks ();
      current_player->set_less_bricks (0);

      /* clear the restore list */
      briqueSave = 0;
      brique_clr = 0;
      brick_redraw *redraw = bricks_redraw;
      for (Uint32 j = 0; j < MAXBRIKCLR; j++, redraw++)
        {
          redraw->is_indestructible = false;
          redraw->is_gigablitz_destroyed = false;
          redraw->xcoord_collision = 0;
          redraw->ycoord_collision = 0;
          redraw->paddle = (sprite_paddle *) NULL;
          redraw->number = 0;
          redraw->is_background = false;
          redraw->pixel_offset = 0;
          redraw->brick_map = (brick_info *) NULL;
        }

      /* initialize current brick level */
      brick_info *map = bricks_map;
      Sint32 c = 0;
      Uint32 color = 239;
      for (Uint32 j = 0; j < MAX_OF_BRICKS_VERTICALLY * brkyoffset;
           j += brkyoffset)
        {
          for (Uint32 i = 0; i < MAX_OF_BRICKS_HORIZONTALLY * brick_width;
               i += brick_width)
            {
              map->source_offset = 0;
              map->is_displayed = false;
              map->pixel_offset = game_screen->get_offset (i, j);
              map->sprite = NULL;
              map->h_pos = 0;
              map->v_pos = 0;
              map->number = c++;
              map->color = color;
              map++;
            }
          if (++color > 255)
            {
              color = 239;
            }
        }

      /*
       * Select one of 10 sets of bricks
       */
      /* load the 10 sets of bricks */
      bitmap_data *bmp_bricks = new bitmap_data ();
      bmp_bricks->load (handler_resources::BITMAP_BRICKS);
      Sint32 i = random_counter & 0x0F;
      if (i >= 10)
        {
          i = i - 10;
        }
      Sint32 bposy = 0;
      Sint32 bposx = 0;
      if (i >= 5)
        {
          bposy = 63 * resolution;
          i = i - 5;
        }
      bposx = i * 112 * resolution;

      //bposx = 0 * 112 * resolution;     //test only
      //bposy = 0 * 63 * resolution;      //test only

      if (is_verbose)
        {
          std::cout << "**** controller_bricks::initialize() " <<
                    "bposx: " << bposx << " bposy: " << bposy << std::endl;
        }
      bitmap_bricks =
        bmp_bricks->cut_to_surface (bposx, bposy, bricks_height,
                                    bricks_width);
      delete bmp_bricks;

      /* replace color 0 by color 29 for color cycling */
      char *pixels = bitmap_bricks->get_pixel_data ();
      Uint32 count =
        bitmap_bricks->get_height () * bitmap_bricks->get_width ();
      for (Uint32 i = 0; i < count; i++, pixels++)
        {
          if (*pixels == 0)
            {
              *pixels = 29;
            }
        }

      /* load one bricks level from the file "tableau.data" */
      Uint32 area_num = current_player->get_area_number ();
      Uint32 level_num = current_player->get_level_number ();
      load_level (area_num, level_num);

      /* read somes values for the graphic routine */
      offsSource = bitmap_bricks->get_line_modulo (brick_width);
      offsDestin = game_screen->get_line_modulo (brick_width);
      adr_source = (Sint32 *) bitmap_bricks->get_pixel_data ();
      adr_desti1 = (Sint32 *) game_screen->get_pixel_data ();
      adr_desti2 = (Sint32 *) background_screen->get_pixel_data ();

      set_bricks_palette ();
    }
  /* save background under bricks */
  save_background ();
  draw_bricks_shadows ();
  draw_bricks ();
}

/**
 * Return the number of bricks in the current level
 * @return number of bricks
 */
Uint32
controller_bricks::get_num_of_bricks ()
{
  return num_of_bricks;
}

/**
 * Load and initialize a new level
 * @param area_nu area number from 1 to 5
 * @param level_nu level number from 1 to 12
 */
void
controller_bricks::load_level (Sint32 area_nu, Sint32 level_nu)
{
  if (is_verbose)
    {
      std::cout << "controller_bricks::load_level() area_nu: " <<
                area_nu << "level_nu: " << level_nu << std::endl;
    }
  /* a table has 17 lines of 10 columns, 172 * 2 = 340 bytes */
  /* clear he number of bricks of the level */
  num_of_bricks = 0;

  /* load bricks levels (34000 bytes => 100 levels) */
  char *all_levels =
    resources->load_data (handler_resources::DATA_BRICKS_LEVELS);
  if (level_nu >= 6)
    {
      level_nu--;
    }
  char *level = all_levels + (SIZEOFAREA * 2 * (area_nu - 1)) +
                (LEVEL_SIZE * 2 * (level_nu - 1));

  //level = all_levels + LEVEL_SIZE * 2 * 8;
  /* Select a level at random.
   * Two different levels are possible for a level */
  if ((random_counter & 0x001))
    {
      //level = level + LEVEL_SIZE * 2 * 50;
    }

  /*
   * copy all bricks of the level in the structure "bricks_map"
   */
  brick_info *map = bricks_map;
  /* 6 first lines are always empty */
  map += (6 * MAX_OF_BRICKS_HORIZONTALLY);
  Sint32 ycoord = 6 * brkyoffset;
  Sint32 xcoord = 0;
  Uint32 bobindex = 6 * MAX_OF_BRICKS_HORIZONTALLY;
  sprite_brick *sprite_template = NULL;

  for (Uint32 j = 0; j < BRICKS_MAP_HEIGHT;
       j++, map += 3, bobindex += 3, ycoord += brkyoffset)
    {
      /* the first 3 columns are always empty */
      map += 3;
      xcoord = brick_width * 3;
      bobindex += 3;
      for (Uint32 i = 0; i < BRICKS_MAP_WIDTH;
           i++, map++, bobindex++, xcoord += brick_width)
        {
          Sint32 offset = 0;
          /* x position in the bitmap source from 0 to 8 */
          char pos_y = *(level++);
          /* y position in the bitmap source from 0 to 12, step 2 */
          char pos_x = *(level++);
          //pos_x = 12; pos_y = 1; //test only

          if (pos_x > 0 || pos_y > 0)
            {
              //pos_x = 12; pos_y = 1;    // test only
              /* save X-coordinate into bricks_map */
              map->h_pos = pos_x;
              /* save Y-coordinate into bricks_map */
              map->v_pos = pos_y;
              offset =
                bitmap_bricks->get_offset (pos_x * 8 * resolution,
                                           pos_y * brick_height);
              /* it's a indestructible brick? */
              if (offset < indestructible_offset)
                {
                  /* not, counter's incremented */
                  num_of_bricks++;
                }

              if (!has_background)
                {
                  sprite_brick *sprite = new sprite_brick ();
                  if (sprite_template == NULL)
                    {
                      sprite->create_sprite (sprite_object::BRICKS,
                                             bitmap_bricks, true, true);
                      sprite_template = sprite;
                    }
                  else
                    {
                      sprite_template->duplicate_to (sprite);
                    }
                  sprites_list[bobindex] = sprite;
                  sprite->set_x_coord (xcoord);
                  sprite->set_y_coord (ycoord);
                  sprite->enable ();
                  sprite->set_image (pos_y * 7 + (pos_x >> 1));
                  sprite->set_color (map->color);
                  map->sprite = sprite;
                }
            }
          map->source_offset = offset;
          map->is_displayed = offset > 0 ? true : false;
        }

    }
  if (all_levels != NULL)
    {
      delete[](char *) all_levels;
    }

  /* Initialize the number of total bricks to destroy */
  right_panel_score *panel = right_panel_score::get_instance ();
  panel->set_bricks_counter (num_of_bricks);
}

/**
 * Add bricks sprites to the sprites global list
 */
void
controller_bricks::add_to_sprites_list ()
{
  if (were_sprites_added || has_background)
    {
      return;
    }
  for (Uint32 i = 0; i < max_of_sprites; i++)
    {
      sprite_object *sprite = sprites_list[i];
      if (sprite == NULL)
        {
          continue;
        }
      sprites->add (sprite);
    }
  were_sprites_added = true;
}

/**
 * Draw the shadows of bricks
 */
void
controller_bricks::draw_bricks_shadows ()
{
  if (!has_background)
    {
      return;
    }
  brick_info *map = bricks_map;
  Sint32 xmax = MAX_OF_BRICKS_HORIZONTALLY * brick_width - shadow_offset;
  for (Uint32 j = shadow_offset;
       j < MAX_OF_BRICKS_VERTICALLY * brkyoffset + shadow_offset;
       j += brkyoffset)
    {
      for (Sint32 i = -shadow_offset; i < xmax; i += brick_width)
        {
          if (map->source_offset)
            {
              background_screen->fill_shadow_rect (i, j, brick_width,
                                                   brick_height);
            }
          map++;
        }
    }
}

/**
 * Draw all bricks
 */
void
controller_bricks::draw_bricks ()
{
  if (!has_background)
    {
      return;
    }
  brick_info *map = bricks_map;
  for (Uint32 j = 0; j < MAX_OF_BRICKS_VERTICALLY * brkyoffset;
       j += brkyoffset)
    {
      for (Uint32 i = 0; i < MAX_OF_BRICKS_HORIZONTALLY * brick_width;
           i += brick_width, map++)
        {
          /* range from x >=0 to x <= 14 */
          Sint32 pos_x = map->h_pos;
          /* range from y >=0 to y <= 8 */
          Sint32 pos_y = map->v_pos;
          if (pos_x != 0 || pos_y != 0)
            {
              /* convert planar to chunky */
              pos_x *= 8 * resolution;
              pos_y *= brick_height;
              char *source = bitmap_bricks->get_pixel_data (pos_x, pos_y);
              draw_brick (source, map->pixel_offset, map->color);
            }
        }
    }
}

/**
 * Draw one brick
 * @param pixels pointer on the graphics of the brick
 * @param offset relative pointer on the graphics of the offscreen
 * @param color colormap number from 239 to 255
 */
void
controller_bricks::draw_brick (char *pixels, Sint32 offset, Sint32 color)
{
  Sint32 offset_src = offsSource;
  Sint32 offset_dst = offsDestin;
  /* pointer to the game offscreen */
  char *screen1 = (char *) adr_desti1;
  /* pointer to offscreen used to restore background */
  char *screen2 = (char *) adr_desti2;
  screen1 += offset;
  screen2 += offset;
  for (Uint32 j = 0; j < brick_height; j++)
    {
      for (Uint32 i = 0; i < brick_width; i++)
        {
          char p = *(pixels++);
          /* check color 29 */
          if (p == 29)
            {
              /* replace by the color of the bricks */
              p = color;
            }
          *(screen1++) = p;
          *(screen2++) = p;
        }
      pixels += offset_src;
      screen1 += offset_dst;
      screen2 += offset_dst;
    }
}


/**
 * Enable the bricks color cycling
 */
void
controller_bricks::start_cycling ()
{
  if (has_background)
    {
      return;
    }
  is_cycling = true;
}

/**
 * Cycle color of all bricks
 */
void
controller_bricks::color_cycling ()
{
  if (has_background or ! is_cycling)
    {
      return;
    }
  brick_info *map = bricks_map;
  map += ((6 + BRICKS_MAP_HEIGHT - 9) * MAX_OF_BRICKS_HORIZONTALLY + 3) - 1;
  map =
    bricks_map + ((6 + BRICKS_MAP_HEIGHT - 1) * MAX_OF_BRICKS_HORIZONTALLY) +
    3 + BRICKS_MAP_WIDTH - 1;
  Sint32 count = cycling_count++;
  for (Uint32 i = 0; i < BRICKS_MAP_HEIGHT;
       i++, map -= MAX_OF_BRICKS_HORIZONTALLY)
    {
      if (count >= (Sint32) BRICKS_MAP_WIDTH)
        {
          if (i == BRICKS_MAP_HEIGHT - 1)
            {
              cycling_count = 0;
              is_cycling = false;
            }
        }
      else
        {
          if (map[-count].sprite != NULL)
            {
              if (!map[-count].sprite->is_cycling ())
                {
                  map[-count].sprite->touch ();
                }
            }
        }
      if (count == 0)
        {
          break;
        }
      count--;
    }
}

/**
 * Initialize the 17 colors of the bricks
 */
void
controller_bricks::set_bricks_palette ()
{
  display->set_color_gradation ();
}

/**
 * save background under bricks
 */
void
controller_bricks::save_background ()
{
  Sint32 *save = (Sint32 *) background_under_bricks;
  brick_info *map = bricks_map;
  Uint32 offs = game_screen->get_row_size () - brick_width;
  for (Uint32 y = 0; y < MAX_OF_BRICKS_VERTICALLY * brkyoffset;
       y += brkyoffset)
    {
      for (Uint32 x = 0; x < MAX_OF_BRICKS_HORIZONTALLY * brick_width;
           x += brick_width, map++)
        {
          Sint32 *screen =
            (Sint32 *) background_screen->get_pixel_data (x, y);
          map->save_background = save;
          for (Uint32 i = 0; i < brick_height;
               i++, screen = (Sint32 *) ((char *) screen + offs))
            {
              for (Uint32 j = 0; j < brick_width / 4; j++)
                {
                  *(save++) = *(screen++);
                }
            }
        }
    }
}

/**
 * Decrease the counter of bricks to be destroyed
 * It is an option bought from the shop
 */
void
controller_bricks::less_bricks ()
{
  if (less_bricks_count < 1)
    {
      return;
    }
  if (--less_bricks_delay > 0)
    {
      return;
    }
  less_bricks_delay = 10;
  less_bricks_count--;
  right_panel_score *panel = right_panel_score::get_instance ();
  panel->decrease_bricks_counter (1);
#ifndef SOUNDISOFF
  audio->play_sound (handler_audio::BALL_HIT_BRICK1);
#endif
}

/**
 * Display a brick or restore the background under the brick
 * @return true if the update is not finished
 */
bool
controller_bricks::update ()
{

  right_panel_score *panel = right_panel_score::get_instance ();
  controller_capsules *capsules = controller_capsules::get_instance ();
  controller_moneys *moneys = controller_moneys::get_instance ();


  brick_redraw *redraw = bricks_redraw + brique_clr;
  /* offscreen offset for drawing brick */
  Sint32 adres = redraw->pixel_offset;
  if (0 == adres)
    {
      return false;
    }
  /* inc. index on the next brick to draw or to clear */
  brique_clr += 1;
  /* index from 0 to 511 */
  brique_clr &= (MAXBRIKCLR - 1);
  redraw->pixel_offset = 0;
  brick_info *map = redraw->brick_map;

  /*
   * redraw a new brick
   */
  if (!redraw->is_background)
    {
      current_player->add_score (10);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::BALL_HIT_BRICK1);
#endif
      moneys->send_money_from_brick (redraw);
      if (!has_background)
        {
          map->sprite->update_image (redraw->brick_map->h_pos);
        }
      else
        {
          char *pixels = bitmap_bricks->get_pixel_data ();
          draw_brick (pixels + redraw->number, adres, map->color);
        }
      return true;
    }


  /*
   * a brick is destroyed
   */

  /* destroyed indestructible brick */
  if (redraw->is_indestructible)
    {
      current_player->add_score (100);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::DESTROY_INDESTRUCTIBLE_BRICK);
#endif
    }
  else
    {
      current_player->add_score (20);
      panel->decrease_bricks_counter (1);
#ifndef SOUNDISOFF
      audio->play_sound (handler_audio::BALL_HIT_BRICK1);
#endif
      if (!redraw->is_gigablitz_destroyed)
        {
          moneys->send_money_from_brick (redraw);
          capsules->send_capsule_from_brick (redraw);
        }
    }

  if (!has_background)
    {
      map->sprite->disable ();
      return true;
    }

  /**
   * restore background
   */
  Sint32 line2 = offsDestin;
  Sint32 *desP1 = adr_desti1;
  Sint32 *desP2 = adr_desti2;
  desP1 = (Sint32 *) ((char *) desP1 + adres);
  desP2 = (Sint32 *) ((char *) desP2 + adres);
  Sint32 *srcPT = map->save_background;


  map->is_displayed = 0;
  for (Uint32 j = 0; j < brick_height; j++)
    {
      for (Uint32 i = 0; i < brick_width / 4; i++)
        {
          *(desP1++) = *srcPT;
          *(desP2++) = *(srcPT++);
        }
      desP1 = (Sint32 *) ((char *) desP1 + line2);
      desP2 = (Sint32 *) ((char *) desP2 + line2);
    }

  /*
   * clear drop shadow (if needed)
   */
  Sint32 j;
  /* bottom */
  if ((map + offBri_BB)->is_displayed)
    {
      /* there is a bottom brick (2 lines to clear) */
      j = shadow_yoff;
    }
  else
    {
      /* there is not bottom brick: (6 lines to clear) */
      j = shadow_offset;
    }
  Sint32 decal = display->ecran_next (adres, 0, brick_height);
  display->clr_shadow (decal, brick_width - shadow_offset, j);
  /* left-bottom */
  if ((map + offBri_BG)->is_displayed)
    {
      j = shadow_yoff;
    }
  else
    {
      j = shadow_offset;
    }
  decal = display->ecran_next (adres, -shadow_offset, brick_height);
  display->clr_shadow (decal, shadow_offset, j);

  /* left */
  if (!(map + offBri_GG)->is_displayed)
    {
      decal = display->ecran_next (adres, -shadow_offset, shadow_offset);
      display->clr_shadow (decal, shadow_offset, shadow_left);
    }

  /*
   * redraw drop shadow (if needed)
   */
  /* exists a top brick? */
  if ((map + offBri_HH)->is_displayed)
    {
      display->set_shadow (adres, brick_width - shadow_offset, shadow_top1);
    }

  /* exists a right-top brick? */
  if ((map + offBri_HD)->is_displayed)
    {
      decal = display->ecran_next (adres, brick_width - shadow_offset, 0);
      display->set_shadow (decal, shadow_offset, shadow_top1);
    }

  /* exists a right-top brick? */
  if ((map + offBri_DD)->is_displayed)
    {
      decal =
        display->ecran_next (adres, brick_width - shadow_offset,
                             shadow_offset);
      display->set_shadow (decal, shadow_offset, shadow_left);
    }

  return true;
}

/**
 * Clear all bricks, in "Game Over"
 */
void
controller_bricks::clr_bricks ()
{
  brick_info *map = bricks_map;
  /* 6 first lines are always empty */
  map += (6 * MAX_OF_BRICKS_HORIZONTALLY);
  for (Uint32 j = 0; j < BRICKS_MAP_HEIGHT; j++, map += 3)
    {
      /* the first 3 columns are always empty */
      map += 3;
      for (Uint32 i = 0; i < BRICKS_MAP_WIDTH; i++, map++)
        {
          if (0 == map->source_offset)
            {
              continue;
            }
          brick_redraw *redraw = get_bricks_redraw_next ();
          redraw->is_gigablitz_destroyed = true;
          redraw->pixel_offset = map->pixel_offset;
          redraw->brick_map = map;
          map->h_pos = -1;
          map->source_offset = 0;
          redraw->number = map->number;
          /* restore background under brick */
          redraw->is_background = true;
        }
    }
}

/**
 * Return pointer to the bricks map of the current level
 * @return a pointer to the bricks bricks_map
 */
brick_info *
controller_bricks::get_bricks_map ()
{
  return bricks_map;
}

/**
 * Return pointer to the bricks map of the current level
 * @param xcoord X-Coordinate in the screen
 * @param ycoord Y-Coordinate in the screen
 * @return a pointer to the bricks bricks_map
 */
brick_info *
controller_bricks::get_bricks_map (Sint32 xcoord, Sint32 ycoord)
{
  return bricks_map +
         (xcoord / brick_width) +
         (ycoord / brkyoffset * MAX_OF_BRICKS_HORIZONTALLY);
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks
 */
brick_redraw *
controller_bricks::get_bricks_redraw ()
{
  return bricks_redraw + (briqueSave & (MAXBRIKCLR - 1));
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks
 */
brick_redraw *
controller_bricks::get_bricks_redraw_next ()
{
  Sint32 save = briqueSave;
  briqueSave += 1;
  briqueSave &= (MAXBRIKCLR - 1);
  return bricks_redraw + save;
}

/**
 * Return pointer to the bricks redraw list
 * @return a pointer to the bricks
 */
void
controller_bricks::bricks_redraw_next ()
{
  briqueSave += 1;
  briqueSave &= (MAXBRIKCLR - 1);
}

/**
 * Return the width of a brick in pixels
 * @return width of a brick in pixels
 */
Sint32
controller_bricks::get_brick_width ()
{
  return brick_width;
}

/**
 * Return pixels offset of the indestructible bricks
 * @return pixels offset distance from the upper left corner
 *         of the bricks bitmap (25088 in 640x480 resolution)
 */
Sint32 controller_bricks::get_indestructible_offset ()
{
  return indestructible_offset;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
Sint32
controller_bricks::getYOffset ()
{
  return brkyoffset;
}
