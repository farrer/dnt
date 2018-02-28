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
#include "selection.h"

#include <assert.h>

#include "../../core/game.h"
#include "../../map/map.h"
#include "../../map/light.h"
#include "../../rules/thing.h"
#include "../../rules/character.h"

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
      npcsNode = nodesTree->addRootElement("NPCs");
      pickableNode = nodesTree->addRootElement("Pickables");

      populateNodes();

      /* Open and set its position */
      window->open();
      window->setPosition(Farso::Controller::getWidth() - 320, 23);
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
bool NodesWindow::checkEvents(PositionEditor* positionEditor)
{
   if((window) && (window->isActive()))
   {
      Farso::Event event = Farso::Controller::getLastEvent();
      if((event.getType() == Farso::EVENT_TREEVIEW_SELECTED) &&
         (event.getWidget() == nodesTree))
      {
         /* Get the selected node */
         Farso::TreeView::TreeViewElement* sel = nodesTree->getSelected();
         if(sel->getParent() == wallsNode)
         {
            /* selected a wall */
            //positionEditor->selectWall(sel->getData);
         }
         else if(sel->getParent() == lightNode)
         {
            /* selected a light */
            Selection::selectLight(
                  static_cast<DNT::LightInfo*>(sel->getData()));
         }
         else if(sel->getData())
         {
            /* selected a DNT::Thing */
            Selection::selectThing(
                  static_cast<DNT::Thing*>(sel->getData()));
         }
         else
         {
            /* Unselect any position editor element */
            Selection::clear();
         }

         return true;
      }
   }

   return false;
}

/************************************************************************
 *                               addThing                               *
 ************************************************************************/
void NodesWindow::addThing(DNT::Thing* thing)
{
   assert(window != NULL);
   assert(thing != NULL);
   
   if(thing->getThingType() == DNT::Thing::THING_TYPE_DOOR)
   {
      doorsNode->addChild(getNodeName(thing->getName()), thing);
   }
   else if(thing->getThingType() == DNT::Thing::THING_TYPE_SCENERY)
   {
      sceneryNode->addChild(getNodeName(thing->getName()), thing);
   }
   else if(thing->getThingType() == DNT::Thing::THING_TYPE_CHARACTER)
   {
      npcsNode->addChild(getNodeName(thing->getName()), thing);
   }
}

/************************************************************************
 *                               addLight                               *
 ************************************************************************/
void NodesWindow::addLight(DNT::LightInfo* light)
{
   assert(window != NULL);
   assert(light != NULL);

   lightNode->addChild(getNodeName("Light"), light);
}

/************************************************************************
 *                            populateNodes                             *
 ************************************************************************/
void NodesWindow::populateNodes()
{
   DNT::Map* map = DNT::Game::getCurrentMap();
   namesCount.clear();
   
   /* Define our lights */
   DNT::LightInfo* light = static_cast<DNT::LightInfo*>(
         map->getLights()->getFirst());
   for(int i = 0; i < map->getLights()->getTotal(); i++)
   {
      addLight(light);
      light = static_cast<DNT::LightInfo*>(light->getNext());
   }

   /* As map editor will load all models as dynamic, make sure no
    * static models were loaded. */
   assert(map->getStaticThings()->getTotal() == 0);

   /* Define our dynamic things */
   DNT::Thing* thing = static_cast<DNT::Thing*>(
         map->getDynamicThings()->getFirst());
   for(int i = 0; i < map->getDynamicThings()->getTotal(); i++)
   {
      addThing(thing);
      thing = static_cast<DNT::Thing*>(thing->getNext());
   }

   /* Add our NPCs */
   DNT::Character* npc = static_cast<DNT::Character*>(
         DNT::Game::getNpcs()->getFirst());
   for(int i = 0; i < DNT::Game::getNpcs()->getTotal(); i++)
   {
      addThing(npc);
      npc = static_cast<DNT::Character*>(npc->getNext());
   }
}

/************************************************************************
 *                       setSelectedNodeByData                          *
 ************************************************************************/
void NodesWindow::setSelectedNodeByData(void* data)
{
   if(window)
   {
      nodesTree->selectNodeByData(data);
   }
}

/************************************************************************
 *                          removeNodeByData                            *
 ************************************************************************/
void NodesWindow::removeNodeByData(void* data)
{
   if(window)
   {
      Farso::TreeView::TreeViewElement* el = nodesTree->getNodeByData(data);
      assert(el != NULL);
      nodesTree->remove(el);
   }
}

/************************************************************************
 *                              unselect                                *
 ************************************************************************/
void NodesWindow::unselect()
{
   if(window)
   {
      nodesTree->unselect();
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

