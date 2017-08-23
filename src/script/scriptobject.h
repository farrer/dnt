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

#ifndef _dnt_script_object_h
#define _dnt_script_object_h

#include <kobold/list.h>
#include <kobold/kstring.h>
#include <OGRE/OgreVector3.h>

namespace DNT
{

   /*! An object representation for scripts. The idea is that each object
    * keep a pointer to the game object it references. When the game object
    * is no more valid, it should invalidate its ScriptObject equivalent 
    * (if any exists), that could be still be referenced under scripts (in this
    * case, any operation to it will just be ignored). When no longer with
    * any references, the ScriptObject should be simple deleted. */
   class ScriptObject: public Kobold::ListElement
   {
      public:
         /*! Type of ScriptObjects */
         enum ScriptObjectType
         {
            TYPE_CHARACTER,
            TYPE_DICE,
            TYPE_OBJECT,
            TYPE_RULE_DEFINITION,
            TYPE_RULE_GROUP
         };

         /*! Constructor
          * \param type of the script object.
          * \param filename file name of the referenced object.
          * \param originalPos original position of it on map, could be ignored
          *        if it makes no sense for the implemented object */
         ScriptObject(ScriptObjectType type, Kobold::String filename, 
               Ogre::Vector3 originalPos);
         /*! Constructor, without position */
         ScriptObject(ScriptObjectType type, Kobold::String filename);
         /*! Constructor, only with type.
          * \note only for full-on-script objects (ie: those without any 
          *       real-world references).
          * \note full-on-script objects won't go to the objects list of
          *       script manager and are properly deleted when its reference
          *       goes down to 0. */
         ScriptObject(ScriptObjectType type);
         /*! Destructor */
         virtual ~ScriptObject();

         /*! Tells the object that we are keeping a reference to it */
         int addReference();
         /*! Tells the object that we are no longer referencing it */
         int release();
         /*! \return current total references to the object */
         const int getTotalReferences() const { return references; };

         /*! \return type of the script object */
         const ScriptObjectType getType() const { return type; };

         /*! Check if the script object is equivalent to the parameters */
         const bool isEquivalent(const Kobold::String& filename);
         /*! \copydoc isEquivalent */
         const bool isEquivalent(const Kobold::String& filename, 
               const Ogre::Vector3& pos);

         /*! Set the referenced pointer to a new one.
          * \param newPtr pointer to reference. Could be NULL. */
         virtual void setPointer(void* newPtr) = 0;

         /*!\ return if current pointer is valid or not */
         virtual const bool isValid() = 0;

      private:
         ScriptObjectType type;      /**< Its type */
         Kobold::String filename;    /**< Object's filename */
         Ogre::Vector3 originalPos;  /**< Object's original position */
         bool usePos;                /**< If use original position */
         int references;             /**< Total references to it on scripts */
         bool fullOnScript;          /**< When not a real-world referenced */
   };

}

#endif


