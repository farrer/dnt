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

#ifndef _dnt_character_h
#define _dnt_character_h

#include "dntconfig.h"

#include <kobold/kstring.h>
#include <kobold/mutex.h>

#include "thing.h"
#include "modeffect.h"
#include "dices.h"

#include <iostream>

namespace DNT
{

   /*! How many classes a character can have at the same time (multiclass) */
   #define CHARACTER_MAX_DISTINCT_CLASSES  3

   /*! A Character is a living Thing. Coulb be playable by human 
    * (PlayableCharacter, or PC) or by the AI (NonPlayableCharacter, 
    * aka NPC). */
   class Character : public Thing
   {
      public:
         /*! Each character animation */
         enum CharacterAnimation
         {
            CHARACTER_ANIMATION_ATTACK = 0,
            CHARACTER_ANIMATION_DEAD,
            CHARACTER_ANIMATION_DIE,
            CHARACTER_ANIMATION_IDLE,
            CHARACTER_ANIMATION_RUN,
            CHARACTER_ANIMATION_WALK,
            CHARACTER_TOTAL_ANIMATIONS,
            CHARACTER_ANIMATION_NONE
         };

         /*! Constructor 
          * \param playable if it's a playable character or not
          * \note will insert the Character at its respective Game::pcs or
          *       Game::npcs list. */
         Character(bool playable);
         /*! Destructor */
         virtual ~Character();

         /*! Add a modEffect to the Character
          * \param mod -> modifier value
          * \param time -> time to expire (0 == forever)
          * \param periodicTime -> time to apply the modEffect again 
          *                        (0 to not re-apply)
          * \param def RuleDefinition target of the effect */
         void addModEffect(int mod, int time, int periodicTime,
               RuleDefinition* def);

         /*! Remove all modEffects from the Character */
         void removeAllModEffects();

         /*! Get the modEffectList pointer
          * \return pointer to the modEffectList */
         ModEffectList* getEffects();

         /*! Kill the Character without calling dead animation
          * \note this is usually used at ModState when loading a level
          *       with dead characters information. */
         void instantKill();

         /*! Load a character model */
         bool load(const Kobold::String& filename);
         
         /*! Called after loaded the character, to set some of its 
          * definitions */ 
         virtual void doAfterLoad();

         /*! \return pointer to character's inventory */
         Inventory* getInventory();

         /*! Set current charcter animation 
          * \param animation to set
          * \param loop if will loop or just run once. */
         void setAnimation(CharacterAnimation animation, bool loop = true);
         /*! Get current character animation */
         CharacterAnimation getCurrentAnimation();

         /*! \return true. Characters are interactive. */
         const bool canInteract() const { return true; };

         /*! \return if character can attack at current fight turn */
         const bool getCanAttack() const { return canAttack; };
         /*! \return if character can move at current fight turn */
         const bool getCanMove() const { return canMove; };

         /*! \return if Character is alive */
         const bool isAlive() const { return !dead; };
         
         /*! For simplicity, characters are only verified by bounding box  */
         virtual const bool shouldDepthCollisionCheck() const { return false; }

         /*! Set if character can attack at current fight turn */
         void setCanAttack(bool value);
         /*! Set if character can move at current fight turn */
         void setCanMove(bool value);

         /*! Set the character to move by the found path of a A* search.
          * \note usually this function is called when the A* was defined by
          *       a script call. 
          * \param aStar A* with found path. */
         void setToMoveByFoundPath(AStar* aStar);

         /*! \return if is moving by path (true) 
          *          or if isn't or no more (false) */
         const bool isMovingByPath() const { return aStar != NULL; };
         
         /*! Update character */
         virtual bool update();

         /*! Set initial character position */
         virtual void setInitialPosition(Ogre::Vector3 pos);

      protected:
         /*! \return default character's animation names */
         virtual Kobold::String* getAnimationList();

         /*! Parse key/value pairs specific to the character thing's 
          * specialization */
         bool doSpecificParse(const Kobold::String& key, 
               const Kobold::String& value);

         /*! Output to file character specific definitions */
         bool doSpecificSave(std::ofstream& file);

         /*! Must add any specific specialization information to be 
          * saved at a character file
          * \return if was successful */
         virtual bool doCharacterSpecializationSave(std::ofstream& file) = 0;

         /*! Parse specifc key/value pair readed from definition's file that
          * doesn't belong to the generic character specification (but to its
          * specialization). */
         virtual bool doCharacterSpecializationParse(const Kobold::String& key, 
               const Kobold::String& value) = 0;
      
         /*! This function should be called after a successfull character move,
          * to update its collision representation */
         void flushAfterMove();

         Kobold::Mutex mutex; /**< Mutex for access control */

      private:
         AStar* aStar; /**< A* with a path found to follow */
         bool dead; /**< If the character is actually dead (just a corpse). */

         bool canAttack; /**< Flag controller on fight mode */
         bool canMove; /**< Move flag controller on fight mode */

         ModEffectList effects;  /**< Current Character effects */

         Inventory* inventory; /**< Character inventory */

         /*! Names of each animation valid for characters */
         static Kobold::String animationList[CHARACTER_TOTAL_ANIMATIONS];
   };


   /*! A list of Characters. */
   class CharacterList: public Kobold::List
   {
      public:
         /*! List Constructor */
         CharacterList();
         /*! List Destructor */
         ~CharacterList();

         /*! Insert a Character on the list
          * \param dude -> pointer to Character to add */
         void insertCharacter(Character* dude);

         /*! Remove a Character from the list
          * \param dude -> Character pointer to remove */
         void removeCharacter(Character* dude);

         /*! Get next hostile Character from the list
          * \param last -> last hostile taken (NULL to get first)
          * \return pointer to the hostile Character */
         Character* getNextEnemyCharacter(Character* last);

         /*! Get the active Character.
          * \return pointer to the active Character. */
         Character* getActiveCharacter();

         /*! Get the first Character with filename on the list
          * \param filename -> filename of the Character to get
          * \return -> character pointer or NULL (if not found) */
         Character* getCharacter(const Kobold::String& filename);

         /*! Get the Character related to the SceneNode
          * \return Character pointer or NULL */
         Character* getCharacter(Ogre::SceneNode* scNode);

         /*! Get the next Character with the same model from the list
          * \param ch -> Character to get next with same model
          * \return -> next Character with same model or NULL if
          *            end of the list was reached. */
         Character* getNextSameCharacter(Character* ch);

         /*! Set the active Character
          * \param dude -> pointer to the new active Character */
         void setActiveCharacter(Character* dude);

         /*! Treat Character's general scripts */
         void treatGeneralScripts();

         /*! Verify if a Character is on the list
          * \param ch -> pointer to the Character
          * \return -> true if the Character is on the list */
         bool isCharacterIn(Character* ch);

         /*! Update effects and positions of all Characters influences */
         void update();

      private:

         Character* activeCharacter;  /**< Active Character's on list */
         Character* curTreat;         /**< Current NPC To treat Scripts */

   };

}

#endif

