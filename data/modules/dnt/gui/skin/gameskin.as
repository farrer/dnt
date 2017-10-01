
class GameSkin : SkinController
{

   GameSkin()
   {
   }

   int getTotalElements()
   {
      return 12;
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
      else if(name == "experienceBar")
      {
         res = 2;
      }
      else if(name == "healthBar")
      {
         res = 3;
      }
      else if(name == "exitIcon")
      {
         res = 4;
      }
      else if(name == "loadIcon")
      {
         res = 5;
      }
      else if(name == "saveIcon")
      {
         res = 6;
      }
      else if(name == "attackIcon")
      {
         res = 7;
      }
      else if(name == "endTurnIcon")
      {
         res = 8;
      }
      else if(name == "questLogIcon")
      {
         res = 9;
      }
      else if(name == "iconsPanel")
      {
         res = 10;
      }
      else if(name == "portraitArea")
      {
         res = 11;
      }

      return res;
   }

   string getSkinFilename()
   {
      return "skin/moderna.skin";
   }

}

