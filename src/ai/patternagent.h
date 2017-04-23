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

#ifndef _dnt_pattAgent_h
#define _dnt_pattAgent_h

#include <kobold/list.h>
#include "agent.h"

namespace DNT
{

/*! Way Point Definition */
class WayPoint: public Kobold::ListElement
{
   public:
      float x,           /**< Way Point X Coordinate */
            z;           /**< Way Point Z Coordinate */

      float angle;       /**< Angle for the WayPoint to previous one */
};

/*! Pattern Agent */
class PatternAgent: public Agent
{
   public:
      /*! Constructor */
      PatternAgent(bool oriented);
      /*! Destructor */
      ~PatternAgent();

      /*! Defines Next Agent position.
       * \param run -> if character is running or walking
       * \param runMultiplier -> multiplier to apply when running
       * \return true if position was defined */
      bool defineNextPosition(bool run, float runMultiplier);

      /*! Set the pattern origin
       * \param x -> origin X position
       * \param z -> origin Z position */
      void setOrigin(float x, float z);

      /*! Add WayPoint to be the "last" on the Agent */
      void addWayPoint(float x, float z);

      /*! Add WayPoint to be the "first" on the Agent */
      void addWayPointFirst(float x, float z);

      /*! Remove waypoint from the path */
      void removeWayPoint(WayPoint* way);
      /*! Remove all linear way points (they are redundant) */
      void removeLinearWayPoints();

      /*! Get total WayPoints defined to the pattAgent
       * \return -> number of WayPoints */
      int getTotalWayPoints();

      /*! Return the number of units current walked by the agent
       * \return current (meters) walked by the agent */
      float getTotalWalked();

      /*! Get current WayPoint of the pattern agent
       * \return pointer to the WayPoint where the pattAgent is going to. */
      WayPoint* getCurrentWayPoint();

   private:
      Kobold::List wayPoints;     /**< List of WayPoints */
      WayPoint* curWayPoint;      /**< Current Way Point Objective */

      int totalWayPoints;         /**< Total WayPoints on List */

      float totalWalked;          /**< Total units current walked */

      float origX,                /**< Original X position */
            origZ;                /**< Original Z position */
      
      float xInc;                 /**< How many units X coordinate uptade */
      float zInc;                 /**< How many units Z coordinate update */

      /*! Define that the agent will go to next Way Point */
      void changeToNextWayPoint();  

      /*! Calculate the angle value for the waypoint
       * \param way-> the waypoint to calculate angle
       * \param previous -> previous WayPoint */
      void calculateAngle(WayPoint* way, WayPoint* previous);
 
};
                   
}

#endif

