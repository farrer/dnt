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

   /* Define Vertices */
   manualObject->position(bottomLeft);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, 0.0f);

   manualObject->position(topRight.x, bottomLeft.y, bottomLeft.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, 0.0f);

   manualObject->position(bottomLeft.x, bottomLeft.y, topRight.z);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, maxV);

   manualObject->position(topRight.x, bottomLeft.y, topRight.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, maxV);

   /* Define Both Triangles */
   if(normal.y > 0)
   {
      manualObject->triangle(curIndex + 2, curIndex + 1, curIndex);
      manualObject->triangle(curIndex + 1, curIndex + 2, curIndex + 3);
   }
   else
   {
      manualObject->triangle(curIndex, curIndex + 1, curIndex + 2);
      manualObject->triangle(curIndex + 3, curIndex + 2, curIndex + 1);
   }
}

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtX(Ogre::ManualObject* manualObject,
      int curIndex, Ogre::Real textureDelta)
{
   Ogre::Real maxU = (topRight.z - bottomLeft.z) / textureDelta;
   Ogre::Real maxV = (topRight.y - bottomLeft.y) / textureDelta;
   
   /* Define Vertices */
   manualObject->position(bottomLeft);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, 0.0f);

   manualObject->position(bottomLeft.x, topRight.y, bottomLeft.z);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, maxV);

   manualObject->position(bottomLeft.x, bottomLeft.y, topRight.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, 0.0f);

   manualObject->position(bottomLeft.x, topRight.y, topRight.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, maxV);

   /* Define Both Triangles */
   if(normal.x < 0)
   {
      manualObject->triangle(curIndex + 2, curIndex + 1, curIndex);
      manualObject->triangle(curIndex + 1, curIndex + 2, curIndex + 3);
   } 
   else
   {
      manualObject->triangle(curIndex, curIndex + 1, curIndex + 2);
      manualObject->triangle(curIndex + 3, curIndex + 2, curIndex + 1);
   }
}


/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtZ(Ogre::ManualObject* manualObject,
      int curIndex, Ogre::Real textureDelta)
{
   Ogre::Real maxU = (topRight.x - bottomLeft.x) / textureDelta;
   Ogre::Real maxV = (topRight.y - bottomLeft.y) / textureDelta;

   /* Define Vertices */
   manualObject->position(bottomLeft);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, 0.0f);

   manualObject->position(topRight.x, bottomLeft.y, bottomLeft.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, 0.0f);

   manualObject->position(bottomLeft.x, topRight.y, bottomLeft.z);
   manualObject->normal(normal);
   manualObject->textureCoord(0.0f, maxV);

   manualObject->position(topRight.x, topRight.y, bottomLeft.z);
   manualObject->normal(normal);
   manualObject->textureCoord(maxU, maxV);

   /* Define Both Triangles */
   if(normal.z < 0)
   {
      manualObject->triangle(curIndex + 2, curIndex + 1, curIndex);
      manualObject->triangle(curIndex + 1, curIndex + 2, curIndex + 3);
   }
   else
   {
      manualObject->triangle(curIndex, curIndex + 1, curIndex + 2);
      manualObject->triangle(curIndex + 3, curIndex + 2, curIndex + 1);
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
IndoorTextureMesh::IndoorTextureMesh(Ogre::String materialName)
{
   this->materialName = materialName;
   this->manualObject = NULL;
   this->sceneNode = NULL;
   this->dirty = false;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/

IndoorTextureMesh::~IndoorTextureMesh()
{
   deleteSceneNode();
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
 *                               deleteSceneNode                          *
 **************************************************************************/
void IndoorTextureMesh::deleteSceneNode()
{
   if(sceneNode != NULL)
   {
      /* Remove from scene and delete */
      sceneNode->detachObject(entity);
      Game::getSceneManager()->destroySceneNode(sceneNode);
      Game::getSceneManager()->destroyEntity(entity);
      Ogre::MeshManager::getSingleton().remove(ogreMesh->getName());
      delete manualObject;

      /* Nullify */
      sceneNode = NULL;
      manualObject = NULL;
   }
}

/**************************************************************************
 *                             updateSceneNode                            *
 **************************************************************************/
void IndoorTextureMesh::updateManualObject(Ogre::String baseName)
{
   if(dirty)
   {
      deleteSceneNode();
      dirty = false;

      /* Only create the manual object if have some square defined. */
      if(getTotal() > 0)
      {
         /* Create the object */
         manualObject = new Ogre::ManualObject(baseName + materialName);

         manualObject->begin(materialName, 
               Ogre::RenderOperation::OT_TRIANGLE_LIST);

         int curVertIndex = 0;

         /* Define each square as two triangles */
         IndoorTextureSquare* square = static_cast<IndoorTextureSquare*>
            (getFirst());
         for(int i = 0; i < getTotal(); i++)
         {
            square->define(manualObject, curVertIndex);
            curVertIndex += 4;

            /* Done, let's check next square */
            square = static_cast<IndoorTextureSquare*>(square->getNext());
         }

         manualObject->end();

         /* Define mesh */
         ogreMesh = manualObject->convertToMesh(baseName + materialName);
         entity = Game::getSceneManager()->createEntity(ogreMesh);

         /* Create the scene node and attach to the scene with manual object */
         sceneNode = 
            Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
         sceneNode->attachObject(entity);
         sceneNode->setPosition(0.0f, 0.0f, 0.0f);
      }
   }
}

/**************************************************************************
 *                                 Constructor                            *
 **************************************************************************/
IndoorTextureMeshes::IndoorTextureMeshes(Ogre::String baseName)
{
   this->baseName = baseName;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
IndoorTextureMeshes::~IndoorTextureMeshes()
{
}

/**************************************************************************
 *                              createTextureMesh                         *
 **************************************************************************/
IndoorTextureMesh* IndoorTextureMeshes::createTextureMesh(
      Ogre::String materialName)
{
   IndoorTextureMesh* mesh = new IndoorTextureMesh(materialName);
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
   IndoorTextureMesh* mesh = static_cast<IndoorTextureMesh*>(getFirst());
   for(int i = 0; i < getTotal(); i++)
   {
      mesh->updateManualObject(baseName);
      mesh = static_cast<IndoorTextureMesh*>(mesh->getNext());
   }
}


