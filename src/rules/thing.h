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

#include "ruledef.h"

#include <goblin/model3d.h>
#include <kobold/list.h>
#include <kobold/kstring.h>
#include <OGRE/OgreSceneManager.h>

#include <map>
#include <iostream>

namespace DNT
{

/*! Base RPG class for characters (PCs and NPCs) and objects */
class Thing : public Kobold::ListElement
{
   public:
      /*! CollisionElement container for Thing */
      class ColElement : public Kobold::ListElement
      {
         public:
            Element* element; /**< The element itself */
      };

      /*! Thing's State to Playable Characters */
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
 
      /*! Do the per-frame thing's update */
      virtual bool update();

      /*! \return Model's position on world. */
      const Ogre::Vector3 getPosition() const { return model->getPosition(); };
      /*! \return Model's orientation (yaw). */
      const float getOrientation() const { return model->getOrientation(); };
      const float getRoll() const { return model->getRoll(); };
      const float getYaw() const { return model->getYaw(); };
      const float getPitch() const { return model->getPitch(); };
      /*! \return model's scale */
      const Ogre::Vector3 getScale() { return model->getScale(); };
      /*! Hide our model */
      void hide() { model->hide(); };
      /*! Show our model */
      void show() { model->show(); };

      /*! \return the scene node use by Model */
      Ogre::SceneNode* getSceneNode() { return model->getSceneNode(); };
      /*! \return Ogre::Item WorldAabb */
      Ogre::Aabb getWorldAabb() { return model->getItem()->getWorldAabb(); };
      /*! \return Ogre::Item updated WorldAabb */
      Ogre::Aabb getWorldAabbUpdated() 
      { return model->getItem()->getWorldAabbUpdated(); };
      /*! \return if is a static model */
      bool isStatic() { return model->isStatic(); };
      /*! Notify that its static model changed its position.
       * \note Only used at editors */
      void notifyStaticDirty() { model->notifyStaticDirty(); };

      /*! Get cached mesh from Goblin::Model */
      void getCachedMesh(size_t &vertexCount, Ogre::Vector3* &vertices,
            size_t &indexCount, Ogre::uint32* &indices)
      { model->getCachedMesh(vertexCount, vertices, indexCount, indices); };


      /*! Set model's position (at next frame update) */
      void setPosition(const Ogre::Vector3& pos);
      /*! Set model's position (for current frame) */
      void setPositionNow(const Ogre::Vector3& pos);
      /*! Set model's target orientation */
      void setTargetOrientation(float ori, int steps);
      /*! Set model's target orienation */
      void setTargetOrientation(const Ogre::Vector3& ori, int steps);
      /*! Clear current model orientation (all components) */
      void clearOrientation() { model->clearOrientation(); };
      /*! Set model's orientation (for next frame) */
      void setOrientation(float ori);
      /*! Set model's orientation (for current frame) */
      void setOrientationNow(const Ogre::Vector3& ori);
      /*! Set scale of the model (for current frame) */
      void setScaleNow(const Ogre::Vector3& scale);

      /*! \return filename of the file used to load the thing */
      Kobold::String getFilename();

      /*! \return #name */
      Kobold::String getName();

      /*! \return #description */
      Kobold::String getDescription();

      /*! Set difficulty */
      void setDifficulty(int value);
      /*! \return #difficulty */
      const int getDifficulty() const { return difficulty; };

      /*! Set hardness */
      void setHardness(int value);
      /*! \return #hardness */
      const int getHardness() const { return hardness; };

      /*! \return #state */
      const int getState() const { return state; };
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

      /*! \return conversation to use. */
      Conversation* getConversation();

      /*! \return if the thing has a conversation dialog file */
      bool hasConversationFile();

      /*! Set conversation file.
       * \param fileName name of the conversation file */
      void setConversationFile(Kobold::String fileName);

      /*! \return the conversation filename */
      const Kobold::String& getConversationFile() const 
      { return conversationFile; };

      /*! Do a RuleDefinition against a fixed value difficulty. */
      bool doCheck(RuleDefinition* ruleDef, int difficulty);
      
      /*! Do a RuleDefinition check against a RuleDefinition */ 
      bool doCheck(RuleDefinition* ruleDef, RuleDefinitionValue* against);

      /*! \return thing's type */
      ThingType getThingType();

      /*! \return our 'playable character walking bounding box':
       * This is a regular squared one, in which rotations will never happen,
       * avoiding the character being 'stucked' when can't move, rotate a bit
       * (maybe increasing regular bound size), an try to move. With a regular
       * immutable one, this won't happen. */
      Ogre::Aabb getWalkableBounds() const { return walkableBounds; };

      /*! \return if interaction between the object and characters 
       * is possible. */
      virtual const bool canInteract() const = 0;

      /*! Possible result of interact function */
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

      /*! \return list of thing's collision elements */
      Kobold::List* getColElements() { return colElements; };

      /*! \return Thing's portrait filename, if any */
      const Kobold::String getPortraitFilename() const { return portraitFile;};

      /*! \return current displacement */
      const float getDisplacement() const { return displacement; };

      /*! \return if should do a depth collision check with Thing's model 
       * triangles or just bounding box check is sufficient */
      virtual const bool shouldDepthCollisionCheck() const { return true; }

      /*! Undefine the defined script object */
      void undefineScriptObject() { scriptObject = NULL; };

      /*! Define the script object related to this thing */
      void defineScriptObject(ScriptObject* sobj) { scriptObject = sobj; };

      /*! \return the script object associated with the Thing. */
      ScriptObject* getScriptObject() { return scriptObject; };

      /*! \return initial position on map */
      const Ogre::Vector3& getInitialPosition() const { return initialPos; };
      /*! Set initial position on map */
      void setInitialPosition(Ogre::Vector3 pos);

      /*! \return distance can move each frame when walking */
      float getWalkInterval(); 
      /*! \return distance can rotate each frame */
      float getTurnAroundInterval();

      /*! \return RuleDefinitionValue for given identifier */
      RuleDefinitionValue* getRuleDefinition(const Kobold::String id);
      /*! \return RuleDefinition Value for given group and identifier */
      RuleDefinitionValue* getRuleDefinition(const Kobold::String groupId, 
            const Kobold::String id);
      /*! \return RuleDefinitionValue or NULL if don't have it */
      RuleDefinitionValue* getRuleDefinition(RuleDefinition* def);
      /*! \return RuleGroupAvailbleInfo for given group identifier */
      RuleGroupAvailableInfo* getRuleGroup(const Kobold::String id);

   protected:

      /*! Set current animation of the animated model */
      void setAnimatedModelAnimation(int animation, bool loop); 
      /*! \return current animation of the animated model */
      int getAnimatedModelAnimation();

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
       * \return animation names vector */ 
      virtual Kobold::String* getAnimationList();

      Kobold::List ruleGroups; /**< List of RuleGroupAvailableInfo */

      Kobold::String portraitFile; /**< Thing's portrait filename, if any */
      ScriptObject* scriptObject; /**< Script Object related to the thing */

   private:
      /*! Populate the thing ruleGroups with all groups and all
       * not-selectable RuleDefs */
      void populateWithRules();

      ThingType thingType; /**< Thing's type */
      int totalAnimations; /**< Expected total animations */

      Kobold::String filename; /**< Filename used to load the thing */
      Kobold::String name; /**< Name of the thing */
      Kobold::String untranslatedName; /**< Untranslated name of the thing.
                                          Used for hash key. */
      Kobold::String description; /**< Text describing the thing */

      int state; /**< Current state. What this represents, depends on 
                      respective implementations. Defaults to 0. */

      int difficulty; /**< Thing's difficulty. Depends on the implementation */
      int hardness; /**< Thing's hardness. Depends on the implementation */

      bool walkable; /**< If the thing could be walkable through or not. */

      Kobold::String modelFileName; /**< File name of the 3d model */
      Ogre::Vector3 initialPos; /**< Initial position on map */
      Goblin::Model3d* model; /**< The 3d model representing the thing */

      Kobold::String conversationFile; /**< Conversation file, if any. */
      Conversation* conversation; /**< Conversation loaded. */

      Thing* currentEnemy; /**< Pointer to current target enemy, if any */
      PsychoState psychoState; /**< State to Playable Characters */

      float displacement;       /**< Thing's Displacement (in meters) */
      float walkInterval; /**< Distance can move each frame */
      float turnAroundInterval; /**< Distance can rotate each frame */

      Ogre::Aabb walkableBounds; /**< Bounds when is a playable thing to use
                                      when walking */

      /*! Map to avoid name's clash with multiple intances of the same 
       * thing model */
      static std::map<Kobold::String, int> namesMap;

      Kobold::List* colElements; /**< List of ColElements */
};

}

#endif

