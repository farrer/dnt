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

#ifndef _dnt_feats_h
#define _dnt_feats_h

#include "dntconfig.h"

#include "ruledef.h"
#include "actions.h"
#include "modifier.h"

#include <kobold/kstring.h>
#include <kobold/list.h>

namespace DNT
{

#define MAX_FEATS          30 /**< Max number of Feats per Character */

#define FEAT_WEAPON_ATTACK   0 /**< Weapon Attack */

/*! The Feature general definitions */
class FeatDescription : public RuleDefinition
{
   public:

      /*! Dependence Feat information. A dependent feat is a feat that have its
       * quantity affected by the use of the owner Feat. \par
       * For Example: 
       * when use \e ameivasII you'll can do -2 \e ameivaI. (1/2 reason). */
      class DepFeat: public Kobold::ListElement
      {
         public:
            /*! Constructor
             * \param reason dependence reason (how much the use of this feature
             *        will affect availability of dependent feat).
             * \param featId identifier of the dependent feat */
            DepFeat(float reason, Kobold::String featId);
            /*! Destructor */
            ~DepFeat();

            /*! \return dependence reason */
            float getReason();

            /*! \return dependent feat string identifier */
            Kobold::String getFeatId();

         private:
            float reason; /**< Dependence Reason (1/1, 1/2, 1/3, 2/1, etc) */
            Kobold::String featId; /**< ID String of the Feat */
      };

      /*! Required Factor: information about some Feat requeriment. */
      class ReqFactor: public Kobold::ListElement, public Factor
      {
         public:
            /*! Constructor */
            ReqFactor(Kobold::String type, Kobold::String id, int level);
            /*! Destructor */
            ~ReqFactor();

            /*! \return level of the factor required */
            int getLevel();

         private:
            int requiredLevel;      /**< Factor level required */
      };

      enum FeatType
      {
         FEAT_TYPE_PERMANENT=0,  /**< Permanent Feat (infinite ModEffect) */
         FEAT_TYPE_ON_TARGET,    /**< Use-on-target Feat */
         FEAT_TYPE_ON_AREA,      /**< Use-on-area Feat */
         FEAT_TYPE_TOTAL
      };


      /*! Constructor */
      FeatDescription();
      /*! Destructor */
      ~FeatDescription();
      
      /*! Load current description from file 
       * \param filename name of the file to load
       * \param index index on feats vector
       * \return success flag */
      bool load(Kobold::String filename, int index);

      /*! \return number of available uses per day */
      int getQuantityPerDay();

      /*! \return cost to use this feat */
      int getCostToUse();

      /*! \return pointer to the dependency feats */
      Kobold::List* getDepFeats();

      /*! \return script filename who define the feat act */
      Kobold::String getScriptFile();

      /*! \return feat action range */
      int getRange();

      /*! \return actionId of the feat */
      Action::ActionId getActionId();

      /*! \return power level. The greater, the powerful */
      int getPowerLevel();

   private:

      FeatType type;               /**< Feature type constant */
      Action::ActionId action;     /**< Feat action identifier */
      bool selectable;             /**< If Feat is selectable or auto-assigned*/

      Kobold::String scriptFile;   /**< Filename of the script to run */

      int powerLevel;              /**< Feat power level (to compare them) */
      
      int costToUse;               /**< Cost (in quantity) to use the Feat */
      int quantityPerDay;          /**< Quantity avaible to use per day*/
      int aditionalQuantity;       /**< Quantity Added per AditionalLevel */
      int aditionalLevel;          /**< Number of Levels to AditionalQuantity */
      int aditionalDiv;            /**< The division Factor */
      Factor* aditionalFactor;     /**< The Factor that will allows aditional */

      int range;                   /**< Range action of the Feat */
      int radius;                  /**< Radius of the Feat, if any */

      Kobold::List reqFactors;     /**< List of required Factors */
      Kobold::List depFeats;       /**< Feat Dependency list */
};

/*! The Feat is the internal Feat definition per characters,
 * with only the requerided fields to avoid loss memory with
 * unnecessary data. */
class Feat
{
   public:
      /*! Constructor */
      Feat();
      /*! Destructor */
      ~Feat();

      /*! Set Feat Description 
       * \param info pointer to the FeatDescription of this Feat. */
      void setFeatDescription(FeatDescription* info);
      /*! \return pointer to the FeatDescription of this Feat */
      FeatDescription* getInfo();

      /*! \return current quantity available to use */
      float getQuantity();
      /*! Set current quantity */
      void setQuantity(int value);
      /*! Decrement value from current quantity available to use from it
       * and from its dependent feats. */
      void decQuantity(int value, Feats* feats);
      /*! Reset quantity value, to max per day use. */
      void resetQuantity();

   private:
      FeatDescription* info;       /**< The Feat info */
      float quantity;  /**< Current quantity to use */
};

/*! the class Feats defines the Character's Feats,
 * and its functions to up-to-date, use, etc. */
class Feats               
{
   public:
      /*! Feats Constuctor */
      Feats();
      /*! Feats Destructior */
      ~Feats();
      
      /*! Clear all Feats */
      void clear();

      /*! Get total Feats */
      int getTotal();

      /*! Return the Feat with number featNumber  
       * \param featNumber -> internal number of Feat to return
       * \return pointer to the found Feat or NULL. */
      Feat* getFeatByNumber(int featNumber);

      /*! Return the Feat with name FeatName
       * \param featName -> name of Feat to return
       * \return pointer to the found Feat or NULL. */
      Feat* getFeatByString(Kobold::String featName);

      /*! Insert a Feat on Character's Feats.
       * \param feat -> FeatDescription of Feat to insert
       * \return \c true if sucefully inserted. */
      bool insert(FeatDescription* feat);

      /*! Refresh Quantities to use on a new day to all Feats. */
      void newDay();

      /*! Get a random attack Feat that can be used.
       * \param pers -> character that will use the Feat
       * \param target -> Thing that will receive the Feat.
       * \return \c number of the Feat got */
      int getRandomNPCAttackFeat(Thing* pers, Thing* target);

      /*! Get the powerfull attack Feat that can be used.
       * \param pers -> character that will use the Feat
       * \param target -> Thing that will receive the Feat.
       * \return \c number of the Feat got */
      int getPowerfullAttackFeat(Thing* pers, Thing* target);

      /*! Get the first heal Feat available
       * \param pers -> Thing owner of the Feat
       * \return Feat Number or -1 if not found */
      int getFirstHealFeat(Thing* pers);

      /*! Get a random heal Feat available
       * \param pers -> Thing owner of the Feat
       * \return Feat Number or -1 if not found */
      int getRandomHealFeat(Thing* pers);

      /*! Get the powerfull heal Feat available
       * \param pers -> Thing owner of the Feat
       * \return Feat Number or -1 if not found */
      int getPowerfullHealFeat(Thing* pers);

      /*! Use a Feat at a target Thing
       * \param actor -> Thing owner of the Feat
       * \param featNumber -> number of the Feat to use
       * \param target -> target to use the Feat 
       * \return true if used the Feat, false if couldn't */
      bool useFeatAtTarget(Thing* actor, int featNumber, Thing* target);

      /*! Use a Feat at a target area
       * \param actor -> Thing owner of the Feat
       * \param featNumber -> number of the Feat to use
       * \param x -> X coordinate to use the Feat
       * \param y -> Y coordinate to use the Feat
       * \param z -> Z coordinate to use the Feat
       * \return true if used the Feat, false if couldn't */
      bool useFeatAtArea(Thing* actor, int featNumber, 
             float x, float y, float z);

      /*! Apply a permanent Feat to the owner
       * \param actor -> Thing owner of the Feat
       * \param featNumber -> number of the Feat to use
       * \return true if used the Feat, false if couldn't */
      bool applyPermanentFeat(Thing* actor, int featNumber);

      /*! Verify if can use the Feat
       * \param featNumber -> number of the Feat
       * \return true if can use */
      bool canUse(int featNumber);
      /*! verify if can use the Feat
       * \param f -> Feat pointer to test
       * \return true if can use */
      bool canUse(Feat* f);

   private:

      Feat feats[MAX_FEATS];  /**< Internal Feats Struct */
      int  totalFeats;        /**< Actual Number of Feats */

      /*! Use a quantity of the Feat and propagate to other dependent 
       * Feats.
       * \param featNumber -> Feat to use */
      void useFeat(int featNumber);
};

/*! List of All Feats on Game */
class FeatsList           
{
   public:
      /*!
       ***************************************************************
       * FeatsList Constructor
       * \param dir -> string with the directory where all Feats are
       * \param arq -> file name with the list of avaible Feats.
       ***************************************************************/
      static void init(Kobold::String dir, Kobold::String arq);
      /*!
       ***************************************************************
       * FeatsList Destructor
       ***************************************************************/
      static void finish();

      /*!
       ************************************************************** 
       * Return the FeatDescription with idString FeatName
       * \param FeatName -> name of Feat to return
       * \return the FeatDescription struct.
       ***************************************************************/
      static FeatDescription* getFeatByString(Kobold::String FeatName);

      /*!
       **************************************************************** 
       * Return the Feat with number featNumber  
       * \param featNumber -> number of Feat to return
       * \return the FeatDescription struct.
       ***************************************************************/
      static FeatDescription* getFeatByNumber(int featNumber);

      /*! Get the total number of Feats */
      static int getTotal();

   private:
      static FeatDescription* feats; /**< Internal Desc Struct */
      static int totalFeats;         /**< Total Declared Feats */
};

}

#endif

