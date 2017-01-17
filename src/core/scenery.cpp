#include "scenery.h"

using namespace DNT;

/**************************************************************************
 *                               Constructor                              *
 **************************************************************************/
Scenery::Scenery()
        :Object(Thing::THING_TYPE_SCENERY)
{
}

/**************************************************************************
 *                                Destructor                              *
 **************************************************************************/
Scenery::~Scenery()
{
}

/**************************************************************************
 *                       doObjectSpecializationParse                      *
 **************************************************************************/
bool Scenery::doObjectSpecializationParse(Ogre::String key, Ogre::String value)
{
   return false;
}

/**************************************************************************
 *                       doObjectSpecializationSave                       *
 **************************************************************************/
bool Scenery::doObjectSpecializationSave(std::ofstream& file)
{
   return true;
}

/**************************************************************************
 *                              canInteract                               *
 **************************************************************************/
bool Scenery::canInteract()
{
   return false;
}

/**************************************************************************
 *                                Interact                                *
 **************************************************************************/
Object::InteractResult Scenery::interact(Character* actor)
{
   return Object::INTERACTION_DONE;
}


