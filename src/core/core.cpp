
#include <farso/controller.h>
#include <farso/font.h>

#include "core.h"
#include "../rules/alignment.h"

#include <OGRE/Terrain/OgreTerrainMaterialGenerator.h>

using namespace DNT;

Core::Core()
{
   terrainGlobalOptions = NULL;
   terrainGroup = NULL;
   map = NULL;
}


Core::~Core()
{
   if(map != NULL)
   {
      delete map;
   }
   if(terrainGlobalOptions != NULL)
   {
      delete terrainGlobalOptions;
   }
   if(terrainGroup != NULL)
   {
      delete terrainGroup;
   }

   Alignments::finish();
   Farso::Controller::finish();
}


bool Core::doInit()
{
#if 0
   terrainGlobalOptions = new Ogre::TerrainGlobalOptions();
   terrainGlobalOptions->setDefaultMaterialGenerator(Ogre::TerrainMaterialGeneratorPtr(new Ogre::TerrainMaterialGenerator));

   terrainGroup = new Ogre::TerrainGroup(ogreSceneManager, 
         Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);

   /* Configure globals */

   /* Terrain accuracy */
   terrainGlobalOptions->setMaxPixelError(8);
   /* Max distance to still apply light to the terrain.  */
   terrainGlobalOptions->setCompositeMapDistance(3000);

   /* Light info */
   terrainGlobalOptions->setLightMapDirection(
         mainLight->getDerivedDirection());
   terrainGlobalOptions->setCompositeMapAmbient(
         ogreSceneManager->getAmbientLight());
   terrainGlobalOptions->setCompositeMapDiffuse(
         mainLight->getDiffuseColour());

   /* Terrain import data definitions */
   Ogre::Terrain::ImportData& importData = 
      terrainGroup->getDefaultImportSettings();
   importData.terrainSize = 513;
   importData.worldSize = 12000.0f;
   importData.inputScale = 600;
   importData.minBatchSize = 33;
   importData.maxBatchSize = 65;

   /* Define terrain textures */
   importData.layerList.resize(3);
   importData.layerList[0].worldSize = 100;
   importData.layerList[0].textureNames.push_back(
         "dirt_grayrocky_diffusespecular.dds");
   importData.layerList[0].textureNames.push_back(
         "dirt_grayrocky_normalheight.dds");
   importData.layerList[1].worldSize = 30;
   importData.layerList[1].textureNames.push_back(
         "grass_green-01_diffusespecular.dds");
   importData.layerList[1].textureNames.push_back(
         "grass_green-01_normalheight.dds");
   importData.layerList[2].worldSize = 200;
   importData.layerList[2].textureNames.push_back(
         "growth_weirdfungus-03_diffusespecular.dds");
   importData.layerList[2].textureNames.push_back(
         "growth_weirdfungus-03_normalheight.dds");
   
   bool terrainsImported = false;

   /* Define and load terrain */
   for(int x = 0; x <= 0; x++) 
   {
      for(int y = 0; y <= 0; ++y)
      {  
         Ogre::String filename = terrainGroup->generateFilename(x, y);
         bool exists =
            Ogre::ResourceGroupManager::getSingleton().resourceExists(
                  terrainGroup->getResourceGroup(), filename);
         if(exists)
         {
            terrainGroup->defineTerrain(x, y);
         }
         else
         {
            Ogre::Image img;
            img.load("terrain.png", "terrain"); 
            terrainGroup->defineTerrain(x, y, &img);

            terrainsImported = true;
         }
      }
   }
   terrainGroup->loadAllTerrains(true);

   if(terrainsImported)
   {
      Ogre::TerrainGroup::TerrainIterator ti = 
         terrainGroup->getTerrainIterator();

      while (ti.hasMoreElements())
      {
         Ogre::Terrain* t = ti.getNext()->instance;
         initBlendMaps(t);
      }
   }

   terrainGroup->freeTemporaryResources();
#endif
   /* Init Farso */
   Farso::Controller::init(Farso::RENDERER_TYPE_OGRE3D, DEFAULT_WINDOW_WIDTH,
       DEFAULT_WINDOW_HEIGHT, 32, "", getSceneManager());
   Farso::FontManager::setDefaultFont("LiberationSans-Regular.ttf");
   Farso::Controller::loadSkin("skins/moderna.skin");
   Farso::Controller::setCursor("cursor/sel.png");

   /* Init our alignments */
   Alignments::init();

   /* Load a map to test. FIXME: remove from here when reimplemented our
    * initial window. */
   map = new DNT::Map(ogreSceneManager);
   if(!map->load("tyrol/house1.map"))
   {
      return false;
   }

   return true;
}

void Core::initBlendMaps(Ogre::Terrain* terrain)
{
   Ogre::Real minHeight0 = 70;
   Ogre::Real fadeDist0 = 40;
   Ogre::Real minHeight1 = 70;
   Ogre::Real fadeDist1 = 15;

   Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
   Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);

   float* pBlend0 = blendMap0->getBlendPointer();
   float* pBlend1 = blendMap1->getBlendPointer();

   for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
   {
      for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
      {
         Ogre::Real tx, ty;

         blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
         Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
         Ogre::Real val = (height - minHeight0) / fadeDist0;
         val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
         *pBlend0++ = val;

         val = (height - minHeight1) / fadeDist1;
         val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
         *pBlend1++ = val;
      }
   }

   blendMap0->dirty();
   blendMap1->dirty();
   blendMap0->update();
   blendMap1->update();
}

void Core::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"gui", "terrain", "textures", "maps",
      "models", "fonts", "rules"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 7;
}

void Core::doLowMemoryClean()
{
}

void Core::doSendToBackground()
{
}

void Core::doSendToForeground()
{
}

void Core::doCycle()
{
   map->update();
   
   //TODO: right button pressed!
   if(Farso::Controller::verifyEvents(leftButtonPressed, false, mouseX, mouseY))
   {
      Farso::Event event = Farso::Controller::getLastEvent();
      printf("Event got: %d\n", event.getType());
   }
}

bool Core::shouldQuit()
{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
   return Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_ESCAPE);
#endif
   return false;
}

