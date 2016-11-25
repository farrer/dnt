

#ifndef _dnt_playable_character_h
#define _dnt_playable_character_h

#include "character.h"

namespace DNT
{

/*! A Character controlled by the human player */
class PlayableCharacter : public Character
{
   public:

   protected:

      /*! Parse key/value pairs related to the PlayableCharacter */
      bool doCharacterSpecializationParse(Ogre::String key, Ogre::String value);

      int upLevels; /**< Number of levels the character can already advance */
      int xp;       /**< Current number of experience points */

      bool canMove;   /**< If still have movements at current fight's turn */
      bool canAttack; /**< If still have actions at current fight's turn */
};

}


#endif


