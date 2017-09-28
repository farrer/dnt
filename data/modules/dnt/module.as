
class DNTModule : ModuleController
{

   DNTModule()
   {
   }

   /*! \return semicolon (;) separated folders to initialize */
   string getBaseFolders()
   {
      string res = "textures/doors;textures/other;textures/furniture;";
      res += "textures/general;textures/humans;textures/portraits;";
      res += "textures/junk;textures/indoor;maps;models;rules;skeletons;";
      res += "sndfx;dialogs;scripts";
      return res;
   }

   /*! \return file with the SkinController to use */
   string getSkinFilename()
   {
      return "skins/gameskin.as";
   }

   /*! \return Rule definition file */
   string getRulesFilename()
   {
      return "rules.dnt"; 
   }

   /*! \return initial map */
   string getInitialMap()
   {
      return "tyrol/house1.map";
   }

   bool characterCreationStep()
   {
      return false;
   }

   void step()
   {
   }


}

