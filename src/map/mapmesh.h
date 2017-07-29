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



#ifndef _dnt_indoor_texture_mesh_h
#define _dnt_indoor_texture_mesh_h

#include "dntconfig.h"

#include <kobold/list.h>

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreMesh2.h>
#include <OGRE/OgreItem.h>

#include <OGRE/Vao/OgreVaoManager.h>


#define MAP_SQUARE_SIZE                 64.0f

namespace DNT
{

/*! Represents a single quad with an specific texture. */
class IndoorTextureSquare : public Kobold::ListElement
{
   public:
      /*! Constructor
       * \param x1 -> bottom left X coordinate
       * \param y1 -> bottom left Y coordinate
       * \param z1 -> bottom left Z coordinate
       * \param x2 -> top right X coordinate
       * \param y2 -> top right Y coordinate
       * \param z2 -> top right Z coordinate 
       * \param normX -> normal X
       * \param normY -> normal Y
       * \param normZ -> normal Z */
      IndoorTextureSquare(Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
                     Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
                     Ogre::Real normX, Ogre::Real normY, Ogre::Real normZ);
      /*! Destructor */
      ~IndoorTextureSquare();

      /*! Define the square with triangles.
       * \note side effect: will increment curVert and curFace
       * \param vertices current Ogre vertex info to define
       * \param curVert current (first) vertex info index to define here
       * \param curIndex current (first) index info to define here
       * \param faces current Ogre face Index do define
       * \param curFace current face (first) index to define here 
       * \param meshMin minimum bounds of the mesh */
      void define(float* vertices, int& curVert, int& curIndex, 
            Ogre::uint16* faces, int& curFace, const Ogre::Vector3 meshMin);

      /*! \return bottom left coordinates */
      Ogre::Vector3 getBottomLeftCorner();
      /*! \return top right coordinates */
      Ogre::Vector3 getTopRightCorner();

   private:
      /*! Define for X oriented */
      void defineAtX(float* vertices, int& curVert, int& curIndex, 
            Ogre::uint16* faces, int& curFace, Ogre::Real textureDelta, 
            const Ogre::Vector3 meshMin);
      /*! Define for Y oriented */
      void defineAtY(float* vertices, int& curVert, int& curIndex, 
            Ogre::uint16* faces, int& curFace, Ogre::Real textureDelta, 
            const Ogre::Vector3 meshMin);
      /*! Define for Z oriented */
      void defineAtZ(float* vertices, int& curVert, int& curIndex, 
            Ogre::uint16* faces, int& curFace, Ogre::Real textureDelta, 
            const Ogre::Vector3 meshMin);
      /*! Define triangles */
      void defineTriangles(int& curIndex, Ogre::uint16* faces, 
            int& curFace, int nValue);

      /*! Set a vertex for the mesh */
      void setVertex(float* vertices, int& curVert, Ogre::Vector3 pos, 
                     Ogre::Vector3 normal, Ogre::Vector2 uv, 
                     const Ogre::Vector3 meshMin);

      Ogre::Vector3 bottomLeft; /**< Bottom Left Corner */
      Ogre::Vector3 topRight;  /**< Top Right Corner */
      Ogre::Vector3 normal; /**< Normal */
};

/*! This class represents a SubMesh for all indoor vertices
 * of a single texture (being them a floor square or a wall face) for
 * a MapMesh. */
class MapSubMesh : public Kobold::List, public Kobold::ListElement
{
   public:
      /*! Constructor.
       * \param mesh pointer to its parent mesh
       * \param datablockName datablock datablock to use for the mesh. */
      MapSubMesh(Ogre::MeshPtr mesh, Ogre::String datablockName);
      /*! Destructor */
      ~MapSubMesh();

      /*! Add a square to the mesh */
      void addSquare(Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
                     Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
                     Ogre::Real normX, Ogre::Real nomrY, Ogre::Real normZ);

      /*! Create or update manual object if needed */
      void update(const Ogre::Vector3 meshMin);

      /*! \return name of the used datablock for this mesh. */
      Ogre::String getDatablockName();

      /*! \return minimun value of its bounds */
      Ogre::Vector3 getMin() { return min; };
      /*! \return maximun value of its bounds */
      Ogre::Vector3 getMax() { return max; };
 
   private:

      Ogre::Vector3 min; /**< Minimum values for x,y,z */
      Ogre::Vector3 max; /**< Maximum values for x,y,z */

      /*! Create a VertexArrayObject for the mesh */
      void createVao(float* verts, Ogre::uint16* faces);

      Ogre::MeshPtr mesh; /**< The mesh owner of this submesh */
      int subMeshIndex; /**< index of our submesh */
      Ogre::SubMesh* subMesh; /**< Submesh to populate with. */

      Ogre::VertexArrayObject* vao; /**< The single VAO of this renderable */
      Ogre::VertexBufferPacked* vertexBuffer; /**< Its vertex buffer */
      Ogre::IndexBufferPacked* indexBuffer; /**< Its index buffer */
      
      Ogre::String datablockName; /**< Datablock to use for the mesh. */
      bool dirty; /**< If the vertices list was modified and the Vao
                       must be redefined. */
};

/*! A List of MapSubMesh'es, representing the floor or a single wall. 
 * \note the mesh will be created at 0,0,0 (its minimum bounds will be defined
 * to that) and positionated at desired position with its SceneNode. */
class MapMesh : public Kobold::List
{
   public:
     /*! Constructor 
      * \param baseName name to use as base (usally wall or floor) */
     MapMesh(Ogre::String baseName);
     /*! Destructor */
     ~MapMesh();

     /*! Create a submesh with dataBlockName as material */
     MapSubMesh* createSubMesh(Ogre::String datablockName);

     /*! \return a submesh with dataBlockName as material */
     MapSubMesh* getSubMesh(Ogre::String datablockName);

     /*! \return if has a mesh with datablock material */
     bool hasTextureMesh(Ogre::String datablockName);

     /*! Create or update all dirty manual objects */
     void updateAllDirty();

   private:

      /*! Delete related SceneNode, if defined. */
      void deleteSceneNode();

      /*! Define meshes bounds */
      void defineBounds();

      Ogre::MeshPtr mesh; /**< The Ogre::Mesh representation */
      Ogre::String baseName;  /**< Basic name (usually 'wall' or 'floor') */
      Ogre::Item* item; /**< Item related to the mesh. */
      Ogre::SceneNode* sceneNode; /**< Scene node related to the mesh. */

      Ogre::Vector3 min; /**< Minimum values for x,y,z */
      Ogre::Vector3 max; /**< Maximum values for x,y,z */


};

}

#endif

