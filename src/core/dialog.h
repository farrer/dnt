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

#ifndef _dnt_core_dialog_h
#define _dnt_core_dialog_h

#include "dntconfig.h"

#include <kobold/list.h>
#include <kobold/kstring.h>

/*! \file dialog.h Define Conversations options.*/

namespace DNT
{

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


/*! The type of actions */
enum TalkActionType
{
   TALK_ACTION_GO_TO_DIALOG = 0, /**< Go To some Conversation point */
   TALK_ACTION_INIT_FIGHT, /**< End talk and initiate a fight */
   TALK_ACTION_FINISH_DIALOG, /**< End Talk */
   TALK_ACTION_MOD_PC, /**< Modify PC attribute */
   TALK_ACTION_MOD_NPC, /**< Modify NPC attribute */
   TALK_ACTION_DIALOG_INIT, /**< Set the new initial Dialog */
   TALK_ACTION_ADD_MISSION, /**< Add a mission */
   TALK_ACTION_COMPLETE_MISSION, /**< Complete Mission */
   TALK_ACTION_GIVE_ITEM, /**< PC gives an item to conversation owner.
                               \note item must be at PC's inventory. */
   TALK_ACTION_RECEIVE_MONEY, /**< PC receives some money from dialog owner */
   TALK_ACTION_CHANGE_OBJECT_STATE, /**< Change Object State */
   TALK_ACTION_RECEIVE_XP, /**< Receive XP ammount */
   TALK_ACTION_KILL_ALL, /**< Kill All NPCs from a map */
   TALK_ACTION_RECEIVE_ITEM, /**< PC receives an Item from dialog owner.
                                  \note item must be at Owner's inventory */
   TALK_ACTION_MAP_TRAVEL, /**< Travel to another map */
   TALK_ACTION_GIVE_MONEY, /**< Give money quantity */
   TALK_ACTION_CALL_SCRIPT, /**< Call a one time finish script */
   TALK_ACTION_OPEN, /**< Open Door or Object Owner */
   TALK_ACTION_PLAY_SOUND /**< Play a sound effect */
};

class Conversation;

/*! TalkAction - defines modifications on dialog attributes by a dialog
 * line selection. */
class TalkAction : public Kobold::ListElement
{
  public:

     /*! Constructor */
     TalkAction(TalkActionType type);
     /*! Destructor */
     ~TalkAction();

     /*! Set Action Attribute (meaning depends of TalkActionType)
      * \param att attribute index */
     void setAttribute(int att);

     /*! Set action quantity attribute 
      * \param qty quantity value*/
     void setQuantity(int qty);

     /*! Set String Factor to act at 
      * \param factor factor identifier */
     void setStringFactor(Kobold::String factor);

     /*! Execute the action */
     void execute(Conversation* conv, PlayableCharacter* pc, Thing* owner,
                  Kobold::String ownerMap);

  private:

     TalkActionType type;   /**< Talk Action type */
     int qty;  /**< Quantity */
     int att;  /**< Attribute to modify */
     Kobold::String satt; /**< String Attribute */
};

/*! The available tests */
enum TalkTestType
{
   TALK_TEST_TRUE = 0,  /**< Always True */
   TALK_TEST_ROLL, /**< Factor against dice roll test */
   TALK_TEST_GREATER, /**< Test if is greater */
   TALK_TEST_LESSER, /**< Test if is lesser */
   TALK_TEST_EQUAL, /**< Test if equal */
   TALK_TEST_DIFFERENT, /**< Test if different */
   TALK_TEST_HAVE_ITEM, /**< Test if have item on inventory */
   TALK_TEST_ALIGNMENT_NOT, /**< Test if alignment not of type */
   TALK_TEST_ALIGNMENT,  /**< Test if alignment is of a type */
   TALK_TEST_ALL_ALIVE, /**< If all Characters are alive on a map */
   TALK_TEST_ALL_DEAD, /**< If all Characters are dead on a map */
   TALK_TEST_HAVE_MONEY, /**< If have money amount */
   TALK_TEST_MISSION_ACTIVE, /**< If some mission is active or not */
   TALK_TEST_HAVE_ITEM_WITH_INFO /**< If have item with some info */
};


/* Definition of a test to be rolled before appearence or after selection of
 * a DialogOption. */
class TalkTest : public Kobold::ListElement
{
   public:

      /*! Constructor.
       * \param token TalkTestType defined as string
       * \param test string with skill or attribute to test with
       * \param against string with skill or attraibute to test against */
      TalkTest(Kobold::String token, Kobold::String test, 
               Kobold::String against);
      /*! Destructor */
      ~TalkTest();

      /*! Do the test against a Character.
       * \param pc -> pointer to the Character to test
       * \param owner -> the owner of the dialog (object or Character)
       * \return -> true if passed the test */
      bool doTest(PlayableCharacter* pc, Thing* owner);

      /*! Get the test name (ie. if a skill, the skill name), when doing it
       * against a Character.
       * \param pc -> pointer to the Character
       * \return -> the test name */
      Kobold::String getTestName(PlayableCharacter* pc);

   protected:

      /*! Get the against value (or skill value) */
      int getAgainstValue(Thing* owner);

      TalkTestType type;         /**< Talk test type */
      Kobold::String test;       /**< The modifier to test */
      Kobold::String against;    /**< The against modifier to test (or value) */
};

/*! A single selectable option within a Dialog. The option appearence will be 
 * defined by passing all of its #preTest tests. When the option is selected,
 * its #postTest is caled, doing its #ifActions when passed or #elseActions
 * when not passed. */
class DialogOption : public Kobold::ListElement
{
   public:

      /*! Constructor */
      DialogOption();
      /*! Destructor */
      ~DialogOption();

      /*! Set option text
       * \param txt new option text */
      void setText(Kobold::String txt);

      /*! Add a PreTest to the DialogOption with following info */
      void addPreTest(Kobold::String token, Kobold::String test, 
            Kobold::String against);

      /*! Set the post Test to the DialogOption with following info */
      void setPostTest(Kobold::String token, Kobold::String test, 
            Kobold::String against);

      /*! Create an if action.
       * \param type action's type
       * \return pointer to the created action */
      TalkAction* createIfAction(TalkActionType type);
      /*! Create an else action.
       * \param type action's type
       * \return pointer to the created action */
      TalkAction* createElseAction(TalkActionType type);

      /*! Run the postTest.
       * \note if without post tests, always return true.
       * \param pc actor
       * \param owner conversation owner
       * \return postTest result. */
      bool doPostTest(PlayableCharacter* pc, Thing* owner);

      /*! Execute all If actions */
      void executeIfActions(Conversation* conv, PlayableCharacter* pc, 
            Thing* owner, Kobold::String ownerMap);

      /*! Execute all Else actions */
      void executeElseActions(Conversation* conv, PlayableCharacter* pc, 
            Thing* owner, Kobold::String ownerMap);

   private:
      Kobold::List preTests; /**< Pre-Tests (if all passed, the option is 
                                  displayed and available to be selected) */

      Kobold::String text;    /**< The option text */

      TalkTest* postTest;   /**< The post test (if true run ifActions,
                                 if false, run elseActions) */

      Kobold::List ifActions;  /**< If postTest == true actions */
      Kobold::List elseActions; /**< If postTest == false actions */
};

/*! A Dialog is a simple Conversation state, where Dialog Owner say something 
 * and PC has some options to select.
 * \note An option could or not appear, depending on its preTest results. */
class Dialog: public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param id identificator */
      Dialog(int id);
      /*! Destructor */
      ~Dialog();

      /*! \return dialog identifier */
      int getId();

      /*! Set text to be displayed by the owner 
       * \param txt text to set */
      void setOwnerText(Kobold::String txt);
      /*! \return text to be disaplayed by the owner */
      Kobold::String getOwnerText();

      /*! \return pointer to the options list */
      Kobold::List* getOptions();

      /*! Get option by its index 
       * \param option index
       * \return pointer to option found or NULL */
      DialogOption* getOption(int option);

      /*! Create a new dialog option for this Dialog.
       * \return pointer to the option created */
      DialogOption* createOption();

   private:
      Kobold::String ownerText; /**< Owner text */
      Kobold::List options;     /**< Each selectable options to talk */
      int id;                   /**< Identificator */
};

/*! Conversation Class. A Conversation is a set of dialogs, usually for
 * a NPC Character or an object. */
class Conversation: public Kobold::List
{
   public:
      /*! Conversation Constructor */
      Conversation();
      /*! Conversation destructor */
      ~Conversation();

      /*! Load Conversation from a file.
       * \param filename -> file name to load
       * \return success flag. */
      bool load(Kobold::String filename);  

      /*! Set the owner of the Conversation
       * \param thing -> pointer to Thing owner of the conversation
       * \param mapFile -> fileName where the Thing is */
      void setOwner(Thing* thing, Kobold::String mapFile);

      /*! Set the PlayableCharacter that is currently talking with 
       * the Conversation owner.
       * \param PC -> pointer to the PlayableCharacter */
      void setPlayableCharacter(PlayableCharacter* PC);

      /*! Get the owner of the Conversation (if one)
       * \return -> pointer to the owner or NULL */
      Thing* getOwner();

      /*! Get the current PC talking to the Conversation's owner.
       * \return -> pointer to the current PlayableCharacter */
      PlayableCharacter* getPlayableCharacter();

      /*! Process a dialogaction, based on selected option.
       * \note this function is usually called after selected an option.
       * \param option -> number option selected */ 
      void proccessAction(int option);

      /*! Change the active Conversation Dialog
       * \param numDialog -> number of the new dialog to make active */
      void changeDialog(int numDialog);

      /*! Set the initial dialog (the one displayed at a click on Character)
       * \param numDialog -> new init dialog number */
      void setInitialDialog(int numDialog);

      /*! Change the active Conversation Dialog to the initial one. */
      void changeDialog();

   protected:

      /*! Create a new Dialog to the Conversation
       * \param id identificatorof the dialog
       * \return pointer to the new dialog */
      Dialog* createDialog(int id);

   protected:

      /*! Get the dialog pointer
       * \param id -> id of the dialog to get
       * \return pointer to the dialog or NULL */
      Dialog* getDialog(int id);

   private: 

      int current;          /**< Current active dialog index */
      int initialDialog;    /**< First dialog to show */

      PlayableCharacter* currentPC;  /**< PC that is talking to owner */
      Thing* owner;  /**< The owner of the Conversation */
      Kobold::String ownerMap; /**< The Map where the Owner is located */
  
      /*! Parse string value (translating when necessary) */
      Kobold::String getString(int& initialPosition, Kobold::String buffer, 
                            char& separator);

      /*! Get action type from token */
      TalkActionType getActionType(Kobold::String token, 
            Kobold::String fileName, int line);

      /*! Log an error on conversation file definition */
      void printError(Kobold::String fileName, Kobold::String error, 
            int lineNumber);
};

}

#endif

