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

#ifndef _dnt_script_instance_h
#define _dnt_script_instance_h

#include <angelscript.h>
#include <OGRE/OgreString.h>
#include <kobold/list.h>
#include "scriptcontroller.h"

namespace DNT
{

/*! DNT implements each of its scripts as an AngelScript class. Thus,
 * each one should have an instance, which information is kept by this class */
class ScriptInstance : public Kobold::ListElement
{
   public:
      ScriptInstance(asIScriptObject* obj, ScriptController* script,
            ScriptManager* manager); 
      ~ScriptInstance();

      asIScriptObject* getObject() { return obj; };
      ScriptController* getScript() { return script; };

   protected:
      asIScriptObject* obj; /**< AngelScript object as instance of script */
      ScriptController* script; /**< The script the object is an instance of */
      ScriptManager* manager; /**< Script manager used */

};

}

#endif
