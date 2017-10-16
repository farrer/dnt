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

#ifndef _dnt_rule_script_h
#define _dnt_rule_script_h

#include "dntconfig.h"
#include "scriptcontroller.h"
#include "scriptinstance.h"


namespace DNT
{

/*! Instance of a RuleScript */
class RuleScriptInstance: public ScriptInstance
{
   public:
      /*! Constructor */
      RuleScriptInstance(asIScriptObject* obj, 
            RuleScript* script, ScriptManager* manager);
      /*! Destructor */
      ~RuleScriptInstance();

      /*! Call its roll function */
      bool callRoll(RuleDefinitionValue* testRule, 
            RuleDefinitionValue* againstRule);

      /*! Call its rollValue function */
      bool callRollValue(RuleDefinitionValue* testRule, int againstValue);

      /*! Call canInteract for a character and a target object */
      bool callCanInteract(ScriptObjectCharacter* actor,
            ScriptObjectObject* target);
};

/*! Script specific for the general Rules of the game. */
class RuleScript : public ScriptController
{
   public:
      /*! Constructor */
      RuleScript(ScriptManager* manager);
      /*! Destructor */
      ~RuleScript();
      
      /*! \return a new instance of the script */
      RuleScriptInstance* createInstance();

      /*! \return pointer to the constructor function */
      asIScriptFunction* getFactoryFunction();
      /*! \return pointer to the function called on every cycle */
      asIScriptFunction* getStepFunction();

      /*! \return pointer to the function called when needed to roll a
       * RuleDefinition against another one. */
      asIScriptFunction* getRollFunction();
      /*! \return pointer to the function called when needed to roll a
       * RuleDefinition against a value. */
      asIScriptFunction* getRollValueFunction();

      /*! \return pointer to the function called to check if a character
       * can interact with an specific object */
      asIScriptFunction* getCanInteractWithObjectFunction();

   protected:
      void setFunctionPointers();
      const Ogre::String getMainTypeInterface() const 
      { 
         return "RuleController";
      };

   private:

      asIScriptFunction* factoryFunction; /**< Pointer to the factory */
      asIScriptFunction* stepFunction;    /**< Pointer to the step() */
      asIScriptFunction* rollFunction;  /**< Pointer to roll */
      asIScriptFunction* rollValueFunction; /**< Pointer to rollValue */
      /*! Pointer to canInteract(Character, Object) */
      asIScriptFunction* canInteractWithObjectFunction; 
};

}

#endif

