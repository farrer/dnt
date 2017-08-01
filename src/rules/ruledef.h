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

#ifndef _dnt_ruledef_h
#define _dnt_ruledef_h

#include "dntconfig.h"

#include <kobold/kstring.h>
#include <kobold/list.h>
#include <farso/controller.h>

namespace DNT
{

/*! Define a way to group rules of the same type. For example, on a d20-like
 * system should be 6 rule groups: Class, Race, Alignment, Attribute,
 * Skill and Feat. */
class RuleGroup : public Kobold::ListElement
{
   public:
      /*! Constructor.
       * \param id unique string identifying the group */
      RuleGroup(Kobold::String id);
      /*! Destructor */
      ~RuleGroup();

      /*! Possible types of the rule group */
      enum RuleGroupType
      {
         /*! Thing always have it and assign values to it. A d20 Skill
          * is of this type. */
         TYPE_VALUE,
         /*! Thing select to have it or not. A d20 Feat or Alignment 
          * could be of this type */
         TYPE_SELECTABLE,
         /*! Thing select to have it or not and latter could apply values.
          * A d20 Class is of this type */
         TYPE_LEVEL_SELECTABLE,
         /*! Thing will always have this, but won't select or set values, 
          * as those should be precalculated */
         TYPE_CALCULATED
      };

      /*! Define current group type.
       * \note usually called once.
       * \param its type. */
      void setType(const RuleGroupType type);
      /*! \return current group type */
      const RuleGroupType& getType() const { return type; };

      /*! \return group identifier */
      const Kobold::String& getId() const { return id; };

      /*! Set its name */
      void setName(Kobold::String name);
      /*! \return group name */
      const Kobold::String& getName() const { return name; };

      /*! Set its description */
      void setDescription(Kobold::String desc);
      /*! \return its description */
      const Kobold::String& getDescription() const { return description; };

      /*! Add a rule definition to the group */
      void insert(RuleDefinition* ruleDef);
      
      /*! \return a RuleDefinition within this group */
      RuleDefinition* getDefinition(const Kobold::String id);

      /*! Populate the groupInfo with all the RuleDefinitions of this group.
       * \param groupInfo pointer to the RuleGroupAvailableInfo relative to the
       *        same group as this one.
       * \param groupsInfo pointer to the list of RuleGroupAvailableInfo,
       *        for related search. */
      void populate(RuleGroupAvailableInfo* groupInfo, 
            Kobold::List* groupsInfo);

      /*! \return the script object relative to the group */
      ScriptObjectRuleGroup* getScriptObject() { return scriptObject; };

   private:
      /*! ScriptObject relative to the group */
      ScriptObjectRuleGroup* scriptObject; 
      RuleGroupType type; /**< Group type */
      Kobold::String id; /**< Group identifier */
      Kobold::String name; /**< Group name */
      Kobold::String description; /**< Group Description */

      Kobold::List definitions; /**< List of RuleDefinition's of the group */
};

/*! A single pre-requisite to have some RuleDefinition. Represented by the
 * required definition and its minimum value */
class RulePreRequisite : public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param requisite required RuleDefiniton 
       * \param minValue minimun value required for the requisite */
      RulePreRequisite(RuleDefinition* requisite, int minValue);
      /*! Destructor */
      ~RulePreRequisite();

      /*! \return required RuleDefinition pointer */
      const RuleDefinition* getRequired() const { return required; };
      /*! \return required RuleDefinition minimum value */
      const int getMinValue() const { return minValue; };

   private:
      RuleDefinition* required; /**< Required RuleDefinition */
      int minValue; /**< Its minimum required value */
};

/*! A single rule definition. Depending on the module, could be a single skill,
 * a single class, a race, or anything else it defines. */
class RuleDefinition : public Kobold::ListElement
{
   public:
      /*! Constructor */
      RuleDefinition(Kobold::String id);
      /*! Destructor */
      ~RuleDefinition();

      /*! Set its group */
      void setGroup(const RuleGroup* ruleGroup);
      /*! \return its group */
      const RuleGroup* getGroup() const {return group;};

      /*! \return RuleDefinition's String identifier */
      const Kobold::String& getId() const { return id; };

      /*! Set a related rule definition to this one */
      void setRelated(const RuleDefinition* ruleDef);
      /*! \return related RuleDefinition, if any */
      const RuleDefinition* getRelated() { return related; };

      /*! Load RuleDefinition's image.
       * \param imageFilename filename of the image to load 
       * \note should be only called once. */
      void loadImage(Kobold::String imageFilename);
      /*! \return RuleDefinition's image */
      Farso::Surface* getImage() { return image; };

      /*! Set RuleDefinition's name.
       * \param name name to set */
      void setName(Kobold::String name);
      /*! \return RuleDefinition's name */
      const Kobold::String& getName() const { return name; };

      /*! Set RuleDefinition's Description.
       * \param desc desctiption to set */
      void setDescription(Kobold::String desc);
      /*! \return RuleDefinition's description */
      const Kobold::String& getDescription() const { return description; };

      /*! Add a prerequisite to the rule definition */
      void addPreRequisite(RulePreRequisite* preRequisite);
      
      //TODO usableAsAction
      //TODO script instance.
      //TODO: function to receive a Thing and check its pre-requisites.

   private:
      Kobold::String id;   /**< String Identifier of the definition */

      const RuleGroup* group; /**< Its group */
      const RuleDefinition* related; /**< A related RuleDefinition */

      Farso::Surface* image;  /**< Image representing the definition */
      Kobold::String name;        /**< Name of the definition */
      Kobold::String description; /**< Description of the definition */

      Kobold::List requisites; /**< Pre-requisites to have it */
};

/*! Represents a RuleDefinition owned by a Thing (or thing). More 
 * specifically, how many points a Thing have for a RuleDefinition. */
class RuleDefinitionValue : public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param def RuleDefinition to which the Thing has some value 
       * \param related pointer to the related RuleDefinitionValue, if any. */
      RuleDefinitionValue(RuleDefinition* def, RuleDefinitionValue* related);
      /*! Destructor */
      ~RuleDefinitionValue();

      /*! Add to the value.
       * \param v value to add to current value */
      void add(int v);
      /*! Set its value
       * \param v new value .*/
      void setValue(int v);

      /*! \return current value */
      const int getValue() const { return value; };

      /*! \return identifier of th RuleDefinitionValue
       * \note it isn't the identifier of the RuleDefinition */
      const Kobold::String& getId() { return id; };

      /*! \return its RuleDefinition pointer */
      RuleDefinition* getDefinition() { return ruleDef; };

      /*! \return ScriptObject pointer relative to this RuleDefinition */
      ScriptObjectRuleDefinition* getScriptObject() { return scriptObject; };

      /*! \return the related RuleDefinitionValue. */
      RuleDefinitionValue* getRelated() { return related;};

   private:
      /*! ScriptObject relative to the definition */
      ScriptObjectRuleDefinition* scriptObject; 

      RuleDefinition* ruleDef; /**< Respective RuleDefinition */
      RuleDefinitionValue* related; /**< Related RuleDefinitionValue */
      int value;/**< How many points the Thing has on the RuleDefinition */
      Kobold::String id; /**< Identifier */

      static int count; /**< Counter for identifier generation */
};

/*! Represents both a list of RuleDefinitionValues of a group and the total 
 * available values to use for select/increase a RuleGroup. For 
 * example, if a class, it represents the current classes the Thing has and
 * how many the user can select for its Thing at this time. 
 * If a skill, how many points it has on any of its skills and how many 
 * remanining it could give now to them, and so on. */
class RuleGroupAvailableInfo : public Kobold::ListElement
{
   public:
      /*! Constructor */
      RuleGroupAvailableInfo(RuleGroup* group);
      /*! Destructor */
      ~RuleGroupAvailableInfo();

      /*! \return the group related */
      RuleGroup* getGroup() const { return group; };
      /*! \return total point available to group */
      const int getTotal() const { return total; };

      /*! Add value to total points available */
      void add(int v);
      /*! Set the total points available */
      void setTotal(int t);

      /*! \return a RuleDefinitionValue for the RuleDefinition id given */
      RuleDefinitionValue* getDefinitionValue(const Kobold::String id);

      /*! Insert a RuleDefinitionValue based on the given RuleDefinition */
      RuleDefinitionValue* insert(RuleDefinition* ruleDef, 
            Kobold::List* groupsInfo);

   private:
      int total; /**< Total available to the group */
      RuleGroup* group; /**< The group which the info is related. */
      Kobold::List defValues; /**< List of RuleDefinitionValues */
};

/*! The group of rules that defines the game. */
class Rules
{
   public:
      /*! Constructor */
      static void init(const Kobold::String filename);
      /*! Destructor */
      static void finish();

      /*! Populate a list of RuleGroupAvailableInfo with all RuleGroups and
       * all RuleDefs that isn't selectable */
      static void populate(Kobold::List* groupInfoList);

      /*! Find a RuleGroup on the list of groups */
      static RuleGroup* getGroup(const Kobold::String id);

      /*! Find a RuleDefinition within a group */
      static RuleDefinition* getDefinition(const Kobold::String groupId,
            const Kobold::String id);
      /*! Find a Rule Definition without knowing its group */
      static RuleDefinition* getDefinition(Kobold::String id);

   private:
      /*! Check if the info if not null */
      static void checkNotNull(void* info, const Kobold::String& filename, 
            const Kobold::String key, const Kobold::String infoDecl);
      
      /*! Check if RuleDefinition either is null or have a group defined */
      static void checkIsNullOrHaveGroup(RuleDefinition* curDef, 
            const Kobold::String& filename);

      /*! Load the rules from a file. */
      static bool load(const Kobold::String filename);

      static RuleScriptInstance* scriptInstance; /**< Rule Script instance */
      static Kobold::List* groups; /**< List of group of rules definitions */
};

}

#endif

