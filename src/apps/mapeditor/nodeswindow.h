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

         /*! Open metadata window (closing it if already opened before 
          *  opening again). */
         void open();

         /*! Close the all metadata windows if some are opened */
         void close();

         /*! Check current Farso::Events for mai gui 
          * \return true if should quit the editor. */
         bool checkEvents(PositionEditor* positionEditor);

         /*! Set the current selected node by its data
          * \param data pointer to the data to define the node */
         void setSelectedNodeByData(void* data);

      private:
         /*! Populate nodes with map ones */
         void populateNodes();
         /*! \return node name with counter */
         Kobold::String getNodeName(Kobold::String name);
         
         Farso::Window* window;
         Farso::TreeView* nodesTree;
         Farso::TreeView::TreeViewElement* lightNode;
         Farso::TreeView::TreeViewElement* sceneryNode;
         Farso::TreeView::TreeViewElement* doorsNode;
         Farso::TreeView::TreeViewElement* wallsNode;
         Farso::TreeView::TreeViewElement* pickableNode;

         std::map<Kobold::String, int> namesCount;

     
   };

}

#endif

