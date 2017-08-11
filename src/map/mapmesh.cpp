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

#define INDOOR_SUBDIVISIONS         1
#define INDOOR_SUBDIV_P1     (INDOOR_SUBDIVISIONS + 1)
#define INDOOR_VERTICES_PER_SQUARE  (INDOOR_SUBDIV_P1) * (INDOOR_SUBDIV_P1)
#define INDOOR_INDEXES_PER_SQUARE 6 * INDOOR_SUBDIVISIONS * INDOOR_SUBDIVISIONS

/*! Number of floats per vertex definition */
#define VERTEX_NUM_FLOATS   12

#include "mapmesh.h"
#include "../core/game.h"

#include <OGRE/OgreMeshManager2.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreSubMesh2.h>

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>

#include <kobold/log.h>
#include <goblin/vertexutils.h>

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
void IndoorTextureSquare::define(float* vertices, int& curVert, int& curIndex, 
      Ogre::uint16* faces, int& curFace, const Ogre::Vector3 meshMin)
{
   Ogre::Real textureDelta = MAP_SQUARE_SIZE;
   if(bottomLeft.y == topRight.y)
   {
      /* Square at Y plane */
      defineAtY(vertices, curVert, curIndex, faces, curFace, textureDelta,
                meshMin);
   }
   else if(bottomLeft.x == topRight.x)
   {
      /* Square at X plane */
      defineAtX(vertices, curVert, curIndex, faces, curFace, textureDelta,
                meshMin);
   }
   else if(bottomLeft.z == topRight.z)
   {
      /* Square at Z plane */
      defineAtZ(vertices, curVert, curIndex, faces, curFace, textureDelta,
                meshMin);
   }
   //else TODO non-planar square.
}

/**************************************************************************
 *                                setVertex                               *
 **************************************************************************/
void IndoorTextureSquare::setVertex(float* vertices, int& curVert, 
      Ogre::Vector3 pos, Ogre::Vector3 normal, Ogre::Vector2 uv, 
      const Ogre::Vector3 meshMin)
{
   /* Position */
   vertices[curVert] = pos.x - meshMin.x;
   vertices[curVert + 1] = pos.y - meshMin.y;
   vertices[curVert + 2] = pos.z - meshMin.z;

   /* Normal */
   vertices[curVert + 3] = normal.x;
   vertices[curVert + 4] = normal.y;
   vertices[curVert + 5] = normal.z;

   /* Texture coordinates */
   vertices[curVert + 6] = uv.x;
   vertices[curVert + 7] = uv.y;

   /* Tangents (inited with 0) */
   vertices[curVert + 8] = 0.0f;
   vertices[curVert + 9] = 0.0f;
   vertices[curVert + 10] = 0.0f;
   vertices[curVert + 11] = 0.0f;

   /* Increment to next vertice */
   curVert += VERTEX_NUM_FLOATS;
}

//FIXME: define positions centered and use the SceneNode to translate!
//TODO: Define with different LODs

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtY(float* vertices, int& curVert, 
      int& curIndex, Ogre::uint16* faces, int& curFace, 
      Ogre::Real textureDelta, const Ogre::Vector3 meshMin)
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
         setVertex(vertices, curVert, cur, normal, uv, meshMin);
                  
         uv.y += incUv.y;
         cur.z += inc.z;
      }
      cur.x += inc.x;
      uv.x += incUv.x;
   }

   defineTriangles(curIndex, faces, curFace, normal.y);
}

/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtX(float* vertices, int& curVert, 
      int& curIndex, Ogre::uint16* faces, int& curFace, 
      Ogre::Real textureDelta, const Ogre::Vector3 meshMin)
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
      uv.y = maxV;
      cur.y = bottomLeft.y;

      for(int j = 0; j <= INDOOR_SUBDIVISIONS; j++)
      {
         if(j == INDOOR_SUBDIVISIONS)
         {
            cur.y = topRight.y;
            uv.y = 0.0f;
         }
         setVertex(vertices, curVert, cur, normal, uv, meshMin);
         
         uv.y -= incUv.y;
         cur.y += inc.y;
      }
      cur.z += inc.z;
      uv.x += incUv.x;
   }

   defineTriangles(curIndex, faces, curFace, normal.x);
}


/**************************************************************************
 *                                   define                               *
 **************************************************************************/
void IndoorTextureSquare::defineAtZ(float* vertices, int& curVert, 
      int& curIndex, Ogre::uint16* faces, int& curFace, 
      Ogre::Real textureDelta, const Ogre::Vector3 meshMin)
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
      uv.y = maxV;
      cur.y = bottomLeft.y;

      for(int j = 0; j <= INDOOR_SUBDIVISIONS; j++)
      {
         if(j == INDOOR_SUBDIVISIONS)
         {
            cur.y = topRight.y;
            uv.y = 0.0f;
         }
         setVertex(vertices, curVert, cur, normal, uv, meshMin);
         
         uv.y -= incUv.y;
         cur.y += inc.y;
      }
      cur.x += inc.x;
      uv.x += incUv.x;
   }

   /* Let's define its triangles */
   defineTriangles(curIndex, faces, curFace, -normal.z);
}

/**************************************************************************
 *                              defineTriangles                           *
 **************************************************************************/
void IndoorTextureSquare::defineTriangles(int& curIndex, Ogre::uint16* faces, 
      int& curFace, int nValue)
{
   int index; 
   for(int i = 0; i < INDOOR_SUBDIVISIONS; i++)
   {
      /* Define index of first element on row */
      index = curIndex + ((INDOOR_SUBDIVISIONS + 1)  * i);

      /* Let's define the 'row' */
      for(int j = 0; j < INDOOR_SUBDIVISIONS; j++)
      {
         if(nValue < 0)
         {
            faces[curFace] = index + INDOOR_SUBDIVISIONS + 1;
            faces[curFace+1] = index + 1;
            faces[curFace+2] = index;
            faces[curFace+3] = index + 1;
            faces[curFace+4] = index + INDOOR_SUBDIVISIONS + 1;
            faces[curFace+5] = index + INDOOR_SUBDIVISIONS + 2;
         }
         else
         {
            faces[curFace] = index;
            faces[curFace+1] = index + 1;
            faces[curFace+2] = index + INDOOR_SUBDIVISIONS + 1;
            faces[curFace+3] = index + INDOOR_SUBDIVISIONS + 2;
            faces[curFace+4] = index + INDOOR_SUBDIVISIONS + 1;
            faces[curFace+5] = index + 1;
         }
         index += 1;
         curFace += 6;
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
MapSubMesh::MapSubMesh(Ogre::MeshPtr mesh, Ogre::String datablockName)
{
   this->mesh = mesh;
   this->datablockName = datablockName;
   this->dirty = false;
   this->subMesh = NULL;
   this->subMeshIndex = -1;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
MapSubMesh::~MapSubMesh()
{
   /* Must destroy if created the submesh: note the VAO is destructed within 
    * the submesh destroy. */
   if(this->subMesh)
   {
      this->mesh->destroySubMesh(this->subMeshIndex);
   }
}

/**************************************************************************
 *                               getDatablockName                          *
 **************************************************************************/
Ogre::String MapSubMesh::getDatablockName()
{
   return datablockName;
}

/**************************************************************************
 *                                 addSquare                              *
 **************************************************************************/
void MapSubMesh::addSquare(Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
      Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
      Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ)
{
   /* Check and define new bounds */
   if(getTotal() == 0)
   {
      /* No squares yet. Must set our min and max as this one */
      min[0] = x1;
      min[1] = y1;
      min[2] = z1;
      max[0] = x2;
      max[1] = y2;
      max[2] = z2;
   }
   else
   {
      /* Check current bound values */
      if(x1 < min[0])
      {
         min[0] = x1;
      }
      if(y1 < min[1])
      {
         min[1] = y1;
      }
      if(z1 < min[2])
      {
         min[2] = z1;
      }
      if(x2 > max[0])
      {
         max[0] = x2;
      }
      if(y2 > max[1])
      {
         max[1] = y2;
      }
      if(z2 > max[2])
      {
         max[2] = z2;
      }
   }

   /* Mark the submesh as dirty and add the square */
   dirty = true;
   insertAtEnd(new IndoorTextureSquare(x1, y1, z1, x2, y2, z2, 
            normX, normY, normZ));
}

/**************************************************************************
 *                                 update                                 *
 **************************************************************************/
void MapSubMesh::update(const Ogre::Vector3 meshMin)
{
   float* vertices = NULL;
   Ogre::uint16* faces = NULL;;

   if(dirty)
   {
      dirty = false;

      /* Only create the submesh if have at last one square defined. */
      if(getTotal() > 0)
      {
         if(subMesh == NULL)
         {
            subMeshIndex = mesh->getNumSubMeshes();
            subMesh = mesh->createSubMesh();
            subMesh->mMaterialName = datablockName;

            /* Alloc vertices and indexes vectors */
            vertices = new float[getTotal() * INDOOR_VERTICES_PER_SQUARE * 
               VERTEX_NUM_FLOATS];
            faces = new Ogre::uint16[getTotal() * INDOOR_INDEXES_PER_SQUARE];
         }
         else
         {
            //TODO
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
                  "Warning: MapSupMesh update isn't implemented!");
            return;
         }

         int curVert = 0, curIndex = 0, curFaceIndex = 0;

         /* Define each square */
         IndoorTextureSquare* square = static_cast<IndoorTextureSquare*>
            (getFirst());
         for(int i = 0; i < getTotal(); i++)
         {
            square->define(vertices, curVert, curIndex, faces, curFaceIndex,
                           meshMin);

            curIndex += INDOOR_VERTICES_PER_SQUARE;

            /* Done, let's check next square */
            square = static_cast<IndoorTextureSquare*>(square->getNext());
         }

         /* Let's make sure total vertices and face indexes where defined */
         assert((getTotal() * INDOOR_VERTICES_PER_SQUARE * VERTEX_NUM_FLOATS) 
               == curVert);
         assert((getTotal() * INDOOR_INDEXES_PER_SQUARE) == curFaceIndex);

         createVao(vertices, faces);
      }
   }
   /* Clean up (FIXME: vertices and faces shouldn't be members?) */
   if(vertices != NULL)
   {
      delete[] vertices;
   }
   if(faces != NULL)
   {
      delete[] faces;
   }
}

/**************************************************************************
 *                                  createVao                             *
 **************************************************************************/
void MapSubMesh::createVao(float* verts, Ogre::uint16* faces)
{
   Ogre::VaoManager* vaoManager = 
      Ogre::Root::getSingletonPtr()->getRenderSystem()->getVaoManager();

   /* Let's calculate our tangents */
   int numVertices = getTotal() * INDOOR_VERTICES_PER_SQUARE;
   int numIndices = getTotal() * INDOOR_INDEXES_PER_SQUARE;
   Goblin::VertexUtils::generateTangents(verts, faces, 
         VERTEX_NUM_FLOATS, numVertices, numIndices,  
         0, 3, 8, 6);

   /* Copy it to a SIMD array (the memory will be managed by the vao */
   Ogre::uint16* faceIndices = reinterpret_cast<Ogre::uint16*>(
         OGRE_MALLOC_SIMD(sizeof(Ogre::uint16) * numIndices,
            Ogre::MEMCATEGORY_GEOMETRY) );
   memcpy(faceIndices, faces, sizeof(Ogre::uint16) * numIndices);

   /* Let's create the index buffer */
   try
   {
      indexBuffer = vaoManager->createIndexBuffer(
            Ogre::IndexBufferPacked::IT_16BIT, numIndices, Ogre::BT_IMMUTABLE,
            faceIndices, true);
   }
   catch(Ogre::Exception& e)
   {
      /* With exceptions, we should need to free it */
      OGRE_FREE_SIMD(indexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
      indexBuffer = NULL;
      throw e;
   }

   /* Define our vertices elements: Position, Normal and UVs */
   Ogre::VertexElement2Vec vertexElements;
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3,
            Ogre::VES_POSITION));
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3,
            Ogre::VES_NORMAL));
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT2,
            Ogre::VES_TEXTURE_COORDINATES));
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT4,
            Ogre::VES_TANGENT));

   /* Let's copy it to a SIMD array */
   float* vertices = reinterpret_cast<float*>(OGRE_MALLOC_SIMD(
            sizeof(float) * numVertices * VERTEX_NUM_FLOATS, 
            Ogre::MEMCATEGORY_GEOMETRY));
   memcpy(vertices, verts, sizeof(float) * numVertices * VERTEX_NUM_FLOATS);

   /* And create our packed vertex buffer */
   try
   {
      vertexBuffer = vaoManager->createVertexBuffer(vertexElements, 
            numVertices, Ogre::BT_IMMUTABLE, vertices, true);
   }
   catch(Ogre::Exception &e)
   {
      OGRE_FREE_SIMD(vertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
      vertexBuffer = NULL;
      throw e;
   }

   /* Finally, the Vao. */
   Ogre::VertexBufferPackedVec vertexBuffers;
   vertexBuffers.push_back(vertexBuffer);

   vao = vaoManager->createVertexArrayObject(vertexBuffers, indexBuffer,
         Ogre::OT_TRIANGLE_LIST);

   //TODO: must set various LOD.
   subMesh->mVao[Ogre::VpNormal].push_back(vao);
   subMesh->mVao[Ogre::VpShadow].push_back(vao);
}

/**************************************************************************
 *                                 Constructor                            *
 **************************************************************************/
MapMesh::MapMesh(Ogre::String baseName)
{
   this->mesh = Ogre::MeshManager::getSingleton().createManual(baseName,
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
   this->sceneNode = NULL;
   this->item = NULL;

   this->baseName = baseName;
}

/**************************************************************************
 *                                 Destructor                             *
 **************************************************************************/
MapMesh::~MapMesh()
{
   /* Remove item and scene node */
   deleteSceneNode();

   /* Let's clear the submesh list from back to front (as the submesh deletion
    * in the mesh is made by its index and removing a previous will change 
    * subsequent indexes). */
   while(getLast() != NULL)
   {
      remove(getLast());
   }

   /* Finally, bye mesh. */
   Ogre::MeshManager::getSingleton().remove(this->mesh->getHandle());
}

/**************************************************************************
 *                                deleteObjects                           *
 **************************************************************************/
void MapMesh::deleteSceneNode()
{
   if(sceneNode != NULL)
   {
      /* Remove from scene and delete */
      sceneNode->detachObject(item);
      Game::getSceneManager()->destroySceneNode(sceneNode);
      Game::getSceneManager()->destroyItem(item);

      /* Nullify */
      sceneNode = NULL;
   }
}


/**************************************************************************
 *                              createTextureMesh                         *
 **************************************************************************/
MapSubMesh* MapMesh::createSubMesh(Ogre::String datablockName)
{
   MapSubMesh* subMesh = new MapSubMesh(mesh, datablockName);
   insert(subMesh);
   return subMesh;
}

/**************************************************************************
 *                                getTextureMesh                          *
 **************************************************************************/
MapSubMesh* MapMesh::getSubMesh(Ogre::String datablockName)
{
   MapSubMesh* subMesh = static_cast<MapSubMesh*>(getFirst());
   for(int i = 0; i < getTotal(); i++)
   {
      if(subMesh->getDatablockName() == datablockName)
      {
         return subMesh;
      }
      subMesh = static_cast<MapSubMesh*>(subMesh->getNext());
   }

   return NULL;
}

/**************************************************************************
 *                                hasTextureMesh                          *
 **************************************************************************/
bool MapMesh::hasTextureMesh(Ogre::String datablockName)
{
   return getSubMesh(datablockName) != NULL;
}

/**************************************************************************
 *                              defineBounds                              *
 **************************************************************************/
void MapMesh::defineBounds()
{
   /* Initial: define our current to the first submesh values.
    * Note that if called defineBounds is that we assured we have
    * at last one submesh. */
   MapSubMesh* subMesh = static_cast<MapSubMesh*>(getFirst());
   min = subMesh->getMin();
   max = subMesh->getMax();
  
   /* Let's compare with sequential submeshes bounds */
   subMesh = static_cast<MapSubMesh*>(subMesh->getNext());
   for(int i = 1; i < getTotal(); i++)
   {
      Ogre::Vector3 subMeshMin = subMesh->getMin();
      Ogre::Vector3 subMeshMax = subMesh->getMax();

      /* Check if lesser than current min */
      if(min.x > subMeshMin.x)
      {
         min.x = subMeshMin.x;
      }
      if(min.y > subMeshMin.y)
      {
         min.y = subMeshMin.y;
      }
      if(min.z > subMeshMin.z)
      {
         min.z = subMeshMin.z;
      }

      /* Check if greater than current max */
      if(max.x < subMeshMax.x)
      {
         max.x = subMeshMax.x;
      }
      if(max.y < subMeshMax.y)
      {
         max.y = subMeshMax.y;
      }
      if(max.z < subMeshMax.z)
      {
         max.z = subMeshMax.z;
      }

      /* Check next */
      subMesh = static_cast<MapSubMesh*>(subMesh->getNext());
   }

   /* Define Aaabb */
   Ogre::Vector3 half = (max - min) / 2;
   Ogre::Vector3 center = half; 
   mesh->_setBounds(Ogre::Aabb(center, half));

   /* define radius, by max half incremented a few */
   Ogre::Real radius = half[0];
   if((half.y > radius) && (half.z <= radius))
   {
      radius = half.y;
   }
   else if(half.z > radius)
   {
      radius = half.z;
   }
   radius += 1.0f;
   mesh->_setBoundingSphereRadius(radius);
}

/**************************************************************************
 *                              updateAllDirty                            *
 **************************************************************************/
void MapMesh::updateAllDirty()
{
   if(getTotal() > 0)
   {
      deleteSceneNode();

      /* Define Mesh bounding box */
      defineBounds();

      MapSubMesh* subMesh = static_cast<MapSubMesh*>(getFirst());
      for(int i = 0; i < getTotal(); i++)
      {
         subMesh->update(min);
         subMesh = static_cast<MapSubMesh*>(subMesh->getNext());
      }

      /* Add mesh to its Item */
      item = Game::getSceneManager()->createItem(mesh);
      item->setName(baseName);

      /* Create the scene node and attach to the scene with manual object */
      sceneNode = 
         Game::getSceneManager()->getRootSceneNode()->createChildSceneNode();
      sceneNode->attachObject(item);
      sceneNode->setPosition(min);
   }
}


