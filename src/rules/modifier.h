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

#ifndef _dnt_modifier_h
#define _dnt_modifier_h

#include "dntconfig.h"

#include <kobold/kstring.h>

namespace DNT
{

#define MOD_TYPE_ALL   "ALL"

#define MOD_TYPE_ATT   "ATTRIBUTE"
#define MOD_TYPE_SKILL "SKILL"
#define MOD_TYPE_THING "THING"

#define MOD_TYPE_CLASS "CLASS"
#define MOD_TYPE_RACE  "RACE"
#define MOD_TYPE_ALIGN "ALIGN"

/**! Factor - defines where a Modifier will act or which is the actor 
 * (for example, if will act with a Skill because of Thing's Race, etc). */
class Factor
{
   public:
      /*! Constructor 
       * \param type of the factor (ex: MOD_TYPE_SKILL, MOD_TYPE_RACE)
       * \param id identifier of the factor affected (ex: "STRENGTH") */
      Factor(Kobold::String type, Kobold::String id);
      /*! Destructor */
      ~Factor();

      /*! \return factor type */
      Kobold::String getType();

      /*! \return identifier of the actuation */
      Kobold::String getId();

   private:

      Kobold::String type; /**< Type of the Factor (ex: Skill, Feat, Class) */
      Kobold::String id;   /**< ID of the Factor (ex: Intimidation, etc) */
};

/*! A modifier modifier */
class Modifier
{
   public:
      /*! Constructor */
      Modifier();
      /*! Destructor */
      ~Modifier();

      /*! Define the modifier */
      void set(int value, Kobold::String causeType, Kobold::String causeId,
               Kobold::String targetType, Kobold::String targetId);
      /*! Parse the modifier 'set' with an string as %d %s %s %s %s
       * (value causeType causeId targetType targetId). 
       * \return success indicator. */
      bool parse(Kobold::String str);

      /*! \return modifier's value */
      int getValue();

      /*! \return factor representing the modifier's cause */
      Factor* getCause();

      /*! \return factor representing the modifier's target */
      Factor* getTarget();

      /*! \return modifier description */
      Kobold::String getDescription();
      /*! Set description
       * \param desc description */
      void setDescription(Kobold::String desc);

   private:
      int value;                  /**< Value of the Modifier */
      Factor* cause;              /**< Cause of the Modifier */
      Factor* target;             /**< Target when modifer is valid */
      Kobold::String description; /**< Modifier Description */
};


/*! Number of Skill Points, based on ATTRIBUTES per level.
 * The calculation formula is (value Operation ATTModifier) * mult */
class LevelPoints
{
   public:
      /*! Constructor */
      LevelPoints();
      /*! Destructor */
      ~LevelPoints();

      /*! Define value by a string "( %d %s %s )x%d" 
       * \param str string to parse.
       * \return success flag */
      bool parse(Kobold::String str);

      /*! Calculate the defined points, base on current attributes / skills.
       * \return number of points. */
      int calculate(Skills* skills);

   private:

      /*! The operation for level ponts calculation */
      enum Operation
      {
         OPERATION_DEC,
         OPERATION_SUM
      };



      /*! Set values */
      void set(int value, Kobold::String op, Kobold::String attId, int mult);

      int value;     /**< Base value of the operation */
      Operation op;  /**< Operation used (- or +) */
      Kobold::String attId; /**< Identifier of attribute (or skill) to use */
      int mult;      /**< Value of Multiplier */
};

}

#endif

