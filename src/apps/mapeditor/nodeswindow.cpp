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

#include "nodeswindow.h"

#include "../../core/game.h"
#include "../../map/map.h"
#include "../../map/light.h"
#include "../../rules/thing.h"

using namespace DNTMapEditor;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
NodesWindow::NodesWindow()
{
   window = NULL;
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
NodesWindow::~NodesWindow()
{
}

/************************************************************************
 *                                 open                                 *
 ************************************************************************/
void NodesWindow::open()
{
   /* Only open if not already opened */
   if(!window)
   {
      /* Define the window */
      window = new Farso::Window(320, 202, "Nodes");
      window->setExternPointer(&window);

      /* Define the tree, with its initial root nodes */
      nodesTree = new Farso::TreeView(true, window);
      lightNode = nodesTree->addRootElement("Lights");
      sceneryNode = nodesTree->addRootElement("Scenery");
      doorsNode = sceneryNode->addChild("Doors");
      wallsNode = sceneryNode->addChild("Walls");
      pickableNode = nodesTree->addRootElement("Pickables");

      populateNodes();

      /* Open and set its position */
      window->open();
      window->setPosition(Farso::Controller::getWidth() - 320, 24);
   }
}

/************************************************************************
 *                                close                                 *
 ************************************************************************/
void NodesWindow::close()
{
   if(window)
   {
      window->close();
      namesCount.clear();
   }
}

/************************************************************************
 *                             checkEvents                              *
 ************************************************************************/
bool NodesWindow::checkEvents()
{
   if((window) && (window->isActive()))
   {
   }

   return false;
}

/************************************************************************
 *                            populateEvents                            *
 ************************************************************************/
void NodesWindow::populateNodes()
{
   DNT::Map* map = DNT::Game::getCurrentMap();
   
   /* Define our lights */
   DNT::LightInfo* light = static_cast<DNT::LightInfo*>(
         map->getLights()->getFirst());
   for(int i = 0; i < map->getLights()->getTotal(); i++)
   {
      lightNode->addChild(getNodeName("Light"), light);
      light = static_cast<DNT::LightInfo*>(light->getNext());
   }

   /* Define our static things */
   DNT::Thing* thing = static_cast<DNT::Thing*>(
         map->getStaticThings()->getFirst());
   for(int i = 0; i < map->getStaticThings()->getTotal(); i++)
   {
      sceneryNode->addChild(getNodeName(thing->getName()), thing);
      thing = static_cast<DNT::Thing*>(thing->getNext());
   }

   /* Define our dynamic things */
   thing = static_cast<DNT::Thing*>(map->getDynamicThings()->getFirst());
   for(int i = 0; i < map->getDynamicThings()->getTotal(); i++)
   {
      if(thing->getThingType() == DNT::Thing::THING_TYPE_DOOR)
      {
         doorsNode->addChild(getNodeName(thing->getName()), thing);
      }
      thing = static_cast<DNT::Thing*>(thing->getNext());
   }
}

/************************************************************************
 *                            getNodeName                               *
 ************************************************************************/
Kobold::String NodesWindow::getNodeName(Kobold::String name)
{
   int count = 1;
   Kobold::String retName = name;

   std::map<Kobold::String, int>::iterator it;
   it = namesCount.find(name);
   if(it != namesCount.end())
   {
      /* Have some, must increment the count to differ them */
      retName += Kobold::String(" #");
      count = it->second + 1;
      retName += Kobold::StringUtil::toString(count);
   }

   /* Redefine current count */
   namesCount[name] = count;

   return retName;
}

