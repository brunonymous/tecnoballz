/**
 * @file handler_levels.cc
 * @brief Levels handler
 * @created 2004-04-06
 * @date 2012-10-13
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
  capsules_list = NULL;
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
      capsules_list = new capsules_struct[capsules_lists_counter];
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
          get_bricks_level (levels_list[i].id);
        }
      else if (GUARDIANS_LEVEL == levels_list[i].type)
        {
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
              capsules_list[capsule_list_index].id = value;
            }
          if (last_element == "id")
            {
              /* collect all the codes caspules */
              capsule_index++;
              capsules_list[capsule_list_index].codes[capsule_index] = value;
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
                get_capsules_list (value);
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
      capsules_struct *capsule = &capsules_list[i];
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
  if (NULL != capsules_list)
    {
      delete[]capsules_list;
      capsules_list = NULL;
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
}
