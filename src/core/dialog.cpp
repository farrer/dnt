/* 
  DNT - a satirical post-apocalyptical RPG.
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

#include "dialog.h"
#include "playablecharacter.h"
#include "game.h"
#include "item.h"
#include "inventory.h"
#include "modstate.h"
#include "door.h"

#include "../rules/thing.h"
#include "../lang/translate.h"

#include "../rules/modifier.h"

#include "../map/npcfile.h"

#include "../gui/briefing.h"
#include "../gui/dialogwindow.h"

#include <kobold/log.h>
#include <kosound/sound.h>

#include <OGRE/OgreDataStream.h>
#include <OGRE/OgreResourceGroupManager.h>

#include <stdlib.h>
#include <assert.h>

using namespace DNT;

/* General Tokens */
#define TK_TRUE "true"
#define TK_FALSE "false"
#define TK_DIALOG "dialog"
#define TK_END_DIALOG "end_dialog"
#define TK_NPC_BEGIN "npc_begin"
#define TK_NPC_END "npc_end"
#define TK_PC_BEGIN "pc_begin"
#define TK_PC_END "pc_end"
#define TK_PRE_TEST "pre_test"
#define TK_POST_TEST "post_test"
#define TK_TEXT "text"
#define TK_ACTION "action"
#define TK_IF_ACTION "if_action"
#define TK_ELSE_ACTION "else_action"
#define TK_OPTION "option"

/* Action Tokens */
#define TK_ACTION_GO_TO_DIALOG "go_to_dialog"
#define TK_ACTION_INIT_FIGHT "init_fight"
#define TK_ACTION_FINISH_DIALOG "finish_dialog"
#define TK_ACTION_DIALOG_INIT "dialog_init"
#define TK_ACTION_ADD_MISSION "add_mission"
#define TK_ACTION_COMPLETE_MISSION "complete_mission"
#define TK_ACTION_GIVE_ITEM "give_item"
#define TK_ACTION_RECEIVE_MONEY "receive_money"
#define TK_ACTION_GIVE_MONEY "give_money"
#define TK_ACTION_CHANGE_OBJECT_STATE "change_object_state"
#define TK_ACTION_RECEIVE_XP "receive_xp"
#define TK_ACTION_KILL_ALL "kill_all"
#define TK_ACTION_RECEIVE_ITEM "receive_item"
#define TK_ACTION_MAP_TRAVEL "map_travel"
#define TK_ACTION_CALL_SCRIPT "call_script"
#define TK_ACTION_OPEN "open"
#define TK_ACTION_PLAY_SOUND "play_sound"

/* Test Tokens */
#define TK_TEST_ROLL "roll"
#define TK_TEST_GREATER "greater"
#define TK_TEST_LESSER "lesser"
#define TK_TEST_EQUAL "equal"
#define TK_TEST_DIFF "diff"
#define TK_TEST_ALIGN "align"
#define TK_TEST_ALIGN_NOT "align_not"
#define TK_TEST_HAVE_ITEM "have_item"
#define TK_TEST_HAVE_ITEM_WITH_INFO "have_item_with_info"
#define TK_TEST_ALL_ALIVE "all_alive"
#define TK_TEST_ALL_DEAD "all_dead"
#define TK_TEST_HAVE_MONEY "have_money"
#define TK_TEST_MISSION_ACTIVE "mission_active"

/* Constant Tokens */
#define TK_CONST_OBJECT_STATE "OBJECT_STATE"



///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                              TalkAction                               //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
TalkAction::TalkAction(TalkActionType type)
{
   this->type = type;
   this->qty = 0;
   this->att = -1;
   this->satt = "";
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
TalkAction::~TalkAction()
{
}

/*************************************************************************
 *                              setAttribute                             *
 *************************************************************************/
void TalkAction::setAttribute(int att)
{
   this->att = att;
}

/*************************************************************************
 *                            setStringFactor                            *
 *************************************************************************/
void TalkAction::setStringFactor(Kobold::String factor)
{
   this->satt = factor;
}

/*************************************************************************
 *                              setQuantity                              *
 *************************************************************************/
void TalkAction::setQuantity(int qty)
{
   this->qty = qty;
}

/*************************************************************************
 *                                execute                                *
 *************************************************************************/
void TalkAction::execute(Conversation* conv, PlayableCharacter* pc, 
      Thing* owner, Kobold::String ownerMap)
{
   switch(type)
   {
      /* Change Dialog */
      case TALK_ACTION_GO_TO_DIALOG:
      {
         conv->changeDialog(att);
      }
      break;
      /* Init a Fight with the owner */
      case TALK_ACTION_INIT_FIGHT:
      {
//TODO: fight init
#if 0
         engine* eng = (engine*)curEngine;
         owner->setAsEnemy();
         eng->enterBattleMode(false);
         dlgWindow.close();
#endif
      }
      break;
      /* Close the Dialog */
      case TALK_ACTION_FINISH_DIALOG:
         DialogWindow::close();
      break;
      case TALK_ACTION_MOD_PC:
         //TODO
         break;
      case TALK_ACTION_MOD_NPC:
         //TODO
         break;
      /* Set new initial Dialog */
      case TALK_ACTION_DIALOG_INIT:
      {
         conv->setInitialDialog(att);
      }
      break;
      /* Open the door */
      case TALK_ACTION_OPEN:
      {
         if(owner->getThingType() == Thing::THING_TYPE_DOOR)
         {
            Door* d = static_cast<Door*>(owner);
            if(d->getOpenStatus() == Door::DOOR_LOCKED)
            {
               d->unlock();
            }
            d->open();
         }
      }
      break;

      case TALK_ACTION_PLAY_SOUND:
      {
         /* Play a sound at owner's position */
         Ogre::Vector3 pos = owner->getModel()->getPosition();
         Kosound::Sound::addSoundEffect(pos.x, pos.y, pos.z,
               SOUND_NO_LOOP, satt);
      }
      break;

         /* Call and run a script */
      case TALK_ACTION_CALL_SCRIPT:
      {
//TODO: scripts
#if 0
         /* Open the script */
         iaScript* script = new iaScript(actions[i].satt, curEngine);

         /* Set the script owner */
         if(owner)
         {
            if(owner->getThingType() == THING_TYPE_OBJECT)
            {
               script->defineObjectOwner((object*)owner);
            }
            else if(owner->getThingType() == THING_TYPE_CHARACTER)
            {
               script->defineCharacterOwner((Character*)owner);
            }
         }
         /* Define the current map */
         script->defineMap( ((engine*)curEngine)->getCurrentMap(),
               ((engine*)curEngine)->NPCs);

         /* Now, full run the script */
         script->run(0);

         /* Done with the script */
         delete(script);
#endif
      }
      break;

      /* Add a misstion */
      case TALK_ACTION_ADD_MISSION:
      {
//TODO: missions
#if 0
         missionsController missions;
         missions.addNewMission(actions[i].satt);
#endif
      }
      break;
      /* Complete a mission */
      case TALK_ACTION_COMPLETE_MISSION:
      {
//TODO: missions
#if 0
         missionsController missions;
         mission* m;
         m=missions.getCurrentMission(actions[i].satt);
         if(m)
         {
            m->setXp(actions[i].att);
            missions.completeMission(m, actions[i].qty);
            char vstr[200];
            sprintf(vstr,gettext("Mission Completed: %d XP!"),m->getXp());
            messageController msgController;
            msgController.addMessage(currentPC->scNode->getPosX(),
                  currentPC->scNode->getBoundingBox().max.y,
                  currentPC->scNode->getPosZ(), vstr, 0.94, 0.8, 0.0);
         }
#endif
      }
      break;
      /* Give a Item */
      case TALK_ACTION_GIVE_ITEM:
      {
         /* Only give item if the owner is a Character */
         if(owner->getThingType() == Thing::THING_TYPE_CHARACTER)
         {
            Character* ownerNPC = static_cast<Character*>(owner);
            /* Search for the item at actor's inventory */
            Item* item = pc->getInventory()->getItemByFilename(satt);
            if(item)
            {
               /* Remove it from there */
               pc->getInventory()->removeFromInventory(item);

               /* Add it to the Owner NPC inventory */
               ownerNPC->getInventory()->addItem(item);

               /* NOTE: The NPC inventory is always saved at modstate when 
                * the PC leaves the map, and reloaded when it come back */
               Briefing::addText(gettext("%s lost."), item->getName().c_str());
            }
            else
            {
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: no object '%s' to give at character's inventory!",
                  satt.c_str());
            }
         }
      }
      break;

      /* Receive Item */
      case TALK_ACTION_RECEIVE_ITEM:
      {
         /* Only receive items from characters */
         if(owner->getThingType() == Thing::THING_TYPE_CHARACTER)
         {
            Character* ownerNPC = static_cast<Character*>(owner);
            /* Search for the item on owner's inventory */
            Item* item = ownerNPC->getInventory()->getItemByFilename(satt);
            if(item)
            {
               /* Remove it from there */
               ownerNPC->getInventory()->removeFromInventory(item);

               /* Add it to the Owner NPC inventory */
               pc->getInventory()->addItem(item);
               //TODO map add object, when inventory is full!

               /* NOTE: The NPC inventory is always saved at modstate when 
                * the PC leaves the map, and reloaded when it come back */
               Briefing::addText(gettext("Received %s"), 
               item->getName().c_str());
            }
         }
         /* Else, show no error, as the character could 'lost' the item on 
          * various ways: bartering, stealing, etc. */
      }
      break;

      /* Receive Money */
      case TALK_ACTION_RECEIVE_MONEY:
      {
         pc->getInventory()->addMoney(att);
         Briefing::addText(gettext("Received $%d."), att);
      }
      break;

      /* Give Money */
      case TALK_ACTION_GIVE_MONEY:
      {
         pc->getInventory()->decMoney(att);
         Briefing::addText(gettext("Lost $%d."), att);
      }
      break;

      /* Change Object State */
      case TALK_ACTION_CHANGE_OBJECT_STATE:
      {
         /* Change the state */
         owner->setState(att);
         Object* obj = static_cast<Object*>(owner);
         /* Tell ModState about the change */
         ModState::addMapObjectAction(MODSTATE_ACTION_OBJECT_CHANGE_STATE,
               obj->getFilename(), ownerMap,
               obj->getModel()->getPosition(), obj->getState());
      }
      break;

      /* Receive XP */
      case TALK_ACTION_RECEIVE_XP:
      {
//TODO: Experience
#if 0
         /* At XP to the Character */
         currentPC->addXP(actions[i].att);

         /* Set the Message */
         char vstr[200];
         sprintf(vstr, "%d XP", actions[i].att);

         /* Put Message at game */
         messageController msgController;
         msgController.addMessage(currentPC->scNode->getPosX(),
               currentPC->scNode->getBoundingBox().max.y,
               currentPC->scNode->getPosZ(), vstr,
               0.94, 0.8, 0.0);

         /* Put Message at Briefing */
         Briefing brief;
         brief.addText(vstr, 251, 209, 12);
#endif
      }
      break;

      /* Kill All NPCs from a file */
      case TALK_ACTION_KILL_ALL:
      {
         NpcFile npcs;
         if(npcs.load(satt))
         {
            npcs.killAll();
         }
      }
      break;

      /* Travel to another map */
      case TALK_ACTION_MAP_TRAVEL:
      {
//TODO: map travel
#if 0
         engine* eng = (engine*)curEngine;
         eng->loadMap(actions[i].satt);
#endif
      }
      break;
   }

}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                               TalkTest                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
TalkTest::TalkTest(Kobold::String token, Kobold::String t, Kobold::String a)
{
   /* Set test and against */
   this->test = t;
   this->against = a;

   /* translate token Kobold::String to id */

   /* roll */
   if(token == TK_TEST_ROLL)
   {
      type = TALK_TEST_ROLL;
   }
   /* greater */
   else if(token == TK_TEST_GREATER)
   {
      type = TALK_TEST_GREATER;
   }
   /* lesser */
   else if(token == TK_TEST_LESSER)
   {
      type = TALK_TEST_LESSER;
   }
   /* equal */
   else if(token == TK_TEST_EQUAL)
   {
      type = TALK_TEST_EQUAL;
   }
   /* diff */
   else if(token == TK_TEST_DIFF)
   {
      type = TALK_TEST_DIFFERENT;
   }
   /* have_item */
   else if(token == TK_TEST_HAVE_ITEM)
   {
      type = TALK_TEST_HAVE_ITEM;
   }
   /* have_item_with_info */
   else if(token == TK_TEST_HAVE_ITEM_WITH_INFO)
   {
      type = TALK_TEST_HAVE_ITEM_WITH_INFO;
   }
   /* align_not */
   else if(token == TK_TEST_ALIGN_NOT)
   {
      type = TALK_TEST_ALIGNMENT_NOT;
   }
   /* align */
   else if(token == TK_TEST_ALIGN)
   {
      type = TALK_TEST_ALIGNMENT;
   }
   /* all_dead */
   else if(token == TK_TEST_ALL_DEAD)
   { 
      type = TALK_TEST_ALL_DEAD;
   }
   /* all_alive */
   else if(token == TK_TEST_ALL_ALIVE)
   {
      type = TALK_TEST_ALL_ALIVE;
   }
   /* have_money */
   else if(token == TK_TEST_HAVE_MONEY)
   {
      type = TALK_TEST_HAVE_MONEY;
   }
   else if(token == TK_TEST_MISSION_ACTIVE)
   {
      type = TALK_TEST_MISSION_ACTIVE;
   }
   else
   {
      /* Unknow test function! */
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: unknow test '%s' at dialog!", token.c_str());
   }
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
TalkTest::~TalkTest()
{
}

/*************************************************************************
 *                              getTestName                              *
 *************************************************************************/
Kobold::String TalkTest::getTestName(PlayableCharacter* pc)
{
   Kobold::String res = "";

   /* Get the skill name */
   if(pc)
   {
      Skill* sk = pc->getSkills()->getSkillByString(test);
      if(sk)
      {
         res = "(" + sk->getDefinition()->getName() + ") ";
      }
   }

   return res;
}


/*************************************************************************
 *                         getAgainstValue                               *
 *************************************************************************/
int TalkTest::getAgainstValue(Thing* owner)
{
   int val = 0;

   /* Try to get as a number */
   if(sscanf(against.c_str(), "%d", &val) == 1)
   {
      /* It's a number */
      return val;
   }
   else
   {
      /* Try to get as a THING factor value. */
      Factor fac(MOD_TYPE_THING, against);
      val = owner->getBonusValue(fac);
      if(val != THING_UNKNOWN_VALUE)
      {
         /* Got it. */
         return val;
      }

      /* Not a Thing value, must be an skill or attribute*/
      fac = Factor(MOD_TYPE_SKILL, against);
      val = owner->getBonusValue(fac);
      if(val != THING_UNKNOWN_VALUE)
      {
         /* Got it. */
         return val;
      }
   }

   /* No value got! */
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
      "Warning: Unknwow against factor '%s' at dialog!", against.c_str());

   return THING_UNKNOWN_VALUE;
}

/*************************************************************************
 *                                 doTest                                *
 *************************************************************************/
bool TalkTest::doTest(PlayableCharacter* pc, Thing* owner)
{
   int value = 0;

   /* Verify if we have a pc to check */
   if(!pc)
   {
      return false;
   }

   /* true*/
   if(type == TALK_TEST_TRUE)
   {
      /* Always return true */
      return true;
   }
   else if(type == TALK_TEST_ROLL)
   {
      /* Get the difficulty value */
      value = getAgainstValue(owner);

      /* Roll the Thing! */
      return pc->doCheck(test, value);
   }
   
   /* Have Item test */
   else if(type == TALK_TEST_HAVE_ITEM)
   {
      return pc->getInventory()->getItemByFilename(test) != NULL;  
   }

   /* Have Item with info test */
   else if(type == TALK_TEST_HAVE_ITEM_WITH_INFO)
   {
      return pc->getInventory()->getItemByInfo(test) != NULL;
   }

   /* Align Not Test */
   else if(type == TALK_TEST_ALIGNMENT_NOT)
   {
      return !pc->isAlignOf(test);
   }

   /* Align Test */
   else if(type == TALK_TEST_ALIGNMENT)
   {
      return pc->isAlignOf(test);
   }

   /* All Alive */
   else if(type == TALK_TEST_ALL_ALIVE)
   {
      return ModState::isAllCharactersAlive(test);
   }

   /* All Dead */
   else if(type == TALK_TEST_ALL_DEAD)
   {
      return ModState::isAllCharactersDead(test);
   }

   /* Have money */
   else if(type == TALK_TEST_HAVE_MONEY)
   {
      int val = 0;
      sscanf(test.c_str(), "%d", &val);
      return pc->getInventory()->getMoneyQuantity() >= val;
   }

//TODO: Missions
#if 0
   /* Mission active */
   else if(id == TALK_TEST_MISSION_ACTIVE)
   {
      /* Verify if mission is at current state */
      missionsController missions;
      return missions.getCurrentMission(test) != NULL;
   }
#endif

   /* Compare-like tests (greater, equal, etc) */
   else
   {
      /* First the the compValue */
      int compValue = 0;

      /* Is comparing an owner state */
      if(test == TK_CONST_OBJECT_STATE)
      {
         compValue = owner->getState();
      }

      /* Must be comparing the active Character skill or attribute */
      else
      {

         /* Get the skill or attribute to compare */
         Skill* sk = pc->getSkills()->getSkillByString(test);

         if(!sk)
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                "Warning: Unknow Attribute or Skill '%s' to test at dialog!",
                test.c_str());

            return false;
         }

         compValue = sk->getPoints();
      }

      /* Get the value to compare with */
      value = getAgainstValue(owner);

      /* Now, finally do the test */

      /* greater */
      if(type == TALK_TEST_GREATER)
      {
         return (compValue > value);
      }
      /* lesser */
      else if(type == TALK_TEST_LESSER)
      {
         return (compValue < value);
      }
      /* equal */
      else if(type == TALK_TEST_EQUAL)
      {
         return (compValue == value);
      }
      /* diff */
      else if(type == TALK_TEST_DIFFERENT)
      {
         return (compValue != value);
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             DialogOption                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
DialogOption::DialogOption()
{
   text = "";
   postTest = NULL;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
DialogOption::~DialogOption()
{
   if(postTest)
   {
      delete postTest;
   }
}

/*************************************************************************
 *                                setText                                *
 *************************************************************************/
void DialogOption::setText(Kobold::String txt)
{
   this->text = txt;
}

/*************************************************************************
 *                              addPreTest                               *
 *************************************************************************/
void DialogOption::addPreTest(Kobold::String token, Kobold::String test, 
      Kobold::String against)
{
   TalkTest* preTest = new TalkTest(token, test, against);
   preTests.insertAtEnd(preTest);
}

/*************************************************************************
 *                             setPostTest                               *
 *************************************************************************/
void DialogOption::setPostTest(Kobold::String token, Kobold::String test, 
      Kobold::String against)
{
   if(postTest)
   {
      delete postTest;
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Warning: redefining post-test for dialog to '%s' '%s' '%s'!",
         token.c_str(), test.c_str(), against.c_str());
   }
   postTest = new TalkTest(token, test, against);
}

/*************************************************************************
 *                           createIfAction                              *
 *************************************************************************/
TalkAction* DialogOption::createIfAction(TalkActionType type)
{
   TalkAction* act = new TalkAction(type);
   ifActions.insertAtEnd(act);

   return act;
}

/*************************************************************************
 *                          createElseAction                             *
 *************************************************************************/
TalkAction* DialogOption::createElseAction(TalkActionType type)
{
   TalkAction* act = new TalkAction(type);
   elseActions.insertAtEnd(act);

   return act;
}

/*************************************************************************
 *                             doPostTest                                *
 *************************************************************************/
bool DialogOption::doPostTest(PlayableCharacter* pc, Thing* owner)
{
   if(!postTest)
   {
      /* If without post test, always passed */
      return true;
   }

   /* Just do it */
   return postTest->doTest(pc, owner);
}

/*************************************************************************
 *                         executeIfActions                              *
 *************************************************************************/
void DialogOption::executeIfActions(Conversation* conv, PlayableCharacter* pc, 
      Thing* owner, Kobold::String ownerMap)
{
   TalkAction* act = static_cast<TalkAction*>(ifActions.getFirst());
   for(int i = 0; i < ifActions.getTotal(); i++)
   {
      act->execute(conv, pc, owner, ownerMap);
      act = static_cast<TalkAction*>(act->getNext());
   }
}

/*************************************************************************
 *                        executeElseActions                             *
 *************************************************************************/
void DialogOption::executeElseActions(Conversation* conv, 
      PlayableCharacter* pc, Thing* owner, Kobold::String ownerMap)
{
   TalkAction* act = static_cast<TalkAction*>(elseActions.getFirst());
   for(int i = 0; i < elseActions.getTotal(); i++)
   {
      act->execute(conv, pc, owner, ownerMap);
      act = static_cast<TalkAction*>(act->getNext());
   }
}


///////////////////////////////////////////////////////////////////////////
//                                                                       //
//                                Dialog                                 //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
Dialog::Dialog(int id)
{
   this->id = id;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
Dialog::~Dialog()
{
}

/*************************************************************************
 *                                 getId                                 *
 *************************************************************************/     
int Dialog::getId()
{
   return id;
}

/*************************************************************************
 *                             setOwnerText                              *
 *************************************************************************/
void Dialog::setOwnerText(Kobold::String txt)
{
   this->ownerText = txt;
}

/*************************************************************************
 *                             getOwnerText                              *
 *************************************************************************/
Kobold::String Dialog::getOwnerText()
{
   return this->ownerText;
}

/*************************************************************************
 *                              getOptions                               *
 *************************************************************************/
Kobold::List* Dialog::getOptions()
{
   return &this->options;
}

/*************************************************************************
 *                               getOption                               *
 *************************************************************************/
DialogOption* Dialog::getOption(int option)
{
   DialogOption* opt = static_cast<DialogOption*>(options.getFirst());
   for(int i = 0; i < options.getTotal(); i++)
   {
      if(i == option)
      {
         return opt;
      }

      opt = static_cast<DialogOption*>(opt->getNext());
   }

   return NULL;
}

/*************************************************************************
 *                            createOption                               *
 *************************************************************************/
DialogOption* Dialog::createOption()
{
   DialogOption* opt = new DialogOption();
   this->options.insertAtEnd(opt);

   return opt;
}

//////////////////////////////////////////////////////////////////////////
//                                                                       //
//                             Conversation                              //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *                              Constructor                              *
 *************************************************************************/
Conversation::Conversation()
{
   currentPC = NULL;
   owner = NULL;
   ownerMap = "";
   current = -1;
   initialDialog = 0;
}

/*************************************************************************
 *                               Destructor                              *
 *************************************************************************/
Conversation::~Conversation()
{
}

/*************************************************************************
 *                              getString                                *
 *************************************************************************/
Kobold::String Conversation::getString(int& initialPosition, 
      Kobold::String buffer, char& separator)
{
   int i = initialPosition;
   Kobold::String ret = "";
   bool endGet = false;
   bool considerSpace = false;
   bool gnuGet = false;

   /* Remove spaces at begining */
   while( ( i < (int)buffer.length() ) && ( buffer[i] == ' ' ) )
   {
      i++;
   }

   /* Verify the gettext */
   if( (buffer.length() > 10) && (buffer[i] == 'g') &&
       (buffer[i+1] == 'e') && (buffer[i+2] == 't') && 
       (buffer[i+3] == 't') && (buffer[i+4] == 'e') &&
       (buffer[i+5] == 'x') && (buffer[i+6] == 't') )
   {
      gnuGet = true;
   }


   while(!endGet)
   {
      if( (i >= (int)buffer.length()-1) )
      {
         endGet = true;
         separator = '\0';
      }

      if(gnuGet)
      {
         /* It's an gettext("STRING") */

         /* Verify the begining */
         if(buffer[i] == '"')
         {
            considerSpace = !considerSpace;
         }

         /* Verify the end */
         if( (buffer[i] == ')') && (!considerSpace))
         {
            endGet = true;
            separator = buffer[i];
         }
         ret += buffer[i];
      }
      else
      {

         if( (!considerSpace) && 
             ( (buffer[i] == '\0') || (buffer[i] == '\n') || 
               (buffer[i] == '(') || (buffer[i] == ')') || 
               (buffer[i] == ',') || (buffer[i] == '=') ||
	   		   (buffer[i] == 13) ) )
         {
            separator = buffer[i];
            endGet = true;
         }
         else if(buffer[i] == '"')
         {
            /* Mark the init/end of the "Kobold::String" */
            considerSpace = !considerSpace;
         }
         else if( ( ( buffer[i] != ' ') || considerSpace) && 
                  (buffer[i] != '"') )
         {
            ret += buffer[i];
         }
      }
      i++;
   }
   initialPosition = (i);

   return ret;
}

/*************************************************************************
 *                             printError                                *
 *************************************************************************/
void Conversation::printError(Kobold::String fileName, Kobold::String error, 
      int lineNumber)
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "Error '%s' on conversation '%s' at line '%d' ",
         error.c_str(), fileName.c_str(), lineNumber);
}

/*************************************************************************
 *                            getActionType                              *
 *************************************************************************/
TalkActionType Conversation::getActionType(Kobold::String token, 
      Kobold::String fileName, int line)
{
   
   if(token == TK_ACTION_GO_TO_DIALOG)
   {
     return TALK_ACTION_GO_TO_DIALOG;
   }
   else if(token == TK_ACTION_INIT_FIGHT)
   {
      return TALK_ACTION_INIT_FIGHT;
   }
   else if(token == TK_ACTION_FINISH_DIALOG)
   {
      return TALK_ACTION_FINISH_DIALOG;
   }
   else if(token == TK_ACTION_DIALOG_INIT)
   {
      return TALK_ACTION_DIALOG_INIT;
   }
   else if(token == TK_ACTION_CALL_SCRIPT)
   {
      return TALK_ACTION_CALL_SCRIPT;
   }
   else if(token == TK_ACTION_ADD_MISSION)
   {
      return TALK_ACTION_ADD_MISSION;
   }
   else if(token == TK_ACTION_COMPLETE_MISSION)
   {
      return TALK_ACTION_COMPLETE_MISSION;
   }
   else if(token == TK_ACTION_GIVE_ITEM)
   {
      return TALK_ACTION_GIVE_ITEM;
   }
   else if(token == TK_ACTION_RECEIVE_ITEM)
   {
      return TALK_ACTION_RECEIVE_ITEM;
   }
   else if(token == TK_ACTION_RECEIVE_MONEY)
   {
      return TALK_ACTION_RECEIVE_MONEY;
   }
   else if(token == TK_ACTION_GIVE_MONEY)
   {
      return TALK_ACTION_GIVE_MONEY;
   }
   else if(token == TK_ACTION_CHANGE_OBJECT_STATE)
   {
      return TALK_ACTION_CHANGE_OBJECT_STATE;
   }
   else if(token == TK_ACTION_RECEIVE_XP)
   {
      return TALK_ACTION_RECEIVE_XP;
   }
   else if(token == TK_ACTION_KILL_ALL)
   {
      return TALK_ACTION_KILL_ALL;
   }
   else if(token == TK_ACTION_MAP_TRAVEL)
   {
      return TALK_ACTION_MAP_TRAVEL;
   }
   else if(token == TK_ACTION_OPEN)
   {
      return TALK_ACTION_OPEN;
   }
   else if(token == TK_ACTION_PLAY_SOUND)
   {
      return TALK_ACTION_PLAY_SOUND;
   }

   assert(false);
   printError(fileName, "Unknow action!", line);
   return TALK_ACTION_GO_TO_DIALOG;
}

/*************************************************************************
 *                                load                                   *
 *************************************************************************/
bool Conversation::load(Kobold::String filename)
{
   Kobold::String buffer;
   Kobold::String stmp;
   Dialog* dlg = NULL;
   int position = 0;
   Kobold::String token;
   char separator;
   bool endDialog = true;
   int line = 0;
   DialogOption* option = NULL;

   bool npcBegin = false;
   bool pcBegin = false;

   /* Let's try to open file */
   Ogre::DataStreamPtr file;
   try
   {
      file = Ogre::ResourceGroupManager::getSingleton().openResource(filename,
            "dialogs");
   }
   catch(Ogre::FileNotFoundException)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "conversation: Couldn't open inner file: '%s'", filename.c_str());
      return false;
   }

   /* Let's read all file */
   while(!file->eof())
   {
      /* Get and increment line counter */
      buffer = file->getLine();
      line++;

      position = 0;
      separator = '\0';

      /* Get the first token */
      token = getString(position, buffer, separator);

      /* Ignore comentaries and empty lines */
      if( (token[0] != '#') && (!token.empty()) )
      {

         /* Create New Dialog */
         if(token == TK_DIALOG)
         {
            if(!endDialog)
            {
               printError(filename, 
                     "Tried to define Dialog before end last one", line);
            }
            else
            {
               endDialog = false;
               token = getString(position, buffer, separator);
               dlg = createDialog(atoi(token.c_str()));
            }
         }
         /* End defined Dialog */
         else if(token == TK_END_DIALOG)
         {
            if( (endDialog) || (dlg == NULL))
            {
               printError(filename, "Tried to end an undefined Dialog", line);
            }

            endDialog = true;
            dlg = NULL;
         }
         /* npc */
         else if(token == TK_NPC_BEGIN)
         {
            if(npcBegin || pcBegin)
            {
               printError(filename, "Tried to begin NPC before end something",
                     line);
            }
            npcBegin = true;
         }
         else if(token == TK_NPC_END)
         {
            if(!npcBegin)
            {
               printError(filename, "Tried to end NPC before begin it", line);
            }
            npcBegin = false;
         }
         /* pc */
         else if(token == TK_PC_BEGIN)
         {
            if(npcBegin || pcBegin)
            {
               printError(filename, "Tried to begin PC before end something", 
                     line);
            }
            pcBegin = true;
            option = NULL;
         }
         else if(token == TK_PC_END)
         {
            if(!pcBegin)
            {
               printError(filename, "Tried to end PC before begin it", line);
            }
            pcBegin = false;
         }
         /* Text */
         else if(token == TK_TEXT)
         {
            if(npcBegin)
            {
               dlg->setOwnerText(translateDataString(getString(position,
                           buffer, separator)));
            }
            else if(pcBegin)
            {
               if(option)
               {
                  option->setText(translateDataString(
                           getString(position, buffer, separator)));
               }
               else
               {
                  printError(filename, "Option Text without option", line);
               }
            }
            else
            {
               printError(filename, "Text without any relation", line);
            }
         }
         /* Option */
         else if(token == TK_OPTION)
         {
            if(pcBegin)
            {
               if(dlg)
               {
                  option = dlg->createOption();
               }
               else
               {
                  printError(filename, "Option without Dialog", line);
               }
            }
            else
            {
               printError(filename, "Option without begin PC", line);
            }
         }
         /* PreTest and PostTest */
         else if((token == TK_PRE_TEST) || (token == TK_POST_TEST))
         {
            if(pcBegin)
            {
               if(option)
               {
                  /* Get function(param,param) */
                  Kobold::String func = getString(position, buffer, separator); 
                  Kobold::String test = getString(position, buffer, separator);
                  Kobold::String against = getString(position, buffer, 
                        separator);

                  bool res = true;

                  /* Assign it to its test */
                  if(token == TK_PRE_TEST)
                  {
                     /* Pre-Test */
                     option->addPreTest(func, test, against);
                  }
                  else
                  {
                     /* Post-Test */
                     option->setPostTest(func, test, against);
                  }

                  /* Verify test function parser error */
                  if(!res)
                  {
                     printError(filename, "Unknow Test Function!", line);
                  }
               }
               else
               {
                  printError(filename, "Test without option!",line);
               }
            }
            else
            {
               printError(filename, "Option Test Without PC!", line);
            }
         }
         /* Action */
         else if( (token == TK_ACTION) || (token == TK_IF_ACTION) ||
               (token == TK_ELSE_ACTION) )
         {
            if(npcBegin)
            {
               printError(filename, "Action isn't yet defined to NPC!", line);
            }
            else if(pcBegin)
            {
               if(option)
               {
                  TalkAction* tact = NULL;
                  
                  /* Get The action and its parameters */
                  TalkActionType actType = getActionType(
                        getString(position, buffer, separator), filename, line);
                  
                  /* Add the Talk action (as if or else one) */
                  if(token == TK_ELSE_ACTION)
                  {
                     tact = option->createElseAction(actType);
                  }
                  else
                  {
                     tact = option->createIfAction(actType);
                  }

                  /* Parse Action Parameters */
                  if( (actType == TALK_ACTION_GO_TO_DIALOG) ||
                      (actType == TALK_ACTION_DIALOG_INIT) ||
                      (actType == TALK_ACTION_RECEIVE_MONEY) ||
                      (actType == TALK_ACTION_GIVE_MONEY) ||
                      (actType == TALK_ACTION_CHANGE_OBJECT_STATE) ||
                      (actType == TALK_ACTION_RECEIVE_XP))
                  {
                     //get number
                     token = getString(position, buffer, separator);
                     tact->setAttribute(atoi(token.c_str()));
                  }
                  else if( (actType == TALK_ACTION_ADD_MISSION) ||
                           (actType == TALK_ACTION_COMPLETE_MISSION))
                  {
                     //get mission
                     token = getString(position, buffer, separator);
                     tact->setStringFactor(token);

                     //get xp, if needed
                     if(actType == TALK_ACTION_COMPLETE_MISSION)
                     {
                        token = getString(position, buffer, separator);
                        tact->setAttribute(atoi(token.c_str()));

                        //and get completion type
                        token = getString(position, buffer, separator);
                        tact->setQuantity(atoi(token.c_str()));
                     }
                  }
                  else if( (actType == TALK_ACTION_GIVE_ITEM) ||
                           (actType == TALK_ACTION_RECEIVE_ITEM) ||
                           (actType == TALK_ACTION_KILL_ALL) ||
                           (actType == TALK_ACTION_MAP_TRAVEL) ||
                           (actType == TALK_ACTION_CALL_SCRIPT) ||
                           (actType == TALK_ACTION_PLAY_SOUND) )
                  {
                     //get name
                     token = getString(position, buffer, separator);
                     tact->setStringFactor(token);
                  }
               }
               else
               {
                  if(option < 0)
                  {
                     printError(filename, "Option Action without option!", 
                           line);
                  }
                  else
                  {
                     printError(filename, "Options overflow!", line);
                  }
               }
            }
            else
            {
               printError(filename, "Action without any relation!\n", line);
            }
         }
         /* Unkown! */
         else
         {
            printError(filename, "Unknow Token!", line);
         }
      }
   }

   file->close();

   return true;
}

/*************************************************************************
 *                               getDialog                               *
 *************************************************************************/
Dialog* Conversation::getDialog(int id)
{
   int i;
   Dialog* dlg = static_cast<Dialog*>(getFirst());

   for(i = 0; i < getTotal(); i++)
   {
      if(dlg->getId() == id)
      {
         return dlg;
      }
      dlg = static_cast<Dialog*>(dlg->getNext());
   }

   return NULL;
}

/*************************************************************************
 *                             insertDialog                              *
 *************************************************************************/
Dialog* Conversation::createDialog(int id)
{
   /* Create the Dialog */
   Dialog* dlg = new Dialog(id);

   /* Insert it on the list */
   insertAtEnd(dlg);

   return dlg;
}

/*************************************************************************
 *                           setInitialDialog                            *
 *************************************************************************/
void Conversation::setInitialDialog(int numDialog)
{
   /* Set the initial Dialog */
   initialDialog = numDialog;

   /* Tell the modeState */
   if(owner)
   {
      if(owner->getThingType() == Thing::THING_TYPE_CHARACTER)
      {
         Character* ownerNPC = static_cast<Character*>(owner);
         ModState::addMapTalkAction(MODSTATE_TALK_ENTER_VALUE, 
               ownerNPC->getFilename(), ownerMap, numDialog);
      }
      //TODO: allowing set initial talk for objects too!
   }
}

/*************************************************************************
 *                               setOwner                                *
 *************************************************************************/
void Conversation::setOwner(Thing* o, Kobold::String mapFile)
{
   owner = o;
   ownerMap = mapFile;
}

/*************************************************************************
 *                                 setPC                                 *
 *************************************************************************/
void Conversation::setPlayableCharacter(PlayableCharacter* PC)
{
   currentPC = PC;
}

/*************************************************************************
 *                                getOwner                               *
 *************************************************************************/
Thing* Conversation::getOwner()
{
   return owner;
}

/*************************************************************************
 *                                 getPC                                 *
 *************************************************************************/
PlayableCharacter* Conversation::getPlayableCharacter()
{
   return currentPC;
}

/*************************************************************************
 *                            proccessAction                             *
 *************************************************************************/
void Conversation::proccessAction(int option)
{
   /* Get Dialog on list */
   int numDialog = current;
   Dialog* dlg = getDialog(numDialog);
   if(dlg == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: dialog with id '%d' not found!", numDialog);
      return;
   }

   /* Get Option */
   DialogOption* opt = dlg->getOption(option);
   if(!opt)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: dialog option with index '%d' not found!", option);
      return;
   }

   /* Do the post check to resolve wich actions take (if or else actions) */
   if(opt->doPostTest(currentPC, owner))
   {
      /* Passed test, so if action */
      opt->executeIfActions(this, currentPC, owner, ownerMap);
   }
   else
   {
      /* Failed test, so else action */
      opt->executeElseActions(this, currentPC, owner, ownerMap);
   }
}

/*************************************************************************
 *                            changeDialog                               *
 *************************************************************************/
void Conversation::changeDialog()
{
   current = -1;
   changeDialog(initialDialog);
}

/*************************************************************************
 *                            changeDialog                               *
 *************************************************************************/
void Conversation::changeDialog(int numDialog)
{
   Kobold::String text;

   Dialog* dlg;
   
   if( (numDialog == current) || (!DialogWindow::isOpened()) )
   {
      /* No change at the same Dialog! */
      return;
   }

   /* Get the Dialog pointer */
   dlg = getDialog(numDialog);
   if(dlg == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Warning: couldn't find dialog '%d'", numDialog);
      return;
   }

   /* Define the current Dialog number */
   current = numDialog;

   /* Define the NPC Text */
   DialogWindow::ownerText->setText(dlg->getOwnerText());

   /* Define the options */
   int curOpt = 0;
   DialogWindow::pcOptions->clearOptions();
   Kobold::List* options = dlg->getOptions();
   DialogOption* opt = static_cast<DialogOption*>(options->getFirst()); 
   for(int i = 0; i < options->getTotal(); i++)
   {
      /* Only insert the option if isn't empty */
      if(!opt->text.empty())
      {
         bool res = true;

         /* And passes all preTests */
         TalkTest* preTest = static_cast<TalkTest*>(opt->preTests.getFirst());
         for(int j = 0; j < opt->preTests.getTotal(); j++)
         {
            res &= preTest->doTest(currentPC, owner);
            preTest = static_cast<TalkTest*>(preTest->getNext());
         }

         /* So, if pass all, add the option */
         if(res)
         {
            text = "";
            if(opt->postTest)
            {
               text += opt->postTest->getTestName(currentPC);
            }

            /* Add the text*/
            text += opt->text;

            DialogWindow::pcOptions->addOption(text, i);
            curOpt++;
         }
      }
      opt = static_cast<DialogOption*>(opt->getNext());
   }
}

