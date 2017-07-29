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

     /*! Set action value */
     void setValue(int val);
     /*! Set action second value */
     void setValue2(int val);

     /*! Set action string value */
     void setStringValue(const Kobold::String strVal);

     /*! Set RuleDefinition id to act to. */ 
     void setRuleDefinition(RuleDefinition* ruleDef);

     /*! Execute the action */
     void execute(Conversation* conv, PlayableCharacter* pc, Thing* owner,
                  Kobold::String ownerMap);

  private:

     TalkActionType type;     /**< Talk Action type */
     int value;               /**< First Value */
     int value2;              /**< Second value */
     Kobold::String strValue; /**< String value */
     RuleDefinition* ruleDef; /**< The rule definition */
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
   TALK_TEST_HAVE_RULE_DEFINITION, /**< Test have a RuleDefinition */
   TALK_TEST_NOT_HAVE_RULE_DEFINITION, /**< Test have a RuleDefinition */
   TALK_TEST_ALL_ALIVE, /**< If all Characters are alive on a map */
   TALK_TEST_ALL_DEAD, /**< If all Characters are dead on a map */
   TALK_TEST_HAVE_MONEY, /**< If have money amount */
   TALK_TEST_MISSION_ACTIVE, /**< If some mission is active or not */
   TALK_TEST_HAVE_ITEM_WITH_INFO /**< If have item with some info */
};


/*! Definition of a test to be rolled before appearence or after selection of
 * a DialogOption. */
class TalkTest : public Kobold::ListElement
{
   public:

      /*! Constructor.
       * \param token TalkTestType defined as string
       * \param test identifier of the RuleDefinition to roll a test
       * \param against identifier of the RuleDefinition to test against,
       *        or single value. */
      TalkTest(const Kobold::String& token, const Kobold::String& test, 
               const Kobold::String& against);
      /*! Destructor */
      ~TalkTest();

      /*! Do the test against a Character.
       * \param pc -> pointer to the Character to test
       * \param owner -> the owner of the dialog (object or Character)
       * \return -> true if passed the test */
      bool doTest(PlayableCharacter* pc, Thing* owner);

      /*! Get the test name.
       * \return -> the rule definition test name */
      Kobold::String getTestName();

   protected:

      /*! Try to get get the against as value. */
      bool getAgainstValue(Thing* owner, int& value);
      /*! Try to get against as a RuleDefinitionValue */
      RuleDefinitionValue* getAgainst(Thing* owner);

      TalkTestType type;         /**< Talk test type */
      RuleDefinition* test;      /**< Definition to test */
      Kobold::String testStr;    /**< Test String */
      Kobold::String against;    /**< Definition to test against */
      int againstVal;            /**< If against is NULL will test to value */
};

/*! A single selectable option within a Dialog. The option appearence will be 
 * defined by passing all of its #preTests tests. When the option is selected,
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

      friend class Conversation;

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

