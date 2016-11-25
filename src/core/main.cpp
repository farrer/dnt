
#include "core.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   #include "windows.h"
#endif
 

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS &&\
    OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
/***********************************************************************
 *                                main                                 *
 ***********************************************************************/
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
      LPSTR lpCmdLine, int nCmdShow) 
#else
int main (int argc, char *argv[])
#endif
{
   /* Create and run the game */
   DNT::Core* core = new DNT::Core();
   core->run();
   delete core;
}
#endif
