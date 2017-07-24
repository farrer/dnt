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

#ifndef _dnt_script_controller_h
#define _dnt_script_controller_h

#include <angelscript.h>
#include <OGRE/OgreString.h>
#include <kobold/list.h>
#include "dntconfig.h"

namespace DNT
{

/*! Base class for implementing specific script controller. Each 
 * child should keep its list of accessible functions and their 
 * particular implementations. */
class ScriptController : public Kobold::ListElement
{
   public:
      enum ScriptType
      {
          SCRIPT_TYPE_MAP
      };
      ScriptController(ScriptType type, ScriptManager* manager);
      virtual ~ScriptController();

      /*! Load and compile the script 
       * \return true if successfull */
      bool loadAndCompile(Ogre::String filename);

      /*!\ return pointer to the factory (constructor) function of the
       * controller (which will receive the specific object handler) */
      virtual asIScriptFunction* getFactoryFunction() = 0;
      
      /*! \return pointer to the function to be called on every cycle */
      virtual asIScriptFunction* getStepFunction() = 0;

      /*! \return type of the implemented script */
      const ScriptType getType() const { return this->type; };

      const Ogre::String getFilename() const { return this->filename; };

      /*! Increment the number of instances the controller has */
      void incInstanceCount();
      /*! Decrement the number of instances the controller has */
      void decInstanceCount();

      /*! \return number of active instances of the controller */
      const int getNumberOfInstances() const { return instCount; };

   protected:
      /*! Set all needed function pointers, for latter quick access. */
      virtual void setFunctionPointers() = 0;

      /*! \return String with the name of the interface to be implemented
       * by the main class of the script. */
      virtual const Ogre::String getMainTypeInterface() const = 0;

      /*! Get the first class which implements an scpecific interface 
       * on the script.
       * \param interfaceName name of the interface to be implemented
       * \return pointer to the type found or NULL, if not found */
      asITypeInfo* getTypeByInterface(Ogre::String interfaceName);

      asIScriptModule* module; /**< The loaded module */
      asITypeInfo* mainType; /**< The principal class of the script */
      asIScriptEngine* asEngine; /**< The AngelScript engine used */
      ScriptManager* manager; /**< The Script manager used */

   private:
      ScriptType type; /**< Script type */
      Ogre::String filename; /**< Loaded script filename */
      int instCount; /**< Number of instances the controller has */
};

}

#endif

