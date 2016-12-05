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

#ifndef dnt_thing_h
#define dnt_thing_h

#include "dntconfig.h"

#include "bonusandsaves.h"
#include "skills.h"

#include <goblin/model3d.h>
#include <kobold/list.h>
#include <kobold/kstring.h>
#include <OGRE/OgreSceneManager.h>

#include <map>
#include <iostream>

namespace DNT
{

#define WALK_PER_MOVE_ACTION  60 /**< Distance that can be walked per move */

#define THING_ARMATURE_CLASS    "ARMATURE_CLASS"
#define THING_INITIATIVE_BONUS  "INITIATIVE"
#define THING_DISPLACEMENT      "DISPLACEMENT"
#define THING_MAX_LIFE_POINTS   "MAX_LIFE_POINTS"

#define THING_UNKNOWN_VALUE  -100

/*! Base RPG class for characters (PCs and NPCs) and objects */
class Thing : public Kobold::ListElement
{
   public:

      /*i! Thing's State to Playable Characters */
      enum PsychoState
      {
         PSYCHO_HOSTILE,
         PSYCHO_NEUTRAL,
         PSYCHO_FRIENDLY
      };

      /*! Constructor */
      Thing();
      /*! Destructor */
      virtual ~Thing();

      /*! Load the thing specification
       * \param fileName file to load
       * \param fullPath if fileName is defined with fullPath (true)
       *                 or inner resources directory (false). */
      bool load(Kobold::String fileName, bool fullPath=false);

      /*! Save the thing specification.
       * \param filename name of the file to save it
       * \param fullPath if true, will use filename as a full path file
       *        definition, if false, will save filename to DNT user's
       *        home as root. */
      bool save(Kobold::String filename, bool fullPath=false);

      /*! \return 3d representation of the thing, if any, or NULL. */
      Goblin::Model3d* getModel();

      /*! \return filename of the file used to load the thing */
      Kobold::String getFilename();

      /*! \return #name */
      Kobold::String getName();

      /*! \return #description */
      Kobold::String getDescription();

      /*! \return #state */
      int getState();

      /*! Set thing's internal state.
       * \param state state value */
      void setState(int state);

      /*! \return if Characters could walk over or not */
      bool isWalkable();
      
      /*! Set if Characters could walk through the thing or not.
       * \param canWalkThrough true to able characters to walk over it. */
      void setWalkable(bool canWalkThrough);

      /*! \return Pointer to current target enemy, if any */
      Thing* getCurrentEnemy();
      /*! Set current target enemy
       * \param target pointher to current target Thing */
      void setCurrentEnemy(Thing* target);

      /*! \return Thing's PsychoState */
      PsychoState getPsychoState();
      /*! Define Thing's state to PlayableCharacters
       * \param state new PsychoState */
      void setPsychoState(PsychoState state);

      /*! Get the bonus Value (modifier or not).
       *  \param something -> number of the definition.
       *  \return bonus (or THING_UNKNOWN_VALUE if not found). */
      int getBonusValue(Factor& something);

      /*! Get a factor value
       * \param something -> factor info
       * \return its value (or THING_UNKNWOWN_VALUE if not found) */
      int getFactorValue(Factor& something);
     
      /*! Increment a factor value by 'inc'
       * \param something factor to increment
       * \param inc value of the increment */
      void incFactorValue(Factor& something, int inc);

      /*! \return current bonus and saves */
      BonusAndSaves* getCurBonusAndSaves();

      /*! \return conversation to use. */
      Conversation* getConversation();

      /*! \return if the thing has a conversation dialog file */
      bool hasConversationFile();

      /*! Set conversation file.
       * \param fileName name of the conversation file */
      void setConversationFile(Kobold::String fileName);

      /*! \return thing's skills */
      Skills* getSkills();

      /*! Do a check (skill, saves, etc) against the difficulty.
       * \param difficulty -> Difficulty of the check
       * \return true if succed, false if failed. */
      bool doCheck(Kobold::String stateToCheck, int difficulty);

      /*! Function that implements the normal interaction (not on batle) 
       * between a PlayableCharacter and the Thing. */
      //virtual bool interact() = 0;
      
      /*! Get the current number of life points
       * \return -> current life points */
      int getLifePoints();
      /*! Add some points to the current life
       * \param points -> points to add */
      void addLifePoints(int points);
      /*! Set the current life points
       * \param points -> new life value */
      void setLifePoints(int points);

      /*! Get the current max life points
       * \return -> max life points */
      int getMaxLifePoints();
      /*! Set the current max life points
       * \param points -> new max life points*/
      void setMaxLifePoints(int points);
      
   protected:

      /*! Parse specifc key/value pair readed from definition's file that
       * doesn't belong to the generic thing specification.
       * \return if treated key/value pair or not. */
      virtual bool doSpecificParse(Kobold::String key, 
            Kobold::String value) = 0;
      /*! Add any specific specialization information to be 
       * saved at a thing's file
       * \param file ofstream with the file to save
       * \return if was successful */
      virtual bool doSpecificSave(std::ofstream& file) = 0;

      Kobold::String filename; /**< Filename used to load the thing */
      Kobold::String name; /**< Name of the thing */
      Kobold::String untranslatedName; /**< Untranslated name of the thing.
                                          Used for hash key. */
      Kobold::String description; /**< Text describing the thing */

      int lifePoints;    /**< Current life points. If <= 0 could be dead
                              or broken (or something near that state). */
      int maxLifePoints; /**< Max number of life points the thing can have */

      int state; /**< Current state. What this represents, depends on 
                      respective implementations. Defaults to 0. */

      bool walkable; /**< If the thing could be walkable through or not. */

      Kobold::String modelFileName; /**< File name of the 3d model */
      Goblin::Model3d* model3d; /**< The 3d model representing the thing */

      Kobold::String conversationFile; /**< Conversation file, if any. */
      Conversation* conversation; /**< Conversation loaded. */

      Thing* currentEnemy; /**< Pointer to current target enemy, if any */
      PsychoState psychoState; /**< State to Playable Characters */

      Skills sk;             /**< skills without images and descriptions */

      int armatureClass; /**< Armature class protection value */
      BonusAndSaves curBonusAndSaves; /**< Thing's bonus and saves */
      int displacement;       /**< Thing's Displacement (in meters) */
      int initiativeBonus;    /**< Thing's initiative bonus value */


      /*! Map to avoid name's clash with multiple intances of the same 
       * thing model */
      static std::map<Kobold::String, int> namesMap;
};

}

#endif

