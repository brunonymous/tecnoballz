/** 
 * @file handler_resources.cc 
 * @brief Handler of the files resources 
 * @created 2004-04-20 
 * @date 2012-11-10
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
#include "../include/handler_resources.h"
#include "../include/bitmap_data.h"
#include "../config.h"
#include <string>
#include <fstream>

#ifndef DATADIR
#define DATADIR "/usr/share/games/tecnoballz"
#endif

#ifndef SCOREFILE
#define SCOREFILE "/var/lib/games/tecnoballz.hi"
#endif

#ifdef _WIN32
#ifndef _S_ISDIR
#define _S_ISDIR(m) (((m) & _S_IFMT) == _S_IFDIR)
#endif
#endif

handler_resources *
handler_resources::handler_resources_singleton = NULL;

const char * handler_resources::fnamescore = SCOREFILE;
const char *
  handler_resources::folder_640 = "hires/";
const char *
  handler_resources::folder_320 = "lores/";
char
  handler_resources::tmp_filename[512];
char
  handler_resources::pathstring[512];

const char *
  handler_resources::standfiles[] = {
  "cosinus128.list",            //RESCOSLIST
  /* MAP_GUARDIANS_20 */
  "tilemap-guardians_20.data",
  "tilemap-guardians_40.data",
  /* MAP_CONGRATULATIONS_20 */
  "tilemap-congratulation_20.data",
  "tilemap-congratulation_40.data",
  /* MAP_MENU_20 */
  "tilemap-menu_20.data",
  "tilemap-menu_40.data",
  "gard_lissa.list",            //RESGCURVES
  /* DATA_BRICKS_LEVELS */
  "tableau.data",
  "min60map.png",                //RES60BACKG
  /* DATA_LEVELS */
  "levels-data.xml"
};

const char *
  handler_resources::musicfiles[] = {
  "area1-game2.mod",
  "area2-game.mod",
  "area3-game.mod",
  "area4-game.mod",
  "area5-game.mod",
  "gardien-go.mod",
  "high-score.mod",
  "over-theme.mod",
  "tecnoballz.mod",
  "tecno-winn.mod",
  "termigator_reg-zbb.mod",
  "in-game-music-1_reg.mod",
  "fridge-in-space_from_reg-zbb.mod",
  "mon-lapin_reg-zbb.mod"
};

const char *
  handler_resources::soundfiles[] = { "rlife_moins.wav",        // 01
  "rexplo_rak.wav",
  "rlife_plus.wav",
  "rmoney.wav",
  "ralarm.wav",                 //05
  "rgadget_sound.wav",
  "rtecno.wav",
  "rappar.wav",
  "rtransfo.wav",
  "rtir_monstre.wav",           //10
  "rtir_rak.wav",
  "rexplo_big.wav",
  "rdetruit_indes.wav",
  "rexeplo_atom.wav",
  "rmontre_touche.wav",         //15
  "ratom.wav",
  "rindes_1.wav",
  "rindes_2.wav",
  "rraquette.wav",
  "rbricote.wav",               //20
  "rbrique1.wav",
  "rbrique2.wav",
  "rbrique3.wav",
  "rbrique4.wav",
  "rbrique5.wav",               //25
  "raspire.wav",
  "reject.wav",                 //27
};

const char * handler_resources::bitmap_files[] =
{
  /* BITMAP_HEAD_ANIMATION */
  "head_animation.png",
  /* BITMAP_RIGHT_PANEL */
  "right_panel.png",
  /* BITMAP_PADDLES_1 */
  "paddles_1.png",
  /* BITMAP_PADDLES_2 */
  "paddles_2.png",
  /* BITMAP_GAME_FONTS */
  "game_fonts.png",
  /* BITMAP_MENU_FONTS */
  "menu_fonts.png",
  /* BITMAP_SMALL_FONTS */
  "small_fonts.png",
  /* BITMAP_GIGABLITZ */
  "gigablitz.png",
  /* BITMAP_TILESMAP */
  "tilesmap.png",
  /* BITMAP_SHOP */
  "shop.png",
  /* BITMAP_ALL_SPRITES */
  "all_sprites.png",
  /* BITMAP_BRICKS */
  "bricks.png",
  /* BITMAP_SCORES_FONTS */
  "font_score.png"
};

const char * handler_resources::texts_files[] =
{
  /* TEXTS_SHOP */
  "shop_%s.txt",
  /* TEXTS_MESSAGES */
  "short_info_messages_%s.txt",
  /* TEXTS_SCROLL_MENU */
  "scrolltext_%s.txt",
  /* TEXTS_POPUP_MENU */
  "popup_menu_%s.txt",
  /* TEXTS_MAIN_MENU */
  "main_menu_%s.txt"
};

//char handler_resources::ze_mapfile[] = "map??.png";

/** 
 * Create the resources manager object
 */
handler_resources::handler_resources ()
{
  last_filesize_loaded = 0;
  set_filesize_loaded(0);
}

/**
 * Get the object instance
 * handler_resources is a singleton
 * @return the handler_resources object 
 */
handler_resources *
handler_resources::get_instance ()
{
  if (NULL == handler_resources_singleton)
    {
      handler_resources_singleton = new handler_resources ();
    }
  return handler_resources_singleton;
}

/**
 * Release the resources manager object
 */
handler_resources::~handler_resources ()
{
  if (table_cosL != NULL)
    {
      delete[](char *)table_cosL;
      table_cosL = (Sint16 *) NULL;
    }
  release_sprites_bitmap ();
}

/** 
 * Load a resources file in memory
 * @param resource_id resource identifier of the data
 * @return file data buffer pointer
 */
char *
handler_resources::load_data (Uint32 resource_id)
{
  char *filename = get_filename (resource_id);
  return load_file (filename);
}

/** 
 * Return valid name from a resource identifier
 * @param resource_id resource identifier
 * @param resolution 0 default, 1 = 320 or 2 640,
 * @return filename with a relative pathname
 */
char *
handler_resources::get_filename (Uint32 resource_id, Uint32 res)
{
  const char *pfile;
  if (resource_id >= BITMAP_OFFSET)
    {
      if (0 == res)
        {
          res = resolution;
        }
      resource_id -= BITMAP_OFFSET;
      pfile = bitmap_files[resource_id];
      if (1 == res)
        {
          strcpy (tmp_filename, folder_320);
        }
      else
        {
          strcpy (tmp_filename, folder_640);
        }
      strcat (tmp_filename, pfile);
    }
  else
    {
      pfile = standfiles[resource_id];
      strcpy (tmp_filename, pfile);
    }
  return tmp_filename;
}

/**
 * Return valid music filename from a resource identifier
 * @param resource_id resource identifier of the music
 * @return music filename with a relative pathname
 */
char *
handler_resources::get_music_filename (Uint32 resource_id)
{
  const char *pfile;
  strcpy (tmp_filename, "musics/");
  pfile = musicfiles[resource_id];
  strcat (tmp_filename, pfile);
  return locate_data_file (tmp_filename);
}

/**
 * Return valid sound filename from a resource identifier
 * @param resource_id resource identifier of the sound
 * @return sound filename with a relative pathname
 */
char *
handler_resources::get_sound_filename (Uint32 resource_id)
{
  strcpy (tmp_filename, "sounds/");
  strcat (tmp_filename, soundfiles[resource_id]);
  return locate_data_file (tmp_filename);
}

/**
 * Return valid tilemaps filename from a tilemap number
 * @param title_num filename number from 1 to 78
 * @return tilemap filename with a relative pathname
 */
char *
handler_resources::get_tilemaps_filename (Uint32 title_num)
{
  sprintf(tmp_filename, "textures/map%02d.png", title_num);
  return tmp_filename;
}

/**
 * Return the full pathname from a resource identifier
 * @param resource_id a resource identifier
 * @return a pointer to the file data buffer
 */
char *
handler_resources::get_full_pathname (Uint32 resource_id)
{
  return locate_data_file (get_filename (resource_id));
}

/** 
 * Directory list to locate a file 
 */
const char * handler_resources::folderlist[] =
{ 
  /* special value meaning "$(PREFIX)/share/games/tecnoballz/" */
  DATADIR,
  "/",
  /* normally unused, except when running from the source directory */
  "./TecnoballZ/",
  /* also marks end of list */
  0
};
/**
 * Locate a file under one of the data directories
 * @param name name of file relative to data directory
 */
char *
handler_resources::locate_data_file (const char *const name)
{
/*
  if (is_verbose)
    {
      std::
        cout << "(*) handler_resources::locate_data_file(" << name << ")" << std::
        endl;
    }
 */

  /* clear path name string */
  for (Sint32 i = 0; i < 256; i++)
    pathstring[i] = 0;

  if (NULL == name)
    {
      std::cerr << "(!)handler_resources::locate_data_file() " <<
        "NULL pointer was passed as an argument!" << std::endl;
      throw std::ios_base::
        failure ("[!] handler_resources::locate_data_file "
                 "NULL pointer was passed as an argument!");
    }

  /* if absolute path, return a pointer to a duplicate string */
  char *pathname;
  if (*name == '/')
    {
      pathname = &pathstring[0];
      strcpy (pathname, name);
      return pathname;
    }

  /* process each folder of the list */
  for (const char **p = folderlist;; p++)
    {
      if (*p != 0)
        {
          /* check if the file is located in current directory */
          pathname = &pathstring[0];
          strcpy (pathname, *p);
          if (pathname[strlen (pathname) - 1] != '/')
            {
              strcat (pathname, "/");
            }
          strcat (pathname, name);
         
        }
      else
        {
          /* file not found, try default folder as last chance */
          const char *subdir = "/share/games/tecnoballz/";
          pathname = &pathstring[0];
          strcpy (pathname, nomprefix);
          strcat (pathname, subdir);
          strcat (pathname, name);
        }
      /*
      if (is_verbose)
        {
          std::cout << "handler_resources::locate_data_file() try " << pathname << std::endl;
        }
      */
#ifdef WIN32
      struct _stat s;
      if (_stat (pathname, &s) == 0 && !_S_ISDIR (s.st_mode))
        {
          return pathname;
        }
#else
      struct stat s;
      if (stat (pathname, &s) == 0 && !S_ISDIR (s.st_mode))
        {
            /*
          if (is_verbose)
            {
              std::
                cout << "handler_resources::locate_data_file(" << pathname <<
                ") find!" << std::endl;
            }
            */
          return pathname;
        }
#endif
      /* end of the list, error file not found! */
      if (*p == 0)
        {
          break;
        }
    }
  std::cerr << "(!)handler_resources::locate_data_file() file '"
    << name << "' not found!" << std::endl;
  throw std::ios_base::failure (std::string
                                ("[!]handler_resources::locate_data_file() File '")
                                + name + std::string ("' not found!"));
}

/** 
 * Load a bitmap of sprites
 * @param resource_id resource identifier of the bitmap
 *                    BITMAP_ALL_SPRITES by default 
 */
void
handler_resources::load_sprites_bitmap (Uint32 resource_id)
{
  release_sprites_bitmap ();
  sprites_bitmap = new bitmap_data ();
  sprites_bitmap->load (resource_id);
  sprites_bitmap->enable_palette ();
}

/**
 * Release the bitmap of sprites
 */
void
handler_resources::release_sprites_bitmap ()
{
  if (sprites_bitmap != NULL)
    {
      delete sprites_bitmap;
    }
  sprites_bitmap = (bitmap_data *) NULL;
}

/**
 * Load texts data into strings list
 * @param resource_id resource identifier of the texts data 
 * @param numof_lines number of lines
 * @param row_length maximum number of chars by string, 0 if preserve the size of
 *                   the original string  
 * @param modulo 0 if non concatenation, 2 concatene strings 3 by 3 
 * @param upper_case Change from 'a' to 'z' chars by 'A' to 'Z' chars 
 */
char **
handler_resources::load_texts(Uint32 resource_id, Uint32 numof_lines, Uint32 row_length, Uint32 modulo, bool upper_case)
{
  resource_id -=TEXTS_OFFSET; 
  const char *file = texts_files[resource_id];
  strcpy (tmp_filename, "texts/");
  strcat (tmp_filename, file);
  Uint32 filesize;
  char *filedata = loadfile_with_lang (tmp_filename, &filesize);
 
  /* 
   * caclulate the number of lines 
   */
  Uint32 offset = 0;
  bool is_first_row = true;
  bool is_comment = false;
  Uint32 row_count = 0;
  Uint32 alloc_size = 0;
  Uint32 list_count = 0;
  Uint32 str_count = 0;
  while (offset < filesize)
    {
      char c = filedata[offset++];
      row_count++;
      if (is_first_row && c == '#')
        {
          is_comment = true;
        } 
      is_first_row = false;
      if (c == '\n') 
        {
          if (!is_comment)
            {
              if (row_length > 0)
                {
                  alloc_size += row_length;
                }
              else
                {
                  alloc_size += row_count - 1;
                }
              str_count++;
              if (modulo == 0 || (str_count % modulo == 0))
                {
                  /* null-terminated string */
                  alloc_size++;
                  list_count++;
                }
            }
          is_first_row = true;
          is_comment = false;
          row_count = 0;
        }
     }

  if (numof_lines > 0 && numof_lines != list_count) 
    {
      std::cerr << "(!)handler_resources::load_texts() " <<
        numof_lines << " exceptes lines, read " << list_count <<
        " lines!" << std::endl;
      throw std::runtime_error ("(!))handler_resources::load_texts() "
          "bad number of lines!");
    }

  /*
   * allocate memory require to create strings list
   */
  alloc_size += sizeof(char *) * list_count;
  char *buffer = NULL;
  try
  {
    buffer = new char[alloc_size];
  }
  catch (std::bad_alloc &)
  {
    std::cerr << "(!)handler_resources::load_texts() " <<
      "not enough memory to allocate " <<
      alloc_size << " bytes!" << std::endl;
    throw;
  }
  char **list = (char**) buffer;
  char *strs = buffer + sizeof(char *) * list_count; 


  offset = 0;
  is_first_row = true;
  is_comment = false;
  row_count = 0;
  char* source = filedata;
  list_count = 0;
  str_count = 0;
  char *str_current = strs;
  while (offset < filesize)
    {
      char c = filedata[offset++];
      row_count++;
      if (is_first_row && c == '#')
        {
          is_comment = true;
        } 
      is_first_row = false;
      if (c == '\n') 
        {
          if (!is_comment)
            {
              /* do not copy the carriage return */
              row_count--;
              if (row_length > 0 && row_count >= row_length) 
                {
                  row_count = row_length;
                }
              for (Uint32 i = 0; i < row_count; i++)
                {
                  char c = source[i];
                  if (upper_case && c >= 'a' && c <= 'z') 
                    {
                      c = c - ('a' - 'A');
                    }
                  if (c < ' ')
                    {
                      c = ' ';
                    }
                  *(strs++) = c;
                }
              for (Uint32 i = row_count; i < row_length; i++)
                {
                  *(strs++) = ' ';
                }
              str_count++;
              if (modulo == 0 || (str_count % modulo == 0))
                {
                  *(strs++) = '\0';
                  list[list_count++] = str_current;
                  str_current = strs;
                }
            }
          is_first_row = true;
          is_comment = false;
          row_count = 0;
          source = &filedata[offset];
        }
     }
  delete[]filedata;
  return list; 
}

/**
 * Allocate memory and load a file (filename with a language code)
 * @param filename specified by path
 * @param fsize pointer on the size of file which will be loaded 
 * @return a pointer to the file data
 */
char *
handler_resources::loadfile_with_lang (const char *const filename, Uint32 * const fsize)
{
  if(filename == NULL || strlen (filename) == 0)
    {
      std::cerr << "(!)handler_resources::loadfile_with_lang() " <<
        "NULL string!" << std::endl; 
      throw std::ios_base::failure ("(!)handler_resources::loadfile_with_lang() "
                                    "can't open a file!");
    }
  char* fname = new char[strlen (filename) + 1];
  strcpy (fname, filename);
  const char* lang = config_file->get_language ();
  sprintf (fname, filename, lang);
  if (is_verbose)
    {
      std::cout << "handler_resources::loadfile_with_lang() " <<
        "file " << fname << " was loaded in memory" << std::endl;
    }
  char* data = load_file (fname, fsize);
  delete[]fname; 
  return data;
}

/**
 * Load a file in memory 
 * @param fname filename specified by path
 */
char *
handler_resources::load_file (const char *fname)
{
  return load_file (fname, &last_filesize_loaded);
}

/**
 * Load a file in memory 
 * @param fname filename specified by path
 * @param fsize pointer on the size of file which will be loaded 
 * return a pointer to the file data 
*/
char *
handler_resources::load_file (const char *fname, Uint32 * fsize)
{
  /* locate a file under one of the data directories */
  char *pname = locate_data_file (fname);

  /* open the file */
#ifdef WIN32
  Sint32 fhand = open (pname, O_RDONLY | O_BINARY, 0);
#else
  Sint32 fhand = open (pname, O_RDONLY, 0);
#endif
  if (fhand == -1)
    {
      std::cerr << "(!)handler_resources::load_file() " <<
        "can't open file " << fname
        << "strerror:" << strerror (errno) << std::endl;
      throw std::ios_base::failure ("(!)handler_resources::load_file() "
                                    "can't open a file!");
    }

  /* read the size of the file */
  struct stat sb;
  if (fstat (fhand, &sb))
    {
      std::cerr << "(!)handler_resources::load_file() " <<
        "can't stat file " << fname
        << "strerror:" << strerror (errno) << std::endl;
      throw std::ios_base::failure ("(!)handler_resources::load_file() "
                                    "can't stat a file!");
    }
  /* save filesize */
  (*fsize) = sb.st_size;

  /* allocate memory require to load the filedata */
  char *buffer = NULL;
  try
  {
    buffer = new char[sb.st_size];
  }
  catch (std::bad_alloc &)
  {
    std::cerr << "(!)handler_resources::load_file() " <<
      "not enough memory to allocate " <<
      sb.st_size << " bytes!" << std::endl;
    throw;
  }

  /* read the file */
  if (read (fhand, buffer, sb.st_size) != sb.st_size)
    {
      std::cerr << "(!)handler_resources::load_file() " <<
        "can't read file " << fname
        << "strerror:" << strerror (errno) << std::endl;
      throw std::ios_base::failure ("(!)handler_resources::load_file() "
                                    "can't read a file!");
      return NULL;
    }

  /* close the file */
  close (fhand);
  return buffer;
}

/**
 * Return size last file loaded in memory
 * @return the size of the last filesize previously
 */
Uint32
handler_resources::get_filesize_loaded ()
{
  return last_filesize_loaded;
}


void
handler_resources::set_filesize_loaded (Uint32 size)
{
  Uint32 i = size;
  i++;
  last_filesize_loaded = size;
}

char*
handler_resources::read_complete_file (const char* filename )
{
  std::ifstream file (filename, std::ios::ate);
  if (! file.is_open () )
    {
      std::cerr << "(!)handler_ressources::read_complete_file()"  
        "can't open file '" << filename << "'" << std::endl;
      //throw std::ios_base::failure ("(!)handler_ressources::read_complete_file() can't read a file!");
      return NULL;
    }
  //Uint32 size = (Uint32) file.tellg();
  std::ifstream::pos_type size = file.tellg();
  //int filesize = (int)size;
  //last_filesize_loaded = 1;
  //last_filesize_loaded = 2;
  //last_filesize_loaded = size;
  char *buffer = NULL; 
  try
  {
    buffer = new char[size];
  }
  catch (std::bad_alloc &)
  {
    file.close();
    std::cerr << "(!)handler_resources::read_complete_file() " <<
      "not enough memory to allocate " <<
      size << " bytes!" << std::endl;
    throw;
  }
  file.seekg (0, std::ios::beg);
  file.read (buffer, size);
  file.close();
  return buffer;
}

/**
 * Load a precalculated sinus & cosinus table (1790 bytes <=> 895 values)
 * 0 to 511 cosinus / 383 to 894 sinus
 */
void
handler_resources::load_sinus ()
{
  table_cosL = (Sint16 *) load_data (handler_resources::RESCOSLIST);
  table_sinL = table_cosL + 383;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  /* convert big endian values to little endian values */    
  Sint32 fsize = get_filesize_loaded ();
  /* convert bytes = 16-bits words */
  fsize = fsize / 2;
  for (Sint32 i = 0; i < fsize; i++)
    {
      char *p = (char *) &table_cosL[i];
      char a = p[0];
      p[0] = p[1];
      p[1] = a;
    }
#endif
}

/**
 * load scores table
 */
char *
handler_resources::load_high_score_file ()
{
  char* filedata = NULL;
  try
    {
      filedata = load_file (fnamescore, &last_filesize_loaded);
    }
  catch (...)
    {
       std::cerr << "(!)handler_resources::load_high_score_file() " <<
         "can't open the scores files!" << std::endl;
       filedata = NULL;
    }
  return filedata;
}

/* 
 * Save scores table
 * @param buffer
 * @pram size
 */
void
handler_resources::save_high_score_file (char *buffer, Uint32 size)
{
  size_t left;
  ssize_t bytes_written;
#ifdef WIN32
  /* set umask so that files are group-writable */
  _umask (0002);
#else
  umask (0002);
#endif
  Sint32 fhand = open (fnamescore, O_WRONLY | O_CREAT, 00666);
  if (fhand == -1)
    {
      fprintf (stderr,
               "handler_resources::saveScores(): file:%s / error:%s\n",
               fnamescore, strerror (errno));
    }
  left = size;
  while (left > 0)
    {
#ifdef WIN32
      bytes_written = _write (fhand, buffer + size - left, left);
#else
      bytes_written = write (fhand, buffer + size - left, left);
      if (bytes_written == -1)
        {
          std::cerr << "(!)handler_resources::saveScores():" <<
             " filename: " << fnamescore << "; error: " << strerror (errno);  
          close (fhand);
          return;
        }
        left -= bytes_written;
    }
#endif
  if (close (fhand) == -1)
    {
      fprintf (stderr,
               "handler_resources::saveScores(): file:%s / error:%s\n",
               fnamescore, strerror (errno));
    }
  else
    {
      if (is_verbose)
        fprintf (stdout, "handler_resources::saveScores(): "
                 "file:%s size:%i\n", fnamescore, size);
    }
}

/**
 * Precalculated cosinus and sinus table
 */
const Sint16
  handler_resources::cosinus360[720] =
  { 0, 2, 4, 7, 9, 11, 13, 15, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41,
  43,
  46, 48, 50, 52, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 75, 76, 78, 80,
  82, 83, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 101, 103, 104, 105,
  107,
  108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121,
  121, 122, 123, 123, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124,
  124, 123, 123, 122, 121, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113,
  112, 111, 110, 109, 108, 107, 105, 104, 103, 101, 100, 99, 97, 96, 94, 93,
  91,
  90, 88, 87, 85, 83, 82, 80, 78, 76, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58,
  56, 54, 52, 50, 48, 46, 43, 41, 39, 37, 35, 33, 31, 29, 26, 24, 22, 20, 18,
  16, 13, 11, 9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22,
  -24,
  -26, -29, -31, -33, -35, -37, -39, -41, -43, -45, -48, -50, -52, -54, -56,
  -58, -60, -62, -63, -65, -67, -69, -71, -73, -75, -76, -78, -80, -82, -83,
  -85, -87, -88, -90, -91, -93, -94, -96, -97, -99, -100, -101, -103, -104,
  -105, -106, -108, -109, -110, -111, -112, -113, -114, -115, -116, -117,
  -118,
  -119, -119, -120, -121, -121, -122, -123, -123, -124, -124, -125, -125,
  -125,
  -126, -126, -126, -127, -127, -127, -127, -127, -127, -127, -127, -127,
  -127,
  -127, -126, -126, -126, -125, -125, -125, -124, -124, -123, -123, -122,
  -121,
  -121, -120, -119, -119, -118, -117, -116, -115, -114, -113, -112, -111,
  -110,
  -109, -108, -107, -105, -104, -103, -101, -100, -99, -97, -96, -94, -93,
  -91,
  -90, -88, -87, -85, -83, -82, -80, -78, -76, -75, -73, -71, -69, -67, -65,
  -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -43, -41, -39, -37, -35,
  -33, -31, -29, -26, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2, 127,
  127, 127, 127, 127, 127, 126, 126, 126, 125, 125, 125, 124, 124, 123, 123,
  122, 121, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110,
  109, 108, 107, 105, 104, 103, 101, 100, 99, 97, 96, 94, 93, 91, 90, 88, 87,
  85, 83, 82, 80, 78, 76, 75, 73, 71, 69, 67, 65, 64, 62, 60, 58, 56, 54, 52,
  50, 48, 46, 43, 41, 39, 37, 35, 33, 31, 29, 26, 24, 22, 20, 18, 15, 13, 11,
  9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -15, -18, -20, -22, -24, -26, -29,
  -31, -33, -35, -37, -39, -41, -43, -45, -48, -50, -52, -54, -56, -58, -60,
  -62, -63, -65, -67, -69, -71, -73, -75, -76, -78, -80, -82, -83, -85, -87,
  -88, -90, -91, -93, -94, -96, -97, -99, -100, -101, -103, -104, -105, -107,
  -108, -109, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119,
  -119,
  -120, -121, -121, -122, -123, -123, -124, -124, -125, -125, -125, -126,
  -126,
  -126, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127,
  -126,
  -126, -126, -125, -125, -125, -124, -124, -123, -123, -122, -121, -121,
  -120,
  -119, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109,
  -108,
  -107, -105, -104, -103, -101, -100, -99, -97, -96, -94, -93, -91, -90, -88,
  -87, -85, -83, -82, -80, -78, -76, -75, -73, -71, -69, -67, -65, -64, -62,
  -60, -58, -56, -54, -52, -50, -48, -46, -43, -41, -39, -37, -35, -33, -31,
  -29, -26, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2, 0, 2, 4, 7, 9,
  11, 13, 15, 18, 20, 22, 24, 26, 29, 31, 33, 35, 37, 39, 41, 43, 45, 48, 50,
  52, 54, 56, 58, 60, 62, 63, 65, 67, 69, 71, 73, 75, 76, 78, 80, 82, 83, 85,
  87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 101, 103, 104, 105, 106, 108, 109,
  110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 121, 122,
  123, 123, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127, 127, 127
};
const Sint16 *
  handler_resources::zesinus360 = handler_resources::cosinus360 + 360;


const
  Uint32
  handler_resources::color_gradations[180] =
  { 0x0400180, 0x0420290, 0x0440392, 0x0500494, 0x0600596, 0x0700698,
  0x0800795, 0x0900893, 0x0A00990, 0x0A20A80, 0x0A40B70, 0x0A60C60,
  0x0A80D50, 0x0AA0E40, 0x0AC0F30, 0x0AE1020, 0x0B01110, 0x0B21200,
  0x0014080, 0x0024590, 0x0034A92, 0x0045094, 0x0056096, 0x0067097,
  0x0078096, 0x0089093, 0x009A090, 0x00AA180, 0x00BA270, 0x00CA360,
  0x00DA450, 0x00EA540, 0x00FA630, 0x010A720, 0x011A810, 0x012A900,
  0x0408001, 0x0459002, 0x04A9203, 0x0509404, 0x0609605, 0x0709806,
  0x0809507, 0x0909308, 0x0A19009, 0x0A2800A, 0x0A3700B, 0x0A4600C,
  0x0A5500D, 0x0A6400E, 0x0A7300F, 0x0A82010, 0x0A91011, 0x0AA0012,
  0x0002080, 0x0102190, 0x0202292, 0x0302394, 0x0402496, 0x0502598,
  0x0602695, 0x0702793, 0x0802890, 0x0782980, 0x0702A90, 0x0602B92,
  0x0502C94, 0x0402D96, 0x0302E98, 0x0202F95, 0x0102A93, 0x0002B92,
  0x0104080, 0x0204590, 0x0304A92, 0x0405094, 0x0506096, 0x0607098,
  0x0708095, 0x0809093, 0x090A090, 0x0A0A280, 0x0B0A470, 0x0C0A660,
  0x0B0A850, 0x0A0AA40, 0x090AC30, 0x080AE20, 0x070B010, 0x060B210,
  0x04080B0, 0x04590A0, 0x04A9290, 0x0509480, 0x0609670, 0x0709860,
  0x0809550, 0x0909340, 0x0A09030, 0x0A18020, 0x0A27010, 0x0A36020,
  0x0A45030, 0x0A54040, 0x0A63050, 0x0A72060, 0x0A81070, 0x0A90080,
  0x0101080, 0x0181290, 0x0201492, 0x0281694, 0x0301896, 0x0381A98,
  0x0401C95, 0x0451E93, 0x04A2080, 0x0502270, 0x0582460, 0x0602650,
  0x0702840, 0x0782B30, 0x0802E20, 0x0853010, 0x08A3205, 0x0903600,
  0x0404080, 0x0484890, 0x0504A91, 0x0605092, 0x0706093, 0x0807094,
  0x0908093, 0x0889092, 0x080A090, 0x081A280, 0x082A670, 0x083A860,
  0x084AA50, 0x085AC40, 0x086AE30, 0x087B020, 0x088B210, 0x089B405,
  0x0458040, 0x04A9045, 0x040924A, 0x0509450, 0x0609655, 0x070985A,
  0x0809562, 0x0909364, 0x0A09066, 0x0A1806A, 0x0A2706C, 0x0A36070,
  0x0A45073, 0x0A54076, 0x0A6307A, 0x0A72080, 0x0A81085, 0x0A90590,
  0x0407080, 0x0457890, 0x04A8091, 0x0508892, 0x0609093, 0x0709894,
  0x080A093, 0x090A892, 0x0A0B090, 0x0A1B880, 0x0A2C070, 0x0A3B860,
  0x0A4B050, 0x0A5A840, 0x0A6A030, 0x0A79820, 0x0A89010, 0x0A98805
};
