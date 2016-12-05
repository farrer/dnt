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

#ifndef _dnt_modstate_h
#define _dnt_modstate_h

#include "dntconfig.h"

#include <kobold/list.h>
#include <kobold/kstring.h>
#include <kobold/defparser.h>

#include <OGRE/OgreVector3.h>

#include <iostream>

namespace DNT
{

enum ModActionType
{
   MODSTATE_ACTION_UNKNOWN = -1,
   MODSTATE_ACTION_OBJECT_REMOVE,      /**< Removed object from map */
   MODSTATE_ACTION_OBJECT_ADD,             /**< Added object to the map */
   MODSTATE_ACTION_OBJECT_CHANGE_STATE,    /**< Changed Object State */
   MODSTATE_ACTION_CHARACTER_DEAD,         /**< Killed Character on map */
   MODSTATE_ACTION_CHARACTER_MOVE,         /**< Character moved to positon */
   MODSTATE_TALK_ENTER_VALUE,              /*< New enter value for a talk */
   MODSTATE_INVENTORY,                     /**< Inventory "Snapshot" */
   MODSTATE_ACTION_CHARACTER_CHANGE_STATE  /**< Character Change State */
};

/*! The Generic Modification Action Class */
class ModAction: public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param tgt -> actor target filename
       * \param mapFile -> name of the map file where action happened
       * \param pos -> position where action happened */
      ModAction(ModActionType act, Kobold::String tgt, Kobold::String mapFile,
                Ogre::Vector3 pos);

      /*! Destructor */
      virtual ~ModAction();

      /*! Set current action target 
       * \param target target's filename */
      void setTarget(Kobold::String target);
      /*! Get the target of the action
       * \return pointer to object of the action */
      Kobold::String getTarget();

      /*! Get the action type
       * \return action type*/
      ModActionType getAction();
      /*! Set action type */
      void setAction(ModActionType action);

      /*! Get map filename
       * \return name of the map file */
      Kobold::String getMapFilename();

      /*! \return action's position. */
      Ogre::Vector3 getPosition();

      /*! Set the position where action happened
       * \param pos -> position */
      void setPosition(Ogre::Vector3 pos);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      virtual Kobold::String toString() = 0;
      /*! Define the action from a string (usually to load) 
       * \param s -> string to load from */
      virtual void fromString(Kobold::String s) = 0;

   private:

      Kobold::String mapFilename;  /**< map's filename where action happened */
      ModActionType action;       /**< type of the action */
      Ogre::Vector3 pos;    /**< Position where action hapenned */
      Kobold::String target;       /**< target of the action (filename) */
};

/*! A Modification Action made to (or by) a Character on map */
class MapCharacterModAction : public ModAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param character -> character filename
       * \param mapFile -> name of the map file where action occurs 
       * \param pos -> current character position
       * \param orientation -> current character orientation angle
       * \param initial -> initial position when loaded to the map */
      MapCharacterModAction(ModActionType act, Kobold::String character, 
            Kobold::String mapFile, Ogre::Vector3 pos, Ogre::Real orientation,
            Ogre::Vector3 initial);

      /*! Constructor from string
       * \param s -> string with info to load */
      MapCharacterModAction(Kobold::String s);

      /*! Destructor */                      
      ~MapCharacterModAction();

      /*! Get the initial position of the Character
       * \return inital position of the Character */
      Ogre::Vector3 getInitialPosition();

      /*! Get the orientation of the Character at action's instant
       * \return Character orientation */
      Ogre::Real getOrientation();

      /*! Set the orientation of the Character at action's instant
       * \param o -> Character orientation */
      void setOrientation(Ogre::Real o);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      Kobold::String toString();

      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(Kobold::String s);

   private:

      Ogre::Vector3 initialPos; /**< initial Position on Map */
      Ogre::Real oriAngle; /**< Orientation Angle */
};

/*! A Modification Action made to objects in map */
class MapObjectModAction: public ModAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param obj -> object filename
       * \param mapFile - name of the map file where action occurs 
       * \param pos -> object's position */
      MapObjectModAction(ModActionType act, Kobold::String obj, 
            Kobold::String mapFile, Ogre::Vector3 pos);

      /*! Constructor from string
       * \param s -> string with info to load */
      MapObjectModAction(Kobold::String s);

      /*! Destructor */
      ~MapObjectModAction();

      /*! Get the action value
       * \return -> action value */
      int getValue();

      /*! Set the action value
       * \param v -> new action value */
      void setValue(int v);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      Kobold::String toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(Kobold::String s);

   private:
       int value;   /**< Some Value Information (usually related to state)  */

};

/*! A modification to a talk on map */
class MapTalkModAction: public ModAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param tgt -> actor target filename
       * \param mapFile - name of the map file where action occurs 
       * \param talkValue -> value position */
      MapTalkModAction(ModActionType act, Kobold::String tgt, 
            Kobold::String mapFile, int talkValue);

      /*! Constructor from string
       * \param s -> string with info to load */
      MapTalkModAction(Kobold::String s);

      /*! Destructor */
      ~MapTalkModAction();

      /*! Get the talk action value
       * \return -> talk action value */
      int getValue();

      /*! Set the talk action value
       * \param v -> new talk action value */
      void setValue(int v);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      Kobold::String toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(Kobold::String s);

   private:
       int value;   /**< Some talk Value Information */
};


/*! The modInventory class is an Inventory state keeper,
 * basically used for populate inventories when back at some map.
 * \note -> basically, a modInventory is created (or modified, if 
 *          already exists) for each NPC when the PC exits the map.
 *          When the PC is back, each NPC's inventory is then populated
 *          by its previously modInventory. */
class ModInventory: public ModAction, public Kobold::List
{
   public:
      /*! Constructor
       * \param inv -> Inventory to keep status
       * \param owner -> Inventory owner
       * \param mapFile -> mapFilename where the owner is */
      ModInventory(Inventory* inv, Kobold::String owner, 
            Kobold::String mapFile);

      /*! Destructor */
      ~ModInventory();

      /*! Flush the objects list to the Inventory,
       * loading needed models to the list
       * \param inv -> Inventory to populate */ 
      void flush(Inventory* inv);

      /*! Create the modInventory list based on an Inventory
       * \param inv -> Inventory to create the object list
       * \note -> this function will clear the previous created list */
      void create(Inventory* inv);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      Kobold::String toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(Kobold::String s);

      /*! Save the modInventory to the file
       * \param file -> the file to save */
      void save(std::ofstream& file);

      /*! Load the modInventory from a file
       * \param file -> filename to load
       * \return -> true if ok  */
      bool load(Kobold::String file);

      /*! Insert a item from a string representing the modInvObj
       * \param s -> string representing modInvObj */
      void insert(Kobold::String s);

   private:
         /*! The modInvObj is just a structure to keep Inventory objects
          * status. It is only used for modInventory. */
         class ModInvObj: public Kobold::ListElement
         {
            public:
               ModInvObj(Kobold::String filename, int x, int y, 
                     int invNumber, int state);
               ModInvObj();
               ~ModInvObj();

               Kobold::String filename; /**< The Object Filename */
               int x,                /**< X position on Inventory */
                   y;                /**< Y position on Inventory */
               int invNumber;        /**< The Inventory number */
               int state;            /**< The current object state */
         };
};

/*! ModMap class has all modifications done at an specific map */
class ModMap: public Kobold::ListElement
{
   public:
      /*! Constructor 
       * \param filename -> map file name */
      ModMap(Kobold::String filename);
      /*! Destructor */
      ~ModMap();

      /*! Save the modMap to the file
       * \param file -> the file to save */
      void save(std::ofstream& file);

      /*! Load the modMap from the definitions parser
       * \param def -> pointer to the DefParser with info for the modMap */
      void load(Kobold::DefParser* def);
      
      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> filename of the object
       * \param mapFilename - name of the map file where action happened
       * \param pos -> position where happened
       * \param value -> some value related to the action */
      void addMapObjectAction(ModActionType action, Kobold::String target, 
            Kobold::String mapFilename, Ogre::Vector3 pos, int value);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> Character filename
       * \param mapFile -> name of the map file where action happened
       * \param pos -> posiiton where action happened
       * \param orientation -> Character orientation angle
       * \param initial -> initial  position when loaded the map */
      void addMapCharacterAction(ModActionType act, Kobold::String character,
            Kobold::String mapFile, Ogre::Vector3 pos,
            Ogre::Real orientation, Ogre::Vector3 initial);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> Character filename
       * \param mapFile -> filename of the map
       * \param value -> the action talk value */
      void addMapTalkAction(ModActionType act, Kobold::String character, 
            Kobold::String mapFile, int value);

      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the Inventory to update
       *  \param owner -> Inventory owner filename */ 
      void addMapInventory(Inventory* inv, Kobold::String owner);

      /*! Do all saved - and related - modifications to current loaded map.
       * (usually when you return to the map and want it to appear exactly 
       * like when you left it). */ 
      void doMapModifications();

      /*! Clear All the modifications states (usually called after death) */
      void clear();      

      /*! Get the mapFilename */
      Kobold::String getMapFilename();

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are dead. 
       * \return -> true if all are dead*/
      bool isAllCharactersDead(Kobold::String npcFilename);

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are alive 
       * \return -> true if all are alive */
      bool isAllCharactersAlive(Kobold::String npcFilename);

   private:

      /*! Remove Action from the list
       * \param act -> action to remove */
      void removeAction(ModAction* act);

      /*! Add Action to the list
       * \param act -> pointer to the action to add */
      void addAction(ModAction* act);

      /*! remove inverse action, if it exists in list. 
       * \param action -> action type
       * \param target -> pointer to object
       * \param mapFilename - name of the map file where action happened
       * \param pos -> position where action happened 
       * \return true if the inverse action is found and removed. */
      bool removeInverseObjectAction(ModActionType action, 
            Kobold::String target, Kobold::String mapFilename, 
            Ogre::Vector3 pos); 

      /*! Search for a ModAction on the list
       * \param action -> action number constant
       * \param target -> target name
       * \return -> pointer to the ModAction found, or NULL if none */
      ModAction* search(ModActionType action, Kobold::String target, 
            Ogre::Vector3 pos = Ogre::Vector3(-1, -1, -1));

      Kobold::String mapFilename;     /**< The map file name */
      Kobold::List modList;       /**< List of map modification actions */
};

/*! All modifications made by users on game are armazened here. It is also
 * the class to load/save states. Static class. */
class ModState
{
   public:
      /*! Constructor */
      static void init();
      /*! Destructor */
      static void finish();

      /*! Load File of state 
       * \param file -> filename to load */
      static bool loadState(Kobold::String file);

      /*! Save File of State 
       * \param file -> filename to save */
      static bool saveState(Kobold::String file);

      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> filename of the object
       * \param mapFilename - name of the map file where action happened
       * \param pos -> position where action happened
       * \param value -> some value related to the action */
      static void addMapObjectAction(ModActionType action, 
            Kobold::String target, Kobold::String mapFilename, 
            Ogre::Vector3 pos, int value = 0);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> Character filename
       * \param mapFile -> name of the map file where action happened
       * \param pos -> position where happened
       * \param orientation -> Character orientation angle
       * \param initial -> initial position when loaded the map */
      static void addMapCharacterAction(ModActionType act, 
            Kobold::String character, Kobold::String mapFile,
            Ogre::Vector3 pos, Ogre::Real orientation,
            Ogre::Vector3 initial);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> Character filename
       * \param mapFile -> filename of the map
       * \param value -> the action talk value */
      static void addMapTalkAction(ModActionType act, Kobold::String character,
            Kobold::String mapFile, int value);
      
      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the Inventory to update
       *  \param owner -> Inventory owner filename 
       *  \param mapFile -> map filename where the Character is */
      static void addMapInventory(Inventory* inv, Kobold::String owner,
            Kobold::String mapFile);

      /*! Do all saved - related - modifications to the current loadede map. */
      static void doMapModifications();

      /*! Clear All the modifications states (usually called after death) */
      static void clear();

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are dead. 
       * \return -> true if all are dead*/
      static bool isAllCharactersDead(Kobold::String npcFilename);

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are alive 
       * \return -> true if all are alive */
      static bool isAllCharactersAlive(Kobold::String npcFilename);

      /*! Verify if the modState have informations for the map
       * \param filename -> map filename to verify
       * \return -> true if have info, false if not */
      static bool haveMap(Kobold::String filename);

   private:

      /*! Create the ModMap related to the filename 
       * \param filename -> map file name
       * \return -> pointer to the created modMap */
      static ModMap* createModMap(Kobold::String filename);

      /*! Find the modMap on the list related to the filename
       * \param filename -> map filename to search
       * \return -> modMap pointer if found or created
       * \note -> if not found a modMap this function create a new one */
      static ModMap* findModMap(Kobold::String filename);
      
      static Kobold::List modList;   /**< List of mod maps */
};

}

#endif

