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

#include <kobold/defparser.h>
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
   Kobold::DefParser def;
   Kobold::String key, value;

   /* Read values */
   NpcParseStruct* npc = NULL;

   this->filename = filename;

   if(!def.load(filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
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
            sscanf(value.c_str(), "%f %f", &npc->posX, &npc->posZ);
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
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: no NPC defined while parsing '%s'", filename.c_str());
      }
   }

   if(mapFilename.empty())
   {
       Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
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
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Couldn't open npc file '%s' for saving it.", filename.c_str());
      return false;
   }

   /* Write its map */
   file << NPC_FILE_KEY_MAP_FILE << " = " << mapFilename << std::endl;

   /* Write each NPC */
   NpcParseStruct* npc = (NpcParseStruct*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      file << NPC_FILE_KEY_NPC << " = " << npc->name << std::endl;
      file << NPC_FILE_KEY_FILE << " = " << npc->filename << std::endl;
      file << NPC_FILE_KEY_POSITION << " = " << npc->posX << " " 
           << npc->posZ << std::endl;
      file << NPC_FILE_KEY_ANGLE << " = " << npc->angle << std::endl;
      file << NPC_FILE_KEY_PSYCHO << " = " << npc->psycho << std::endl;

      npc = (NpcParseStruct*) npc->getNext();
   }

   return true;
}


/***********************************************************************
 *                          insertCharacter                            *
 ***********************************************************************/
void NpcFile::insertCharacter(Kobold::String name, Kobold::String filename,
      float posX, float posZ, float angle, Thing::PsychoState psycho)
{
   /* create and define the character struct */
   NpcParseStruct* npc = new NpcParseStruct();
   npc->name = name;
   npc->filename = filename;
   npc->posX = posX;
   npc->posZ = posZ;
   npc->angle = angle;
   npc->psycho = psycho;

   /* insert on the list */
   insert(npc);
}

/***********************************************************************
 *                           getNextCharacter                          *
 ***********************************************************************/
bool NpcFile::getNextCharacter(Kobold::String& name, Kobold::String& filename, 
      float& posX, float& posZ, float& angle, Thing::PsychoState& psycho)
{
   bool res = false;
   
   if(getTotal() > 0)
   {
      if(!current)
      {
         current = (NpcParseStruct*) getFirst();
         res = true;
      }
      else
      {
         current = (NpcParseStruct*) current->getNext();
         res = (current != first);
      }
   }

   /* If defined, so get the return values */
   if(res)
   {
      name = current->name;
      filename = current->filename;
      posX = current->posX;
      posZ = current->posZ;
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
   NpcParseStruct* npc = (NpcParseStruct*)first;
   int i;

   /* Tell modState that All Elements are dead */
   for(i=0; i < getTotal(); i++)
   {
//TODO: ModState!
#if 0
      modif.mapCharacterAddAction(MODSTATE_ACTION_CHARACTER_DEAD,
                                  npc->filename, mapFilename,
                                  npc->posX, 0.0, npc->posZ, npc->angle,
                                  npc->posX, npc->posZ);
#endif

      npc = (NpcParseStruct*) npc->getNext();
   }
}

