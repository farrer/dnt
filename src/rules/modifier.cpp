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

#include "modifier.h"

#include "skills.h"

#include <kobold/log.h>
#include <OGRE/OgreString.h>
#include <stdio.h>

using namespace DNT;

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                 Factor                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                          Constructor                                  *
 *************************************************************************/
Factor::Factor(Kobold::String type, Kobold::String id)
{
   this->type = type;
   this->id = id;
}

/*************************************************************************
 *                           Destructor                                  *
 *************************************************************************/
Factor::~Factor()
{
}

/*************************************************************************
 *                               getType                                 *
 *************************************************************************/
Kobold::String Factor::getType()
{
   return type;
}

/*************************************************************************
 *                                 getId                                 *
 *************************************************************************/
Kobold::String Factor::getId()
{
   return id;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                Modifier                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                            Constructor                                *
 *************************************************************************/
Modifier::Modifier()
{
   value = 0;
   cause = NULL;
   target = NULL;
}

/*************************************************************************
 *                             Destructor                                *
 *************************************************************************/
Modifier::~Modifier()
{
   if(cause)
   {
      delete cause;
   }
   if(target)
   {
      delete target;
   }
}

/*************************************************************************
 *                                 set                                   *
 *************************************************************************/
void Modifier::set(int value, Kobold::String causeType, Kobold::String causeId,
      Kobold::String targetType, Kobold::String targetId)
{
   this->value = value;
   if(this->cause)
   {
      delete this->cause;
   }
   this->cause = new Factor(causeType, causeId);
   if(this->target)
   {
      delete this->target;
   }
   this->target = new Factor(targetType, targetId);
}

/*************************************************************************
 *                               parse                                   *
 *************************************************************************/
bool Modifier::parse(Kobold::String str)
{
   /* Break the string */
   Ogre::vector<Ogre::String>::type res = Ogre::StringUtil::split(str, " ");
   if(res.size() != 5)
   {
      return false;
   }
   
   /* Parse value and set */
   sscanf(res[0].c_str(), "%d", &value);
   set(value, res[1], res[2], res[3], res[4]);

   return true;
}

/*************************************************************************
 *                              getValue                                 *
 *************************************************************************/
int Modifier::getValue()
{
   return value;
}

/*************************************************************************
 *                              getCause                                 *
 *************************************************************************/
Factor* Modifier::getCause()
{
   return cause;
}

/*************************************************************************
 *                             getTarget                                 *
 *************************************************************************/
Factor* Modifier::getTarget()
{
   return target;
}

/*************************************************************************
 *                          getDescription                               *
 *************************************************************************/
Kobold::String Modifier::getDescription()
{
   return description;
}

/*************************************************************************
 *                          setDescription                               *
 *************************************************************************/
void Modifier::setDescription(Kobold::String desc)
{
   description = desc;
}



///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               LevelPoints                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                            Constructor                                *
 *************************************************************************/
LevelPoints::LevelPoints()
{
   this->value = 0;
   this->op = OPERATION_SUM;
   this->mult = 0;
   this->attId = "";
}

/*************************************************************************
 *                            Destructor                                 *
 *************************************************************************/
LevelPoints::~LevelPoints()
{
}

/*************************************************************************
 *                               parse                                   *
 *************************************************************************/
bool LevelPoints::parse(Kobold::String str)
{
   /* Break the string */
   Ogre::vector<Ogre::String>::type res = Ogre::StringUtil::split(str, " ()x");
   if(res.size() != 4)
   {
      return false;
   }
   
   /* Parse values and set */
   sscanf(res[0].c_str(), "%d", &value);
   sscanf(res[3].c_str(), "%d", &mult);
   set(value, res[1], res[2], mult);

   return true;
}

/*************************************************************************
 *                          calculatePoints                              *
 *************************************************************************/
int LevelPoints::calculate(Skills* skills)
{
   Skill* att = skills->getSkillByString(attId);
   int total = 0;
   
   if(att == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Unknown skill: '%s' at LevelPoints::calculate!",
            attId.c_str());
   }
   else
   {
      /* Calculate, based on operator */
      if(op == OPERATION_DEC)
      {
         total = (value - att->getAttributeBonus()) * mult;
      }
      else
      {
         total = (value + att->getAttributeBonus()) * mult;
      }

      /* Make sure isn't negative */
      if(total < 0)
      {
         total = 0;
      }
   }

   return total;
}

/*************************************************************************
 *                                set                                    *
 *************************************************************************/
void LevelPoints::set(int value, Kobold::String op, Kobold::String attId, 
      int mult)
{
   this->value = value;
   if(op == "-")
   {
      this->op = OPERATION_DEC;
   }
   else
   {
      this->op = OPERATION_SUM;
   }
   this->attId = attId;
   this->mult = mult;
}



