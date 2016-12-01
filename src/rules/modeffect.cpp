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

#include "modeffect.h"

#include "modifier.h"
#include "skills.h"
#include "character.h"

#include <kobold/log.h>

using namespace DNT;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                             ModEffect                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
ModEffect::ModEffect(ModEffect* obj)
{
   /* Copy values */
   mod = obj->mod;
   timer.reset();
   applyTimer.reset();
   time = obj->time;
   applied = obj->applied;
   periodicTime = obj->periodicTime;
   cause = new Factor(obj->cause->getType(), obj->cause->getId());
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
ModEffect::ModEffect(int mod, int time, int periodicTime, 
            Kobold::String factorId, Kobold::String factorType)
{
   /* set values */
   this->mod = mod;
   this->applied = false;
   this->time = time;
   this->periodicTime = periodicTime;
   this->cause = new Factor(factorType, factorId);
   timer.reset();
   applyTimer.reset();
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
ModEffect::ModEffect(Kobold::String saveTxt)
{
   fromSaveText(saveTxt);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
ModEffect::~ModEffect()
{
   if(cause)
   {
      delete cause;
   }
}

/***********************************************************************
 *                             toSaveText                              *
 ***********************************************************************/
Kobold::String ModEffect::toSaveText()
{
   char buf[512];

   sprintf(buf, "%d,%lu,%lu,%d,%d,%s %s", 
         mod, timer.getMilliseconds(), applyTimer.getMilliseconds(), 
         time, periodicTime, 
         cause->getId().c_str(), cause->getType().c_str());
   return buf;
}

/***********************************************************************
 *                            fromSaveText                             *
 ***********************************************************************/
void ModEffect::fromSaveText(Kobold::String txt)
{
   char i[64], t[64];
   unsigned long tInit=0L, tApInit=0L;
   sscanf(txt.c_str(), "%d,%lu,%lu,%d,%d,%s %s", 
         &mod, &tInit, &tApInit, &time, &periodicTime, 
         i, t);

   if(cause)
   {
      delete cause;
   }
   cause = new Factor(t, i);

   applied = false;

   timer.reset(tInit);
   applyTimer.reset(tApInit);
}

/***********************************************************************
 *                          toReadableText                             *
 ***********************************************************************/
Kobold::String ModEffect::toReadableText(Character* actor)
{
   char buf[128];

   Skill* s;

   if(actor == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: ModEffect::toReadableText: NULL actor!");
      return "";
   }

   /* Attribute and skills */
   if((cause->getType() == MOD_TYPE_ATT) || 
      (cause->getType() == MOD_TYPE_SKILL)) 
   {
      s = actor->getSkills()->getSkillByString(cause->getId());

      if(s)
      {
         if(mod <= 0)
         {
            sprintf(buf, "%s: %d", s->getDefinition()->getName().c_str(), mod);
         }
         else
         {
            sprintf(buf, "%s: +%d", s->getDefinition()->getName().c_str(), mod);
         }
         return buf;
      }
      else
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: unknow cause '%s' for character '%s' ModEffect",
               cause->getId().c_str(), actor->getName().c_str());
      }
   }
   else if(cause->getType() == MOD_TYPE_THING)
   {
      Kobold::String text = cause->getId();
      if(cause->getId() == THING_ARMATURE_CLASS)
      {
         text = gettext("Armature Class");
      }
      else if(cause->getId() == THING_INITIATIVE_BONUS)
      {
         text = gettext("Initiative");
      }
      else if(cause->getId() == THING_MAX_LIFE_POINTS)
      {
         text = gettext("Life Points");
      }
      else if(cause->getId() == THING_DISPLACEMENT)
      {
         text = gettext("Displacement");
      }
      else if(cause->getId() == DNT_BS_FORTITUDE)
      {
         text = gettext("Fortitude");
      }
      else if(cause->getId() == DNT_BS_REFLEXES)
      {
         text = gettext("Reflexes");
      }
      else if( (cause->getId() == DNT_BS_I_AM_NOT_A_FOOL) ||
               (cause->getId() == DNT_BS_WILL) )
      {
         text = gettext("I Am Not A Fool");
      }
      
      if(mod <= 0)
      {
         sprintf(buf, "%s: %d", text.c_str(), mod);
      }
      else
      {
         sprintf(buf, "%s: +%d", text.c_str(), mod);
      }
      return buf;
   }

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Error: Invalid factor '%s' '%s' for ModEffect!",
         cause->getType().c_str(), cause->getId().c_str());
   
   return gettext("Invalid Modifier!");
}

/***********************************************************************
 *                              getValue                               *
 ***********************************************************************/
int ModEffect::getValue()
{
   return mod;
}

/***********************************************************************
 *                                 apply                               *
 ***********************************************************************/
void ModEffect::apply(Character* actor)
{
   /* Apply */
   doApply(actor, mod);
   applied = true;
   applyTimer.reset();
}

/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void ModEffect::unApply(Character* actor)
{
   /* Apply with inverse value */
   doApply(actor, -mod);
   applied = false;
}

/***********************************************************************
 *                               doApply                               *
 ***********************************************************************/
void ModEffect::doApply(Character* actor, int value)
{
   if(actor != NULL)
   {
      actor->incFactorValue(*cause, value);
   }
   else
   { 
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: ModEffect::doApply: NULL Character!");
   }
}

/***********************************************************************
 *                             resetTimer                              *
 ***********************************************************************/
void ModEffect::resetTimer()
{
   timer.reset();
   applyTimer.reset();
}

/***********************************************************************
 *                            isElapsed                                *
 ***********************************************************************/
bool ModEffect::isElapsed()
{
   return timer.getMilliseconds() > time;
}

/***********************************************************************
 *                            isTemporally                             *
 ***********************************************************************/
bool ModEffect::isTemporally()
{
   return time != 0;
}

/***********************************************************************
 *                              isPeriodic                             *
 ***********************************************************************/
bool ModEffect::isPeriodic()
{
   return periodicTime != 0;
}

/***********************************************************************
 *                         isPeriodicElapsed                           *
 ***********************************************************************/
bool ModEffect::isPeriodicElapsed()
{
   return applyTimer.getMilliseconds() > periodicTime;
}

/***********************************************************************
 *                            wasApplied                               *
 ***********************************************************************/
bool ModEffect::wasApplied()
{
   return applied;
}


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                           ModEffectList                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
ModEffectList::ModEffectList()
{
   owner = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
ModEffectList::~ModEffectList()
{
   clearList();
}

/***********************************************************************
 *                               setOwner                              *
 ***********************************************************************/
void ModEffectList::setOwner(Character* actor)
{
   owner = actor;
}

/***********************************************************************
 *                                insert                               *
 ***********************************************************************/
bool ModEffectList::insert(ModEffect* obj, bool applyEffect)
{
   if(obj)
   {
      /* Insert it on list */
      Kobold::List::insert(obj);
      
      /* Set its initial timer to now */
      obj->resetTimer();

      /* Apply the effect, if owner is defined */
      if( (owner != NULL) && (applyEffect) )
      {
         obj->apply(owner);
      }
      return true;
   }

   return false;
}

/***********************************************************************
 *                              insertCopy                             *
 ***********************************************************************/
bool ModEffectList::insertCopy(ModEffect* obj, bool applyEffect)
{
   ModEffect* eff;

   if(obj != NULL)
   {
      /* Create the copy */
      eff = new ModEffect(obj);
      
      /* Insert on list */
      return insert(eff, applyEffect);
   }

   return false;
}

/***********************************************************************
 *                                  rest                               *
 ***********************************************************************/
void ModEffectList::rest()
{
   int i;
   int curTotal = total;
   ModEffect* eff = (ModEffect*)first;
   ModEffect* efaux;

   /* Search the list for expired ModEffects */
   for(i = 0; i < curTotal; i++)
   {
      /* Getting the next to not lose the pointer */
      efaux = eff;
      eff = (ModEffect*)eff->getNext();

      /* Verify if is timed */
      if(efaux->isTemporally() != 0)
      {
         /* Must deapply it */
         if(owner != NULL)
         {
            efaux->unApply(owner);
         }

         /* remove it from the list */
         remove(efaux);
      }
   }
}

/***********************************************************************
 *                                 doStep                              *
 ***********************************************************************/
void ModEffectList::doStep()
{
   int i;
   int curTotal = total;
   ModEffect* eff = (ModEffect*)first;
   ModEffect* efaux;

   /* Search the list for expired ModEffects */
   for(i = 0; i < curTotal; i++)
   {
      /* Getting the next to not lose the pointer */
      efaux = eff;
      eff = (ModEffect*)eff->getNext();

      /* Verify if expired */
      if(efaux->isElapsed())
      {
         /* Must deapply it */
         if(owner != NULL)
         {
            efaux->unApply(owner);
         }

         /* remove it from the list */
         remove(efaux);
      }
      /* Verify if is time to apply again */
      else if((efaux->isPeriodic()) && (efaux->isPeriodicElapsed()))
      {
         /* Apply the effect, if owner is defined */
         if(owner != NULL)
         {
            eff->apply(owner);
         }
      }
   }
}


/***********************************************************************
 *                               unApply                               *
 ***********************************************************************/
void ModEffectList::unApply()
{
   int i;

   ModEffect* eff = (ModEffect*)first;

   for(i=0; i < total; i++)
   {
      if(eff->wasApplied())
      {
         eff->unApply(owner);
      }
      eff = (ModEffect*)eff->getNext();
   }
}

/***********************************************************************
 *                                apply                                *
 ***********************************************************************/
void ModEffectList::apply()
{
   int i;

   ModEffect* eff = (ModEffect*)first;

   for(i=0; i < total; i++)
   {
      if(!eff->wasApplied())
      {
         eff->apply(owner);
      }
      eff = (ModEffect*)eff->getNext();
   }
}

