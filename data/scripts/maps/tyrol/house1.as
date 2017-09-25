
class House1Ctrl : MapController
{
   House1Ctrl(string mapname)
   {
      mapName = mapname;
   }

   void onLoad()
   {
      playSound(0, 0, 0, "other/battleMode-yourTurn.ogg");
      sleep(1);
      Character@ c = getCharacter("humans/padre.npc");
      c.moveToPosition(4.6, 20.95);
   }

   void step()
   {
      sleep(3);
      playSound(0, 0, 0, "other/battleMode-yourTurn.ogg");
   }

   string mapName;
   Widget widget("maps/tyrol/header_menu.as");
}

