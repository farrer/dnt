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

#define INDOOR_SUBDIVISIONS         2
#define INDOOR_SUBDIV_P1     (INDOOR_SUBDIVISIONS + 1)
#define INDOOR_VERTICES_PER_SQUARE  (INDOOR_SUBDIV_P1) * (INDOOR_SUBDIV_P1)

#include "indoortexturemesh.h"
#include "../core/game.h"

#include <OGRE/OgreMeshManager.h>

using namespace DNT;

/**************************************************************************
 *                                 Constructor                            *
 **************************************************************************/
IndoorTextureSquare::IndoorTextureSquare(Ogre::Real x1, Ogre::Real y1, 
      Ogre::Real z1, Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
      Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ)
{
   /* Define coordinates */
   bottomLeft = Ogre::Vector3(x1, y1, z1);
   topRight = Ogre::Vector3(x2, y2, z2);
   normal = Ogre::Vector3(normX, normY, normZ);
   normal.normalise();
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
IndoorTextureSquare::~IndoorTextureSquare()
{
}

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::define(Ogre::ManualObject* manualObject, int curIndex)
{
   Ogre::Real textureDelta = MAP_SQUARE_SIZE;
   if(bottomLeft.y == topRight.y)
   {
      /* Square at Y plane */
      defineAtY(manualObject, curIndex, textureDelta);
   }
   else if(bottomLeft.x == topRight.x)
   {
      /* Square at X plane */
      defineAtX(manualObject, curIndex, textureDelta);
   }
   else if(bottomLeft.z == topRight.z)
   {
      /* Square at Z plane */
      defineAtZ(manualObject, curIndex, textureDelta);
   }
   //else TODO non-planar square.
}

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtY(Ogre::ManualObject* manualObject,
      int curIndex, Ogre::Real textureDelta)
{
   Ogre::Real maxU = (topRight.x - bottomLeft.x) / textureDelta;
   Ogre::Real maxV = (topRight.z - bottomLeft.z) / textureDelta;

   Ogre::Vector3 inc = (topRight - bottomLeft) / INDOOR_SUBDIVISIONS;
   Ogre::Vector3 cur = bottomLeft;
   Ogre::Vector2 incUv = Ogre::Vector2(maxU / INDOOR_SUBDIVISIONS,
                                       maxV / INDOOR_SUBDIVISIONS);
   Ogre::Vector2 uv = Ogre::Vector2(0.0f, 0.0f); 

   /* Let's declare all vertices */
   for(int i = 0; i <= INDOOR_SUBDIVISIONS; i++)
   {
      if(i == INDOOR_SUBDIVISIONS)
      {
         cur.x = topRight.x;
         uv.x = maxU;
      }
      uv.y = 0.0f;
      cur.z = bottomLeft.z;
      for(int j = 0; j <= INDOOR_SUBDIVISIONS; j++)
      {
         if(j == INDOOR_SUBDIVISIONS)
         {
            cur.z = topRight.z;
            uv.y = maxV;
         }
         manualObject->position(cur);
         manualObject->normal(normal);
         manualObject->textureCoord(uv);
         
         uv.y += incUv.y;
         cur.z += inc.z;
      }
      cur.x += inc.x;
      uv.x += incUv.x;
   }

   defineTriangles(manualObject, curIndex, normal.y);
}

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtX(Ogre::ManualObject* manualObject,
      int curIndex, Ogre::Real textureDelta)
{
   Ogre::Real maxU = (topRight.z - bottomLeft.z) / textureDelta;
   Ogre::Real maxV = (topRight.y - bottomLeft.y) / textureDelta;

   Ogre::Vector3 inc = (topRight - bottomLeft) / INDOOR_SUBDIVISIONS;
   Ogre::Vector3 cur = bottomLeft;
   Ogre::Vector2 incUv = Ogre::Vector2(maxU / INDOOR_SUBDIVISIONS,
                                       maxV / INDOOR_SUBDIVISIONS);
   Ogre::Vector2 uv = Ogre::Vector2(0.0f, 0.0f); 

   /* Let's declare all vertices */
   for(int i = 0; i <= INDOOR_SUBDIVISIONS; i++)
   {
      if(i == INDOOR_SUBDIVISIONS)
      {
         cur.z = topRight.z;
         uv.x = maxU;
      }
      uv.y = textureDelta - maxV;
      cur.y = bottomLeft.y;

      for(int j = 0; j <= INDOOR_SUBDIVISIONS; j++)
      {
         if(j == INDOOR_SUBDIVISIONS)
         {
            cur.y = topRight.y;
            uv.y = textureDelta;
         }
         manualObject->position(cur);
         manualObject->normal(normal);
         manualObject->textureCoord(uv);
         
         uv.y += incUv.y;
         cur.y += inc.y;
      }
      cur.z += inc.z;
      uv.x += incUv.x;
   }

   defineTriangles(manualObject, curIndex, normal.x);
}


/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtZ(Ogre::ManualObject* manualObject,
      int curIndex, Ogre::Real textureDelta)
{
   Ogre::Real maxU = (topRight.x - bottomLeft.x) / textureDelta;
   Ogre::Real maxV = (topRight.y - bottomLeft.y) / textureDelta;

   Ogre::Vector3 inc = (topRight - bottomLeft) / INDOOR_SUBDIVISIONS;
   Ogre::Vector3 cur = bottomLeft;

   Ogre::Vector2 incUv = Ogre::Vector2(maxU / INDOOR_SUBDIVISIONS,
                                       maxV / INDOOR_SUBDIVISIONS);
   Ogre::Vector2 uv = Ogre::Vector2(0.0f, 0.0f); 

   /* Let's declare all vertices */
   for(int i = 0; i <= INDOOR_SUBDIVISIONS; i++)
   {
      if(i == INDOOR_SUBDIVISIONS)
      {
         cur.x = topRight.x;
         uv.x = maxU;
      }
      uv.y = textureDelta - maxV;
      cur.y = bottomLeft.y;

      for(int j = 0; j <= INDOOR_SUBDIVISIONS; j++)
      {
         if(j == INDOOR_SUBDIVISIONS)
         {
            cur.y = topRight.y;
            uv.y = textureDelta;
         }
         manualObject->position(cur);
         manualObject->normal(normal);
         manualObject->textureCoord(uv);
         
         uv.y += incUv.y;
         cur.y += inc.y;
      }
      cur.x += inc.x;
      uv.x += incUv.x;
   }

   /* Let's define its triangles */
   defineTriangles(manualObject, curIndex, -normal.z);
}

/**************************************************************************
 *                              defineTriangles                           *
 **************************************************************************/
void IndoorTextureSquare::defineTriangles(Ogre::ManualObject* manualObject,
      int index, int nValue)
{
   int curIndex; 
   for(int i = 0; i < INDOOR_SUBDIVISIONS; i++)
   {
      /* Define index of first element on row */
      curIndex = index + ((INDOOR_SUBDIVISIONS + 1)  * i);

      /* Let's define the 'row' */
      for(int j = 0; j < INDOOR_SUBDIVISIONS; j++)
      {
         if(nValue < 0)
         {
            manualObject->triangle(curIndex + INDOOR_SUBDIVISIONS + 1, 
                                   curIndex + 1, curIndex);
            manualObject->triangle(curIndex + 1, 
                                   curIndex + INDOOR_SUBDIVISIONS + 1, 
                                   curIndex + INDOOR_SUBDIVISIONS + 2);
         }
         else
         {
            manualObject->triangle(curIndex, curIndex + 1, 
                                   curIndex + INDOOR_SUBDIVISIONS + 1);
            manualObject->triangle(curIndex + INDOOR_SUBDIVISIONS + 2, 
                                   curIndex + INDOOR_SUBDIVISIONS + 1, 
                                   curIndex + 1);
         }
         curIndex += 1;
      }
   }
}


/**************************************************************************
 *                             getBottomLeftCorner                        *
 **************************************************************************/
Ogre::Vector3 IndoorTextureSquare::getBottomLeftCorner()
{
   return bottomLeft;
}

/**************************************************************************
 *                              getTopRightCorner                         *
 **************************************************************************/
Ogre::Vector3 IndoorTextureSquare::getTopRightCorner()
{
   return topRight;
}

/**************************************************************************
 *                                 Constructor                            *
 **************************************************************************/
IndoorTextureMesh::IndoorTextureMesh(Ogre::ManualObject* manualObject,
      Ogre::String materialName, bool castShadows)
{
   this->materialName = materialName;
   this->manualObject = manualObject;
   this->dirty = false;
   this->castShadows = castShadows;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/

IndoorTextureMesh::~IndoorTextureMesh()
{
}

/**************************************************************************
 *                               getMaterialName                          *
 **************************************************************************/
Ogre::String IndoorTextureMesh::getMaterialName()
{
   return materialName;
}

/**************************************************************************
 *                                 addSquare                              *
 **************************************************************************/
void IndoorTextureMesh::addSquare(Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
      Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
      Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ)
{
   dirty = true;
   insert(new IndoorTextureSquare(x1, y1, z1, x2, y2, z2, 
            normX, normY, normZ));
}

/**************************************************************************
 *                             updateSceneNode                            *
 **************************************************************************/
void IndoorTextureMesh::updateManualObject(Ogre::String baseName)
{
   if(dirty)
   {
      dirty = false;

      /* Only create the manual object if have some square defined. */
      if(getTotal() > 0)
      {
         manualObject->begin(materialName, 
               Ogre::RenderOperation::OT_TRIANGLE_LIST);

         int curVertIndex = 0;

         /* Define each square as two triangles */
         IndoorTextureSquare* square = static_cast<IndoorTextureSquare*>
            (getFirst());
         for(int i = 0; i < getTotal(); i++)
         {
            square->define(manualObject, curVertIndex);
            curVertIndex += INDOOR_VERTICES_PER_SQUARE;

            /* Done, let's check next square */
            square = static_cast<IndoorTextureSquare*>(square->getNext());
         }

         manualObject->end();
      }
   }
}

/**************************************************************************
 *                                 Constructor                            *
 **************************************************************************/
IndoorTextureMeshes::IndoorTextureMeshes(Ogre::String baseName, 
      bool castShadows)
{
   this->manualObject = Game::getSceneManager()->createManualObject(baseName);
   this->sceneNode = NULL;
   this->entity = NULL;

   this->baseName = baseName;
   this->castShadows = castShadows;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
IndoorTextureMeshes::~IndoorTextureMeshes()
{
   Game::getSceneManager()->destroyManualObject(manualObject);
   deleteSceneNode();
}

/**************************************************************************
 *                                deleteObjects                           *
 **************************************************************************/
void IndoorTextureMeshes::deleteSceneNode()
{
   if(sceneNode != NULL)
   {
      /* Remove from scene and delete */
      sceneNode->detachObject(entity);
      Game::getSceneManager()->destroySceneNode(sceneNode);
      Game::getSceneManager()->destroyEntity(entity);
      Ogre::MeshManager::getSingleton().remove(ogreMesh->getName());

      /* Nullify */
      sceneNode = NULL;
   }
}


/**************************************************************************
 *                              createTextureMesh                         *
 **************************************************************************/
IndoorTextureMesh* IndoorTextureMeshes::createTextureMesh(
      Ogre::String materialName)
{
   IndoorTextureMesh* mesh = new IndoorTextureMesh(manualObject, 
         materialName, castShadows);
   insert(mesh);
   return mesh;
}

/**************************************************************************
 *                                getTextureMesh                          *
 **************************************************************************/
IndoorTextureMesh* IndoorTextureMeshes::getTextureMesh(
      Ogre::String materialName)
{
   IndoorTextureMesh* mesh = static_cast<IndoorTextureMesh*>(getFirst());
   for(int i = 0; i < getTotal(); i++)
   {
      if(mesh->getMaterialName() == materialName)
      {
         return mesh;
      }
      mesh = static_cast<IndoorTextureMesh*>(mesh->getNext());
   }

   return NULL;
}

/**************************************************************************
 *                                hasTextureMesh                          *
 **************************************************************************/
bool IndoorTextureMeshes::hasTextureMesh(Ogre::String materialName)
{
   return getTextureMesh(materialName) != NULL;
}

/**************************************************************************
 *                              updateAllDirty                            *
 **************************************************************************/
void IndoorTextureMeshes::updateAllDirty()
{
   if(getTotal() > 0)
   {
      deleteSceneNode();

      IndoorTextureMesh* mesh = static_cast<IndoorTextureMesh*>(getFirst());
      for(int i = 0; i < getTotal(); i++)
      {
         mesh->updateManualObject(baseName);
         mesh = static_cast<IndoorTextureMesh*>(mesh->getNext());
      }

      /* Define mesh */
      ogreMesh = manualObject->convertToMesh(baseName);
      entity = Game::getSceneManager()->createEntity(ogreMesh);
      entity->setCastShadows(castShadows);

      /* Create the scene node and attach to the scene with manual object */
      sceneNode = 
         Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
      sceneNode->attachObject(entity);
      sceneNode->setPosition(0.0f, 0.0f, 0.0f);
   }
}


