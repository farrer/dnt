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

#include "feats.h" 

#include <kobold/log.h>
#include <kobold/defparser.h>

#include "actions.h"
#include "ruledef.h"
#include "thing.h"
#include "modifier.h"

#include "../lang/translate.h"
#include "../core/util.h"
#include "../gui/briefing.h"

using namespace DNT;

Kobold::String dntFeatTypeName[] =
{
   "FEAT_TYPE_PERMANENT",
   "FEAT_TYPE_ON_TARGET",
   "FEAT_TYPE_ON_AREA"
};

/**************************************************************************
 *                             Dependent Feat                             *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
FeatDescription::DepFeat::DepFeat(float reason, Kobold::String featId)
{
   this->reason = reason;
   this->featId = featId;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
FeatDescription::DepFeat::~DepFeat()
{
}

/***************************************************************
 *                          getReason                          *
 ***************************************************************/
float FeatDescription::DepFeat::getReason()
{
   return reason;
}

/***************************************************************
 *                          getFeatId                          *
 ***************************************************************/
Kobold::String FeatDescription::DepFeat::getFeatId()
{
   return featId;
}


/**************************************************************************
 *                             Required Factor                            *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
FeatDescription::ReqFactor::ReqFactor(Kobold::String type, Kobold::String id, 
                                      int level)
          :Factor(type, id)
{
   this->requiredLevel = level;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
FeatDescription::ReqFactor::~ReqFactor()
{
}

/***************************************************************
 *                           getLevel                          *
 ***************************************************************/
int FeatDescription::ReqFactor::getLevel()
{
   return requiredLevel;
}

/**************************************************************************
 *                            FEAT_DESCRIPTION                            *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
FeatDescription::FeatDescription()
{
   quantityPerDay = 0;
   aditionalQuantity = 0;
   aditionalLevel = 0;
   costToUse = 0;
   powerLevel = 0;
   range = 0;
   radius = 0;
   selectable = true;
   scriptFile = "";
   aditionalFactor = NULL;

   action = Action::ACT_NONE;
   type = FEAT_TYPE_PERMANENT;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
FeatDescription::~FeatDescription()
{
   if(aditionalFactor)
   {
      delete aditionalFactor;
   }
}

/***************************************************************
 *                           load                              *
 ***************************************************************/
bool FeatDescription::load(Kobold::String filename, int index)
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
         "   Loading '%s'", filename.c_str());

   Kobold::DefParser desc;
   Kobold::String key, value;
   char buf[128];
   char buf2[128];

   /* Open the Feat's description file */
   if(!desc.load(filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Error: Couldn't open talent file '%s'", filename.c_str());
      return false;
   }

   /* Now parse all Feat tuples */
   while(desc.getNextTuple(key, value))
   {
      /* Text Keys */
      if(key == "title")
      {
         /* Feat Title */
         setName(translateDataString(value));
      }
      else if(key == "description")
      {
         /* Feat Description Text */
         setDescription(translateDataString(value));
      }

      /* Identifiers */
      else if(key == "id")
      {
         setId(index, value);
      }
      /* Type */
      else if(key == "type")
      {
         for(int i = 0; i < FEAT_TYPE_TOTAL; i++)
         {
            if(value == dntFeatTypeName[i])
            {
               type = (FeatType) i;
               break;
            }
         }
      }
      /* Selectable */
      else if(key == "selectable")
      {
         selectable = (value == "true");
      }
      /* Action */
      else if(key == "action")
      {
         action = Action::getId(value);
      }
      /* Icon */
      else if(key == "icon")
      {
         loadImage(value);
      }

      /* Power Level */
      else if(key == "powerLevel")
      {
         sscanf(value.c_str(), "%d", &powerLevel);
      }

      /* Pre-Requistes */
      else if(key == "required")
      {
         int reqLevel = 0;
         sscanf(value.c_str(), "%s %s %d", 
               &buf[0], &buf2[0], &reqLevel);
         reqFactors.insertAtEnd(new ReqFactor(buf, buf2, reqLevel));
      }

      /* Range */
      else if(key == "range")
      {
         sscanf(value.c_str(),"%d", &range);
         range *= METER_TO_DNT;
      }

      /* Radius */
      else if(key == "radius")
      {
         sscanf(value.c_str(), "%d", &radius);
         radius *= METER_TO_DNT;
      }

      /* Cost To Use */
      else if(key == "costToUse")
      {
         sscanf(value.c_str(), "%d", &costToUse);
      }

      /* Quantity Per Day */
      else if( (key == "quantityPerDay") || (key == "quantity") )
      {
         sscanf(value.c_str(), "%d", &quantityPerDay);
      }
      /* Aditional Quantity */
      else if(key == "aditionalQuantity")
      {
         sscanf("%s %d %d %d", 
               &buf[0], &aditionalQuantity,
               &aditionalLevel,
               &aditionalDiv);
         aditionalFactor = new Factor("CLASS", buf);
      }

      /* Script */
      else if(key == "script")
      {
         scriptFile = value;
      }

      /* Dependent Feats */
      else if(key == "depFeat")
      {
         float r = 0.0f;
         sscanf(value.c_str(),"%f %s", &r, &buf[0]);
         depFeats.insert(new DepFeat(r, buf));
      }

      /* Unknow Key!! */
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: Unknow key '%s' at talent file '%s'", 
               key.c_str(), filename.c_str());
      }

   }

   return true;
}

/***************************************************************
 *                       getCostToUse                          *
 ***************************************************************/
int FeatDescription::getCostToUse()
{
   return costToUse;
}

/***************************************************************
 *                     getQuantityPerDay                       *
 ***************************************************************/
int FeatDescription::getQuantityPerDay()
{
   return quantityPerDay;
}

/***************************************************************
 *                         getDepFeats                         *
 ***************************************************************/
Kobold::List* FeatDescription::getDepFeats()
{
   return &depFeats;
}

/***************************************************************
 *                        getScriptFile                        *
 ***************************************************************/
Kobold::String FeatDescription::getScriptFile()
{
   return scriptFile;
}

/***************************************************************
 *                           getRange                          *
 ***************************************************************/
int FeatDescription::getRange()
{
   return range;
}

/***************************************************************
 *                         getActionId                         *
 ***************************************************************/
Action::ActionId FeatDescription::getActionId()
{
   return action;
}

/***************************************************************
 *                       getPowerLevel                         *
 ***************************************************************/
int FeatDescription::getPowerLevel()
{
   return powerLevel;
}

/**************************************************************************
 *                                    FEAT                                *
 **************************************************************************/
/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
Feat::Feat()
{
   quantity = 0.0f;
   info = NULL;
}

/***************************************************************
 *                          Destructor                         *
 ***************************************************************/
Feat::~Feat()
{
}

/***************************************************************
 *                       setFeatDescription                    *
 ***************************************************************/
void Feat::setFeatDescription(FeatDescription* info)
{
   this->info = info;
   resetQuantity();
}

/***************************************************************
 *                          getInfo                            *
 ***************************************************************/
FeatDescription* Feat::getInfo()
{
   return info;
}

/***************************************************************
 *                         getQuantity                         *
 ***************************************************************/
float Feat::getQuantity()
{
   return quantity;
}

/***************************************************************
 *                         setQuantity                         *
 ***************************************************************/
void Feat::setQuantity(int value)
{
   quantity = value;
}

/***************************************************************
 *                         decQuantity                         *
 ***************************************************************/
void Feat::decQuantity(int value, Feats* feats)
{
   /* Decrement its avaiable quantity */
   quantity -= value;
   
   /* Decrement avaiable quantity of dependent Feats */
   FeatDescription::DepFeat* dft = static_cast<FeatDescription::DepFeat*>
      (info->getDepFeats()->getFirst());

   for(int i=0; i < info->getDepFeats()->getTotal(); i++)
   {
      Feat* ft = feats->getFeatByString(dft->getFeatId());
      ft->setQuantity(ft->getQuantity() - 
            info->getCostToUse() / dft->getReason());

      /* next dependency */
      dft = static_cast<FeatDescription::DepFeat*>(dft->getNext());
   }
}

/***************************************************************
 *                         setQuantity                         *
 ***************************************************************/
void Feat::resetQuantity()
{
   this->quantity = info->getQuantityPerDay();
}

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
Feats::Feats()
{
   totalFeats = 0;
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
Feats::~Feats()
{
   clear();
}

/***************************************************************
 *                           clear                             *
 ***************************************************************/
void Feats::clear()
{
   totalFeats = 0;
}

/***************************************************************
 *                         getTotal                            *
 ***************************************************************/
int Feats::getTotal()
{
   return totalFeats;
}

/***************************************************************
 *                       FeatByNumber                          *
 ***************************************************************/
Feat* Feats::getFeatByNumber(int featNumber)
{
   if( (featNumber >= 0) && (featNumber < totalFeats) )
   {
      return &feats[featNumber];
   }
   return NULL;
}

/***************************************************************
 *                       FeatByName                            *
 ***************************************************************/
Feat* Feats::getFeatByString(Kobold::String featName)
{
   int i;
   for(i = 0; i < totalFeats; i++)
   {
      if(featName.compare(feats[i].getInfo()->getStringId()) == 0)
      {
         return &feats[i];
      }
   }
   return NULL;
}

/***************************************************************
 *                       insertFeat                            *
 ***************************************************************/
bool Feats::insert(FeatDescription* feat)
{
   if(feat == NULL)
   {
      return false;
   }

   /* Verify if already inserted */
   if(getFeatByString(feat->getStringId()))
   {
      /* Already insert. No need for duplicate */
      return true;
   }

   /* Verify if not max */
   if(totalFeats < MAX_FEATS)
   {
      /* Really insert it */
      feats[totalFeats].setFeatDescription(feat);
      totalFeats++;

      return true;
   }

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, "Error: Feat overflow!");
   return false;
}

/***************************************************************
 *                            canUse                           *
 ***************************************************************/
bool Feats::canUse(int featNumber)
{
   /* Verify if the Feat is valid */
   if( (featNumber < 0) || (featNumber >= totalFeats) )
   {
      Briefing::addWarning(gettext("Invalid Talent"));
      return false;
   }

   return canUse(&feats[featNumber]);
}

/***************************************************************
 *                            canUse                           *
 ***************************************************************/
bool Feats::canUse(Feat* f)
{
   /* Verify if have the Feat points to use it */
   if(f->getQuantity() - f->getInfo()->getCostToUse() >= 0)
   {
      return true;
   }
   else
   {
      /* Can't use due to points! */
      Briefing::addWarning(gettext("Not enought points to use!"));
   }

   return false;
}

/***************************************************************
 *                          useFeat                            *
 ***************************************************************/
void Feats::useFeat(int featNumber)
{
   /* Apply Cost */
   if(feats[featNumber].getInfo()->getCostToUse() > 0)
   {
      feats[featNumber].decQuantity(feats[featNumber].getInfo()->getCostToUse(),
            this);
   }
}

/***************************************************************
 *                       useFeatAtTarget                       *
 ***************************************************************/
bool Feats::useFeatAtTarget(Thing* actor, int featNumber, Thing* target)
{
   if( (canUse(featNumber)) && 
       (!feats[featNumber].getInfo()->getScriptFile().empty()))
   {
      /* Show Feature name */
      if(featNumber != FEAT_WEAPON_ATTACK)
      {
         Briefing::addText(feats[featNumber].getInfo()->getName()); 
      }

      /* Verify range */
      Ogre::Vector3 actorPos = actor->getModel()->getPosition();
      Ogre::Vector3 targetPos = target->getModel()->getPosition();
      if(!Action::isInRange(actorPos, targetPos,
               feats[featNumber].getInfo()->getRange()))
      {
         Briefing::addWarning(gettext("Too far away for action!"));
         return false;
      }

      //TODO!
#if 0
      /* Init the script to use */
      sc = new iaScript(feats[featNumber].info->scriptFile, uEngine);

      /* Set infos */
      sc->defineCharacterOwner((Character*) actor);
      sc->defineMap(uEngine->getCurrentMap(), uEngine->NPCs);

      /* Set parameters */
      sc->setParameter("target", target);

      /* Run it! */
      sc->run(0);

      /* Finish with the script */
      delete(sc);
#endif

      /* Apply costs */
      useFeat(featNumber);

      return true;
   }

   return false;
}

/***************************************************************
 *                       useFeatAtArea                         *
 ***************************************************************/
bool Feats::useFeatAtArea(Thing* actor, int featNumber, 
             float x, float y, float z)
{
   if( (canUse(featNumber)) && 
       (!feats[featNumber].getInfo()->getScriptFile().empty()))
   {
      /* Show Feature name */
      if(featNumber != FEAT_WEAPON_ATTACK)
      {
         Briefing::addText(feats[featNumber].getInfo()->getName());
      }

      /* Verify range */
      if(!Action::isInRange(actor->getModel()->getPosition(),
               Ogre::Vector3(x, 0.0f, z), 
               feats[featNumber].getInfo()->getRange()))
      {
         Briefing::addWarning(gettext("Too far away for action!"));
         return false;
      }
     
      //TODO: script implementation!
#if 0
      /* Init the script to use */
      sc = new iaScript(feats[featNumber].info->scriptFile, uEngine);

      /* Set infos */
      sc->defineCharacterOwner((Character*) actor);
      sc->defineMap(uEngine->getCurrentMap(), uEngine->NPCs);

      /* Set parameters */
      sc->setParameter("x", &x);
      sc->setParameter("y", &y);
      sc->setParameter("z", &z);

      /* Run it! */
      sc->run(0);

      /* Finish with the script */
      delete(sc);
#endif

      /* Apply costs */
      useFeat(featNumber);

      return true;
   }

   return false;
}

/***************************************************************
 *                      applyPermanentFeat                     *
 ***************************************************************/
bool Feats::applyPermanentFeat(Thing* actor, int featNumber)
{
   if( (canUse(featNumber)) && 
       (!feats[featNumber].getInfo()->getScriptFile().empty()))
   {
       //TODO: scripts!
#if 0
      /* Init the script to use */
      sc = new iaScript(feats[featNumber].info->scriptFile, uEngine);

      /* Set infos */
      sc->defineCharacterOwner((Character*) actor);
      sc->defineMap(uEngine->getCurrentMap(), uEngine->NPCs);

      /* Run it! */
      sc->run(0);

      /* Finish with the script */
      delete(sc);
#endif
      return true;
   }

   return false;
}

/***************************************************************
 *                         New Day                             *
 ***************************************************************/
void Feats::newDay()
{
   for(int i = 0; i < totalFeats; i++)
   {
      feats[i].resetQuantity();
   }
}

/***************************************************************
 *                      getNPCAttackFeat                       *
 ***************************************************************/
int Feats::getRandomNPCAttackFeat(Thing* pers, Thing* target)
{
   int ft;

   if( (target != NULL) && (pers != NULL))
   {
      ft = (int)(totalFeats*(rand() / (RAND_MAX + 1.0)));

      //FIXME verify if the Feat is in range to use!

      if( (ft != FEAT_WEAPON_ATTACK) &&
          (feats[ft].getInfo()->getActionId() == Action::ACT_ATTACK)  && 
          (feats[ft].getQuantity() - feats[ft].getInfo()->getCostToUse() >= 0))
      {
          /* is avaible */
          return ft;
      }
      else
      {
          /* otherwise, use base weapon attack (melee or ranged) */
         return FEAT_WEAPON_ATTACK;
      }
   }
   
   return -1;
}

/***************************************************************
 *                   getPowerFullAttackFeat                    *
 ***************************************************************/
int Feats::getPowerfullAttackFeat(Thing* pers, Thing* target)
{
   int ft = -1;
   int power = 0;
   int i;

   /* FIXME test range of the Feats! */

   /* Take the initial Feat */
   ft = FEAT_WEAPON_ATTACK;
   power = feats[ft].getInfo()->getPowerLevel();

   /* Run over all Feats searching for a powerfull one */
   for(i = 0; i < totalFeats; i++)
   {
      /* Verify if is an attack Feat and is avaible */
      if( (i != ft) && (i != FEAT_WEAPON_ATTACK) && 
          (feats[i].getInfo()->getActionId() == Action::ACT_ATTACK) && 
          (feats[i].getQuantity() - feats[i].getInfo()->getCostToUse() >= 0) )
      {
         /* verify if is powerfull */
         if(feats[i].getInfo()->getPowerLevel() > power)
         {
            /* Is powerfull, take the Feat */
            ft = i;
            power = feats[i].getInfo()->getPowerLevel();
         }
      }
   }

   return ft;
}

/***************************************************************
 *                       getFirstHealFeat                      *
 ***************************************************************/
int Feats::getFirstHealFeat(Thing* pers)
{
   int i = -1;

   if(pers)
   {
      /* Run over all Feats searching for a heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an heal Feat and is available */
         if( (feats[i].getInfo()->getActionId() == Action::ACT_HEAL) && 
             (feats[i].getQuantity()-feats[i].getInfo()->getCostToUse() >= 0))
         {
            return i;
         }
      }
   }

   return -1;
}

/***************************************************************
 *                     getRandomHealFeat                       *
 ***************************************************************/
int Feats::getRandomHealFeat(Thing* pers)
{
   int ft;

   if((pers != NULL))
   {
      ft = (int)(totalFeats*(rand() / (RAND_MAX + 1.0)));

      if( (feats[ft].getInfo()->getActionId() == Action::ACT_HEAL)  && 
          (feats[ft].getQuantity() - feats[ft].getInfo()->getCostToUse() >= 0))
      {
          /* is avaible */
          return ft;
      }
   }
   return getFirstHealFeat(pers);
}

/***************************************************************
 *                     getPowerfullHealFeat                    *
 ***************************************************************/
int Feats::getPowerfullHealFeat(Thing* pers)
{
   int ft = getFirstHealFeat(pers);
   int i = 0;

   if( (pers) && (ft != -1) )
   {
      /* Run over all Feats searching for a powerfull heal one */
      for(i = 0; i < totalFeats; i++)
      {
         /* Verify if is an heal Feat and is available */
         if( (i != ft) && 
             (feats[i].getInfo()->getActionId() == Action::ACT_HEAL) && 
             (feats[i].getQuantity() - feats[i].getInfo()->getCostToUse() >= 0))
         {
            /* Verify if is powerfull */
            if(feats[i].getInfo()->getPowerLevel() > 
               feats[ft].getInfo()->getPowerLevel())
            {
               ft = i;
            }
         }
      }
   }

   return ft;
}

/**************************************************************************
 *                               FEATS LIST                               *
 **************************************************************************/

/***************************************************************
 *                       Constructor                           *
 ***************************************************************/
void FeatsList::init(Kobold::String dir, Kobold::String arq)
{
   Kobold::Log::add("Initing Talents");

   Kobold::String key, value;
   Kobold::DefParser defParser;

   if(!defParser.load(dir + arq))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
          "Error while opening feats list file '%s' ",
          (dir + arq).c_str());
       return;
   }

   /* Define totalFeats */
   defParser.getNextTuple(key, value);
   if(key != "totalFeats")
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
          "Error: feats list file without totalFeats as first definition!");
      return;
   }
   sscanf(value.c_str(), "%d", &totalFeats);
   if(totalFeats > 0)
   {
      feats = new FeatDescription[totalFeats];
   }
   else
   {
      feats = NULL;
   }
 
   /* Load each Feat */
   int aux = 0;
   while(defParser.getNextTuple(key, value))
   {
      if(aux < totalFeats)
      {
         feats[aux].load(dir + value, aux);
      }
      aux++;
   }
}

/***************************************************************
 *                        Destructor                           *
 ***************************************************************/
void FeatsList::finish()
{
   Kobold::Log::add("Finishing Talents");
   if(feats)
   {
      delete[] feats;
   }
}

/***************************************************************
 *                        FeatByName                           *
 ***************************************************************/
FeatDescription* FeatsList::getFeatByString(Kobold::String featName)
{
   for(int i = 0; i < totalFeats; i++)
   {
      if(featName == feats[i].getStringId())
      {
         return &feats[i];
      }
   }
   return NULL;
}

/***************************************************************
 *                        FeatByNumber                         *
 ***************************************************************/
FeatDescription* FeatsList::getFeatByNumber(int featNumber)
{
   if( (featNumber >= 0) && (featNumber < totalFeats) )
   {
      return &feats[featNumber];
   }
   return NULL;
}

/***************************************************************
 *                          getTotal                           *
 ***************************************************************/
int FeatsList::getTotal()
{
   return totalFeats;
}

/***************************************************************
 *                       static members                        *
 ***************************************************************/
FeatDescription* FeatsList::feats=NULL;
int FeatsList::totalFeats=0;

