


#ifndef _dnt_non_playable_character_h
#define _dnt_non_playable_character_h

#include "character.h"

namespace DNT
{

/*! A Character controlled by the computer master (ie: AI). */
class NonPlayableCharacter : public Character
{
   public:

      /*! State of the NPC to the playable character(s) */
      enum PsychoState
      {
         PS_HOSTILE = 0,
         PS_NEUTRAL = 1,
         PS_FRIENDLY = 2
      };


   protected:

      /*! Parse key/value pairs related to the NonPlayableCharacter */
      bool doCharacterSpecializationParse(Ogre::String key, Ogre::String value);

      PsychoState psychoState; /**< Psycho state of the thing relative to
                                    current PCs */

      //Conversation* conversation; /**< NPC possible conversation */

};

}


#endif



