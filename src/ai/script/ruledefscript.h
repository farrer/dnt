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

#ifndef _dnt_ruledef_script_h
#define _dnt_ruledef_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"
#include <kobold/kstring.h>

namespace DNT
{

/*! Instance of a RuleDefinitionScript */
class RuleDefinitionScriptInstance: public ScriptInstance
{
   public:
      /*! Constructor */
      RuleDefinitionScriptInstance(asIScriptObject* obj, 
            RuleDefinitionScript* script, ScriptManager* manager);
      /*! Destructor */
      ~RuleDefinitionScriptInstance();

      /*! Call its use function */
      void callUse(ScriptObjectCharacter* owner, 
            ScriptObjectCharacter* target);

      /*! Call its onInsert function */
      void callOnInsert(ScriptObjectCharacter* owner);

};

/*! Script for implementation of each RuleDefinition behaviour. */
class RuleDefinitionScript : public ScriptController
{
   public:
      /*! Constructor */
      RuleDefinitionScript(ScriptManager* manager); 

      /*! Destructor */
      ~RuleDefinitionScript();
      
      /*! \return a new instance of the script */
      RuleDefinitionScriptInstance* createInstance();

      /*! \return pointer to the constructor function */
      asIScriptFunction* getFactoryFunction();
      /*! \return pointer to the function called on every cycle */
      asIScriptFunction* getStepFunction();
      /*! \return pointer to the function called when used the RuleDefinition
       * on a target character */
      asIScriptFunction* getUseFunction();
      /*! \return pointer to the function called when a character inserted
       * a RuleDefinition on its owned RuleDefinitons */
      asIScriptFunction* getOnInsertFunction();

   protected:
      void setFunctionPointers();
      const Ogre::String getMainTypeInterface() const 
      { 
         return "RuleDefinitionController";
      };

   private:

      asIScriptFunction* factoryFunction; /**< Pointer to the factory */
      asIScriptFunction* stepFunction;    /**< Pointer to the step() */
      asIScriptFunction* useFunction;  /**< Pointer to use */
      asIScriptFunction* onInsertFunction; /**< Pointer to onInsert */
};

}

#endif

