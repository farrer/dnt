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

#include "classes.h"

#include "../lang/translate.h"
#include <kobold/log.h>
#include <kobold/defparser.h>

using namespace DNT;

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
Class::Class()
{
   totalModifiers = 0;
   classModifiers = NULL;
   totalFeats = 0;
   classFeats = NULL;
   totalSkills = 0;
   classSkills = NULL;
   reqLevel = NULL;
   previous = NULL;
   next = NULL;
}

/******************************************************************
 *                             Destructor                         *
 ******************************************************************/
Class::~Class()
{
   if(classModifiers)
   {
      delete [] classModifiers;
      classModifiers = NULL;
   }
   if(classFeats)
   {
      delete[] classFeats;
      classFeats = NULL;
   }
   if(reqLevel)
   {
      delete[] reqLevel;
      reqLevel = NULL;
   }
   if(classSkills)
   {
      delete[] classSkills;
      classSkills = NULL;
   }
}

/******************************************************************
 *                               load                             *
 ******************************************************************/
bool Class::load(Kobold::String filename, Kobold::String imgFile, 
      Kobold::String idString, int idInt)
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL,
         "   Loading '%s'", filename.c_str());

   Kobold::DefParser def;
   Kobold::String key, value;

   /* Set identifier and image */
   setId(idInt, idString);
   loadImage(imgFile);

   /* Load definition's file */
   if(!def.load(filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: couldn't load class '%s'", filename.c_str());
      return false;
   }

   /* Now parse all tokens got */
   char buf1[128];
   int curBonus=-1, curMod=-1, curTalent=-1, curSkill=-1;

   while(def.getNextTuple(key, value))
   {
      /* Name */
      if(key == "name")
      {
         setName(translateDataString(value));
      }
      /* Epigraph */
      else if(key == "epigraph")
      {
         citation = translateDataString(value);
      }
      /* Description */
      else if(key == "description")
      {
         setDescription(translateDataString(value));
      }
      /* LifeDice */
      else if(key == "lifeDice")
      {
         sscanf(value.c_str(), "d%d", &lifeDiceID);
      }
      /* Total Modifiers */
      else if(key == "totalModifiers")
      {
         sscanf(value.c_str(), "%d", &totalModifiers);
         if(totalModifiers > 0)
         {
            classModifiers = new Modifier[totalModifiers];
         }
      }
      /* Modifier */
      else if(key == "modifier")
      {
         curMod++;
         if(!classModifiers[curMod].parse(value))
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't parse modifier '%s'", value.c_str());
         }
      }
      /* Modifier Text */
      else if(key == "modifierText")
      {
         classModifiers[curMod].setDescription(translateDataString(value));
      }
      /* TotalFeats */
      else if(key == "totalTalents")
      {
         sscanf(value.c_str(), "%d", &totalFeats);
         if(totalFeats > 0)
         {
            classFeats = new Kobold::String[totalFeats];
            reqLevel = new int[totalFeats];
         }
      }
      /* Feat */
      else if(key == "talent")
      {
         curTalent++;
         sscanf(value.c_str(), "%d %s", &reqLevel[curTalent], &buf1[0]);
         classFeats[curTalent] = buf1;
      }
      /* Total Skills */
      else if(key == "totalSkills")
      {
         sscanf(value.c_str(),"%d", &totalSkills);
         if(totalSkills > 0)
         {
            classSkills = new Kobold::String[totalSkills];
         }
      }
      /* Skill  */
      else if(key == "skill")
      {
         curSkill++;
         classSkills[curSkill] = value;
      }
      /* First Skill Points */
      else if(key == "firstSkillPoints")
      {
         if(!firstLevelSP.parse(value))
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't parse points '%s'", value.c_str());
         }
      }
      /* Next Skill Points */
      else if(key == "nextSkillPoints")
      {
         if(!otherLevelsSP.parse(value))
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't parse points '%s'", value.c_str());
         }
      }
      /* Bonus Attack and Mods Per Level */
      else if(key == "bonus")
      {
         curBonus++;
         if(!bonus[curBonus].parse(value))
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't parse bonus '%s'", value.c_str());
         }
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Unknow token: '%s'", key.c_str());
      }
   }

   return true;
}


/******************************************************************
 *                          applySkillCosts                       *
 ******************************************************************/
void Class::applySkillCosts(Skills* sk)
{
   int i;
   Skill* s = NULL;
   for(i=0; i<totalSkills; i++)
   {
      s = sk->getSkillByString(classSkills[i]);
      if(s != NULL)
      {
         s->setModifier(1);
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow class skill: '%s'",
               classSkills[i].c_str());
      }
   }
}

/******************************************************************
 *                          getAttModifiers                       *
 ******************************************************************/
void Class::getAttModifiers(int mods[6], bool sum, Skills* sk)
{
   int i;
   /* If isn't to sum, zero's to the mods */
   if(!sum)
   {
      for(i=0; i<6; i++)
      {
         mods[i] = 0;
      }
   }
   
   /* Now Apply all attributes modifiers */
   for(i=0; i < totalModifiers; i++)
   {
      if(classModifiers[i].getCause()->getType() == MOD_TYPE_ATT)
      {
         Skill* s = sk->getSkillByString(classModifiers[i].getCause()->getId());
         int n = (s != NULL)?s->getDefinition()->getId():-1;
         if((n >= 0) && (n <6))
         {
            mods[n] += classModifiers[i].getValue();
         }
         else
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow Attribute: '%s'", 
               classModifiers[i].getCause()->getId().c_str());
         }
      }
   }

}

/******************************************************************
 *                                init                            *
 ******************************************************************/
void Classes::init()
{
   Kobold::Log::add("Initing Classes");

   Kobold::DefParser def;
   int idInt = 0;
   char classFile[256], imgFile[256];
   Kobold::String key="", value="";

   list =  new Kobold::List();

   /* Open the list */
   if(!def.load("classes/classes.lst"))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error opening class list file!");
      return;
   }

   /* Get and Insert All Races */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(),"%d %s %s",&idInt,&classFile[0],&imgFile[0]);
      insertClass(classFile, imgFile, key, idInt);
   }
}

/******************************************************************
 *                              finish                            *
 ******************************************************************/
void Classes::finish()
{
   Kobold::Log::add("Finishing Classes");
   if(list)
   {
      delete list;
   }
}

/******************************************************************
 *                            insertClass                         *
 ******************************************************************/
void Classes::insertClass(Kobold::String fileName, Kobold::String imgFile, 
                          Kobold::String idString, int idInt)
{
   Class* c = new Class();
   if(c->load("classes/" + fileName, imgFile, idString, idInt))
   {
      list->insert(c);
   }
}

/******************************************************************
 *                         getClassByInteger                      *
 ******************************************************************/
Class* Classes::getClassByInteger(int id)
{
   Class* ret = (Class*)list->getFirst();
   int i;
   for(i = 0; i < list->getTotal(); i++)
   {
      if(ret->getId() == id)
      {
         return ret;
      }
      ret = (Class*) ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                          getClassByString                      *
 ******************************************************************/
Class* Classes::getClassByString(Kobold::String id)
{
   Class* ret = (Class*)list->getFirst();
   int i;
   for(i = 0; i < list->getTotal(); i++)
   {
      if(ret->getStringId() == id)
      {
         return ret;
      }
      ret = (Class*)ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                          getTotalClasses                       *
 ******************************************************************/
int Classes::getTotalClasses()
{
   return list->getTotal();
}

/******************************************************************
 *                          static members                        *
 ******************************************************************/
Kobold::List* Classes::list = NULL;

