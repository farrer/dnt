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

#ifndef _dnt_module_h_
#define _dnt_module_h_

#include "dntconfig.h"
#include "../script/modulescript.h"
#include <farso/progressbar.h>

namespace DNT
{
 
   /*! A Module of the engine. Each module has its own models, rules, skins,
    * maps, etc., being able to implement completely different game experiences
    * on top of DNT engine. */
   class Module
   {
      public:
         /*! Constructor
          * \param filename file with the ModuleController script 
          *        implementation. */
         Module(const Kobold::String& filename);
         /*! Destructor */
         ~Module();

         /*! Do a cycle of initialization of all base data directories 
          * defined by the Module, and by its skin and rules. 
          * \note This function should be called until returns true.
          * \param changeSkin if should change skin if it's time or not.
          * \param dataPath path to the base module's data directory
          * \return true if finished, false if should be called again on next
          *         cycle */
         bool doCycleInit(bool changeSkin, const Kobold::String& dataPath,
               Farso::ProgressBar* progressBar);

         /*! \return total cycles needed to init */
         const int getTotalInitCycles() const { return totalInitCycles; };

         /*! Load the initial defined map 
          * \return if successful */
         bool loadInitialMap();

      private:
         ModuleScriptInstance* script; /**< ModulerController script */
         Ogre::vector<Kobold::String>::type dirs; /**< Vector of base dirs */
         int curLoad; /**< Current loaded at cycle init */
         Farso::ProgressBar* progressBar; /**< For load signal */
         int totalInitCycles; /**< Total Cycles needed to init */
   };

}

#endif

