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
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreMesh.h>
#include <OGRE/OgreEntity.h>


#define MAP_SQUARE_SIZE                 64

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
                     Ogre::Real normX, Ogre::Real nomrY, Ogre::Real normZ);
      /*! Destructor */
      ~IndoorTextureSquare();

      /* Define the square inside a manual object as two triangles.
       * \param manualObject pointer to object to define the square
       * \param curIndex current (first) index of manual object to define. */
      void define(Ogre::ManualObject* manualObject, int curIndex);

      /*! \return bottom left coordinates */
      Ogre::Vector3 getBottomLeftCorner();
      /*! \return top right coordinates */
      Ogre::Vector3 getTopRightCorner();

   private:
      void defineAtX(Ogre::ManualObject* manualObject, int curIndex,
                     Ogre::Real textureDelta);
      void defineAtY(Ogre::ManualObject* manualObject, int curIndex,
                     Ogre::Real textureDelta);
      void defineAtZ(Ogre::ManualObject* manualObject, int curIndex,
                     Ogre::Real textureDelta);

      Ogre::Vector3 bottomLeft; /**< Bottom Left Corner */
      Ogre::Vector3 topRight;  /**< Top Right Corner */
      Ogre::Vector3 normal; /**< Normal */
};

/*! This class represents a Manual Object for all indoor vertices
 * of a single texture (being them a floor square or a wall face). */
class IndoorTextureMesh : public Kobold::List, public Kobold::ListElement
{
   public:
      /*! Constructor.
       * \param materialName material to use for the mesh itself. */
      IndoorTextureMesh(Ogre::String materialName);
      /*! Destructor */
      ~IndoorTextureMesh();

      /*! Add a square to the mesh */
      void addSquare(Ogre::Real x1, Ogre::Real y1, Ogre::Real z1,
                     Ogre::Real x2, Ogre::Real y2, Ogre::Real z2,
                     Ogre::Real normX, Ogre::Real nomrY, Ogre::Real normZ);

      /*! Create or update manual object if needed */
      void updateManualObject(Ogre::String baseName);

      /*! \return name of the used material for this mesh. */
      Ogre::String getMaterialName();

   private:

      void deleteSceneNode();

      Ogre::String materialName; /**< Material to use for the mesh. */
      bool dirty; /**< If the vertices list was modified and the manual object
                       must be recreated. */
      Ogre::ManualObject* manualObject; /**< Manual Object for the mesh. */
      Ogre::MeshPtr ogreMesh; /**< Mesh related to the manual object. */
      Ogre::Entity* entity; /**< Entity related to the manual object mesh. */
      Ogre::SceneNode* sceneNode; /**< Scene node related to the mesh. */
};

/*! A List of IndoorTextureMesh'es */
class IndoorTextureMeshes : public Kobold::List
{
   public:
     IndoorTextureMeshes(Ogre::String baseName);
     ~IndoorTextureMeshes();

     IndoorTextureMesh* createTextureMesh(Ogre::String materialName);

     IndoorTextureMesh* getTextureMesh(Ogre::String materialName);

     bool hasTextureMesh(Ogre::String materialName);

     /*! Create or update all dirty manual objects */
     void updateAllDirty();

   private:
      Ogre::String baseName;
};

}

#endif

