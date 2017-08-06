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

#ifndef _dnt_map_editor_vector3_text_entry_h
#define _dnt_map_editor_vector3_text_entry_h

#include <farso/controller.h>
#include <OGRE/OgreVector3.h>

namespace DNTMapEditor
{
   /*! A widget to edit a float vector of 3 elements. It only generate a 
    * single event, when done editing some of its text entry components. */
   class Vector3TextEntry : public Farso::Widget
   {
      public:
         enum Vector3TextEntryType
         {
            /*! x,y,z */
            TYPE_AXIS,
            /*! r,g,b */
            TYPE_RGB
         };
         /*! Constructor */
         Vector3TextEntry(int x, int y, Vector3TextEntryType type, 
               Kobold::String label, Farso::Widget* parent);
         /*! Destructor */
         ~Vector3TextEntry();

         /*! \return parent's body */
         Farso::Rect getBody() { return body; };
         /*! Set dirty. */
         void setDirty();
         /*! enable the widget (and its children) */
         void enable();
         /*! disable the widget (and its children) */
         void disable();

         /*! Set current value */
         void setValue(const Ogre::Vector3& val);
         /*! Set current value */
         void setValue(const Ogre::ColourValue& val);
         /*! Set current value */
         void setValue(const float x, const float y, const float z);
         /*! \return current value */
         const Ogre::Vector3& getValue() {return value;};

      protected:
         /*! Draw the widget */
         void doDraw(Farso::Rect pBody);
         /*! Treat widget events */
         bool doTreat(bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY, int mrX, int mrY);
         /*! Called after the children were treat */
         void doAfterChildTreat();
         /*! Parse a value from text entry text */
         float parse(Farso::TextEntry* textEntry, float previousValue);

      private:
         Farso::TextEntry* textEntry[3]; /**< The text entry */ 
         Farso::Rect body; /**< Our body */
         Ogre::Vector3 value; /**< Current text value */

   };
}

#endif

