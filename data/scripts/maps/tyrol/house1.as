
class House1Ctrl : MapController
{
   House1Ctrl(string mapname)
   {
      mapName = mapname;
   }

   void onLoad()
   {
      playSound(0, 0, 0, "other/battleMode-yourTurn.ogg");
   }

   string mapName;
}

