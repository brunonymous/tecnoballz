/** 
 * @file handler_levels.cc 
 * @brief Levels handler
 * @created 2004-04-06 
 * @date 2012-09-04 
 * @copyright 1991-2007 TLK Games
 * @author Bruno Ethvignot
 * @version $Revision: 1.2 $
 */
/* 
 * copyright (c) 1991-2007 TLK Games all rights reserved
 * $Id: handler_levels.cc,v 1.2 2007-09-12 06:32:48 gurumeditation Exp $
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
using namespace std;
#include "../include/handler_levels.h"
#include "../include/handler_resources.h"
#include <tinyxml.h>
#include <string.h>
#include <sstream>


/**
 * Create a level handler objet
 */
handler_levels::handler_levels ()
{
  object_init ();
  time_multiplier = TIME_MULTIPLIER;
  levels_list = NULL;
  bricks_levels = NULL;
  guardians_levels = NULL;
  caspsules_list = NULL;
  levels_counter = 0;
  bricks_levels_counter = 0;
  guardians_levels_counter = 0;
  capsules_lists_counter = 0;
  level_index = -1;
  bricks_level_index = -1;
  guardians_level_index = -1;
  appearance_index = -1;
  capsule_list_index = -1;
  capsule_index = -1;
  char *fpath = resources->get_full_pathname (handler_resources::DATA_LEVELS);
  xml_levels = new TiXmlDocument (fpath);
  if (!xml_levels->LoadFile ())
    {
      cerr << "(!)handler_levels::handler_levels() " <<
        "failed to load file " << fpath << endl;
      throw runtime_error ("(!)handler_levels::handler_levels()"
                           "Fail to load XML file!");
    }

  /* count number of structures elements */
  check_xml (xml_levels, ROOT);
  if (is_verbose)
    {
      cout << "handler_levels::handler_levels() " <<
        " levels_counter: " << levels_counter <<
        "; guardians_levels_counter: " << guardians_levels_counter <<
        "; capsules_lists_counter: " << capsules_lists_counter << endl;
    }

  /* allocate levels structures */
  try
  {
    bricks_levels = new bricks_level_desc[bricks_levels_counter];
    guardians_levels = new guardians_level_desc[guardians_levels_counter];
    caspsules_list = new capsules_struct[capsules_lists_counter];
    levels_list = new level_desc[levels_counter];
  }
  catch (bad_alloc &)
  {
    cerr << "(!)handler_levels::handler_levels() " <<
      "not enough memory to allocate levels structures!" << endl;
    throw;
  }
  /* clear levels structure */
  for (Uint32 i = 0; i < levels_counter; i++)
    {
      levels_list[i].id = 0;
      levels_list[i].type = BRICKS_LEVEL;
    }
  /* copy XML file data in levels structures */
  parse (xml_levels, ROOT);
  check_levels ();
}

/**
 * Check is levels structure is valid
 */
void
handler_levels::check_levels ()
{
  for (Uint32 i = 0; i < levels_counter; i++)
    {
      if (BRICKS_LEVEL == levels_list[i].type)
        {
          //bricks_level_desc *level =
          get_bricks_level (levels_list[i].id);
        }
      else if (GUARDIANS_LEVEL == levels_list[i].type)
        {
          //guardians_level_desc *level =
          get_guardians_level (levels_list[i].id);
        }
      else
        {
          cerr << "(!)handler_levels::check_levels() '" <<
            levels_list[i].type << "' is a invalid level type!" << endl;
          throw runtime_error ("(!)handler_levels::check_levels()"
                               "bad level type!");
        }
    }
}

/**
 * Check XML file and count nodes 
 * @param parent current node
 * @param node node indentifier of the seconde level
 */
void
handler_levels::check_xml (TiXmlNode * parent, Uint32 node)
{
  if (NULL == parent)
    {
      return;
    }
  Sint32 type = parent->Type ();
  switch (type)
    {
    case TiXmlNode::TINYXML_ELEMENT: 
      {
        string element = parent->Value ();
        if (element == "level")
          {
            node = LEVEL_NODE;
            levels_counter++;
            break;
          }
        if (element == "bricks_level")
          {
            node = BRICKS_LEVEL_NODE;
            bricks_levels_counter++;
            break;
          }
        if (element == "guardians_level")
          {
            node = GUARDIANS_LEVEL_NODE;
            guardians_levels_counter++;
            break;
          }
        if (element == "capsules")
          {
            if (capsules_lists_counter > 0)
              {
                if (capsules_counter != MAX_OF_CASPULES)
                  {
                    cerr << "(!)handler_levels::check_xml() " <<
                      MAX_OF_CASPULES << " <id> childnodes "
                      << " of <capsules> chilnodes expected."
                      << " But " << capsules_counter << " found!" << endl;
                    throw runtime_error ("(!)handler_levels::check_xml()"
                                         "Bad number of <id> childnodes!");
                  }
              }
            capsules_counter = 0;
            node = CAPSULES_NODE;
            capsules_lists_counter++;
            break;
          }
        if (element == "id" and node == CAPSULES_NODE)
          {
            capsules_counter++;
            break;
          }
      }
      break;
    }
  for (TiXmlNode * child = parent->FirstChild (); NULL != child;
       child = child->NextSibling ())
    {
      check_xml (child, node);
    }
}

/**
 * Copy XML file data in levels structures 
 * @param parent current node
 * @param node node indentifier of the seconde level
 */
void
handler_levels::parse (TiXmlNode * parent, Uint32 node)
{
  if (NULL == parent)
    {
      return;
    }
  Sint32 type = parent->Type ();
  string value_str;

  TiXmlText *text;
  TiXmlElement *element;
  TiXmlAttribute *attribute;
  stringstream input_stream;
  Sint32 value;
  string name;
  switch (type)
    {
    case TiXmlNode::TINYXML_ELEMENT: 
      {
        element = parent->ToElement ();
        last_element = parent->Value ();
        if (last_element == "level")
          {
            node = LEVEL_NODE;
            level_index++;
            attribute = element->FirstAttribute ();
            while (NULL != attribute)
              {
                name = attribute->Name ();
                if (attribute->QueryIntValue (&value) != TIXML_SUCCESS)
                  {
                    cerr << "(!)handler_levels::parse() '" << name <<
                      "' attribute is not a integer!" << endl;
                    throw runtime_error ("(!)handler_levels::parse()"
                                         "an attribute is not a integer!");
                  }
                if (value < 0)
                  {
                    cerr << "(!)handler_levels::parse() '" << name <<
                      "' attribute is a negative integer!" << endl;
                    throw runtime_error ("(!)handler_levels::parse()"
                                         "an attribute is negative integer!");
                  }
                if ("id" == name)
                  {
                    levels_list[level_index].id = value;
                  }
                if ("type" == name)
                  {
                    if (value >= MAX_OF_LEVEL_TYPES)
                      {
                        cerr << "(!)handler_levels::parse() '" << value <<
                          "' is a invalid level type!" << endl;
                        throw runtime_error ("(!)handler_levels::parse()"
                                             "bad level type attribute!");
                      }
                    levels_list[level_index].type = value;
                  }
                attribute = attribute->Next ();
              }
            break;
          }
        if (last_element == "bricks_level")
          {
            node = BRICKS_LEVEL_NODE;
            appearance_index = -1;
            bricks_level_index++;
            break;
          }
        if (last_element == "guardians_level")
          {
            node = GUARDIANS_LEVEL_NODE;
            guardians_level_index++;
            break;
          }
        if (last_element == "capsules")
          {
            node = CAPSULES_NODE;
            capsule_list_index++;
            capsule_index = -1;
            break;
          }
      }
      break;


    case TiXmlNode::TINYXML_TEXT:  
      text = parent->ToText ();
      value_str = text->Value ();

      if (!sscanf (value_str.c_str (), "%d", &value))
        {
          cerr << "(!)handler_levels::parse() '" << last_element <<
            "' element is not a integer!" << endl;
          throw runtime_error ("(!)handler_levels::parse()"
                               "an element is not a integer!");
        }
      if (value < 0)
        {
          cerr << "(!)handler_levels::parse() '" << last_element <<
            "' element is a negative integer!" << endl;
          throw runtime_error ("(!)handler_levels::parse()"
                               "an element is negative integer!");
        }

      switch (node)
        {
        case LEVEL_NODE:
          break;
        case CAPSULES_NODE:
          if (last_element == "identifier")
            {
              caspsules_list[capsule_list_index].id = value;
            }
          if (last_element == "id")
            {
              /* collect all the codes caspules */
              capsule_index++;
              caspsules_list[capsule_list_index].codes[capsule_index] = value;
            }
          break;
        case BRICKS_LEVEL_NODE:
          if (last_element == "id")
            {
              bricks_levels[bricks_level_index].id = value;
            }
          if (last_element == "appearance")
            {
              appearance_index++;
              switch (appearance_index)
                {
                case 0:
                  bricks_levels[bricks_level_index].ship_appearance_delay1 =
                    value * time_multiplier;
                  break;
                case 1:
                  bricks_levels[bricks_level_index].ship_appearance_delay2 =
                    value * time_multiplier;
                  break;
                case 2:
                  bricks_levels[bricks_level_index].ship_appearance_delay3 =
                    value * time_multiplier;
                  break;
                case 3:
                  bricks_levels[bricks_level_index].ship_appearance_delay4 =
                    value * time_multiplier;
                  break;
                default:
                  cerr << "(!) handler_levels::parse() " <<
                    "ranking values must be between 0 and 3 inclusive!" <<
                    endl;
                  break;
                }
            }
          if (last_element == "reappearance")
            {
              bricks_levels[bricks_level_index].reappearance =
                value * time_multiplier;
            }
          if (last_element == "strength")
            {
              bricks_levels[bricks_level_index].ships_strength = value;
            }
          if (last_element == "penalties_frequency")
            {
              bricks_levels[bricks_level_index].penalties_frequency = value;
            }
          if (last_element == "moneys_frequency")
            {
              bricks_levels[bricks_level_index].moneys_frequency = value;
            }
          if (last_element == "penalties_list_id")
            {
              bricks_levels[bricks_level_index].malusListe =
                get_capsules_list (value);;
            }
          if (last_element == "starting_speed")
            {
              bricks_levels[bricks_level_index].starting_speed = value;
            }
          if (last_element == "acceleration_delay")
            {
              bricks_levels[bricks_level_index].acceleration_delay =
                value * time_multiplier;
            }
          if (last_element == "ball_release_time")
            {
              bricks_levels[bricks_level_index].ball_release_time =
                value * time_multiplier;
            }
          if (last_element == "glue_time")
            {
              bricks_levels[bricks_level_index].glue_time =
                value * time_multiplier;
            }
          if (last_element == "tilt_delay")
            {
              bricks_levels[bricks_level_index].tilt_delay =
                value * time_multiplier;
            }
          break;
        case GUARDIANS_LEVEL_NODE:
          if (last_element == "id")
            {
              guardians_levels[guardians_level_index].id = value;
            }
          if (last_element == "starting_speed")
            {
              guardians_levels[guardians_level_index].starting_speed = value;
            }
          if (last_element == "ball_release_time")
            {
              guardians_levels[guardians_level_index].ball_release_time =
                value * time_multiplier;
            }
          if (last_element == "tilt_delay")
            {
              guardians_levels[guardians_level_index].tilt_delay =
                value * time_multiplier;
            }
          if (last_element == "scroll_delay")
            {
              guardians_levels[guardians_level_index].scroll_delay =
                value * time_multiplier;
            }
          if (last_element == "scroll_id")
            {
              guardians_levels[guardians_level_index].scroll_id = value;
            }
          if (last_element == "capsules_frequency")
            {
              guardians_levels[guardians_level_index].capsules_frequency =
                value;
            }
          if (last_element == "capsules_list_id")
            {
              guardians_levels[guardians_level_index].capsules_list =
                get_capsules_list (value);
            }
          break;

        default:
          break;
        }
      break;
    default:
      break;
    }
  for (TiXmlNode * child = parent->FirstChild (); NULL != child;
       child = child->NextSibling ())
    {
      parse (child, node);
    }
}

/**
 * Search and returns the guardians level corresponding
 * to a given identifier 
 * @param guardians level identifier
 * @return a pointer to guardians level 
 */
guardians_level_desc *
handler_levels::get_guardians_level (Uint32 id)
{
  for (Uint32 i = 0; i < guardians_levels_counter; i++)
    {
      guardians_level_desc *level = &guardians_levels[i];
      if (level->id == id)
        {
          return level;
        }
    }
  cerr << "(!)handler_levels::get_guardians_level() " <<
    "id " << id << " not found!" << endl;
  throw runtime_error ("(!)handler_levels::get_guardians_level()"
                       "ID not found!");
}

/**
 * Search and returns the bricks level corresponding
 * to a given identifier 
 * @param bricks level identifier
 * @return a pointer to a bricks level 
 */
bricks_level_desc *
handler_levels::get_bricks_level (Uint32 id)
{
  for (Uint32 i = 0; i < bricks_levels_counter; i++)
    {
      bricks_level_desc *level = &bricks_levels[i];
      if (level->id == id)
        {
          return level;
        }
    }
  cerr << "(!)handler_levels::get_bricks_level() " <<
    "id " << id << " not found!" << endl;
  throw runtime_error ("(!)handler_levels::get_bricks_level()"
                       "ID not found!");
}

/**
 * Search and returns the list of capsules corresponding
 * to a given identifier 
 * @param capsules list identifier
 * @return a pointer to the list of capsules codes
 */
Uint32 *
handler_levels::get_capsules_list (Uint32 id)
{
  for (Uint32 i = 0; i < capsules_lists_counter; i++)
    {
      capsules_struct *capsule = &caspsules_list[i];
      if (capsule->id == id)
        {
          return &capsule->codes[0];
        }
    }
  cerr << "(!)handler_levels::get_capsules_list() " <<
    "id " << id << " not found!" << endl;
  throw runtime_error ("(!)handler_levels::get_capsules_list()"
                       "ID not found!");
}


/**
 * Release the level handler object
 */
handler_levels::~handler_levels ()
{
  if (NULL != levels_list)
    {
      delete[]levels_list;
      levels_list = NULL;
    }
  if (NULL != bricks_levels)
    {
      delete[]bricks_levels;
      bricks_levels = NULL;
    }
  if (NULL != caspsules_list)
    {
      delete[]caspsules_list;
      caspsules_list = NULL;
    }
  if (NULL != guardians_levels)
    {
      delete[]guardians_levels;
      guardians_levels = NULL;
    }
}

/**
 * Return the params of a guardian level 
 * @param area_num area number from 1 to 5
 * @param level_num level number: 6, 12 or 13
 * @return a pointer to a guardians level
 */
const bricks_level_desc *
handler_levels::get_bricks_levels_params (Uint32 area_num, Uint32 level_num)
{
  if (area_num < 1 || area_num > 5)
    {
      cerr << "(!)handler_levels::get_bricks_levels_params() " << "area_num:"
        << area_num << "out of range!" << endl;
      area_num = 1;
    }
  if (level_num < 1 || level_num > NUM_OF_LEVELS_PER_AREA)
    {
      cerr << "(!)handler_levels::get_bricks_levels_params() " << "level:" <<
        level_num << "out of range!" << endl;
      level_num = 1;
    }
  if (level_num == 6 || level_num == NUM_OF_LEVELS_PER_AREA)
    {
      cerr << "(!)handler_levels::get_bricks_levels_params() " << "level:" <<
        level_num << " is a guardians level!" << endl;
      level_num--;
    }
  Uint32 index = (area_num - 1) * NUM_OF_LEVELS_PER_AREA + (level_num - 1);
  index = levels_list[index].id;
  return get_bricks_level (index);
  //return giga_amiga[i];
}

/**
 * Return the params of a guardian level 
 * @param area_num area number from 1 to 5
 * @param level_num level number: 6, 12 or 13
 * @return a pointer to a guardians level
 */
const guardians_level_desc *
handler_levels::get_guardians_levels_params (Uint32 area_num,
                                             Uint32 level_num)
{
  if (area_num < 1 || area_num > 5)
    {
      cerr << "(!)handler_levels::get_guardians_levels_params() " <<
        "area_num:" << area_num << "out of range!" << endl;
      area_num = 1;
    }
  if (level_num < 1 || (area_num < 5 && level_num > NUM_OF_LEVELS_PER_AREA) ||
      (area_num == 5 && level_num > (NUM_OF_LEVELS_PER_AREA + 1)))
    {
      cerr << "(!)handler_levels::get_guardians_levels_params() " << "level:"
        << level_num << "out of range!" << endl;
      level_num = 6;
    }
  if (level_num != 6 && level_num != NUM_OF_LEVELS_PER_AREA
      && level_num != (NUM_OF_LEVELS_PER_AREA + 1))
    {
      cerr << "(!)handler_levels::get_guardians_levels_params() " << "level:"
        << level_num << " is a bricks level!" << endl;
      level_num = 6;
    }
  Uint32 index = (area_num - 1) * NUM_OF_LEVELS_PER_AREA + (level_num - 1);
  index = levels_list[index].id;
  return get_guardians_level (index);
  //return (guardians_level_desc *) giga_amiga[i];
}

/*
// :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)   (-: (-: (-: (-: (-: (-: (-: (-: (-:   
// Liste des malus/bonus pouvant tomber pendant le jeu
// :-) :-) :-) :-) :-) :-) :-) :-) :-) :-)   (-: (-: (-: (-: (-: (-: (-: (-: (-:   
// Table des gardiens
const Uint32
  handler_levels::zeCourseXX[] = {
  4, 22, 20, 24, 20, 20, 4, 20, // 4=GAD_PROTEC 22=GAD_POWER1 20=GAD_BALLE3
  20, 4, 20, 24, 20, 4, 22, 4,  // 24=GAD_POWER2 16=GAD_LIFE_P
  20, 22, 4, 16, 4, 24, 22, 20,
  20, 22, 20, 4, 24, 20, 22, 20,
  4, 22, 20, 24, 20, 20, 4, 20, // 4=GAD_PROTEC 22=GAD_POWER1 20=GAD_BALLE3
  20, 4, 20, 24, 20, 4, 22, 4,  // 24=GAD_POWER2 16=GAD_LIFE_P
  20, 22, 4, 16, 4, 24, 22, 20,
  20, 22, 20, 4, 24, 20, 22, 20
};

// Area 1 : level 1
const
  Uint32
  handler_levels::zeCourse10[] =
  { 40, 06, 18, 12, 06, 06, 18, 18, 06, 18, 20, 52, 42, 18, 20, 40,
  18, 40, 50, 40, 42, 44, 54, 06, 22, 18, 06, 06, 52, 40, 18, 06,
  18, 06, 18, 12, 06, 42, 18, 18, 06, 44, 20, 18, 18, 18, 20, 18,
  44, 44, 50, 18, 42, 18, 20, 06, 22, 40, 06, 54, 22, 40, 18, 06
};

// Area 1 : levels 2 to 5
const
  Uint32
  handler_levels::zeCourse11[] =
  { 40, 10, 40, 12, 18, 10, 40, 12, 06, 10, 02, 18, 18, 42, 02, 12,
  18, 02, 44, 18, 18, 02, 42, 10, 10, 18, 12, 02, 18, 18, 52, 12,
  40, 10, 18, 12, 40, 10, 40, 12, 06, 46, 02, 18, 18, 42, 02, 12,
  18, 02, 18, 44, 54, 02, 42, 10, 10, 44, 12, 02, 18, 18, 10, 12
};

// Area 1 : levels 7 to 11 
const
  Uint32
  handler_levels::zeCourse12[] =
  { 40, 26, 42, 18, 02, 26, 18, 18, 06, 52, 02, 42, 18, 16, 02, 18,
  40, 02, 18, 14, 10, 02, 20, 10, 10, 18, 10, 02, 26, 18, 10, 10,
  18, 26, 10, 18, 02, 26, 54, 44, 06, 46, 02, 10, 44, 16, 02, 44,
  40, 02, 40, 14, 10, 02, 20, 10, 10, 18, 10, 02, 26, 40, 10, 10
};

// Area 2 
const
  Uint32
  handler_levels::zeCourse20[] =
  { 42, 26, 10, 10, 10, 42, 18, 44, 10, 14, 10, 10, 10, 10, 10, 10,
  42, 10, 26, 10, 10, 16, 10, 44, 10, 18, 10, 42, 10, 10, 18, 10,
  42, 26, 10, 40, 10, 10, 18, 10, 46, 14, 10, 10, 10, 10, 10, 10,
  42, 10, 26, 10, 10, 16, 44, 54, 10, 18, 10, 10, 42, 10, 18, 10
};

// Area 3
const
  Uint32
  handler_levels::zeCourse30[] =
  { 10, 26, 10, 50, 50, 10, 18, 18, 14, 14, 10, 10, 40, 40, 10, 18,
  10, 10, 26, 10, 42, 16, 42, 14, 44, 18, 10, 10, 40, 40, 18, 14,
  10, 26, 10, 44, 46, 10, 54, 18, 14, 14, 40, 40, 10, 10, 10, 18,
  10, 10, 26, 10, 10, 16, 44, 14, 42, 18, 40, 40, 10, 52, 18, 14
};

// Aera 4
const
  Uint32
  handler_levels::zeCourse40[] =
  { 10, 26, 26, 20, 42, 44, 20, 10, 44, 14, 26, 10, 40, 40, 10, 10,
  10, 10, 26, 10, 52, 16, 10, 10, 10, 20, 26, 10, 40, 40, 20, 10,
  10, 26, 26, 20, 52, 50, 20, 54, 44, 14, 26, 10, 40, 40, 46, 10,
  10, 10, 26, 10, 42, 16, 10, 50, 10, 20, 26, 10, 40, 40, 20, 10
};

// Area 5  : levels 1 to 5 
const
  Uint32
  handler_levels::zeCourse50[] =
  { 42, 26, 26, 10, 10, 10, 20, 44, 10, 14, 26, 10, 10, 10, 20, 10,
  42, 10, 26, 10, 50, 14, 20, 44, 46, 18, 26, 10, 10, 10, 20, 14,
  10, 26, 26, 10, 50, 10, 20, 54, 10, 14, 26, 10, 52, 10, 20, 10,
  10, 10, 26, 10, 10, 14, 20, 44, 10, 18, 26, 10, 52, 10, 20, 14
};

// Area 5 : levels 7 to 11
const
  Uint32
  handler_levels::zeCourse55[] =
  { 14, 26, 26, 10, 16, 10, 20, 14, 10, 14, 26, 42, 52, 44, 20, 10,
  14, 10, 26, 44, 18, 14, 20, 10, 10, 18, 26, 42, 52, 10, 20, 14,
  14, 26, 26, 10, 16, 10, 20, 54, 46, 14, 26, 42, 42, 44, 20, 10,
  14, 10, 26, 44, 18, 14, 20, 50, 50, 18, 26, 42, 42, 10, 20, 14
};

// AREA 1 : level 1
const
  bricks_level_desc
  handler_levels::amigaTab10 = { 0, // 0 = bricks level
  0,
  1 * 50,                       //time before appearance of the Atom 1
  45 * 50,                      //time before appearance of the Atom 2
  35 * 50,                      //time before appearance of the Atom 3
  10 * 50,                      //time before appearance of the Atom 4
  80 * 50,                      //time before reappearance of the Atoms
  1,                            //resistance
  8,
  2,
  zeCourse10,
  1,
  50 * 99,
  50 * 20,
  50 * 50,
  7 * 50
};

// AREA 1 : level 2 to 5
const
  bricks_level_desc
  handler_levels::amigaTab11 = { 0,
  1,
  1 * 50,
  45 * 50,
  25 * 50,
  12 * 50,
  80 * 50,
  1,
  15,
  5,
  zeCourse11,
  1,
  50 * 77,
  50 * 15,
  50 * 40,
  7 * 50
};

// AREA 1 : level 6 to 11
const
  bricks_level_desc
  handler_levels::amigaTab12 = { 0,
  2,
  1 * 50,
  35 * 50,
  25 * 50,
  12 * 50,
  70 * 50,
  1,
  17,
  6,
  zeCourse12,
  1,
  50 * 67,
  50 * 10,
  50 * 30,
  7 * 50
};

// AREA 2
const
  bricks_level_desc
  handler_levels::amigaTab20 = { 0,
  3,
  1 * 50,
  40 * 50,
  30 * 50,
  10 * 50,
  50 * 50,
  5,
  19,
  7,
  zeCourse20,
  1,
  50 * 67,
  50 * 10,
  50 * 10,
  10 * 50
};

// AREA 3
const
  bricks_level_desc
  handler_levels::amigaTab30 = { 0,
  4,
  1 * 50,
  30 * 50,
  20 * 50,
  10 * 50,
  40 * 50,
  10,
  20,
  8,
  zeCourse30,
  1,
  50 * 50,
  50 * 10,
  50 * 10,
  10 * 50
};

// AREA 4
const
  bricks_level_desc
  handler_levels::amigaTab40 = { 0,
  5,
  1 * 50,
  21 * 50,
  14 * 50,
  17 * 50,
  30 * 50,
  20,
  15,
  8,
  zeCourse40,
  1,
  50 * 50,
  50 * 10,
  50 * 7,
  11 * 50
};

// AREA 5 : level 1 to 5
const
  bricks_level_desc
  handler_levels::amigaTab50 = { 0,
  6,
  1 * 50,
  15 * 50,
  10 * 50,
  05 * 50,
  20 * 50,
  30,
  15,
  8,
  zeCourse50,
  1,
  50 * 40,
  50 * 10,
  50 * 7,
  11 * 50
};

// AREA 5 : level 7 to 11
const
  bricks_level_desc
  handler_levels::amigaTab55 = { 0,
  7,
  1 * 50,
  4 * 50,
  3 * 50,
  2 * 50,
  5 * 50,
  50,
  15,
  15,
  zeCourse55,
  2,
  50 * 30,
  50 * 10,
  50 * 5,
  15 * 50
};

//#######################################################################
// guards levls
//#######################################################################
// area 1 : level 6
const
  guardians_level_desc
  handler_levels::atariTab00 = { 1, // 1 = guards levels
  0,
  3,                            // speed ball
  50 * 20,                      // time before ball start
  10 * 50,                      // time before enable tilt
  10 * 50,                      // time before scroll start
  1,                            // scroll type
  7,                            // frequency of gadgets
  zeCourseXX
};

// Area  1 : Level 12
const
  guardians_level_desc
  handler_levels::atariTab04 = { 1,
  1,
  3,
  50 * 15,
  10 * 50,
  10 * 50,
  2,
  8,
  zeCourseXX
};

// Area 2 : Level 6
const
  guardians_level_desc
  handler_levels::atariTab08 = { 1,
  2,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  3,
  9,
  zeCourseXX
};

// Area 2 : Level 12
const
  guardians_level_desc
  handler_levels::atariTab12 = { 1,
  3,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  4,
  9,
  zeCourseXX
};

// Area 3 : Level 6
const
  guardians_level_desc
  handler_levels::atariTab16 = { 1,
  4,
  4,
  50 * 10,
  10 * 50,
  5 * 50,
  5,
  10,
  zeCourseXX
};

// Area 3 : Level 12
const
  guardians_level_desc
  handler_levels::atariTab20 = { 1,
  5,
  4,
  50 * 10,
  10 * 50,
  3 * 50,
  6,
  10,
  zeCourseXX
};

// Area 4 : Level 6
const
  guardians_level_desc
  handler_levels::atariTab24 = { 1,
  6,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  7,
  12,
  zeCourseXX
};

// Area 4 : Level 12
const
  guardians_level_desc
  handler_levels::atariTab28 = { 1,
  7,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  8,
  12,
  zeCourseXX
};

// Area 5 : Level 6
const
  guardians_level_desc
  handler_levels::atariTab32 = { 1,
  8,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  9,
  15,
  zeCourseXX
};

// Area 5 : Level 12
const
  guardians_level_desc
  handler_levels::atariTab36 = { 1,
  9,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  10,
  15,
  zeCourseXX
};

// Area 5 : Level 13
const
  guardians_level_desc
  handler_levels::atariTab40 = { 1,
  10,
  4,
  50 * 10,
  10 * 50,
  1 * 50,
  11,
  15,
  zeCourseXX
};


// Une table par chacun des 61 niveaux
const bricks_level_desc *
  handler_levels::giga_amiga[] = {
  &amigaTab10,                  // area 1 ; level 1
  &amigaTab11,                  // area 1 ; level 2
  &amigaTab11,                  // area 1 ; level 3
  &amigaTab11,                  // area 1 ; level 4
  &amigaTab11,                  // area 1 ; level 5
  (bricks_level_desc *) & atariTab00,   // area 1 ; level 6
  &amigaTab12,                  // area 1 ; level 7
  &amigaTab12,                  // area 1 ; level 8
  &amigaTab12,                  // area 1 ; level 9
  &amigaTab12,                  // area 1 ; level 10
  &amigaTab12,                  // area 1 ; level 11
  (bricks_level_desc *) & atariTab04,   // area 1 ; level 12
  &amigaTab20,                  // area 2 ; level 1
  &amigaTab20,                  // area 2 ; level 2
  &amigaTab20,                  // area 2 ; level 3
  &amigaTab20,                  // area 2 ; level 4
  &amigaTab20,                  // area 2 ; level 5
  (bricks_level_desc *) & atariTab08,   // area 2 ; level 6
  &amigaTab20,                  // area 2 ; level 7
  &amigaTab20,                  // area 2 ; level 8
  &amigaTab20,                  // area 2 ; level 9
  &amigaTab20,                  // area 2 ; level 10
  &amigaTab20,                  // area 2 ; level 11
  (bricks_level_desc *) & atariTab12,   // area 2 ; level 12
  &amigaTab30,                  // area 3 ; level 1
  &amigaTab30,                  // area 3 ; level 2
  &amigaTab30,                  // area 3 ; level 3
  &amigaTab30,                  // area 3 ; level 4
  &amigaTab30,                  // area 3 ; level 5
  (bricks_level_desc *) & atariTab16,   // area 3 ; level 6
  &amigaTab30,                  // area 3 ; level 7
  &amigaTab30,                  // area 3 ; level 8
  &amigaTab30,                  // area 3 ; level 9
  &amigaTab30,                  // area 3 ; level 10
  &amigaTab30,                  // area 3 ; level 11
  (bricks_level_desc *) & atariTab20,   // area 3 ; level 12
  &amigaTab40,                  // area 4 ; level 1
  &amigaTab40,                  // area 4 ; level 2
  &amigaTab40,                  // area 4 ; level 3
  &amigaTab40,                  // area 4 ; level 4
  &amigaTab40,                  // area 4 ; level 5
  (bricks_level_desc *) & atariTab24,   // area 4 ; level 6
  &amigaTab40,                  // area 4 ; level 7
  &amigaTab40,                  // area 4 ; level 8
  &amigaTab40,                  // area 4 ; level 9
  &amigaTab40,                  // area 4 ; level 10
  &amigaTab40,                  // area 4 ; level 11
  (bricks_level_desc *) & atariTab28,   // area 4 ; level 12
  &amigaTab50,                  // area 5 ; level 1
  &amigaTab50,                  // area 5 ; level 2
  &amigaTab50,                  // area 5 ; level 3
  &amigaTab50,                  // area 5 ; level 4
  &amigaTab50,                  // area 5 ; level 5
  (bricks_level_desc *) & atariTab32,   // area 5 ; level 6
  &amigaTab55,                  // area 5 ; level 7
  &amigaTab55,                  // area 5 ; level 8
  &amigaTab55,                  // area 5 ; level 9
  &amigaTab55,                  // area 5 ; level 10
  &amigaTab55,                  // area 5 ; level 11
  (bricks_level_desc *) & atariTab36,   // area 5 ; level 12
  (bricks_level_desc *) & atariTab40    // area 5 ; level 13
};

*/
