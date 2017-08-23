
class GameSkin : SkinController
{

   GameSkin()
   {
   }

   int getTotalElements()
   {
      return 4;
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

      return res;
   }

   string getSkinFilename()
   {
      return "skins/moderna.skin";
   }

}

