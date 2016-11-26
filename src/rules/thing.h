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

#include <goblin/model3d.h>
#include <kobold/list.h>
#include <OGRE/OgreSceneManager.h>
#include <map>


namespace DNT
{

#define THING_ARMATURE_CLASS    "ARMATURE_CLASS"

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
       * \param sceneManager pointer to scenemanager in use.
       * \param fileName file to load
       * \param fullPath if fileName is defined with fullPath (true)
       *                 or inner resources directory (false). */
      bool load(Ogre::SceneManager* sceneManager, Ogre::String fileName, 
            bool fullPath=false);

      /*! \return 3d representation of the thing, if any, or NULL. */
      Goblin::Model3d* getModel();

      /*! \return #name */
      Ogre::String getName();

      /*! \return #description */
      Ogre::String getDescription();

      /*! \return #state */
      int getState();

      /*! \return #lifePoints */
      int getLifePoints();

      /*! \return #maxLifePoints */
      int getMaxLifePoints();

      /*! Add value to life points */
      void addLifePoints(int value);

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

      /*! \return filename of the conversation owned */
      Ogre::String getConversationFile();

      /*! \return if the thing has a conversation dialog file */
      bool hasConversationFile();

      /*! Set conversation file.
       * \param fileName name of the conversation file */
      void setConversationFile(Ogre::String fileName);

      /*! Function that implements the normal interaction (not on batle) 
       * between a PlayableCharacter and the Thing. */
      //virtual bool interact() = 0;
      
   protected:

      /*! Parse specifc key/value pair readed from definition's file that
       * doesn't belong to the generic thing specification. */
      virtual bool doSpecificParse(Ogre::String key, Ogre::String value) = 0;

      Ogre::String name; /**< Name of the thing */
      Ogre::String untranslatedName; /**< Untranslated name of the thing.
                                          Used for hash key. */
      Ogre::String description; /**< Text describing the thing */

      int lifePoints;    /**< Current life points. If <= 0 could be dead
                              or broken (or something near that state). */
      int maxLifePoints; /**< Max number of life points the thing can have */

      int state; /**< Current state. What this represents, depends on 
                      respective implementations. Defaults to 0. */

      bool walkable; /**< If the thing could be walkable through or not. */

      Ogre::String modelFileName; /**< File name of the 3d model */
      Goblin::Model3d* model3d; /**< The 3d model representing the thing */

      Ogre::String conversationFile; /**< Conversation file, if any. */

      Thing* currentEnemy; /**< Pointer to current target enemy, if any */
      PsychoState psychoState; /**< State to Playable Characters */

      /*! Map to avoid name's clash with multiple intances of the same 
       * thing model */
      static std::map<Ogre::String, int> namesMap;
};

}

#endif

