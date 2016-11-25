#ifndef _dnt_core_h_
#define _dnt_core_h

#include <goblin/baseapp.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>
#include "../map/map.h"

namespace DNT
{

#define APP_NAME                "dnt"
#define DEFAULT_WINDOW_WIDTH     1024
#define DEFAULT_WINDOW_HEIGHT     768
#define WIDTH_TO_DOUBLE_SIZE     2000
#define BASE_DATA_DIR           "../data"

/*! The core (main entry) implementation of DNT. */
class Core : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      Core();
      /*! Destructor */
      ~Core();
   protected:

      Ogre::String getApplicationUnixName() { return APP_NAME; };
      int getDefaultWindowWidth() { return DEFAULT_WINDOW_WIDTH; };
      int getDefaultWindowHeight() { return DEFAULT_WINDOW_HEIGHT; };
      Orientation getDefaultOrientation() { return LANDSCAPE; };
      int getMinWidthToUseDoubleSizedGui() { return WIDTH_TO_DOUBLE_SIZE; };
      Ogre::String getBaseDataDir() {return BASE_DATA_DIR;};

      bool doInit();
      void getDataDirectories(Ogre::String** dataDirectories,
            Ogre::String** dataGroups, int& total);

      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();
      void doCycle();
      bool shouldQuit();

   private:

      void initBlendMaps(Ogre::Terrain* terrain);

      Ogre::TerrainGlobalOptions* terrainGlobalOptions;
      Ogre::TerrainGroup* terrainGroup;

      DNT::Map* map;
};

}

#endif


