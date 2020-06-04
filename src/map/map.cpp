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


#include "map.h"

#include "light.h"

#include "../rules/thing.h"
#include "../core/scenery.h"
#include "../core/door.h"
#include "../core/game.h"
#include "../core/nonplayablecharacter.h"
#include "../lang/translate.h"
#include "../collision/collision.h"
#include "../script/mapscript.h"
#include "../script/scriptmanager.h"
#include "../core/dialog.h"

#include <kobold/ogre3d/ogredefparser.h>
#include <kobold/log.h>
#include <assert.h>

using namespace DNT;

#define MAP_WALL_HEIGHT                 3.2

#define MAP_TOKEN_SIZE                  "size"

#define MAP_TOKEN_SQUARE                "square"
#define MAP_TOKEN_USE_TEXTURE           "useTexture"

#define MAP_TOKEN_NAME                  "name"
#define MAP_TOKEN_MUSIC_FILE            "musicFile"
#define MAP_TOKEN_SCRIPT                "script"

#define MAP_TOKEN_WALL                  "wall"
#define MAP_TOKEN_WALL_TEXTURE_FRONT    "wtf"
#define MAP_TOKEN_WALL_TEXTURE_LEFT     "wtl"
#define MAP_TOKEN_WALL_TEXTURE_RIGHT    "wtr"
#define MAP_TOKEN_WALL_TEXTURE_BACK     "wtb"

#define MAP_TOKEN_THING                 "thing"
#define MAP_TOKEN_THING_POSITION        "thingPos"
#define MAP_TOKEN_THING_ORIENTATION     "thingOri"
#define MAP_TOKEN_THING_WALKABLE        "thingWalkable"

#define MAP_TOKEN_DOOR                  "door"
#define MAP_TOKEN_DOOR_POSITION         "doorPos"
#define MAP_TOKEN_DOOR_ORIENTATION      "doorOri"
#define MAP_TOKEN_DOOR_LOCK             "doorLock"
#define MAP_TOKEN_DOOR_LOCK_DIALOG      "doorLockDialog"

#define MAP_TOKEN_NPC                   "npc"
#define MAP_TOKEN_NPC_POSITION          "npcPos"
#define MAP_TOKEN_NPC_ORIENTATION       "npcOri"
#define MAP_TOKEN_NPC_DIALOG            "npcDialog"
#define MAP_TOKEN_NPC_PSYCHO_STATE      "npcPsycho"

#define MAP_TOKEN_LIGHT                 "light"
#define MAP_TOKEN_LIGHT_POS             "lightPos"
#define MAP_TOKEN_LIGHT_DIR             "lightDir"
#define MAP_TOKEN_LIGHT_DIFFUSE         "lightDiffuse"
#define MAP_TOKEN_LIGHT_SPECULAR        "lightSpecular"
#define MAP_TOKEN_LIGHT_SPOT_RANGE      "lightSpotRange"
#define MAP_TOKEN_LIGHT_ATTENUATION     "lightAttenuation"
#define MAP_TOKEN_LIGHT_POWER_SCALE     "lightPowerScale"
#define MAP_TOKEN_LIGHT_HDR             "lightHdr"
#define MAP_TOKEN_LIGHT_CAST_SHADOWS    "lightCastShadows"

#define MAP_VALUE_LIGHT_POINT           "point"
#define MAP_VALUE_LIGHT_SPOTLIGHT       "spot"
#define MAP_VALUE_LIGHT_DIRECTIONAL     "directional"

#define MAP_TOKEN_INITIAL               "initial"

#define MAP_VALUE_TRUE                  "true"
#define MAP_VALUE_FALSE                 "false"

/**************************************************************************
 *                                Constructor                             *
 **************************************************************************/
Map::Map()
    :floor("floor")
{
   this->name = "";
   this->filename = "";
   this->musicFilename = "";
   this->xSize = 0;
   this->zSize = 0;
   this->staticThings = new Kobold::List(Kobold::LIST_TYPE_ADD_AT_END);
   this->dynamicThings = new Kobold::List(Kobold::LIST_TYPE_ADD_AT_END);
   this->lights = new MapLights();
   this->script = NULL;
   this->scriptFilename = "";
   this->floorMaterials = NULL;
   this->initialPos = Ogre::Vector3(0.0f, 0.0f, 0.0f);
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
Map::~Map()
{
  Collision::finish();
  delete this->staticThings;
  delete this->dynamicThings;
  delete this->lights;
  if(this->floorMaterials)
  {
     delete[] floorMaterials;
  }
  if(this->script)
  {
     Game::getScriptManager()->removeInstance(script);
  }
}

/**************************************************************************
 *                                 update                                 *
 **************************************************************************/
void Map::update(Ogre::Vector3 refPos)
{
   /* Update our dynamic objects */
   Thing* thing = static_cast<Thing*>(dynamicThings->getFirst());
   for(int i = 0; i < dynamicThings->getTotal(); i++)
   {
      thing->update();
      thing = static_cast<Thing*>(thing->getNext());
   }
}

/**************************************************************************
 *                              getFilename                               *
 **************************************************************************/
Kobold::String Map::getFilename()
{
   return filename;
}

/**************************************************************************
 *                                 getName                                *
 **************************************************************************/
Kobold::String Map::getName()
{
   return name;
}

/**************************************************************************
 *                                 setName                                *
 **************************************************************************/
void Map::setName(Kobold::String name)
{
   this->name = name;
}

/**************************************************************************
 *                           setScriptFilename                            *
 **************************************************************************/
void Map::setScriptFilename(const Kobold::String& filename)
{
   assert(script == NULL);
   this->scriptFilename = filename;
}

/**************************************************************************
 *                           getMusicFilename                             *
 **************************************************************************/
Kobold::String Map::getMusicFilename()
{
   return musicFilename;
}

/**************************************************************************
 *                           setMusicFilename                             *
 **************************************************************************/
void Map::setMusicFilename(Kobold::String filename)
{
   musicFilename = filename;
}

/**************************************************************************
 *                                create                                  *
 **************************************************************************/
void Map::create(int sizeX, int sizeZ)
{
   /* Define its size */
   xSize = sizeX;
   zSize = sizeZ;

   /* Default initial pos at center */
   initialPos = Ogre::Vector3(xSize / 2.0f, 0.0f, zSize / 2.0f);

   Collision::init(xSize, zSize, MAP_SQUARE_SIZE);
   createAuxiliarEditStructs();

   /* Define the upper wall texture */
   //walls.createTextureMesh(MAP_UPPER_WALL_MATERIAL);

   /* Create the mesh for default texture */
   Kobold::String matName = "concrete6";
   MapSubMesh* mesh = floor.createSubMesh(matName);
   for(int squareZ = 0; squareZ < sizeZ; squareZ++)
   {
      for(int squareX = 0; squareX < sizeX; squareX++)
      {
         floorMaterials[squareZ * xSize + squareX] = matName;
         mesh->addSquare(squareX * MAP_SQUARE_SIZE, 0.0f, 
                         squareZ * MAP_SQUARE_SIZE, 
                         (squareX + 1) * MAP_SQUARE_SIZE, 0.0f,
                         (squareZ + 1) * MAP_SQUARE_SIZE,
                         0.0f, 1.0f, 0.0f);
      }
   }
   floor.updateAllDirty();
   updateAllDirtyWalls();
}

/**************************************************************************
 *                        updateAllDirtyWalls                             *
 **************************************************************************/
void Map::updateAllDirtyWalls()
{
   Wall* wall = static_cast<Wall*>(walls.getFirst());
   for(int i = 0; i < walls.getTotal(); i++)
   {
      wall->updateMeshIfDirty();
      wall = static_cast<Wall*>(wall->getNext());
   }
}

/**************************************************************************
 *                           insertThing                                  *
 **************************************************************************/
Thing* Map::insertThing(Kobold::String filename, bool forceDynamic)
{
   Thing* thing = Game::createObject(filename, forceDynamic);
   if(thing)
   {
      /* Insert it at our SceneNodes x Thing map */
      nodesPerThingMap[thing->getSceneNode()] = thing;
      if(thing->isStatic())
      {
         staticThings->insert(thing);
      }
      else
      {
         dynamicThings->insert(thing);
      }
   }

   return thing;
}

/**************************************************************************
 *                           insertThing                                  *
 **************************************************************************/
Thing* Map::insertThing(Kobold::String filename, bool forceDynamic,
      const Ogre::Vector3& pos, const Ogre::Vector3& ori)
{
   Thing* thing = insertThing(filename, forceDynamic);
   if(thing)
   {
      thing->setPositionNow(pos);
      thing->setOrientationNow(ori);
      Collision::addElement(thing);
   }

   return thing;
}

/**************************************************************************
 *                      createAuxiliarEditStructs                         *
 **************************************************************************/
void Map::createAuxiliarEditStructs()
{
   if(floorMaterials)
   {
      delete floorMaterials;
      floorMaterials = NULL;
   }
   /* Create the auxiliar material name keeper for edit */
   floorMaterials = new Kobold::String[xSize * zSize];
}

/**************************************************************************
 *                                 load                                   *
 **************************************************************************/
bool Map::load(Ogre::String mapFileName, bool fullPath, bool editMode)
{
   Kobold::OgreDefParser parser;
   Ogre::String key, value;
   LightInfo* lastLight = NULL;
   Thing* lastThing = NULL;
   NonPlayableCharacter* lastNpc = NULL;
   Wall* lastWall = NULL;

   this->filename = mapFileName;

   if(!parser.load(mapFileName, fullPath, false))
   {
      return false;
   }

   /* Read size of the map */
   if( (!parser.getNextTuple(key, value)) || (key != MAP_TOKEN_SIZE) )
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Error: Map size not defined at '%s', found '%s' instead of 'size'",
            mapFileName.c_str(), key.c_str());
      return false;
   }
   sscanf(value.c_str(), "%dx%d", &xSize, &zSize);
   assert(xSize > 0);
   assert(zSize > 0);
   Collision::init(xSize, zSize, MAP_SQUARE_SIZE);
   if(editMode)
   {
      createAuxiliarEditStructs();
   }

   /* Reset square position counters */
   int squareX = -1;
   int squareZ = 0;

   float wX1=0.0f, wY1=0.0f, wZ1=0.0f, wX2=0.0f, wY2=0.0f, wZ2=0.0f;

   while(parser.getNextTuple(key, value))
   {
      /* Map's name */
      if(key == MAP_TOKEN_NAME)
      {
         name = translateDataString(value);
      }
      /* Music file to use */
      else if(key == MAP_TOKEN_MUSIC_FILE)
      {
         musicFilename = value;
      }
      else if(key == MAP_TOKEN_SCRIPT)
      {
         assert(script == NULL);
         if(!editMode)
         {
            script = Game::getScriptManager()->createMapScriptInstance(value,
                  mapFileName);
         }
         scriptFilename = value;
      }
      /* Square definition */
      else if(key == MAP_TOKEN_SQUARE)
      {
         if(squareX == xSize - 1)
         {
            squareX = 0;
            squareZ++;
         }
         else
         {
            squareX++;
         }
         //TODO: read heights
      }
      /* Square texture definition */
      else if(key == MAP_TOKEN_USE_TEXTURE)
      {
         MapSubMesh* subMesh = floor.getSubMesh(value);
         if(!subMesh)
         {
            subMesh = floor.createSubMesh(value);
         }
         if(editMode)
         {
            floorMaterials[squareZ * xSize + squareX] = value;
         }
         subMesh->addSquare(squareX * MAP_SQUARE_SIZE, 0.0f, 
                         squareZ * MAP_SQUARE_SIZE, 
                         (squareX + 1) * MAP_SQUARE_SIZE, 0.0f,
                         (squareZ + 1) * MAP_SQUARE_SIZE,
                         0.0f, 1.0f, 0.0f);
         Collision::setSquareWalkable(squareX, squareZ, true);
      }
      /* Wall definition */
      else if(key == MAP_TOKEN_WALL)
      {
         /* Read current wall corners */
         sscanf(value.c_str(),"%f,%f,%f,%f,%f,%f",
                &wX1, &wY1, &wZ1, &wX2, &wY2, &wZ2);
         /* make sure 1 <= 2 */
         Ogre::Real tmp;
         if(wX2 < wX1)
         {
            tmp = wX2;
            wX2 = wX1;
            wX1 = tmp;
         }
         if(wZ2 < wZ1)
         {
            tmp = wZ2;
            wZ2 = wZ1;
            wZ1 = tmp;
         }
         if(wY2 < wY1)
         {
            tmp = wY2;
            wY2 = wY1;
            wY1 = tmp;
         }

         /* Create the wall and add it to the list and collision system */
         lastWall = new Wall(editMode);
         if(editMode)
         {
            lastWall->setInfo(Ogre::Vector3(wX1, wY1, wZ1),
               Ogre::Vector3(wX2, wY2, wZ2));
         }
         walls.insertAtEnd(lastWall);
         Collision::addElement(Ogre::Vector3(wX1, wY1, wZ1),
               Ogre::Vector3(wX2, wY2, wZ2));

         /* Define the upper wall face */
         lastWall->addFace(MAP_UPPER_WALL_MATERIAL,
               wX1, wY2, wZ1, wX2, wY2, wZ2, 0.0f, 1.0f, 0.0f);
         if(wY1 != 0.0f)
         {
            /* Also define a square for bottom, as not at floor */
            lastWall->addFace(MAP_UPPER_WALL_MATERIAL,
                  wX1, wY1, wZ1, wX2, wY1, wZ2, 0.0f, -1.0f, 0.0f);
         }
      }
      /* Define Current Wall Front Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_FRONT)
      {
         assert(lastWall != NULL);
         lastWall->addFace(value, wX1, wY1, wZ1, wX2, wY2, wZ1,
               0.0f, 0.0f, -1.0f);
      }
      /* Define Current Wall Back Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_BACK)
      {
         assert(lastWall != NULL);
         lastWall->addFace(value, wX1, wY1, wZ2, wX2, wY2, wZ2,
               0.0f, 0.0f, 1.0f);
      }
      /* Define Current Wall Left Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_LEFT)
      {
         assert(lastWall != NULL);
         lastWall->addFace(value, wX1, wY1, wZ1, wX1, wY2, wZ2,
               -1.0f, 0.0f, 0.0f);
      }
      /* Define Current Wall Left Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_RIGHT)
      {
         assert(lastWall != NULL);
         lastWall->addFace(value, wX2, wY1, wZ1, wX2, wY2, wZ2,
               1.0f, 0.0f, 0.0f);
      }
      /* Define a NPC on map */
      else if(key == MAP_TOKEN_NPC)
      {
         NonPlayableCharacter* npc = new NonPlayableCharacter(value);
         if(npc)
         {
            lastNpc = npc;
            nodesPerThingMap[npc->getSceneNode()] = npc;
         }
      }
      else if(key == MAP_TOKEN_NPC_POSITION)
      {
         if(lastNpc)
         {
            Ogre::Real pX=0.0f, pY=0.0f, pZ=0.0f;
            sscanf(value.c_str(), "%f,%f,%f", &pX, &pY, &pZ);
            lastNpc->setInitialPosition(Ogre::Vector3(pX, pY, pZ));
         }
      }
      else if(key == MAP_TOKEN_NPC_ORIENTATION)
      {
         if(lastNpc)
         {
            Ogre::Real oY=0.0f;
            sscanf(value.c_str(), "%f", &oY);
            lastNpc->setOrientationNow(Ogre::Vector3(0.0f, oY, 0.0f));
            /* Orientation is the last 'world-location' element of a npc,
             * we can insert it for collision now. */
            Collision::addElement(lastNpc);
         }
      }
      else if(key == MAP_TOKEN_NPC_PSYCHO_STATE)
      {
         if(lastNpc)
         {
            if(value == "hostile")
            {
               lastNpc->setPsychoState(NonPlayableCharacter::PS_HOSTILE);
            }
            else if(value == "friendly")
            {
               lastNpc->setPsychoState(NonPlayableCharacter::PS_FRIENDLY);
            }
            else
            {
               lastNpc->setPsychoState(NonPlayableCharacter::PS_NEUTRAL);
            }
         }
      }
      else if(key == MAP_TOKEN_NPC_DIALOG)
      {
         if(lastNpc)
         {
            lastNpc->setConversationFile(value);
         }
      }
      /* Define a thing (object, item, scenery, etc) on the map */
      else if((key == MAP_TOKEN_THING) || (key == MAP_TOKEN_DOOR))
      {
         Thing* thing = insertThing(value, editMode);
         if(thing)
         {
            /* Notify last thing dirty state, if needed. */
            if((lastThing) && (lastThing->isStatic()))
            {
               lastThing->notifyStaticDirty();
            }
            
            /* Set new lastThing */
            lastThing = thing;
         }
      }
      else if((key == MAP_TOKEN_THING_POSITION) ||
              (key == MAP_TOKEN_DOOR_POSITION))
      {
         /* Define last thing's position. */
         if(lastThing != NULL)
         {
            Ogre::Real pX=0.0f, pY=0.0f, pZ=0.0f;
            sscanf(value.c_str(), "%f,%f,%f", &pX, &pY, &pZ);
            lastThing->setInitialPosition(Ogre::Vector3(pX, pY, pZ));
         }
      }
      else if(key == MAP_TOKEN_THING_ORIENTATION)
      {
         /* Define last thing's orientation */
         if(lastThing != NULL)
         {
            Ogre::Real oX=0.0f, oY=0.0f, oZ=0.0f;
            sscanf(value.c_str(), "%f,%f,%f", &oX, &oY, &oZ);
            lastThing->setOrientationNow(Ogre::Vector3(oX, oY, oZ));
            /* Note: thingOri must be the last parameter to define a thing,
             * and should be mandatory to all thing declarations on map. */
            Collision::addElement(lastThing);
         }
      }
      else if(key == MAP_TOKEN_THING_WALKABLE)
      {
         /* Define last thing's walkable flag */
         if(lastThing != NULL)
         {
            lastThing->setWalkable(value == MAP_VALUE_TRUE);
         }
      }
      else if(key == MAP_TOKEN_DOOR_ORIENTATION)
      {
         /* Define last door orientation */
         Door* last = static_cast<Door*>(lastThing);
         if(last != NULL)
         {
            Ogre::Real oY=0.0f;
            sscanf(value.c_str(), "%f", &oY);
            last->setOrientationNow(Ogre::Vector3(0.0f, oY, 0.0f));
            last->setClosedAngle(oY);
            Collision::addElement(last);
         }
      }
      else if(key == MAP_TOKEN_DOOR_LOCK)
      {
         /* Lock the door */
         int ope=0, burg=0;
         sscanf(value.c_str(), "%d %d", &ope, &burg);
         Door* last = static_cast<Door*>(lastThing);
         if(last != NULL)
         {
            last->lock(ope, burg);
         }
      }
      else if(key == MAP_TOKEN_DOOR_LOCK_DIALOG)
      {
         /* Define door unlock conversation file */
         Thing* last = static_cast<Thing*>(lastThing);
         if(last != NULL)
         {
            last->setConversationFile(value);
         }
      }
      else if(key == MAP_TOKEN_INITIAL)
      {
         float ix = 0.0f;
         float iz = 0.0f;
         /* Define initial position */
         sscanf(value.c_str(), "%f,%f", &ix, &iz);

         //TODO: get map height at position and set it.
         initialPos = Ogre::Vector3(ix, 0.0f, iz);
      }
      else if(key == MAP_TOKEN_LIGHT)
      {
         if(lastLight)
         {
            lastLight->flush();
         }
         /* Create a new light */
         if(value == MAP_VALUE_LIGHT_POINT)
         {
            lastLight = lights->createLightInfo(Ogre::Light::LT_POINT);
         }
         else if(value == MAP_VALUE_LIGHT_SPOTLIGHT)
         {
            lastLight = lights->createLightInfo(Ogre::Light::LT_SPOTLIGHT);
         }
         else if(value == MAP_VALUE_LIGHT_DIRECTIONAL)
         {
            lastLight = lights->createLightInfo(Ogre::Light::LT_DIRECTIONAL);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_POS)
      {
         /* Set last light position */
         if(lastLight)
         {
            float lx=0.0f, ly=0.0f, lz=0.0f;
            sscanf(value.c_str(), "%f %f %f", &lx, &ly, &lz);
            lastLight->setPosition(Ogre::Vector3(lx, ly, lz));
         }
      }
      else if(key == MAP_TOKEN_LIGHT_DIR)
      {
         /* Set last light direction */
         if(lastLight)
         {
            float lx=0.0f, ly=0.0f, lz=0.0f;
            sscanf(value.c_str(), "%f %f %f", &lx, &ly, &lz);
            lastLight->setDirection(Ogre::Vector3(lx, ly, lz));
         }
      }
      else if(key == MAP_TOKEN_LIGHT_DIFFUSE)
      {
         /* Set last light diffuse color */
         if(lastLight)
         {
            float lr=0.0f, lg=0.0f, lb=0.0f;
            sscanf(value.c_str(), "%f %f %f", &lr, &lg, &lb);
            lastLight->setDiffuse(lr, lg, lb);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_SPECULAR)
      {
         /* Set last light specular color */
         if(lastLight)
         {
            float lr=0.0f, lg=0.0f, lb=0.0f;
            sscanf(value.c_str(), "%f %f %f", &lr, &lg, &lb);
            lastLight->setSpecular(lr, lg, lb);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_SPOT_RANGE)
      {
         if(lastLight)
         {
            float ang=0.0f;
            sscanf(value.c_str(), "%f", &ang);
            lastLight->setSpotlightRange(Ogre::Degree(ang));
         }
      }
      else if(key == MAP_TOKEN_LIGHT_ATTENUATION)
      {
         if(lastLight)
         {
            float r=0.0f, c=0.0f, l=0.0f, q=0.0f;
            sscanf(value.c_str(), "%f %f %f %f", &r, &c, &l, &q);
            lastLight->setAttenuation(r, c, l, q);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_POWER_SCALE)
      {
         if(lastLight)
         {
            float power=1.0f;
            sscanf(value.c_str(), "%f", &power);
            lastLight->setPowerScale(power);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_HDR)
      {
         if(lastLight)
         {
            lastLight->setHdr(value == MAP_VALUE_TRUE);
         }
      }
      else if(key == MAP_TOKEN_LIGHT_CAST_SHADOWS)
      {
         if(lastLight)
         {
            lastLight->setCastShadows(value == MAP_VALUE_TRUE);
         }
      }
      else 
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "Warning: unknow key '%s' at map's definition file '%s'",
               key.c_str(), mapFileName.c_str());
      }
   }

   if(lastLight)
   {
      lastLight->flush();
   }

   /* Notify last thing dirty state, if needed. */
   if((lastThing) && (lastThing->isStatic()))
   {
      lastThing->notifyStaticDirty();
   }

   /* Update our floor and walls */
   floor.updateAllDirty();
   updateAllDirtyWalls();

   return true;
}

/**************************************************************************
 *                                   save                                 *
 **************************************************************************/
bool Map::save(Kobold::String filename)
{
   std::ofstream file;

   /* let's try to create it */
   file.open(filename.c_str(), std::ios::out | std::ios::binary);
   if(!file)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "Couldn't save map file '%s'", filename.c_str());
      return false;
   }
   this->filename = filename;

   /* Add a comment about the creator version */
   file << "# Created with DNT MapEditor, " << DNT_VERSION << std::endl;

   /* Save size and metadata information */
   file << MAP_TOKEN_SIZE << " = " << xSize << "x" << zSize << std::endl;
   file << MAP_TOKEN_INITIAL << " = " << initialPos.x << "," 
        << initialPos.z << std::endl;
   file << MAP_TOKEN_NAME << " = " 
        << "gettext(\"" << name << "\")" << std::endl;
   if(!scriptFilename.empty())
   {
      file << MAP_TOKEN_SCRIPT << " = " << scriptFilename << std::endl;
   }
   if(!musicFilename.empty())
   {
      file << MAP_TOKEN_MUSIC_FILE << " = " << musicFilename << std::endl;
   }

   /* Save all lights */
   LightInfo* light = static_cast<LightInfo*>(lights->getFirst());
   for(int i=0; i < lights->getTotal(); i++)
   {
      file << MAP_TOKEN_LIGHT << " = ";
      switch(light->getType())
      {
         default:
         case Ogre::Light::LT_POINT:
            file << MAP_VALUE_LIGHT_POINT << std::endl;
         break;
         case Ogre::Light::LT_DIRECTIONAL:
            file << MAP_VALUE_LIGHT_DIRECTIONAL << std::endl;
         break;
         case Ogre::Light::LT_SPOTLIGHT:
            file << MAP_VALUE_LIGHT_SPOTLIGHT << std::endl;
            file << MAP_TOKEN_LIGHT_SPOT_RANGE << " = " 
                 << light->getSpotlightRange().valueDegrees() << std::endl;
         break;
      }
      file << MAP_TOKEN_LIGHT_POS << " = " << light->getPosition().x 
           << " " << light->getPosition().y << " " << light->getPosition().z
           << std::endl;
      if(light->getType() != Ogre::Light::LT_POINT)
      {
         file << MAP_TOKEN_LIGHT_DIR << " = " << light->getDirection().x
              << " " << light->getDirection().y << " " 
              << light->getDirection().z << std::endl;
      }
      file << MAP_TOKEN_LIGHT_DIFFUSE << " = " << light->getDiffuse().r
           << " " << light->getDiffuse().g << " " 
           << light->getDiffuse().b << std::endl;
      file << MAP_TOKEN_LIGHT_SPECULAR << " = " << light->getSpecular().r
           << " " << light->getSpecular().g << " " 
           << light->getSpecular().b << std::endl;
      file << MAP_TOKEN_LIGHT_POWER_SCALE << " = " << light->getPowerScale()
           << std::endl;
      file << MAP_TOKEN_LIGHT_HDR << " = " 
           << (light->getHdr() ? MAP_VALUE_TRUE : MAP_VALUE_FALSE) 
           << std::endl;
      if(!light->getCastShadows())
      {
         file << MAP_TOKEN_LIGHT_CAST_SHADOWS << " = " 
              << MAP_VALUE_FALSE << std::endl;
      }
      if(light->getType() != Ogre::Light::LT_DIRECTIONAL)
      {
         file << MAP_TOKEN_LIGHT_ATTENUATION << " = " 
              << light->getAttenuationRange() << " " 
              << light->getAttenuationConstant() << " " 
              << light->getAttenuationLinear() << " " 
              << light->getAttenuationQuadric() << std::endl; 
      }

      light = static_cast<LightInfo*>(light->getNext());
   }

   /* Save all Walls */
   Wall* w = static_cast<Wall*>(walls.getFirst());
   for(int i=0; i < walls.getTotal(); i++)
   {
      file << MAP_TOKEN_WALL << " = " << w->getMin().x << ","
           << w->getMin().y << "," << w->getMin().z << "," 
           << w->getMax().x << "," << w->getMax().y  << ","
           << w->getMax().z << std::endl;
      if(!w->getMaterialInfo(0).empty())
      {
         file << MAP_TOKEN_WALL_TEXTURE_FRONT << " = " 
              << w->getMaterialInfo(0) << std::endl;
      }
      if(!w->getMaterialInfo(1).empty())
      {
         file << MAP_TOKEN_WALL_TEXTURE_BACK << " = " 
              << w->getMaterialInfo(1) << std::endl;
      } 
      if(!w->getMaterialInfo(2).empty())
      {
         file << MAP_TOKEN_WALL_TEXTURE_LEFT << " = " 
              << w->getMaterialInfo(2) << std::endl;
      }
      if(!w->getMaterialInfo(3).empty())
      {
         file << MAP_TOKEN_WALL_TEXTURE_RIGHT << " = " 
              << w->getMaterialInfo(3) << std::endl;
      }
      w = static_cast<Wall*>(w->getNext());
   }

   /* Save all Squares */
   for(int z=0; z < zSize; z++)
   {
      file << "#Z: " << z << std::endl;
      for(int x=0; x < xSize; x++)
      {
         //TODO: square height.
         file << MAP_TOKEN_SQUARE << " = "
              << "1,0.000000,0.000000,0.000000,0.000000" << std::endl;
         file << MAP_TOKEN_USE_TEXTURE << " = " 
              << floorMaterials[z * xSize + x] << std::endl;
      }
   }

   /* Save all objects (dynamic and static ones) */
   Kobold::List* curList = dynamicThings;
   for(int l=0; l < 2; l++)
   {
      Thing* curThing = static_cast<Thing*>(curList->getFirst());
      for(int i = 0; i < curList->getTotal(); i++)
      {
         if(curThing->getThingType() == Thing::THING_TYPE_DOOR)
         {
            /* Save as door */
            Door* door = static_cast<Door*>(curThing); 
            file << MAP_TOKEN_DOOR << " = " << door->getFilename() << std::endl;
            file << MAP_TOKEN_DOOR_POSITION << " = "
                 << door->getInitialPosition().x << ","
                 << door->getInitialPosition().y << ","
                 << door->getInitialPosition().z << std::endl;
            file << MAP_TOKEN_DOOR_ORIENTATION << " = " 
                 << door->getOrientation() << std::endl;
            if(door->getOpenStatus() == Door::DOOR_LOCKED)
            {
               file << MAP_TOKEN_DOOR_LOCK << " = " 
                    << door->getDifficulty() << " " 
                    << door->getHardness() << std::endl;
               file << MAP_TOKEN_DOOR_LOCK_DIALOG << " = " 
                    << door->getConversationFile() << std::endl;
            }
         }
         else
         {
            /* Save as thing */
            file << MAP_TOKEN_THING << " = " 
                 << curThing->getFilename() << std::endl;
            file << MAP_TOKEN_THING_POSITION << " = "
                 << curThing->getInitialPosition().x << ","
                 << curThing->getInitialPosition().y << ","
                 << curThing->getInitialPosition().z << std::endl;
            file << MAP_TOKEN_THING_ORIENTATION << " = " 
                 << curThing->getPitch() << ","
                 << curThing->getYaw() << ","
                 << curThing->getRoll() << std::endl;
            if(curThing->isWalkable())
            {
               file << MAP_TOKEN_THING_WALKABLE << " = " 
                    << MAP_VALUE_TRUE << std::endl;
            }
         }
         curThing = static_cast<Thing*>(curThing->getNext());
      }

      /* now save the static ones */
      curList = staticThings;
   }

   /* Save all NPCs */
   NonPlayableCharacter* npc = static_cast<NonPlayableCharacter*>(
         Game::getNpcs()->getFirst());
   for(int i=0; i < Game::getNpcs()->getTotal(); i++)
   {
      file << MAP_TOKEN_NPC << " = " << npc->getFilename() << std::endl;
      file << MAP_TOKEN_NPC_POSITION << " = "
           << npc->getInitialPosition().x << ","
           << npc->getInitialPosition().y << ","
           << npc->getInitialPosition().z << std::endl;
      file << MAP_TOKEN_NPC_ORIENTATION << " = " 
           << npc->getOrientation() << std::endl;
      if(npc->getPsychoState() == NonPlayableCharacter::PS_HOSTILE)
      {
         file << MAP_TOKEN_NPC_PSYCHO_STATE << " = hostile" << std::endl;
      }
      else if(npc->getPsychoState() == NonPlayableCharacter::PS_FRIENDLY)
      {
         file << MAP_TOKEN_NPC_PSYCHO_STATE << " = friendly" << std::endl;
      }
      if(npc->hasConversationFile())
      {
         file << MAP_TOKEN_NPC_DIALOG << " = "
              << npc->getConversationFile() << std::endl;
      }

      npc = static_cast<NonPlayableCharacter*>(npc->getNext());
   }
   
   file.close();
   return true;
}

/**************************************************************************
 *                                  getThing                              *
 **************************************************************************/
Thing* Map::getThing(Ogre::SceneNode* sceneNode)
{
   std::map<Ogre::SceneNode*, Thing*>::iterator it;
   it = nodesPerThingMap.find(sceneNode);
   if (it != nodesPerThingMap.end())
   {
      return it->second;
   }

   return NULL;
}

/**************************************************************************
 *                               removeThing                              *
 **************************************************************************/
void Map::removeThing(Thing* thing)
{
   /* Remove from nodesPerThing */
   nodesPerThingMap[thing->getSceneNode()] = NULL;

   /* Remove from respective list */
   if(dynamicThings->hasElement(thing))
   {
      dynamicThings->remove(thing);
   }
   else
   { 
      staticThings->remove(thing);
   }
}

/**************************************************************************
 *                                 getObject                              *
 **************************************************************************/
Object* Map::getObject(Kobold::String filename)
{
   return getObject(filename, Ogre::Vector3(0, 0, 0), false);
}
Object* Map::getObject(Kobold::String filename, const Ogre::Vector3& pos)
{
   return getObject(filename, pos, true);
}
Object* Map::getObject(Kobold::String filename, const Ogre::Vector3& pos, 
      bool usePos)
{
   /* Let's first search on dynamic things */
   Kobold::List* curList = dynamicThings;
   for(int l=0; l < 2; l++)
   {
      Thing* t = static_cast<Thing*>(curList->getFirst());
      for(int i=0; i < curList->getTotal(); i++)
      {
         if((t->getThingType() != Thing::THING_TYPE_CHARACTER) &&
            (t->getFilename() == filename) &&
            ((!usePos) || (t->getInitialPosition() == pos)))
         {
            assert(dynamic_cast<Object*>(t));
            return static_cast<Object*>(t);
         }
         t = static_cast<Thing*>(t->getNext());
      }
      /* Not found on dynamic, let's try on static. */
      curList = staticThings;
   }

   /* Not found */
   return NULL;
}

/**************************************************************************
 *                             getInitialPosition                         *
 **************************************************************************/
Ogre::Vector3 Map::getInitialPosition()
{
   return initialPos;
}

