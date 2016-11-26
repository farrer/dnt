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

#include "skills.h"
#include "dices.h"

#include <kobold/defparser.h>
#include <kobold/log.h>

#include "../lang/translate.h"

#include <OGRE/OgreMath.h>

using namespace DNT;

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                           SkillDefinition                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                          Constructor                                  *
 *************************************************************************/
SkillDefinition::SkillDefinition()
                :RuleDefinition()
{
   isAttr = false;
   baseAttribute = 0;
}

/*************************************************************************
 *                           Destructor                                  *
 *************************************************************************/
SkillDefinition::~SkillDefinition()
{
}

/*************************************************************************
 *                          setBaseAttributeId                           *
 *************************************************************************/
void SkillDefinition::setBaseAttributeId(int id)
{
   baseAttribute = id;
}

/*************************************************************************
 *                          getBaseAttributeId                           *
 *************************************************************************/
int SkillDefinition::getBaseAttributeId()
{
   return baseAttribute;
}

/*************************************************************************
 *                             setAsAttribute                            *
 *************************************************************************/
void SkillDefinition::setAsAttribute()
{
   isAttr = true;
}

/*************************************************************************
 *                               isAttribute                             *
 *************************************************************************/
bool SkillDefinition::isAttribute()
{
   return isAttr;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                Skill                                  //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
Skill::Skill()
{
   points = 0;
   prevPoints = 0;
   mod = 2;
   definition = NULL;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
Skill::~Skill()
{
}

/*************************************************************************
 *                             setDefinition                             *
 *************************************************************************/
void Skill::setDefinition(SkillDefinition* def)
{
   definition = def;
}

/*************************************************************************
 *                             getDefinition                             *
 *************************************************************************/
SkillDefinition* Skill::getDefinition()
{
   return definition;
}

/*************************************************************************
 *                          storeCurrentPoints                           *
 ************************************************************************/
void Skill::storeCurrentPoints()
{
   prevPoints = points;
}

/*************************************************************************
 *                         restoreLastStoredPoints                       *
 *************************************************************************/
void Skill::restoreLastStoredPoints()
{
   points = prevPoints;
}

/*************************************************************************
 *                              getPoints                                *
 *************************************************************************/
int Skill::getPoints()
{
   return points;
}

/*************************************************************************
 *                              setPoints                                *
 *************************************************************************/
void Skill::setPoints(int points)
{
   this->points = points;
}

/*************************************************************************
 *                             getModifier                               *
 *************************************************************************/
int Skill::getModifier()
{
   return mod;
}

/*************************************************************************
 *                             setModifier                               *
 *************************************************************************/
void Skill::setModifier(int mod)
{
   this->mod = mod;
}

/*************************************************************************
 *                          getAttributeBonus                            *
 *************************************************************************/
int Skill::getAttributeBonus()
{
   if(definition->isAttribute())
   {
      return Ogre::Math::Floor((points - 10) / 2.0f);
   }

   return 0;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                         SkillsDefinitions                             //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************
 *                          Init                             *
 *************************************************************/
void SkillsDefinitions::init()
{
   Kobold::Log::add("Initing Skills");

   Kobold::DefParser defList;
   Kobold::DefParser def;
   
   Kobold::String key="", value="";
   Kobold::String skKey="", skValue="";

   /* Load the definitions file */ 
   if(!defList.load("skills/skills.skl"))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Couldn't parse skills.skl file!");
      return;
   }

   /* Define total Skills */
   defList.getNextTuple(key, value);
   if(key != "totalSkills")
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error: Invalid skills.skl file (missing 'totalSkills')!");
      return;
   }
   sscanf(value.c_str(), "%d", &totalSkills);

   /* Create the skills definitions vector */
   if(totalSkills > 0)
   {
      skillsDefs = new SkillDefinition[totalSkills];
   }
   else
   {
      skillsDefs = NULL;
   }

   /* Now define and load each skill definition file */ 
   int cur = 0;
   while(defList.getNextTuple(key, value))
   {
      /* Load the definitions file */ 
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
            "   Loading '%s'", value.c_str());
      if(def.load("skills/" + value + ".ski"))
      {
         /* Define the Skill */
         skillsDefs[cur].setId(cur, key);

         while(def.getNextTuple(skKey, skValue))
         {
            if(skKey == "name")
            {
               skillsDefs[cur].setName(translateDataString(skValue));
            }
            else if(skKey == "description")
            {
               skillsDefs[cur].setDescription(translateDataString(skValue));
            }
            else if(skKey == "baseAttribute")
            {
               int val = 0;
               sscanf(skValue.c_str(), "%d", &val);
               skillsDefs[cur].setBaseAttributeId(val);
            }
            else if(skKey == "isAttribute")
            {
               if(skValue == "true")
               {
                  skillsDefs[cur].setAsAttribute();
               }
            }
            else
            {
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
                     "Error: Unknown key '%s' at skill file: '%s'", 
                     skKey.c_str(), value.c_str());
            }
         }
         
         /* Load its image and go next. */
         skillsDefs[cur].loadImage("skills/images/" + value + ".png");

         /* Done with current parser. */
         def.clear();

         cur++;
      } 
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "Error: Couldn't parse skill file: '%s'", value.c_str());
      }
   }
}

/*************************************************************
 *                         Finish                            *
 *************************************************************/
void SkillsDefinitions::finish()
{
   Kobold::Log::add("Finishing Skills");
   if(skillsDefs)
   {
      delete [] skillsDefs;
   }
}

/*************************************************************
 *                      getTotalSkills                       *
 *************************************************************/
int SkillsDefinitions::getTotalSkills()
{
   return totalSkills;
}

/*************************************************************
 *                  getSkillDefinition                       *
 *************************************************************/
SkillDefinition* SkillsDefinitions::getSkillDefinition(Kobold::String id)
{
   int i;
   for(i = 0; i < totalSkills; i++)
   {
      if(skillsDefs[i].getStringId() == id)
      {
         return &skillsDefs[i];
      }
   }

   return NULL;
}

/*************************************************************
 *                  getSkillDefinition                       *
 *************************************************************/
SkillDefinition* SkillsDefinitions::getSkillDefinition(int i)
{
   if( (i >= 0) && (i < totalSkills) )
   {
      return &skillsDefs[i];
   }

   return NULL;
}

/*************************************************************
 *                      static members                       *
 *************************************************************/
SkillDefinition* SkillsDefinitions::skillsDefs = NULL;
int SkillsDefinitions::totalSkills = 0;


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                Skills                                 //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************
 *                      Constructor                          *
 *************************************************************/
Skills::Skills()
{
   int i;
   SkillsDefinitions skDef;

   /* Set Values */
   availablePoints = 0;
   totalSkills = skDef.getTotalSkills();

   /* Define Skills */
   if(totalSkills > 0)
   {
      /* Alloc Vector */
      skillsVector = new Skill[totalSkills];

      /* Define initial values and definitions */
      for(i = 0; i < totalSkills; i++)
      {
         skillsVector[i].setDefinition(skDef.getSkillDefinition(i));
      }
   }
   else
   {
      skillsVector = NULL;
      totalSkills = 0;
   }
}

/*************************************************************
 *                       Destructor                          *
 *************************************************************/
Skills::~Skills()
{
   if(skillsVector != NULL)
   {
      delete[] skillsVector;
   }
}


/*************************************************************
 *                    getSkillByString                       *
 *************************************************************/
Skill* Skills::getSkillByString(Kobold::String idString)
{
   int i;
   for(i=0; i<totalSkills; i++)
   {
      if(skillsVector[i].getDefinition() != NULL)
      {
         if(idString == skillsVector[i].getDefinition()->getStringId())
         {
            return &skillsVector[i];
         }
      }
   }

   return NULL;
}

/*************************************************************
 *                    getSkillByString                       *
 *************************************************************/
Skill* Skills::getSkillByInt(int i)
{
   if( (i >= 0) && (i < totalSkills) )
   {
      return &skillsVector[i];
   }

   return NULL;
}

/*************************************************************
 *                       doSkillCheck                        *
 *************************************************************/
bool Skills::doSkillCheck(Kobold::String idString, int difficulty)
{
   return doSkillCheck(getSkillByString(idString), difficulty);
}

/*************************************************************
 *                       doSkillCheck                        *
 *************************************************************/
bool Skills::doSkillCheck(Skill* sk, int difficulty)
{
   int res = 0;
   Dice d20(Dice::DICE_TYPE_D20);
   Skill* att;
   int mod = 0;
   int value = 0;

   if(sk != NULL)
   {
      /* Define the attribute modifier */
      att = getSkillByInt(sk->getDefinition()->getBaseAttributeId());
      if(att)
      {
         mod = (int)Ogre::Math::Floor((att->getPoints() - 10) / 2.0);
      }

      /* Finally, roll the dices */
      if(sk->getDefinition()->isAttribute())
      {
         /* It's an attribute, the roll is with the modifier then */
         value = d20.roll() + mod;
      }
      else
      {
         /* It's a Skill, do the normal roll */
         value = d20.roll() + sk->getPoints() + mod;
      }
   }

   res = (value >= difficulty);

   /* Brief the result */
   //TODO: brief.addCheckText(sk->getDefinition()->getName(), value, 
   //difficulty);

   /* Done */
   return res;
}

/*************************************************************
 *                   setAvailablePoints                        *
 *************************************************************/
void Skills::setAvailablePoints(int points)
{
   availablePoints = points;
}

/*************************************************************
 *                   getAvailablePoints                        *
 *************************************************************/
int Skills::getAvailablePoints()
{
   return availablePoints;
}

/*************************************************************
 *                   getTotalSkills                          *
 *************************************************************/
int Skills::getTotalSkills()
{
   return totalSkills;
}

/*************************************************************
 *                      clearSkills                          *
 *************************************************************/
void Skills::clear()
{
   for(int i = 0; i < totalSkills; i++)
   {
      /* Clear only Skills */
      if(!skillsVector[i].getDefinition()->isAttribute())
      {
         skillsVector[i].setPoints(0);
         skillsVector[i].setModifier(2);
      }
   }
}

/*************************************************************
 *                      clearCosts                           *
 *************************************************************/
void Skills::clearCosts()
{
   for(int i = 0; i < totalSkills; i++)
   {
      /* Clear only Skills */
      if(!skillsVector[i].getDefinition()->isAttribute())
      {
         skillsVector[i].setModifier(2);
      }
   }
}

