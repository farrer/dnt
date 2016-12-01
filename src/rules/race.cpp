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

#include "race.h"

#include "modifier.h"
#include "feats.h"
#include "skills.h"

#include "../lang/translate.h"

#include <kobold/defparser.h>
#include <kobold/log.h>
using namespace DNT;

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                               race                                 //
//                                                                    //
////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
Race::Race()
{
   totalModifiers = 0;
   raceModifiers = NULL;
   totalFeats = 0;
   raceFeats = NULL;
   totalSkills = 0;
   raceSkills = NULL;
   raceSkillsJustify = NULL;
   previous = NULL;
   next = NULL;
}

/******************************************************************
 *                             Destructor                         *
 ******************************************************************/
Race::~Race()
{
   if(raceModifiers)
   {
      delete [] raceModifiers;
      raceModifiers = NULL;
   }
   if(raceFeats)
   {
      delete[] raceFeats;
      raceFeats = NULL;
   }
   if(raceSkills)
   {
      delete[] raceSkills;
      raceSkills = NULL;
   }
   if(raceSkillsJustify)
   {
      delete[] raceSkillsJustify;
      raceSkillsJustify = NULL;
   }
}

/******************************************************************
 *                                load                            *
 ******************************************************************/
bool Race::load(Kobold::String filename)
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
         "   Loading '%s'", filename.c_str());

   Kobold::DefParser def;
   Kobold::String key="", value="";

   int curMod=-1, curTalent=-1, curSkill=-1;

   /* Read the file */
   if(!def.load("races/" + filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error while opening race file 'races/%s'", filename.c_str());
      return false;
   }

   /* Now parse all tokens got */
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
      /* Total Modifiers */
      else if(key == "totalModifiers")
      {
         sscanf(value.c_str(), "%d", &totalModifiers);
         if(totalModifiers > 0)
         {
            raceModifiers = new Modifier[totalModifiers];
         }
      }
      /* Modifier */
      else if(key == "modifier")
      {
         curMod++;
         if(!raceModifiers[curMod].parse(value))
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
                  "Warning: error while parsing race modifier '%s'", 
                  value.c_str());
         }
      }
      /* Modifier Text */
      else if(key == "modifierText")
      {
         raceModifiers[curMod].setDescription(translateDataString(value));
      }
      /* TotalFeats */
      else if(key == "totalTalents")
      {
         sscanf(value.c_str(),"%d", &totalFeats);
         if(totalFeats > 0)
         {
            raceFeats = new Kobold::String[totalFeats];
         }
      }
      /* Feat */
      else if(key == "talent")
      {
         curTalent++;
         raceFeats[curTalent] = value;
      }
      /* Total Skills */
      else if(key == "totalSkills")
      {
         sscanf(value.c_str(), "%d", &totalSkills);
         if(totalSkills > 0)
         {
            raceSkills = new Kobold::String[totalSkills];
            raceSkillsJustify = new Kobold::String[totalSkills];
         }
      }
      /* Skill  */
      else if(key == "skill")
      {
         curSkill++;
         raceSkills[curSkill] = value;
      }
      /* Skill Text */
      else if(key == "skillText")
      {
         raceSkillsJustify[curSkill] = value;
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "Unknow Token '%s'", key.c_str());
      }
   }


   return true;
}

/******************************************************************
 *                          applySkillCosts                       *
 ******************************************************************/
void Race::applySkillCosts(Skills* sk)
{
   int i;
   Skill* s = NULL;
   for(i = 0; i < totalSkills; i++)
   {
      s = sk->getSkillByString(raceSkills[i]);
      if(s != NULL)
      {
         s->setModifier(1);
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "Warning: Unknow race Skill: '%s'", 
               raceSkills[i].c_str());
      }
   }
}

/******************************************************************
 *                          getAttModifiers                       *
 ******************************************************************/
void Race::getAttModifiers(int mods[6], bool sum, Skills* sk)
{
   /* If isn't to sum, zero's to the mods */
   if(!sum)
   {
      for(int i = 0; i < 6; i++)
      {
         mods[i] = 0;
      }
   }
   
   /* Now Apply all attributes modifiers */
   for(int i=0; i < totalModifiers; i++)
   {
      if(raceModifiers[i].getCause()->getType() == MOD_TYPE_ATT)
      {
         Skill* s = sk->getSkillByString(raceModifiers[i].getCause()->getId());
         if((s != NULL) && (s->getDefinition()->isAttribute()))
         {
            mods[s->getDefinition()->getId()] += raceModifiers[i].getValue();
         }
         else
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
                  "Warning: unknow attribute: '%s'",
                  raceModifiers[i].getCause()->getId().c_str());
         }
      }
   }

}

////////////////////////////////////////////////////////////////////////
//                                                                    //
//                              races                                 //
//                                                                    //
////////////////////////////////////////////////////////////////////////

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
void Races::init()
{
   Kobold::Log::add("Initing Races");

   Kobold::DefParser def;
   Kobold::String key, value;
   int idInt=0;
   char raceFile[256], imgFile[256];

   list = new Kobold::List();

   /* Open the list */
   if(!def.load("races/races.lst"))
   {
      return;
   }

   /* Get and Insert All Races */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(),"%d %s %s",&idInt,&raceFile[0],&imgFile[0]);
      insertRace(raceFile, imgFile, key, idInt);
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
void Races::finish()
{
   Kobold::Log::add("Finishing Races");

   if(list != NULL)
   {
      delete list;
   }
}

/******************************************************************
 *                            insertRace                         *
 ******************************************************************/
void Races::insertRace(Kobold::String fileName, Kobold::String imgFile,
      Kobold::String idString, int idInt)
{
   Race* race = new Race;

   race->loadImage(imgFile);
   race->setId(idInt, idString);
   race->load(fileName);

   list->insert(race);
}

/******************************************************************
 *                          getRaceByInteger                      *
 ******************************************************************/
Race* Races::getRaceByInteger(int id)
{
   Race* ret = (Race*)list->getFirst();
   int i;
   for(i = 0; i < list->getTotal(); i++)
   {
      if(ret->getId() == id)
      {
         return ret;
      }
      ret = (Race*) ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                          getRaceByString                       *
 ******************************************************************/
Race* Races::getRaceByString(Kobold::String id)
{
   Race* ret = (Race*)list->getFirst();
   int i;
   for(i = 0; i < list->getTotal(); i++)
   {
      if(ret->getStringId() == id)
      {
         return ret;
      }
      ret = (Race*) ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                          getTotalRaces                         *
 ******************************************************************/
int Races::getTotalRaces()
{
   return list->getTotal();
}

/******************************************************************
 *                          Static Members                        *
 ******************************************************************/
Kobold::List* Races::list = NULL;

