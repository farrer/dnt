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

#ifndef _dnt_astar_h
#define _dnt_astar_h

#include "dntconfig.h"
#include <kobold/list.h>
#include <kobold/timer.h>

namespace DNT
{

/*! The A* Point Class */
class PointAStar: public Kobold::ListElement
{
   public:
      float x,           /**< Way Point X Coordinate */
            z;           /**< Way Point Z Coordinate */
      float heuristic;   /**< Calue of the heuristic to this point */
      float gone;        /**< How much percorred to this point */
      float parentX,     /**< Node parent X position */
            parentZ;     /**< Node parent Z position */
};

/*! The A* Point List Class */
class ListAStar: public Kobold::List
{
   public:
      /*! Constructor */
      ListAStar();
      /*! Destructor */
      ~ListAStar();

      /*! Insert Node on List 
       * \param x -> x map position
       * \param z -> z map position
       * \param gone -> distance travelled
       * \param heuristic -> heuristic to the goal 
       * \param parentX -> X position of the parent
       * \param parentZ -> Z position of the parent */
      PointAStar* insert(float x, float z, float gone, float heuristic,
                         float parentX, float parentZ);
      /*! Remove Node from list 
       * \param node -> node to be removed */
      void remove(PointAStar* node);
      /*! Find a specific node on list
       * \param x -> node X position
       * \param z -> node Z position
       * \return pointer to the founded node. NULL if not found */
      PointAStar* find(float x, float z);
      /*! Find the node with lowest value
       * \return pointer to the founded node. NULL if not found */
      PointAStar* findLowest();
      /*! Verify if the list is empty
       * \return true if empty, false otherwise */
      bool isEmpty(){ return getTotal() <= 0; };
      /*! return the size of the List */
      int size(){ return getTotal(); };

};

#define ASTAR_STATE_OTHER     0  /**< A* Other State */
#define ASTAR_STATE_RUNNING   1  /**< A* Running State (searching for path) */
#define ASTAR_STATE_FOUND     2  /**< A* Found Path State */   
#define ASTAR_STATE_NOT_FOUND 3  /**< A* Not Found Path State */

/*! A* implementation. The A* will be searching few nodes per cycle. 
 *  When running, state is RUNNING. When end running, state is
 *  FOUND or NOT_FOUND, based on the result of the search. */
class AStar
{
   public:
      /*! Constructor */
      AStar();
      /*! Destructor */
      ~AStar();

      /*! Calls A* thread to find path
       * \param actor -> the Character that will search for path
       * \param x -> destiny x position
       * \param z -> desired z position
       * \param stepSize -> size of the Step 
       * \param fightMode -> true if is in the fight mode
       * \param forceCall -> true to force the call (there's a counter inner
       *                      the function that don't allow too often calls)
       * */
      void findPath(Character* actor, float x, float z, float stepSize,
            bool fightMode, bool forceCall=false);

      /* Do the a* cycle (if is current searching for something)
       * \param fightMode -> true if is in the fight mode
       * \param isPC -> true if is searching for a playable Character */
      void doCycle(bool fightMode, bool isPC);

      /*! Get the New Character Position, based on Path Found previously 
       * \param posX -> new X position
       * \param posZ -> new Z position
       * \param ori -> new orientation
       * \param fightMode -> true if is in the fight mode
       * \param run -> true if Character is running or false if walking
       * \return true if can actualize, false otherwise */
      bool getNewPosition(float& posX, float& posZ, float& ori,
            bool fightMode, bool run, float runMultiplier);

      /*! Gets the Destiny of The Character
       * \param destX -> X destiny Position
       * \param destZ -> Z destiny Position */
      void getDestiny(float& destX, float& destZ);

      /*! Get Actual State of the aStar and put it on OTHER mode
       * \return state value if the state is nor RUNNING. */
      int getState();

      /*! Set Character orientation value
       * \param ori -> new orientation value */
      void setOrientation(float ori);

      /*! Clear the a* state */
      void clear();

   private:

      /*! Clear search structures */
      void clearSearch();

      float destinyX,           /**< Destiny X position */
            destinyZ;           /**< Destiny Z position */
      PatternAgent* patt;       /**< The Pattern Agent to Follow created path */
      int state;                /**< Internal State of the aStar */
      Character* curActor;      /**< Current Actor */
      float curStepSize;        /**< Current Step Size */
      bool walking;             /**< True if the Character is walking with
                                     at a* at the momment. */

      ListAStar* opened;         /**< Opened nodes list */
      ListAStar* closed;         /**< Closed nodes list */

      float dX, 
            dZ, 
            heuristic,
            maxDist;

      Kobold::Timer callTimer; /**< Timer for subsequent calls */
};

}


#endif

