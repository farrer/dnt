
class GameSkin : SkinController
{

   GameSkin()
   {
   }

   int getTotalElements()
   {
      return 11;
   }

   int getElementType(string name)
   {
      int res = -1; // unknow element

      if(name == "panelGrid")
      {
         res = 0;
      }
      else if(name == "portrait")
      {
         res = 1;
      }
      else if(name == "exitIcon")
      {
         res = 2;
      }
      else if(name == "loadIcon")
      {
         res = 3;
      }
      else if(name == "saveIcon")
      {
         res = 4;
      }
      else if(name == "attackIcon")
      {
         res = 5;
      }
      else if(name == "endTurnIcon")
      {
         res = 6;
      }
      else if(name == "questLogIcon")
      {
         res = 7;
      }
      else if(name == "iconsPanel")
      {
         res = 8;
      }
      else if(name == "portraitArea")
      {
         res = 9;
      }
      else if(name == "lifeBarFill")
      {
         res = 10;
      }

      return res;
   }

   string getSkinFilename()
   {
      return "skin/moderna.skin";
   }

}

