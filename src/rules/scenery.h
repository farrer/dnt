
#ifndef _dnt_scenery_h
#define _dnt_scenery_h

#include "object.h"

namespace DNT
{

/*! The Scenery is an static not living Thing. Just compose the ambience,
 * but usually couldn't do anything more to it than that. */
class Scenery : public Object
{
   public:
      Scenery();
      ~Scenery();

   protected:

      bool doObjectSpecializationParse(Ogre::String key, Ogre::String value);

};

}

#endif

