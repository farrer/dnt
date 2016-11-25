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

#ifndef _dnt_skills_h
#define _dnt_skills_h

#include "ruledef.h"

namespace DNT
{

/*! The individual Skill definition. 
 * \note Skills are also used for Attributes (argh). */
class SkillDefinition : public RuleDefinition
{
   public:
      /*! Constructor */
      SkillDefinition();
      /*! Destructor */
      ~SkillDefinition();

      /*! \return skill's base attribute identifier */
      int getBaseAttributeId();
      /*! Set identifier of skill's base attribute */
      void setBaseAttributeId(int id);

      /*! \return if the definition is of an attribute (true) or of
       * a common Skill (false). */
      bool isAttribute();
      /*! Define the skill as an attribute */
      void setAsAttribute();

   private:
      int baseAttribute;    /**< Base Attibute */
      bool isAttr;     /**< True if the Skill is an attribute */
};

/*! The individual Skill representation (used to store current Thing's points
 * on a skill, it's modifier, etc.) */
class Skill
{
   public:
      /*! Constructor */
      Skill();
      /*! Destructor */
      ~Skill();

      /*! Set the definition of this skill.
       * \param def pointer to the SkillDefinition of this Skill */
      void setDefinition(SkillDefinition* def);

      /*! \return SkillDefinition of this skill. */
      SkillDefinition* getDefinition();

      /*! Store current points. Usually called before editing skill points */
      void storeCurrentPoints();
      /*! Restore current points to the last stored ones. Usually called when
       * editing skill points and called 'cancel' action. */
      void restoreLastStoredPoints();

      /*! \return current skill points */
      int getPoints();
      /*! Set current skill points
       * \param points number of points */
      void setPoints(int points);

      /*! \return needed 'total points' to add a single point at this skill. */
      int getModifier();
      /*! Set modifier increment for this skill.
       * \param mod 'total points' cost to add a single point on this skill */
      void setModifier(int mod);

   private:

      int points;             /**< Total Skill points */
      int prevPoints;         /**< Previous points, to be used when adding 
                                   to be restored on cancel. */
      int mod;                /**< Needed points to add +1 on 'points' */
      SkillDefinition* definition; /**< Pointer to definition of this Skill */
};

/*! The static list of Skill definitions. All game Skills Definitions 
 * are here. */
class SkillsDefinitions
{
   public:
      /*! Init the the Skills Definitions, loading them all from the
       * definition file Skills/Skills.lst */
      static void init();

      /*! Finish the use of the Skills definitions,
       * freeing the memory of them all */
      static void finish();

      /*! Get total Skills */
      static int getTotalSkills();

      /*! Return the wanted Skill definition
       * \param id -> Skill's String identifier
       * \return -> pointer to the SkillDefinition found (or NULL) */
      static SkillDefinition* getSkillDefinition(Kobold::String id);

      /*! Get an specific Skill definition
       * \param i -> index (id) of the Skill
       * \return -> pointer to the Skill description or NULL */
      static SkillDefinition* getSkillDefinition(int i);

   protected:
      static SkillDefinition* skillsDefs;
      static int totalSkills;
  
};

/*! The Skills of a Character or Thing. It has the pointers to the
 * definitions needed. */
class Skills
{
   public:
      /*! Skills contructor. Read all Skills from file.
       *  \param dir -> directory of the Skills files
       *  \param arq -> filename of the Skills list. */
      //Skills(string dir, string arq); 
      /*! Skills contructor. Read all Skills from a Skills. Do not load 
       *  descriptions. 
       *  \param sk -> other Skills pointer.
       */
      Skills(Skills* sk);
      /*! Skills contructor. Read all Skills from a Skills. Do not load 
       *  descriptions and images.
       */
      Skills();
      /*! Skills destructor. */
      ~Skills();

      /*! Return the wanted Skill
       * \param idString -> String Identifier of the Skill 
       * \return -> pointer to the Skill */
      Skill* getSkillByString(Kobold::String idString);
      
      /*! Return the wanted Skill
       * \param i -> integer representing the Skill 
       * \return -> pointer to the Skill */
      Skill* getSkillByInt(int i);

      /*! Do a Skill check (getting the value got at the check,
       *  to compare later with another Skill check or a fixed value) 
       *  \param idString -> String identifier of the Skill
       *  \param difficulty -> check difficulty
       *  \return -> true if success, false if fail */
      bool doSkillCheck(Kobold::String idString, int difficulty);

      /*! Do a Skill check (getting the value got at the check,
       *  to compare later with another Skill check or a fixed value) 
       *  \param sk -> pointer to the Skill to check 
       *  \param difficulty -> check difficulty
       *  \return -> true if succes, false if fail */
      bool doSkillCheck(Skill* sk, int difficulty);

      /*! set available points
       * \param points -> availablePoints */
      void setAvailablePoints(int points);

      /*! get available points
       * \return availablePoints */
      int getAvailablePoints();

      /*! clear all Skills points */
      void clear();

      /*! clear Skill costs */
      void clearCosts();

      /*! Get Total Skills on the list
       * \return -> number of Skills */
      int getTotalSkills();

   private:
     Skill* skillsVector;     /**< the internal Skills */

     int totalSkills;         /**< Total Number of Skills on List */
     int availablePoints;       /**< Total Available Points on List */
};

}

#endif
