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

#include "npcfile.h"

#include <kobold/ogre3d/ogredefparser.h>
#include <kobold/log.h>

#include <stdio.h>
#include <iostream>

using namespace DNT;

#define NPC_FILE_KEY_MAP_FILE "mapFile"

#define NPC_FILE_KEY_NPC "npc"
#define NPC_FILE_KEY_FILE "file"
#define NPC_FILE_KEY_POSITION "position"
#define NPC_FILE_KEY_ANGLE "angle"
#define NPC_FILE_KEY_PSYCHO "psycho"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
NpcFile::NpcFile()
{
   current = NULL;
   filename = "";
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
NpcFile::~NpcFile()
{
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool NpcFile::load(Kobold::String filename)
{
   Kobold::OgreDefParser def;
   Kobold::String key, value;

   /* Read values */
   NpcParseStruct* npc = NULL;

   this->filename = filename;

   if(!def.load(filename, false))
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: couldn't open NPCs file: '%s'", filename.c_str());
      return false;
   }

   while(def.getNextTuple(key, value))
   {
      if(key == NPC_FILE_KEY_MAP_FILE)
      {
         mapFilename = value;
      }
      else if(key == NPC_FILE_KEY_NPC)
      {
         NpcParseStruct* npc = new NpcParseStruct();
         npc->name = value;
         insert(npc);
      }
      else if(npc)
      {
         if(key == NPC_FILE_KEY_FILE)
         {
            npc->filename = value;
         }
         else if(key == NPC_FILE_KEY_POSITION)
         {
            float pX=0.0f, pY=0.0f, pZ=0.0f;
            sscanf(value.c_str(), "%f %f %f", &pX, &pY, &pZ);
            npc->position = Ogre::Vector3(pX, pY, pZ);
         }
         else if(key == NPC_FILE_KEY_ANGLE)
         {
            sscanf(value.c_str(), "%f", &npc->angle);
         }
         else if(NPC_FILE_KEY_PSYCHO)
         {
            int val = 0;
            sscanf(value.c_str(), "%d", &val);
            npc->psycho = (Thing::PsychoState) val;
         }
      }
      else
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Error: no NPC defined while parsing '%s'", filename.c_str());
      }
   }

   if(mapFilename.empty())
   {
       Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
             "Warning: map is undefined for npc file '%s'", 
             filename.c_str());
   }

   return true;
}

/***********************************************************************
 *                                save                                 *
 ***********************************************************************/
bool NpcFile::save(Kobold::String filename)
{
   std::ofstream file;
   /* Let's try to open it */
   
   file.open(filename.c_str(), std::ios::out | std::ios::binary);
   if(!file)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Couldn't open npc file '%s' for saving it.", filename.c_str());
      return false;
   }

   /* Write its map */
   file << NPC_FILE_KEY_MAP_FILE << " = " << mapFilename << std::endl;

   /* Write each NPC */
   NpcParseStruct* npc = static_cast<NpcParseStruct*>(getFirst());
   for(int i = 0; i < getTotal(); i++)
   {
      file << NPC_FILE_KEY_NPC << " = " << npc->name << std::endl;
      file << NPC_FILE_KEY_FILE << " = " << npc->filename << std::endl;
      file << NPC_FILE_KEY_POSITION << " = " << npc->position.x << " " 
           << npc->position.y << " " << npc->position.z << std::endl;
      file << NPC_FILE_KEY_ANGLE << " = " << npc->angle << std::endl;
      file << NPC_FILE_KEY_PSYCHO << " = " << npc->psycho << std::endl;

      npc = static_cast<NpcParseStruct*>(npc->getNext());
   }

   return true;
}


/***********************************************************************
 *                          insertCharacter                            *
 ***********************************************************************/
void NpcFile::insertCharacter(Kobold::String name, Kobold::String filename,
      Ogre::Vector3 pos, float angle, Thing::PsychoState psycho)
{
   /* create and define the character struct */
   NpcParseStruct* npc = new NpcParseStruct();
   npc->name = name;
   npc->filename = filename;
   npc->position = pos;
   npc->angle = angle;
   npc->psycho = psycho;

   /* insert on the list */
   insert(npc);
}

/***********************************************************************
 *                           getNextCharacter                          *
 ***********************************************************************/
bool NpcFile::getNextCharacter(Kobold::String& name, Kobold::String& filename, 
      Ogre::Vector3& pos, float& angle, Thing::PsychoState& psycho)
{
   bool res = false;
   
   if(getTotal() > 0)
   {
      if(!current)
      {
         current = static_cast<NpcParseStruct*>(getFirst());
         res = true;
      }
      else
      {
         current = static_cast<NpcParseStruct*>(current->getNext());
         res = (current != static_cast<NpcParseStruct*>(getFirst()));
      }
   }

   /* If defined, so get the return values */
   if(res)
   {
      name = current->name;
      filename = current->filename;
      pos = current->position;
      angle = current->angle;
      psycho = current->psycho;
   }

   return res;
}

/***********************************************************************
 *                                killAll                              *
 ***********************************************************************/
void NpcFile::killAll()
{
   NpcParseStruct* npc = static_cast<NpcParseStruct*>(getFirst());

   /* Tell modState that All Elements are dead */
   for(int i=0; i < getTotal(); i++)
   {
//TODO: ModState!
#if 0
      modif.mapCharacterAddAction(MODSTATE_ACTION_CHARACTER_DEAD,
                                  npc->filename, mapFilename,
                                  npc->posX, 0.0, npc->posZ, npc->angle,
                                  npc->posX, npc->posZ);
#endif

      npc = static_cast<NpcParseStruct*>(npc->getNext());
   }
}

