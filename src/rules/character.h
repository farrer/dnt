
#ifndef _dnt_character_h
#define _dnt_character_h

#include "thing.h"

namespace DNT
{

/*! A Character is a living Thing. Coulb be playable by human 
 * (PlayableCharacter, or PC) or by the AI (NonPlayableCharacter, aka NPC). */
class Character : public Thing
{
   public:
      /*! Constructor */
      Character();
      /*! Destructor */
      virtual ~Character();

   protected:

      /*! Parse key/value pairs specific to the character thing's 
       * specialization */
      bool doSpecificParse(Ogre::String key, Ogre::String value);

      /*! Parse specifc key/value pair readed from definition's file that
       * doesn't belong to the generic character specification (but to its
       * specialization). */
      virtual bool doCharacterSpecializationParse(Ogre::String key, 
            Ogre::String value) = 0;


      bool dead; /**< If the character is actually dead (just a corpse). */
};

}

#endif

