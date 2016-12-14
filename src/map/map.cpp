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

#include "indoortexturemesh.h"
#include "light.h"

#include "../rules/thing.h"
#include "../core/scenery.h"
#include "../core/door.h"
#include "../core/game.h"
#include "../lang/translate.h"

#include <kobold/defparser.h>
#include <kobold/log.h>

using namespace DNT;

#define MAP_WALL_HEIGHT                 50
#define MAP_UPPER_WALL_MATERIAL         "upper_wall"

#define MAP_TOKEN_SIZE                  "size"

#define MAP_TOKEN_SQUARE                "square"
#define MAP_TOKEN_USE_TEXTURE           "useTexture"

#define MAP_TOKEN_NAME                  "name"
#define MAP_TOKEN_MUSIC_FILE            "musicFile"

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

#define MAP_TOKEN_LIGHT                 "light"
#define MAP_TOKEN_LIGHT_POS             "lightPos"
#define MAP_TOKEN_LIGHT_DIR             "lightDir"
#define MAP_TOKEN_LIGHT_DIFFUSE         "lightDiffuse"
#define MAP_TOKEN_LIGHT_SPECULAR        "lightSpecular"
#define MAP_TOKEN_LIGHT_SPOT_RANGE      "lightSpotRange"
#define MAP_TOKEN_LIGHT_AREA            "lightArea"
#define MAP_TOKEN_LIGHT_ATTENUATION     "lightAttenuation"

#define MAP_VALUE_LIGHT_POINT           "point"
#define MAP_VALUE_LIGHT_SPOTLIGHT       "spot"
#define MAP_VALUE_LIGHT_DIRECTIONAL     "directional"

#define MAP_TOKEN_INITIAL               "initial"

#define MAP_VALUE_TRUE                  "true"

/**************************************************************************
 *                                Constructor                             *
 **************************************************************************/
Map::Map()
    :floor("floor", false),
     walls("wall", true)
{
   this->name = "";
   this->filename = "";
   this->musicFilename = "";
   this->xSize = 0;
   this->zSize = 0;
   this->things = new Kobold::List(Kobold::LIST_TYPE_ADD_AT_END);
   this->lights = new MapLights();
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
Map::~Map()
{
  delete this->things;
  delete this->lights;
}

/**************************************************************************
 *                                 update                                 *
 **************************************************************************/
void Map::update(Ogre::Vector3 floorMouse)
{
   /* FIXME: must use character coordinate, not mouse as here. */
   lights->setActiveLight(floorMouse.x, floorMouse.z);

   /* FIXME: would be better to just use a list of animated or scriptable
    * things, instead of updating everything. */
   Thing* thing = static_cast<Thing*>(things->getFirst());
   for(int i = 0; i < things->getTotal(); i++)
   {
      Goblin::Model3d* model = thing->getModel();
      if(model)
      {
         model->update();
      }
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

   /* Define the upper wall texture */
   //walls.createTextureMesh(MAP_UPPER_WALL_MATERIAL);

   /* Create the mesh for default texture */
   IndoorTextureMesh* mesh = floor.createTextureMesh("concrete6");
   for(int squareZ = 0; squareZ < sizeZ; squareZ++)
   {
      for(int squareX = 0; squareX < sizeX; squareX++)
      {
         mesh->addSquare(squareX * MAP_SQUARE_SIZE, 0.0f, 
                         squareZ * MAP_SQUARE_SIZE, 
                         (squareX + 1) * MAP_SQUARE_SIZE, 0.0f,
                         (squareZ + 1) * MAP_SQUARE_SIZE,
                         0.0f, 1.0f, 0.0f);
      }
   }
   floor.updateAllDirty();
   walls.updateAllDirty();
   lights->setActiveLight(0.0f, 0.0f);
}

/**************************************************************************
 *                             getSizeX                                   *
 **************************************************************************/
int Map::getSizeX()
{
   return xSize;
}

/**************************************************************************
 *                             getSizeZ                                   *
 **************************************************************************/
int Map::getSizeZ()
{
   return zSize;
}

/**************************************************************************
 *                                 load                                   *
 **************************************************************************/
bool Map::load(Ogre::String mapFileName, bool fullPath)
{
   Kobold::DefParser parser;
   Ogre::String key, value;
   LightInfo* lastLight = NULL;

   this->filename = mapFileName;

   if(!parser.load(mapFileName, fullPath))
   {
      return false;
   }

   /* Read size of the map */
   if( (!parser.getNextTuple(key, value)) || (key != MAP_TOKEN_SIZE) )
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: Map size not defined at '%s', found '%s' instead of 'size'",
            mapFileName.c_str(), key.c_str());
      return false;
   }
   sscanf(value.c_str(), "%dx%d", &xSize, &zSize);

   /* Define the upper wall texture */
   walls.createTextureMesh(MAP_UPPER_WALL_MATERIAL);

   /* Reset square position counters */
   int squareX = -1;
   int squareZ = 0;

   float wX1=0.0f, wZ1=0.0f, wX2=0.0f, wZ2=0.0f;

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
         IndoorTextureMesh* mesh = floor.getTextureMesh(value);
         if(!mesh)
         {
            mesh = floor.createTextureMesh(value);
         }
         mesh->addSquare(squareX * MAP_SQUARE_SIZE, 0.0f, 
                         squareZ * MAP_SQUARE_SIZE, 
                         (squareX + 1) * MAP_SQUARE_SIZE, 0.0f,
                         (squareZ + 1) * MAP_SQUARE_SIZE,
                         0.0f, 1.0f, 0.0f);
      }
      /* Wall definition */
      else if(key == MAP_TOKEN_WALL)
      {
         /* Read current wall corners */
         sscanf(value.c_str(),"%f,%f,%f,%f",&wX1, &wZ1, &wX2, &wZ2);
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

         /* Define the upper wall square */
         IndoorTextureMesh* mesh = walls.getTextureMesh(
               MAP_UPPER_WALL_MATERIAL);
         if(mesh)
         {
            mesh->addSquare(wX1, MAP_WALL_HEIGHT, wZ1, 
                            wX2, MAP_WALL_HEIGHT, wZ2,
                            0.0f, 1.0f, 0.0f);
         }

      }
      /* Define Current Wall Front Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_FRONT)
      {
         IndoorTextureMesh* mesh = walls.getTextureMesh(value);
         if(!mesh)
         {
            mesh = walls.createTextureMesh(value);
         }
         mesh->addSquare(wX1, 0.0f, wZ1, wX2, MAP_WALL_HEIGHT, wZ1,
               0.0f, 0.0f, -1.0f);
      }
      /* Define Current Wall Back Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_BACK)
      {
         IndoorTextureMesh* mesh = walls.getTextureMesh(value);
         if(!mesh)
         {
            mesh = walls.createTextureMesh(value);
         }
         mesh->addSquare(wX1, 0.0f, wZ2, wX2, MAP_WALL_HEIGHT, wZ2,
               0.0f, 0.0f, 1.0f);
      }
      /* Define Current Wall Left Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_LEFT)
      {
         IndoorTextureMesh* mesh = walls.getTextureMesh(value);
         if(!mesh)
         {
            mesh = walls.createTextureMesh(value);
         }
         mesh->addSquare(wX1, 0.0f, wZ1, wX1, MAP_WALL_HEIGHT, wZ2,
               -1.0f, 0.0f, 0.0f);
      }
      /* Define Current Wall Left Texture */
      else if(key == MAP_TOKEN_WALL_TEXTURE_RIGHT)
      {
         IndoorTextureMesh* mesh = walls.getTextureMesh(value);
         if(!mesh)
         {
            mesh = walls.createTextureMesh(value);
         }
         mesh->addSquare(wX2, 0.0f, wZ1, wX2, MAP_WALL_HEIGHT, wZ2,
               1.0f, 0.0f, 0.0f);
      }
      /* Define a thing (object, item, scenery, etc) on the map */
      else if((key == MAP_TOKEN_THING) || (key == MAP_TOKEN_DOOR))
      {
         /* We define thing's type by file extension. */
         Kobold::String fileExtension, baseName, path;
         Ogre::StringUtil::splitFullFilename(value, baseName, 
               fileExtension, path);
         Thing* thing = NULL;
         if(fileExtension == "scn")
         {
            thing = new Scenery();
         }
         else if(fileExtension == "dor")
         {
            thing = new Door();
         }

         if(thing)
         {
            if(!thing->load(value))
            {
               /* Couldn't load, no need to keep it at list. */
               delete thing;
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Error: couldn't load thing from file '%s'", value.c_str());
            }
            else 
            {
               things->insert(thing);
            }
         }
         else
         {
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Error: couldn't define thing's type for '%s'", value.c_str());
         }
      }
      else if((key == MAP_TOKEN_THING_POSITION) ||
              (key == MAP_TOKEN_DOOR_POSITION))
      {
         /* Define last thing's position. */
         Thing* last = static_cast<Thing*>(things->getLast());
         if(last != NULL)
         {
            Ogre::Real pX=0.0f, pY=0.0f, pZ=0.0f;
            sscanf(value.c_str(), "%f,%f,%f", &pX, &pY, &pZ);
            last->getModel()->setPosition(pX, pY, pZ);
         }
      }
      else if(key == MAP_TOKEN_THING_ORIENTATION)
      {
         /* Define last thing's orientation */
         Thing* last = static_cast<Thing*>(things->getLast());
         if(last != NULL)
         {
            Ogre::Real oX=0.0f, oY=0.0f, oZ=0.0f;
            sscanf(value.c_str(), "%f,%f,%f", &oX, &oY, &oZ);
            last->getModel()->setOrientation(oX, oY, oZ);
         }
      }
      else if(key == MAP_TOKEN_THING_WALKABLE)
      {
         /* Define last thing's walkable flag */
         Thing* last = static_cast<Thing*>(things->getLast());
         if(last != NULL)
         {
            last->setWalkable(value == MAP_VALUE_TRUE);
         }
      }
      else if(key == MAP_TOKEN_DOOR_ORIENTATION)
      {
         /* Define last door orientation */
         Door* last = static_cast<Door*>(things->getLast());
         if(last != NULL)
         {
            Ogre::Real oY=0.0f;
            sscanf(value.c_str(), "%f", &oY);
            last->getModel()->setOrientation(0.0f, oY, 0.0f);
            last->setClosedAngle(oY);
         }
      }
      else if(key == MAP_TOKEN_DOOR_LOCK)
      {
         /* Lock the door */
         int ope=0, burg=0;
         sscanf(value.c_str(), "%d %d", &ope, &burg);
         Door* last = static_cast<Door*>(things->getLast());
         if(last != NULL)
         {
            last->lock(ope, burg);
         }
      }
      else if(key == MAP_TOKEN_DOOR_LOCK_DIALOG)
      {
         /* Define door unlock conversation file */
         Thing* last = static_cast<Thing*>(things->getLast());
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
      //FIXME: should define light areas on indoor maps, and change 
      // between active lights based on current character position.
      else if(key == MAP_TOKEN_LIGHT)
      {
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
      else if(key == MAP_TOKEN_LIGHT_AREA)
      {
         if(lastLight)
         {
            int x1=0, y1=0, x2=0, y2=0;
            sscanf(value.c_str(), "%d %d %d %d", &x1, &y1, &x2, &y2);
            lastLight->addArea(x1, y1, x2, y2);
         }
      }
      else 
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "Warning: unknow key '%s' at map's definition file '%s'",
               key.c_str(), mapFileName.c_str());
      }
   }

   floor.updateAllDirty();
   walls.updateAllDirty();
   lights->setActiveLight(0.0f, 0.0f);

   return true;
}

/**************************************************************************
 *                             getInitialPosition                         *
 **************************************************************************/
Ogre::Vector3 Map::getInitialPosition()
{
   return initialPos;
}

