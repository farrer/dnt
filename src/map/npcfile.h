/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_npc_file_h
#define _dnt_npc_file_h

#include <kobold/kstring.h>
#include <kobold/list.h>

#include "../rules/character.h"

namespace DNT
{

/*! The NPC file is used to load/save lists of npcs, with its positions,
 * to later really load its models or just change the state of them all. */
class NpcFile: public Kobold::List
{
   public:

      /*! A temporaly parse struct for npc file */
      class NpcParseStruct: public Kobold::ListElement
      {
         public:
            Kobold::String name;     /**< The NPC name */
            Kobold::String filename; /**< The file of NPC */
            Ogre::Vector3 position;  /**< The NPC Initial Position */
            float angle;     /**< The NPC Initial Orientation Angle */
            Thing::PsychoState psycho; /**< The NPC Initial Psycho state */
      };

      /*! Constructor */
      NpcFile();

      /*! Destructor */
      ~NpcFile();

      /*! Load the npc file
       * \param filename -> relative fileName of the npc's file
       * \return -> true if loaded, false otherwise */
      bool load(Kobold::String filename);

      /*! Save the npc file
       * \param filename -> full filename (with path) of where to save.
       * \return -> true if saved, false otherwise */
      bool save(Kobold::String filename);

      /*! Get the next character readed from the file
       * \param name -> name of the npc
       * \param fileName -> fileName of the character got
       * \param pos -> initial position of the character got 
       * \param angle -> Orientation angle of the character got
       * \param psycho -> Psycho state of the character got
       * \return -> true if can get the next character, false otherwise */
      bool getNextCharacter(Kobold::String& name, Kobold::String& fileName,
            Ogre::Vector3& pos, float& angle, Thing::PsychoState& psycho);

      /*! Insert a character at the list
       * \param name -> name of the npc
       * \param fileName -> fileName of the character got
       * \param pos -> initial position of the character got 
       * \param angle -> Orientation angle of the character got
       * \param psycho -> Character psycho state */
      void insertCharacter(Kobold::String name, Kobold::String fileName, 
            Ogre::Vector3 pos, float angle, Thing::PsychoState psycho);

      /*! Kill All NPCs defined by the NPC file */
      void killAll();

   private:

      Kobold::String mapFilename; /**< Name of the map file that own 
                                       this npc list */
      Kobold::String filename;  /**< Name of the NPC list file opened */
      NpcParseStruct* current;  /**< Current NPC */

};

}

#endif

