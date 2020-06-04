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

#include "modstate.h"
#include "money.h"
#include "game.h"
#include "inventory.h"

#include "../rules/character.h"
#include "../map/npcfile.h"
#include "../map/map.h"
#include "../lang/translate.h"

#include <kobold/log.h>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace DNT;


#define MODSTATE_TOKEN_MAP                  "map"
#define MODSTATE_TOKEN_MAP_END              "mapEnd"
#define MODSTATE_TOKEN_VERSION              "version"
#define MODSTATE_TOKEN_CHARACTER_MOD_ACTION "characterModAction"
#define MODSTATE_TOKEN_OBJECT_MOD_ACTION    "objectModAction"
#define MODSTATE_TOKEN_TALK_MOD_ACTION      "talkModAction"
#define MODSTATE_TOKEN_MOD_INVENTORY        "ModInventory"
#define MODSTATE_TOKEN_INVENTORY_ITEM       "inventoryItem"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               ModAction                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
ModAction::ModAction(ModActionType act, Kobold::String obj, 
      Kobold::String mapFile, Ogre::Vector3 pos)
{
   this->action = act;
   this->target = obj;
   this->mapFilename = mapFile;
   this->pos = pos;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
ModAction::~ModAction()
{
}

/************************************************************
 *                         setTarget                        *
 ************************************************************/
void ModAction::setTarget(Kobold::String target)
{
   this->target = target;
}

/************************************************************
 *                         getTarget                        *
 ************************************************************/
Kobold::String ModAction::getTarget()
{
   return target;
}

/************************************************************
 *                      getMapFilename                      *
 ************************************************************/
Kobold::String ModAction::getMapFilename()
{
   return mapFilename;
}

/************************************************************
 *                         getAction                        *
 ************************************************************/
ModActionType ModAction::getAction()
{
   return action;
}

/************************************************************
 *                         setAction                        *
 ************************************************************/
void ModAction::setAction(ModActionType action)
{
   this->action = action;
}

/************************************************************
 *                       getPosition                        *
 ************************************************************/
Ogre::Vector3 ModAction::getPosition()
{
   return pos;
}

/************************************************************
 *                       setPosition                        *
 ************************************************************/
void ModAction::setPosition(Ogre::Vector3 pos)
{
   this->pos = pos;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                         MapCharacterModAction                          //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapCharacterModAction::MapCharacterModAction(ModActionType act, 
            Kobold::String character, Kobold::String mapFile,
            Ogre::Vector3 pos, Ogre::Real orientation, Ogre::Vector3 initial) 
      :ModAction(act, character, mapFile, pos)
{
   oriAngle = orientation;
   initialPos = initial;
}

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapCharacterModAction::MapCharacterModAction(Kobold::String s)
   :ModAction(MODSTATE_ACTION_UNKNOWN, "", "", Ogre::Vector3(0,0,0))
{
   fromString(s);
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
MapCharacterModAction::~MapCharacterModAction()
{
}

/************************************************************
 *                      getOrientation                      *
 ************************************************************/
Ogre::Real MapCharacterModAction::getOrientation()
{
   return oriAngle;
}

/************************************************************
 *                      setOrientation                      *
 ************************************************************/
void MapCharacterModAction::setOrientation(Ogre::Real o)
{
   oriAngle = o;
}

/************************************************************
 *                  getInitialPosition                       *
 ************************************************************/
Ogre::Vector3 MapCharacterModAction::getInitialPosition()
{
   return initialPos;
}

/************************************************************
 *                         toString                         *
 ************************************************************/
Kobold::String MapCharacterModAction::toString()
{
   Ogre::StringStream ss;

   /* Definition */
   ss << MODSTATE_TOKEN_CHARACTER_MOD_ACTION;
   ss << " = ";

   /* Action Type */
   ss << getAction() << " ";

   /* Target */
   ss << getTarget() << " ";

   /* Position */
   Ogre::Vector3 pos = getPosition();
   ss << pos.x << " " << pos.y << " " << pos.z << " ";

   /* Orientation and Initial Position  */
   ss << oriAngle << " " << initialPos.x << " " << initialPos.y << " " 
      << initialPos.z;

   /* Done */
   return ss.str();
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void MapCharacterModAction::fromString(Kobold::String s)
{
   char buf[256];
   int act = -1;
   float x=0.0f, y=0.0f, z=0.0f;
   float initx=0.0f, inity=0.0f, initz=0.0f;

   /* Parse string */
   sscanf(s.c_str(),"%d %s %f %f %f %f %f %f %f", &act, &buf[0],
                    &x, &y, &z, &oriAngle, &initx, &inity, &initz);
   /* define values */
   setAction((ModActionType) act);
   setTarget(buf);
   setPosition(Ogre::Vector3(x, y, z));
   initialPos = Ogre::Vector3(initx, inity, initz);
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                         MapObjectModAction                             //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapObjectModAction::MapObjectModAction(ModActionType act, Kobold::String obj, 
                      Kobold::String mapFile, Ogre::Vector3 pos)
                   :ModAction(act, obj, mapFile, pos)
{
}

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapObjectModAction::MapObjectModAction(Kobold::String s)
                   :ModAction(MODSTATE_ACTION_UNKNOWN, "", "", 
                         Ogre::Vector3(0, 0, 0))
{
   fromString(s);
}


/************************************************************
 *                        Destructor                        *
 ************************************************************/
MapObjectModAction::~MapObjectModAction()
{
}

/************************************************************
 *                          getValue                        *
 ************************************************************/
int MapObjectModAction::getValue()
{
   return value;
}

/************************************************************
 *                          setValue                        *
 ************************************************************/
void MapObjectModAction::setValue(int v)
{
   value = v;
}

/************************************************************
 *                         toString                         *
 ************************************************************/
Kobold::String MapObjectModAction::toString()
{
   Ogre::StringStream ss;
   
   /* Definition */
   ss << MODSTATE_TOKEN_OBJECT_MOD_ACTION;
   ss << " = ";

   /* Action Type */
   ss << getAction() << " ";

   /* Target */
   ss << getTarget() << " ";

   /* Position */
   Ogre::Vector3 pos = getPosition();
   ss << pos.x << " " << pos.y << " " << pos.z << " ";

   /* Value  */
   ss << value;

   /* Done */
   return ss.str();
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void MapObjectModAction::fromString(Kobold::String s)
{
   char buf[256];
   int act = -1;
   float x = 0.0f, y = 0.0f, z = 0.0f;

   sscanf(s.c_str(),"%d %s %f %f %f %d", &act, &buf[0],
         &x, &y, &z, &value);
   /* define value */
   setAction((ModActionType) act);
   setTarget(buf);
   setPosition(Ogre::Vector3(x, y, z));
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                          MapTalkModAction                              //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapTalkModAction::MapTalkModAction(ModActionType act, Kobold::String character,
                                   Kobold::String mapFile, int talkValue) 
                 :ModAction(act, character, mapFile, Ogre::Vector3(0, 0, 0))
{
   value = talkValue;
}

/************************************************************
 *                       Constructor                        *
 ************************************************************/
MapTalkModAction::MapTalkModAction(Kobold::String s)
                   :ModAction(MODSTATE_ACTION_UNKNOWN, "", "", 
                              Ogre::Vector3(0, 0, 0))
{
   fromString(s);
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
MapTalkModAction::~MapTalkModAction()
{
}

/************************************************************
 *                          getValue                        *
 ************************************************************/
int MapTalkModAction::getValue()
{
   return value;
}

/************************************************************
 *                          setValue                        *
 ************************************************************/
void MapTalkModAction::setValue(int v)
{
   value = v;
}

/************************************************************
 *                         toString                         *
 ************************************************************/
Kobold::String MapTalkModAction::toString()
{
   Ogre::StringStream ss;

   /* Definition */
   ss << MODSTATE_TOKEN_TALK_MOD_ACTION;
   ss << " = ";

   /* Action Type */
   ss << getAction() << " ";

   /* Target */
   ss << getTarget() << " ";

   /* Value  */
   ss << value;

   /* Done */
   return ss.str();
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void MapTalkModAction::fromString(Kobold::String s)
{
   char buf[256];
   int act = -1;

   sscanf(s.c_str(),"%d %s %d", &act, &buf[0], &value);
   setAction((ModActionType) act);
   setTarget(buf);
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                            ModInventory                                //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
ModInventory::ModInvObj::ModInvObj(Kobold::String filename, int x, int y, 
      int invNumber, int state)
{
   this->filename = filename;
   this->x = x;
   this->y = y;
   this->invNumber = invNumber;
   this->state = state;
}
ModInventory::ModInvObj::ModInvObj()
{
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
ModInventory::ModInvObj::~ModInvObj()
{
}


/************************************************************
 *                       Constructor                        *
 ************************************************************/
ModInventory::ModInventory(Inventory* inv, Kobold::String owner, 
                           Kobold::String mapFile)
             : ModAction(MODSTATE_INVENTORY, owner, mapFile, 
                         Ogre::Vector3(-1 ,-1, -1)),
               Kobold::List(Kobold::LIST_TYPE_ADD_AT_END)
{
   /* Create the list from the Inventory */
   if(inv != NULL)
   {
      create(inv);
   }
}

/************************************************************
 *                         Destructor                       *
 ************************************************************/
ModInventory::~ModInventory()
{
   /* clear the list */
   clear();
}

/************************************************************
 *                            flush                         *
 ************************************************************/
void ModInventory::flush(Inventory* inv)
{
   ModInvObj* invObj = static_cast<ModInvObj*>(getFirst());
   Item* curItem = NULL;
   int invNumber;
   bool res;

   /* Insert all objects at its previous Inventory positions */
   for(int i = 0; i < getTotal(); i++)
   {
      /* create the item */
      curItem = static_cast<Item*>(Game::createObject(invObj->filename)); 

      /* Now insert it at the Inventory */
      if(curItem)
      {
         res = true;
         /* Set its state */
         curItem->setState(invObj->state);

         /* Must set money quantity as the state */
         if(curItem->getType() == Item::ITEM_TYPE_MONEY)
         {
            Money* m = static_cast<Money*>(curItem);
            m->setQuantity(invObj->state);
         }

         if(invObj->invNumber >= 0)
         {
            /* Insert at Inventory */
            res = inv->addItem(curItem, invObj->x, invObj->y, 
                  invObj->invNumber);
         }
         else
         {
            /* Insert at equipped place: as saved as -1 - equipValue
             * shoud  increment by 1 and change signal. */
            invNumber = invObj->invNumber + 1;
            invNumber *= -1;
            res = inv->equipItem(curItem, invNumber);
         }
         
         if(!res)
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
                  "Error: Couldn't put object '%s' at inventory position '%d"\
                  ",%d' of invNumber: '%d'", invObj->filename.c_str(), 
                  invObj->x, invObj->y, invObj->invNumber);
         }
      }
      else
      {
         /* Can't get the object, must show error */
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Error: Couldn't load object '%s'", invObj->filename.c_str());
      }

      /* Let's do the next */
      invObj = static_cast<ModInvObj*>(invObj->getNext());
   }
}

/************************************************************
 *                           create                         *
 ************************************************************/
void ModInventory::create(Inventory* inv)
{
   int x=0, y=0;
   Item* item = NULL;
   ModInvObj* invObj = NULL;

   /* Clear the current one */
   clear();

   /* Add All Equipped Places */
   for(int curInv = 0; curInv < Inventory::INVENTORY_TOTAL_PLACES; curInv++)
   {
      item = inv->getFromPlace(curInv);
      if(item)
      {
         /* Insert it with -1-curInv to designate a place, not a position
          * Note: using inverted -1 values for current inventory. */
         invObj =  new ModInvObj(item->getFilename(), 0, 0, (-1 - curInv), 
                                 item->getState());

         /* Insert it at the ModInventory */
         Kobold::List::insert(invObj);
      }
   }

   /* For all inventories */
   for(int curInv = 0; curInv < INVENTORY_PER_CHARACTER; curInv++)
   {
      item = inv->getFirstItem(x,y, curInv);
      while(item != NULL)
      {
         /* Create the item representation */
         invObj = new ModInvObj(item->getFilename(), x, y, curInv, 
               item->getState());

         /* Insert it at the ModInventory */
         Kobold::List::insert(invObj);
         
         /* Try to get next item */
         item = inv->getNextItem(x,y, curInv);
      }
   }
}

/************************************************************
 *                           load                           *
 ************************************************************/
bool ModInventory::load(Kobold::String file)
{
   Kobold::OgreDefParser def;
   Kobold::String key="", value="";
   
   if(!def.load(file, false))
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: Couldn't load inventory file '%s'", file.c_str());
      return false;
   }

   /* Parse all the file */
   while(def.getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MOD_INVENTORY)
      {
         /* The Owner */
         setTarget(value);
      }
      else if(key == MODSTATE_TOKEN_INVENTORY_ITEM)
      {
         /* Insert the item! */
         insert(value);
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Warning: unknow token: '%s' at ModInventory load '%s'",
               key.c_str(), file.c_str());
      }
   }

   return true;
}

/************************************************************
 *                           save                           *
 ************************************************************/
void ModInventory::save(std::ofstream& file)
{
   /* Define the owner (target) */
   file << MODSTATE_TOKEN_MOD_INVENTORY << " = " << getTarget() << std::endl;

   /* Save all objects here */
   ModInvObj* obj = static_cast<ModInvObj*>(getFirst());

   for(int i = 0; i < getTotal(); i++)
   {
      file << MODSTATE_TOKEN_INVENTORY_ITEM << " = " 
           << obj->filename << " " << obj->x << " " 
           << obj->y << " " << obj->invNumber << " " 
           << obj->state << std::endl;
      obj = static_cast<ModInvObj*>(obj->getNext());
   }
}

/************************************************************
 *                         insert                           *
 ************************************************************/
void ModInventory::insert(Kobold::String s)
{
   ModInvObj* obj = new ModInvObj();
   char fName[256];

   /* Get Values */
   sscanf(s.c_str(), "%s %d %d %d %d", &fName[0], &obj->x, &obj->y, 
          &obj->invNumber, &obj->state);
   obj->filename = fName;

   /* Insert it! */
   Kobold::List::insert(obj);
}

/************************************************************
 *                         toString                         *
 ************************************************************/
Kobold::String ModInventory::toString()
{
   /* Not used for ModInventories */
   return("");
}

/************************************************************
 *                       fromString                         *
 ************************************************************/
void ModInventory::fromString(Kobold::String s)
{
   /* Not used for ModInventories */
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                ModMap                                  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
ModMap::ModMap(Kobold::String filename)
{
   mapFilename = filename;
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
ModMap::~ModMap()
{
   clear();
}

/************************************************************
 *                      getMapFilename                      *
 ************************************************************/
Kobold::String ModMap::getMapFilename()
{
   return mapFilename;
}

/************************************************************
 *                           clear                          *
 ************************************************************/
void ModMap::clear()
{
   modList.clearList();
}

/************************************************************
 *                            load                          *
 ************************************************************/
void ModMap::load(Kobold::OgreDefParser* def)
{
   Kobold::String key="", value="";
   ModInventory* modInv = NULL;

   while(def->getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MAP_END)
      {
         /* Done with the current ModMap, so exit! */
         return;
      }
      else if(key == MODSTATE_TOKEN_CHARACTER_MOD_ACTION)
      {
         /* Parse and add Character ModAction */
         MapCharacterModAction* mChar = new MapCharacterModAction(value);
         addAction(mChar);
      }
      else if(key == MODSTATE_TOKEN_OBJECT_MOD_ACTION)
      {
         /* Parse and add Object ModAction */
         MapObjectModAction* mObj = new MapObjectModAction(value);
         addAction(mObj);
      }
      else if(key == MODSTATE_TOKEN_TALK_MOD_ACTION)
      {
         /* Parse and add Talk ModAction */
         MapTalkModAction* mTalk = new MapTalkModAction(value);
         addAction(mTalk);
      }
      else if(key == MODSTATE_TOKEN_MOD_INVENTORY)
      {
         /* Add the ModInventory */
         modInv = new ModInventory(NULL, value, mapFilename);
         addAction(modInv);
      }
      else if(key == MODSTATE_TOKEN_INVENTORY_ITEM)
      {
         /* Add item to the current mod Inventory */
         if(modInv)
         {
            modInv->insert(value);
         }
         else
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
                  "Warning: item without ModInventory!");
         }
      }
   }
}

/************************************************************
 *                            save                          *
 ************************************************************/
void ModMap::save(std::ofstream& file)
{
   ModAction* act;
   ModInventory* inv;

   /* Save the map filename */
   file << MODSTATE_TOKEN_MAP << " = " << mapFilename << std::endl;

   /* Now save each ModAction */
   act = static_cast<ModAction*>(modList.getFirst());
   for(int i = 0; i < modList.getTotal(); i++)
   {
      /* Save normal actions */
      if(act->getAction() != MODSTATE_INVENTORY)
      {
         file << act->toString() << std::endl;
      }

      /* Save a ModInventory */
      else
      {
         inv = static_cast<ModInventory*>(act);
         inv->save(file);
      }
      act = static_cast<ModAction*>(act->getNext());
   }

   /* Mark end of map */
   file << MODSTATE_TOKEN_MAP_END << " = " << mapFilename << std::endl;
}

/************************************************************
 *                         addAction                        *
 ************************************************************/
void ModMap::addAction(ModAction* act)
{
   modList.insert(act);
}

/************************************************************
 *                    MapObjectAddAction                    *
 ************************************************************/
void ModMap::addMapObjectAction(ModActionType action, Kobold::String target, 
      Kobold::String mapFilename, Ogre::Vector3 pos, int value)
{
   MapObjectModAction* n;

   /* Object Add or Object Remove Actions */
   if( (action == MODSTATE_ACTION_OBJECT_REMOVE) ||
       (action == MODSTATE_ACTION_OBJECT_ADD) )
   {
      if(removeInverseObjectAction(action, target, mapFilename, pos)) 
      {
         /*! Add is similar to remove inverse, so it is added! */
         return;
      }
      else
      {
         n = new MapObjectModAction(action, target, mapFilename, pos); 
         addAction(n);
      }
   }
   /* Change State Action */
   else if(action == MODSTATE_ACTION_OBJECT_CHANGE_STATE)
   {
      /* search for already created one */
      n = static_cast<MapObjectModAction*>(search(action, target, pos));

      /* not found, so must create a new one */
      if(n == NULL)
      {
         n = new MapObjectModAction(action, target, mapFilename, pos);
         addAction(n);
      }

      /* Define the new state :value */
      n->setValue(value);
   }
   /* Invalid Action  */
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: invalid modification object action: %d", action);
   }
}

/************************************************************
 *                  MapCharacterAddAction                   *
 ************************************************************/
void ModMap::addMapCharacterAction(ModActionType act, Kobold::String character, 
      Kobold::String mapFile, Ogre::Vector3 pos,
      Ogre::Real orientation, Ogre::Vector3 initial)
{
   if( (act != MODSTATE_ACTION_CHARACTER_DEAD) && 
       (act != MODSTATE_ACTION_CHARACTER_MOVE) && 
       (act != MODSTATE_ACTION_CHARACTER_CHANGE_STATE) )
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: Invalid modification Character action: %d", act);
   }

   /* Search for a previous ModAction with same info */
   MapCharacterModAction* n = static_cast<MapCharacterModAction*>(
         search(act, character, initial));

   if(n != NULL)
   {
      /* Just update the one found */
      n->setPosition(pos);
      n->setOrientation(orientation);
   }
   else
   {
      /* None found, must create a new one */
      n = new MapCharacterModAction(act, character, mapFile, pos,
            orientation, initial);
      addAction(n);
   }
}

/************************************************************
 *                      MapTalkAddAction                    *
 ************************************************************/
void ModMap::addMapTalkAction(ModActionType act, Kobold::String character, 
      Kobold::String mapFile, int value)
{
   if(act == MODSTATE_TALK_ENTER_VALUE)
   {
      /* Search List for a previous value */
      MapTalkModAction* n = static_cast<MapTalkModAction*>(
            search(act, character));

      if(n != NULL)
      {
         /* Use the one found */
         n->setValue(value);
      }
      else
      {
         /* Create a new one */
         n = new MapTalkModAction(act, character, mapFile, value);
         addAction(n);
      }
   }
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: Invalid Talk Action '%d'", act);
   }
}

/************************************************************
 *                      mapInventoryAdd                     *
 ************************************************************/
void ModMap::addMapInventory(Inventory* inv, Kobold::String owner)
{
   /* Search for a previous ModInventory here */
   ModInventory* modInv = static_cast<ModInventory*>(
         search(MODSTATE_INVENTORY, owner));

   if(modInv != NULL)
   {
      /* Found, so just update this one */
      modInv->create(inv);
   }
   else
   {
      /* None found, so must create a new one */
      modInv = new ModInventory(inv, owner, mapFilename);
      addAction(modInv);
   }
}

/************************************************************
 *                           search                         *
 ************************************************************/
ModAction* ModMap::search(ModActionType action, Kobold::String target, 
      Ogre::Vector3 pos)
{
   ModAction* mod = static_cast<ModAction*>(modList.getFirst());
   for(int i = 0; (i < modList.getTotal()); i++)
   {
      if( (mod->getAction() == action) && 
          (mod->getTarget() == target) )
      {
         if((pos.x != -1) && (pos.y != -1) && (pos.z != -1))
         {
            /* verify the initial or current position */
            Ogre::Vector3 cmpPos;

            if( (action == MODSTATE_ACTION_CHARACTER_DEAD) ||
                (action == MODSTATE_ACTION_CHARACTER_MOVE) ||
                (action == MODSTATE_ACTION_CHARACTER_CHANGE_STATE) )
            {
               /* For Character Ones, verify the initial position */
               MapCharacterModAction* charAct = 
                  static_cast<MapCharacterModAction*>(mod);
               cmpPos = charAct->getInitialPosition();
            }
            else
            {
               /* For all other types, verify the current position */
               cmpPos = mod->getPosition();
            }

            /* Verify the position */
            if(pos == cmpPos)
            {
               /* Found it at the desired position */
               return mod;
            }
         }
         else
         {
            /* No Position Verification, so found it! */
            return mod;
         }
      }
      mod = static_cast<ModAction*>(mod->getNext());
   }

   /* No ModActions found */
   return NULL;
}

/************************************************************
 *                       removeAction                       *
 ************************************************************/
void ModMap::removeAction(ModAction* act)
{
   if(act != NULL)
   {
      modList.remove(act);
   }
}

/************************************************************
 *                   removeInverseAction                    *
 ************************************************************/
bool ModMap::removeInverseObjectAction(ModActionType action, 
      Kobold::String target, Kobold::String mapFilename, Ogre::Vector3 pos)
{
   ModAction* tmp;

   if( (action != MODSTATE_ACTION_OBJECT_REMOVE) && 
       (action != MODSTATE_ACTION_OBJECT_ADD) )
   {
      /* Invalid action: not an object one! */
      return false;
   }

   /* If action is ADD, we can remove action REMOVE,
    * if is REMOVE, we can remove ADD. */
   ModActionType invAction = MODSTATE_ACTION_OBJECT_REMOVE;

   /* If is an remove action, we must verify if no change state actions 
    * related to the object exists. if one exist, we must remove it,
    * to avoid Unknow Object to change at ModState errors. */
   if(action == MODSTATE_ACTION_OBJECT_REMOVE)
   {
      tmp = search(MODSTATE_ACTION_OBJECT_CHANGE_STATE, target, pos); 
      if(tmp != NULL)
      {
         //cerr << "DEBUG: found the change state, so removing it!" << endl;
         removeAction(tmp);
      }
      invAction = MODSTATE_ACTION_OBJECT_ADD;
   }

   /* Search for the inverse action */
   tmp = search(invAction, target, pos);
   if(tmp != NULL)
   {
      /* Found the Inverse, so remove it */
      removeAction(tmp);
      return true;
   }
   return false;
}

/************************************************************
 *                    doMapModifications                    *
 ************************************************************/
void ModMap::doMapModifications() 
{
   Character* ch  = NULL;
   Ogre::Vector3 pos;
   ModAction* tmpMobj = static_cast<ModAction*>(modList.getFirst());

   for(int i = 0; i < modList.getTotal(); i++)
   {
      /* Object Remove */
      if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_REMOVE)
      {
//TODO: Map remove Object!
#if 0
         /* Get the object from the list */
         object* obj = objectsList::search(tmpMobj->getTarget(), 
               tmpMobj->getPosition());

         if(obj)
         {
            /* Remove it from the map */
            actualMap->removeObject(obj);
            /* And remove it from game! */
            delete(obj);
         }
         else
         {
            cerr << "Error: Unknow object: " << tmpMobj->getTarget() 
               << " to remove at ModState" << endl;
         }
#endif
      }

      /* Object Add */
      else if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_ADD)
      {
//TODO: map add object!
#if 0 
         object* obj = objectsList::search(tmpMobj->getTarget(),
               x, actualMap->getHeight(x,z),z);
         if(obj == NULL)
         {
            /* Load it to the map */
            obj = createObject(tmpMobj->getTarget(), actualMap->getFilename());
         }
         /* Insert the Object  */
         actualMap->insertObject(x, actualMap->getHeight(x,z), z, 
               0.0f, 0.0f, 0.0f, obj, 0);
#endif
      }

      /* Object Change State */
      else if(tmpMobj->getAction() == MODSTATE_ACTION_OBJECT_CHANGE_STATE)
      {
//TODO map search object to change its state
#if 0
         /* Get the object from the list */
         MapObjectModAction* act = static_cast<MapObjectModAction*>(tmpMobj);
         object* obj = objectsList::search(act->getTarget(), x, y, z);

         if(obj)
         {
            /* Change its state */
            obj->setState(act->getValue());
         }
         else
         {
            cerr << "Error: Unknow object: " << act->getTarget() 
               << " " << x << " " << y << " " << z 
               << " to change at ModState" << endl;
         }
#endif
      }

      /* Character Dead */
      else if( (tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_DEAD) ||
               (tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_CHANGE_STATE))
      {
         //FIXME: when the dead one is a PC from party.
         /* Get The Character Pointer */
         MapCharacterModAction* charAct = static_cast<MapCharacterModAction*>(
               tmpMobj);
         ch = Game::getNpcs()->getCharacter(charAct->getTarget());

         bool done = false;
         while( (ch != NULL) && (!done) )
         {
            if(ch->getInitialPosition() == charAct->getInitialPosition())
            {
               if(tmpMobj->getAction() == MODSTATE_ACTION_CHARACTER_DEAD)
               {
                  /* Put it as dead at the position */
                  Ogre::Vector3 pos;
                  ch->instantKill();
                  pos = charAct->getPosition();

                  ch->setPosition(pos);
                  ch->setOrientation(charAct->getOrientation());
//TODO: character occupy square
#if 0                  
                  ch->defineOcSquare(actualMap);
#endif
               }
               else if(tmpMobj->getAction() == 
                                         MODSTATE_ACTION_CHARACTER_CHANGE_STATE)
               {
//TODO and FIXME: change state should really use orientation??
#if 0
                  /* Change the state to the desired one! */
                  float pX=0.0f,pY=0.0f,pZ=0.0f;
                  ch->setPsychoState(static_cast<PsychoState>(static_cast<int>( 
                        charAct->getOrientation())));
                  /* Set the Character position */
                  charAct->getPosition(pX, pY,pZ);
                  ch->scNode->setPosition(pX, pY, pZ);
                  ch->defineOcSquare(actualMap);
#endif
               }

               done = true;
            }
            else
            {
               /* Not the one, get the next */
               ch = Game::getNpcs()->getNextSameCharacter(ch);
            }
         }

         if(!done)
         {
            Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
                  "Error: couldn't find character '%s'", 
                  charAct->getTarget().c_str());
         }
      }

      /* Talk Initial Dialog */
      else if(tmpMobj->getAction() == MODSTATE_TALK_ENTER_VALUE)
      {
//TODO: dialog at thing's
#if 0
         /* Get the Character pointer */
         ch = Game::getNpcs()->getCharacter(tmpMobj->getTarget());
         if(ch)
         {
            MapTalkModAction* mTalk = static_cast<MapTalkModAction*>(tmpMobj);
            ch->setInitialConversation(mTalk->getValue());
         }
         //TODO and FIXME else: could changed an object's dialog!
#endif
      }

      /* Inventory */
      else if(tmpMobj->getAction() == MODSTATE_INVENTORY)
      {
         /* Get the modified Inventory */
         ModInventory* mInv = static_cast<ModInventory*>(tmpMobj);
         /* Get the Character */
         ch = Game::getNpcs()->getCharacter(tmpMobj->getTarget());
         if(ch)
         {
            mInv->flush(ch->getInventory());
         }
      }

      /* Unknow */
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Error: unknown saved action '%d' at '%d' element!",
               tmpMobj->getAction(), i);
      }
      
      tmpMobj = static_cast<ModAction*>(tmpMobj->getNext());
   }
}

/************************************************************
 *                     allCharacterDead                     *
 ************************************************************/
bool ModMap::isAllCharactersDead(Kobold::String npcFilename)
{
   //FIXME: we should keep the NpcFile preloaded somewhere.
   NpcFile npcs;
   Kobold::String name="", arq ="";
   Ogre::Vector3 pos;
   float angle = 0.0f;
   Thing::PsychoState psycho = Thing::PSYCHO_NEUTRAL;

   if(!npcs.load(npcFilename))
   {
      /* No File, so always false */
      return false;
   }

   /* Try to search for all Characters dead definition here */
   while(npcs.getNextCharacter(name, arq, pos, angle, psycho))
   {
      /* If one isn't found, not all Character are dead, so exit! */
      if(search(MODSTATE_ACTION_CHARACTER_DEAD, arq, pos) == NULL)
      {
         /* Not dead, so false! */
         return false;
      }
   }

   /*  All found (or no one defined), so all dead! */
   return true;
}

/************************************************************
 *                    allCharacterAlive                     *
 ************************************************************/
bool ModMap::isAllCharactersAlive(Kobold::String npcFilename)
{
   //FIXME: we should keep the NpcFile preloaded somewhere.
   NpcFile npcs;
   Kobold::String name="", arq ="";
   Ogre::Vector3 pos;
   float angle=0;
   Thing::PsychoState psycho = Thing::PSYCHO_NEUTRAL;

   if(!npcs.load(npcFilename))
   {
      /* No File, so always false */
      return false;
   }

   /* Try to search for all Characters dead definition here */
   while(npcs.getNextCharacter(name, arq, pos, angle, psycho))
   {
      /* If one is found, not all Character are alive, so exit! */
      if(search(MODSTATE_ACTION_CHARACTER_DEAD, arq, pos) != NULL)
      {
         /* Not dead, so false! */
         return false;
      }
   }

   /*  No one found (or no one defined), so all alive! */
   return true;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                               ModState                                 //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/************************************************************
 *                       Constructor                        *
 ************************************************************/
void ModState::init()
{
}

/************************************************************
 *                        Destructor                        *
 ************************************************************/
void ModState::finish()
{
   clear();
}

/************************************************************
 *                         loadState                        *
 ************************************************************/
bool ModState::loadState(Kobold::String file)
{
   Kobold::OgreDefParser def;
   Kobold::String key, value;
   ModMap* mod = NULL;

   /* Clear current ModState */
   clear();

   /* Try to open the ModState file */
   if(!def.load(file, false))
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: couldn't open ModState file '%s'", file.c_str());
      return false;
   }

   /* Get version */
   def.getNextTuple(key, value);
   if(key == MODSTATE_TOKEN_VERSION)
   {
      //TODO do some later version check
   }
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: invalid ModState file: '%s'", file.c_str());
      return false;
   }

   /* Now parse the file */
   while(def.getNextTuple(key, value))
   {
      if(key == MODSTATE_TOKEN_MAP)
      {
         /* Get the ModMap */
         mod = findModMap(value);

         /* And load things for it! */
         mod->load(&def);
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Error: unexpected token '%s' at file '%s'", 
               key.c_str(), file.c_str());
      }
   }

   return true;
}

/************************************************************
 *                         saveState                        *
 ************************************************************/
bool ModState::saveState(Kobold::String file)
{
   std::ofstream f;
   ModMap* cur;

   /* Create the file */
   f.open(file.c_str(), std::ios::out | std::ios::binary);
   if(!f)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: couldn't save file: '%s'", file.c_str());
      return false;
   }

   /* Save the version */
   f << MODSTATE_TOKEN_VERSION << " = " << DNT_VERSION << std::endl;

   /* Now call to save each ModMap */
   cur = static_cast<ModMap*>(modList.getFirst());
   for(int i = 0; i < modList.getTotal(); i++)
   {
      cur->save(f);
      cur = static_cast<ModMap*>(cur->getNext());
   }

   /* Close the file and done! */
   f.close();
   return true;
}

/************************************************************
 *                          haveMap                         *
 ************************************************************/
bool ModState::haveMap(Kobold::String filename)
{
   int i;
   ModMap* mod = static_cast<ModMap*>(modList.getFirst());
   for(i = 0; i < modList.getTotal(); i++)
   {
      if(mod->getMapFilename() == filename)
      {
         /* Found it! */
         return true;
      }
      mod = static_cast<ModMap*>(mod->getNext());
   }

   return false;
}

/************************************************************
 *                        findModMap                        *
 ************************************************************/
ModMap* ModState::findModMap(Kobold::String filename)
{
  int i;
  ModMap* mod = static_cast<ModMap*>(modList.getFirst());
  for(i = 0; i < modList.getTotal(); i++)
  {
     if(mod->getMapFilename() == filename)
     {
        /* Found it! */
        return mod;
     }
     mod = static_cast<ModMap*>(mod->getNext());
  }
  /* None found, must create one */
  return createModMap(filename);
}

/************************************************************
 *                       createModMap                       *
 ************************************************************/
ModMap* ModState::createModMap(Kobold::String filename)
{
   ModMap* m = new ModMap(filename);

   /* Insert it at the list */
   modList.insert(m);

   return m;
}

/************************************************************
 *                    MapObjectAddAction                    *
 ************************************************************/
void ModState::addMapObjectAction(ModActionType action, Kobold::String target, 
      Kobold::String mapFilename, Ogre::Vector3 pos, int value)
{
   ModMap* mod = findModMap(mapFilename);

   if(mod != NULL)
   {
      mod->addMapObjectAction(action, target, mapFilename, pos, value); 
   }
}

/************************************************************
 *                  MapCharacterAddAction                   *
 ************************************************************/
void ModState::addMapCharacterAction(ModActionType act, 
      Kobold::String character, Kobold::String mapFile,
      Ogre::Vector3 pos, Ogre::Real orientation, Ogre::Vector3 initial)
{
   ModMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->addMapCharacterAction(act, character, mapFile, pos,
            orientation, initial);
   }
}

/************************************************************
 *                      MapTalkAddAction                    *
 ************************************************************/
void ModState::addMapTalkAction(ModActionType act, Kobold::String character, 
      Kobold::String mapFile, int value)
{
   ModMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->addMapTalkAction(act, character, mapFile, value);
   }
}

/************************************************************
 *                       mapInventoryAdd                    *
 ************************************************************/
void ModState::addMapInventory(Inventory* inv, Kobold::String owner, 
      Kobold::String mapFile)
{
   ModMap* mod = findModMap(mapFile);
   if(mod != NULL)
   {
      mod->addMapInventory(inv, owner);
   }
}

/************************************************************
 *                    doMapModifications                    *
 ************************************************************/
void ModState::doMapModifications()
{
   ModMap* mod = findModMap(Game::getCurrentMap()->getFilename());
   if(mod != NULL)
   {
      mod->doMapModifications();
   } 
}

/************************************************************
 *                           clear                          *
 ************************************************************/
void ModState::clear()
{
   modList.clear();
}

/************************************************************
 *                    allCharactersDead                     *
 ************************************************************/
bool ModState::isAllCharactersDead(Kobold::String npcFilename)
{
   ModMap* mod;

   /* FIXME: trick, removing .npc and getting map Name! */
   Kobold::String mapFile = npcFilename;
   mapFile.erase(mapFile.length()-4,4);

   /* Find its ModMap */
   mod = findModMap(mapFile);
   if(mod)
   {
      return mod->isAllCharactersDead(npcFilename);
   }

   /* No Mod, so all alive */
   return false;
}


/************************************************************
 *                    allCharactersAlive                    *
 ************************************************************/
bool ModState::isAllCharactersAlive(Kobold::String npcFilename)
{
   ModMap* mod;

   /* FIXME: trick, removing .npc and getting map Name! */
   Kobold::String mapFile = npcFilename;
   mapFile.erase(mapFile.length()-4,4);

   /* Find its ModMap */
   mod = findModMap(mapFile);
   if(mod)
   {
      return mod->isAllCharactersAlive(npcFilename);
   }

   /* No Mod, so all alive */
   return true;
}

/************************************************************
 *                      static fields                       *
 ************************************************************/
Kobold::List ModState::modList;

