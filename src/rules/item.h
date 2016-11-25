
#ifndef _dnt_item_h
#define _dnt_item_h

#include "object.h"

namespace DNT
{

/*! The Item is an interactive object. Could be pickable or not by 
 * PlayableCharacters. */
class Item : public Object
{
   public:
      Item();
      ~Item();

   protected:
      
      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);

};

}

#endif

