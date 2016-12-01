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

#include "alignment.h"

#include "../lang/translate.h"

#include <kobold/defparser.h>
#include <kobold/log.h>

#include <assert.h>

using namespace DNT;

/******************************************************************
 *                            Constructor                         *
 ******************************************************************/
Alignment::Alignment(int id, Kobold::String strId)
          :RuleDefinition(id, strId)
{
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
Alignment::~Alignment()
{
}

/******************************************************************
 *                                init                            *
 ******************************************************************/
void Alignments::init()
{
   Kobold::DefParser def;

   Kobold::Log::add("Initing Alignments");

   int idInt = 0;
   char alignmentFile[256];
   Kobold::String key="", value="";

   /* Open the list file */
   if(!def.load("alignment/alignment.lst"))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error opening Alignment list file!");
      return;
   }

   /* Get and insert all defined aligments */
   while(def.getNextTuple(key, value))
   {
      /* Break Value */
      sscanf(value.c_str(), "%d %s", &idInt, &alignmentFile[0]);
      insertAlign(alignmentFile, key, idInt);
   }
}

/******************************************************************
 *                              finish                            *
 ******************************************************************/
void Alignments::finish()
{
   Kobold::Log::add("Finishing Alignments");
   list.clear();
}

/******************************************************************
 *                            insertAlign                         *
 ******************************************************************/
void Alignments::insertAlign(Kobold::String filename, Kobold::String idString, 
      int idInt)
{
   Kobold::DefParser def;
   Kobold::String key="", value="";
   Alignment* ins;

   Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
         "   Loading '%s'", filename.c_str());
   
   if(!def.load("alignment/" + filename))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "Error while opening alignmentment file: '%s'", filename.c_str());
      return;
   }

   /* Create the Alignment */
   ins = new Alignment(idInt, idString);

   while(def.getNextTuple(key, value))
   {
      /* Name */
      if(key == "name")
      {
         ins->setName(translateDataString(value));
      }
      /* Description */
      else if(key == "description")
      {
         ins->setDescription(translateDataString(value));
      }
      /* Image */
      else if(key == "image")
      {
         ins->loadImage(value);
      }
   }

   /* Really insert on list */
   list.insert(ins);
}


/******************************************************************
 *                         getAlignByInteger                      *
 ******************************************************************/
Alignment* Alignments::getAlignmentByInteger(int id)
{
   Alignment* ret = (Alignment*) list.getFirst();
   
   for(int i = 0; i < list.getTotal(); i++)
   {
      if(ret->getId() == id)
      {
         return ret;
      }
      ret = (Alignment*) ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                          getAlignByString                      *
 ******************************************************************/
Alignment* Alignments::getAlignmentByString(Kobold::String id)
{
   Alignment* ret = (Alignment*)list.getFirst();
   
   for(int i = 0; i < list.getTotal(); i++)
   {
      if(ret->getStringId() == id)
      {
         return ret;
      }
      ret = (Alignment*) ret->getNext();
   }

   return NULL;
}

/******************************************************************
 *                           Static Members                       *
 ******************************************************************/
Kobold::List Alignments::list;

