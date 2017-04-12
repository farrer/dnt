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

      /*! The incredible Thing type */
      enum ThingType
      {
         /*! Character derived type */
         THING_TYPE_CHARACTER,
         /*! Object derived type */
         THING_TYPE_OBJECT,
         THING_TYPE_SCENERY,
         THING_TYPE_DOOR,
         THING_TYPE_ITEM
      };

      /*! Constructor */
      Thing(ThingType type, int totalAnimations);
      /*! Destructor */
      virtual ~Thing();

      /*! Load the thing specification
       * \param fileName file to load
       * \param modelType if will use static or dynamic model
       * \param fullPath if fileName is defined with fullPath (true)
       *                 or inner resources directory (false). */
      bool load(Kobold::String fileName, 
            Goblin::Model3d::Model3dType modelType, bool fullPath=false);

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

      /*! \return thing's type */
      ThingType getThingType();

      /*! \return current armature class value */
      int getArmatureClass();
      /*! Set current armature class
       * \param value armature class value */
      void setArmatureClass(int value);

      /* \return our 'playable character walking bounding box':
       * This is a regular squared one, in which rotations will never happen,
       * avoiding the character being 'stucked' when can't move, rotate a bit
       * (maybe increasing regular bound size), an try to move. With a regular
       * immutable one, this won't happen. */
      Ogre::Aabb getWalkableBounds() const { return walkableBounds; };

      /*! \return if interaction between the object and characters 
       * is possible. */
      virtual const bool canInteract() const = 0;

      enum InteractResult
      {
         /* The interaction is done. */
         INTERACTION_DONE,
         /* Should open a conversation with object's conversation file
          * to allow the interaction */
         INTERACTION_OPEN_CONVERSATION
      };

      /*! Interact with the object.
       * \note this function is usually called when player's is in range
       *       and clicked into the object (and the object is, by itself,
       *       interactive - catInteract() with true result).
       * \param actor pointer to the character that is interacting with
       * \return interaction result */
      virtual InteractResult interact(Character* actor) = 0;
      
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

      /*! Get the animation names list for the AnimatedModel, if the thing's 
       * implementation support it.
       * \note those supporting it, should override this function.
       * \param total total names on vector.
       * \return animation names vector */ 
      virtual Kobold::String* getAnimationList();

      /*! \return distance can move each frame when walking */
      float getWalkInterval(); 
      /*! \return distance can rotate each frame */
      float getTurnAroundInterval(); 

   private:
      ThingType thingType; /**< Thing's type */
      int totalAnimations; /**< Expected total animations */

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
      Goblin::Model3d* model; /**< The 3d model representing the thing */

      Kobold::String conversationFile; /**< Conversation file, if any. */
      Conversation* conversation; /**< Conversation loaded. */

      Thing* currentEnemy; /**< Pointer to current target enemy, if any */
      PsychoState psychoState; /**< State to Playable Characters */

      Skills sk;             /**< skills without images and descriptions */

      int armatureClass; /**< Armature class protection value */
      BonusAndSaves curBonusAndSaves; /**< Thing's bonus and saves */
      int displacement;       /**< Thing's Displacement (in meters) */
      int initiativeBonus;    /**< Thing's initiative bonus value */

      float walkInterval; /**< Distance can move each frame */
      float turnAroundInterval; /**< Distance can rotate each frame */

      Ogre::Aabb walkableBounds; /**< Bounds when is a playable thing to use
                                      when walking */

      /*! Map to avoid name's clash with multiple intances of the same 
       * thing model */
      static std::map<Kobold::String, int> namesMap;
};

}

#endif

