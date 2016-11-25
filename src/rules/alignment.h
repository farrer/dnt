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

#ifndef _dnt_alignment_h
#define _dnt_alignment_h

#include <kobold/kstring.h>
#include <kobold/list.h>
#include <farso/controller.h>

namespace DNT
{

/*! Single Alignment definition. */
class Alignment: public Kobold::ListElement
{
   public:

      /*! Constructor */
      Alignment(int id, Kobold::String strId);
      /*! Destructor */
      ~Alignment();

      /*! \return Alignment's integer identifier */
      int getId();

      /*! \return Alignment's String identifier */
      Kobold::String getStringId();

      /*! Load Alignment's image.
       * \param imageFilename filename of the image to load 
       * \note should be only called once. */
      void loadImage(Kobold::String imageFilename);
      /*! \return Alignment's image */
      Farso::Surface* getImage();

      /*! Set Alignment's name.
       * \param name name to set */
      void setName(Kobold::String name);
      /*! \return Alignment's name */
      Kobold::String getName();

      /*! Set Alignment's Description.
       * \param desc desctiption to set */
      void setDescription(Kobold::String desc);
      /*! \return Alignment's description */
      Kobold::String getDescription();

   private:

      int intID;              /**< Integer Identifier of the Alignmentment */
      Kobold::String strID;   /**< String Identifier of the Alignmentment */
      Farso::Surface* image;  /**< Image of the Alignment */

      Kobold::String name;            /**< Name of the Alignment */
      Kobold::String description;     /**< Description of the Alignment */
};

/**! The Static List of Alignments */
class Alignments
{
   public:
      /*! Initialize the Alignment's list (loading it from disk) */
      static void init();
      /*! Finish the use of the Alignment's list. */
      static void finish();

      /*! Get an Alignment from list
       * \param id -> integer ID of the Alignmentment
       * \return aligment found, if found one. */
      static Alignment* getAlignByInteger(int id);

      /*! Get an Alignment from list 
       * \param id -> string ID of the Alignmentment
       * \return aligment found, if found one. */
      static Alignment* getAlignByString(Kobold::String id);
      
   private:
      static Kobold::List list;

      /*! Insert an Alignmentment on List
       * \param filename -> fileName of the Alignmentment to insert 
       * \param idString -> String Identificator of the Alignmentment 
       * \param idInt -> Integer Identificator of the the Alignmentment */
      static void insertAlign(Kobold::String filename, Kobold::String idString, 
            int idInt);
};

}

#endif
