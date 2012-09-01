/**
 * @file handler_resources.h
 * @brief Handler of the files resources 
 * @created 2004-04-20 
 * @date 2012-08-30 
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.20 $
 */
/*
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_resources.h,v 1.20 2012-08-30 13:19:05 gurumeditation Exp $
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
#ifndef __HANDLER_RESOURCES__
#define __HANDLER_RESOURCES__
#include "../include/tecnoballz.h"

class handler_resources:public virtual tecnoballz
  {
  private:
    static const char *folderlist[];
    static const char *bitmap_files[];
    static const char *texts_files[];
    static const char *standfiles[];
    static const char *musicfiles[];
    static const char *soundfiles[];
    static const char *folderdata;
    static const char *folder_320;
    static const char *folder_640;
    static const char *fnamescore;
    static char tmp_filename[512];
    static char pathstring[512];
    static const Uint32 TEXTS_OFFSET = 2048;
    static const Uint32 BITMAP_OFFSET = 4096;
  public:

    /** Different identifiers of the resources */
    typedef enum
    {
      RESCOSLIST = 0,
      MAP_GUARDIANS_20,
      MAP_GUARDIANS_40,
      MAP_CONGRATULATIONS_20,
      MAP_CONGRATULATIONS_40,
      MAP_MENU_20,
      MAP_MENU_40,
      RESGCURVES,
      DATA_BRICKS_LEVELS,
      RES60BACKG,
      DATA_LEVELS,

      TEXTS_SHOP = TEXTS_OFFSET,
      TEXTS_MESSAGES,
      TEXTS_SCROLL_MENU,
      TEXTS_POPUP_MENU,
      TEXTS_MAIN_MENU,

      BITMAP_HEAD_ANIMATION = BITMAP_OFFSET,
      BITMAP_RIGHT_PANEL,
      BITMAP_PADDLES_1,
      BITMAP_PADDLES_2,
      BITMAP_GAME_FONTS,
      BITMAP_MENU_FONTS,
      BITMAP_SMALL_FONTS,
      BITMAP_GIGABLITZ,
      BITMAP_TILESMAP,
      BITMAP_SHOP,
      BITMAP_ALL_SPRITES,
      BITMAP_BRICKS,
      BITMAP_SCORES_FONTS,

      MUSICAREA1 = 8192,

    }
    RESOURCE_IDENTFIERS;

    static const Sint16 cosinus360[720];
    static const Sint16 *zesinus360;
    static const Uint32 color_gradations[180];

  private:
    /** Size last file loaded in memory */
    Uint32 last_filesize_loaded;

  public:
    handler_resources ();
    ~handler_resources ();
    char *load_data (Uint32 resource_id);
    char *get_music_filename (Uint32 resource_id);
    char *get_sound_filename (Uint32 resource_id);
    char *get_tilemaps_filename (Uint32 tile_num);
    char *get_full_pathname (Uint32 resource_id);
    void load_sprites_bitmap (Uint32 resource_id = BITMAP_ALL_SPRITES);
    void release_sprites_bitmap ();
    Uint32 get_filesize_loaded ();
    void load_sinus ();
    char *load_high_score_file ();
    void save_high_score_file (char *buffer, Uint32 size);
    char *locate_data_file (const char *const name);
    char **load_texts (Uint32 resource_id, Uint32 maxof_lines =
                         0, Uint32 row_length = 0, Uint32 modulo = 0, bool upper_case = true);
    char *get_filename (Uint32 resource_id, Uint32 res = 0);

  private:
    char *loadfile_with_lang (const char *const filename, Uint32 * const fsize);
    char *load_file (const char *fname);
    char *load_file (const char *fname, Uint32 * fsize);
  };

#endif
