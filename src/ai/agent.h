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

#ifndef _dnt_agent_h
#define _dnt_agent_h

//#define MAX_ROTATION  TURN_VALUE*4

namespace DNT
{

/*! This Class Defines the Generic AI Agent */
class Agent
{
   public:
      /*! Constructor 
       * \param oriented if orientation is relevant for the agent or not */
      Agent(bool oriented);
      /*! Destructor */
      virtual ~Agent();

      /*! Update current values */
      void update();

      /*! Define Destiny 
       * \param x -> x destiny coordinate
       * \param z -> z destiny coordinate */
      void defineDestiny(float x, float z);

      /*! Define Step Size for each update call 
       * \param size -> step size */
      void defineStepSize(float size);

      /*! Define current Agent Position 
       * \param x -> x position coordinate
       * \param z -> z position coordinate */
      void definePosition(float x, float z);

      /*! Get current Agent Position 
       * \param x -> will have X agent coordinate
       * \param z -> will have Z agent coordinate */
      void getPosition(float& x, float& z);

      /*! Define agent's current orientation */
      void defineOrientation(float ori){orientation = ori;};

      /*! \return if orientation is relevant for the agent */
      const bool oriented() const { return withOrientation; };
      /*! \return current orientation angle */
      const float orientationValue() const { return orientation; };
      /*! \return current step size for each update */
      const float getStepSize() const { return stepSize; };

      /*! Do the angle update for the agent.
       * \return if updated angle. */
      bool doAngle();

      /*! Define agent's bouding box */
      void defineBoundingBox(float xa, float za, float xb, float zb);
      /*! Get agent's bouding box */
      void getBoundingBox(float &xa, float &za, float &xb, float &zb);


      /* Virtual Methods */

      /*! Define Next Agent position. Virtual.
       * \param run -> true if running, false if walking
       * \param runMultiplier -> multiplier to apply when running
       * \return true if position was defined */
      virtual bool defineNextPosition(bool run, float runMultiplier)=0;

   protected:

         /* DESTINY */
      float destinyX;      /**< Destiny X  Coordinate */
      float destinyZ;      /**< Destiny Z Coordinate */

         /* POSITION */
      float actualX;       /**< Agent current X Coordinate */
      float actualZ;       /**< Agent current Z Coordinate */
        
         /* BOUNDING BOX */
      float x1;            /**< X1 Bounding Box */
      float z1;            /**< Z1 Bounding Box */
      float x2;            /**< X2 Bounding Box */
      float z2;            /**< Z2 Bounding Box */

         /* STEP */
      float stepSize;      /**< Agent Step Size per update */

         /* ORIENTATION */
      bool withOrientation;  /**< If agent has orientation or not */
      float orientation;   /**< Agent orientation angle, if has */
      float desiredAngle;  /**< The desired angle to the agent.
                                  Only start walk after orientation 
                                  is in this angle. */
};

}

#endif

