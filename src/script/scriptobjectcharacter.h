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

#ifndef _dnt_script_object_character_h
#define _dnt_script_object_character_h

#include "dntconfig.h"
#include "scriptobject.h"
#include <angelscript.h>
#include <kobold/mutex.h>
#include <OGRE/Math/Simple/OgreAabb.h>

namespace DNT
{

   /*! A ScriptObject implementation for characters */
   class ScriptObjectCharacter : public ScriptObject
   {
      public:

         /*! Register the related class */
         static void registerClass(asIScriptEngine* asEngine);
         /*! Register the related functions */
         static void registerFunctions(asIScriptEngine* asEngine);

         /*! Constructor */
         ScriptObjectCharacter(Kobold::String filename, 
               Ogre::Vector3 originalPos);
         /*! Constructor */
         ScriptObjectCharacter(Kobold::String filename);
         /*! Constructor */
         ScriptObjectCharacter(Character* c);
         /*! Destructor */
         ~ScriptObjectCharacter();
        
         /*! Set the character pointer.
          * \param newPtr pointer to the character this object 
          *               represents, or NULL. */
         void setPointer(void* newPtr);
         
         /*! \return if the related pointer is valid */
         const bool isValid();

         /*! Set the character to move by the found path of a A* search. */
         void setToMoveByFoundPath(AStar* aStar);

         /*! \return if is moving by path (true)
          *          or if isn't or no more (false) */
         const bool isMovingByPath();
         
         /*! Get the current position of the character */
         const Ogre::Vector3 getPosition();
         /*! Set position of the character on next frame */
         void setPosition(const Ogre::Vector3& pos);

         /*! Get the current orientation of the character */
         const float getOrientation();
         /*! Set the orientation of the character on next frames */
         void setOrientation(const float ori);

         /*! \return current displacement */
         const int getDisplacement();

         /*! \return walkable bounding box of the model */
         Ogre::Aabb getWalkableBounds();

         /*! Move to a position */
         void moveToPosition(float posX, float posZ);
         /*! Mote near to a character */
         void moveToCharacter(ScriptObjectCharacter* target);
         /*! Move away from a character */
         void moveFromCharacter(ScriptObjectCharacter* target);
         /*! Call its animation (once) */
         void callAnimation(int index);
         /*! Set its looping animation */
         void setAnimation(int index);
         /*! Open its dialog */
         void openDialog();
         /*! \return rule definition defined by groupId and ruleDefId */
         ScriptObjectRuleDefinition* getRuleDefinition(Kobold::String groupId,
               Kobold::String ruleDefId);

     private:
         Character* character; /**< referenced character */
         Kobold::Mutex mutex; /**< For pointer access changes */

   };

}

#endif

