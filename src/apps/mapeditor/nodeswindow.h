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

#ifndef _dnt_map_editor_nodes_window_h
#define _dnt_map_editor_nodes_window_h

#include <farso/controller.h>
#include <map>
#include "positioneditor.h"
#include "../../map/light.h"

namespace DNTMapEditor
{
   /*! Window that keeps all map nodes for selection */
   class NodesWindow
   {
      public:
         /*! Constructor */
         NodesWindow();
         /*! Destructor */
         ~NodesWindow();

         /*! Open nodes window (closing it if already opened before 
          *  opening again). */
         void open();

         /*! Close the node window, if opened. */
         void close();

         /*! \return if node window is opened or nor */
         const bool isOpened() const { return window != NULL; };

         /*! Add a thing to the node's tree 
          * \note window must be opened */
         void addThing(DNT::Thing* thing);
         /*! Add a light to the node's tree.
          * \note window must be opened */
         void addLight(DNT::LightInfo* light);

         /*! Check current Farso::Events for nodes window .
          * \return true treated some event. */
         bool checkEvents(PositionEditor* positionEditor);

         /*! Set the current selected node by its data
          * \param data pointer to the data to define the node */
         void setSelectedNodeByData(void* data);

         /*! If the window is opened, remove a node defined by its data 
          * \param data pointer to the node's data.
          * \note It will not remove the data, just the tree's node. */
         void removeNodeByData(void* data);

         /*! Unselect any selected node if the window is opened */
         void unselect();

      private:
         /*! Populate nodes with map ones */
         void populateNodes();
         /*! \return node name with counter */
         Kobold::String getNodeName(Kobold::String name);
         
         Farso::Window* window; /**< The node window */
         Farso::TreeView* nodesTree; /**< A selector tree */
         Farso::TreeView::TreeViewElement* lightNode; /**< Light subtree */
         Farso::TreeView::TreeViewElement* sceneryNode; /**< Scenery subtree */
         Farso::TreeView::TreeViewElement* doorsNode; /**< Door subtree */
         Farso::TreeView::TreeViewElement* wallsNode; /**< Wall subtree */
         Farso::TreeView::TreeViewElement* pickableNode; /**< Pickable tree */
         Farso::TreeView::TreeViewElement* npcsNode; /**< Npcs tree */

         std::map<Kobold::String, int> namesCount; /**< Map for same name */

     
   };

}

#endif

