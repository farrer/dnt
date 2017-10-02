/* Main Screen implementation */
class MainWidget : WidgetController
{
   MainWidget()
   {
      curXp = 0;
      maxXp = 0;
      curHp = 0;
      maxHp = 0;
   }

   string getWidgetFilename()
   {
      return "widgets/main.json";
   }

   void onEvent(int type, string id)
   {
   }

   bool shouldQuit()
   {
      return false;
   }

   void updatePortraitBars()
   {
      Character@ pc = getPlayableCharacter();
      RuleDefinition@ xp = pc.getRuleDefinition("trait", "xp");
      RuleDefinition@ hp = pc.getRuleDefinition("trait", "life_points");
 
      int hpVal = hp.getValue();
      int hpMax = hp.getMaxValue();
      int xpVal = xp.getValue();
      int xpMax = xp.getMaxValue();

      if((xpVal != curXp) || (xpMax != maxXp))
      {
         curXp = xpVal;
         maxXp = xpMax;
         progressBarSet("xpBar", curXp, maxXp);
      }
      if((hpVal != curHp) || (hpMax != maxHp))
      {
         curHp = hpVal;
         maxHp = hpMax;
         progressBarSet("hpBar", curHp, maxHp);
      }
   }

   void step()
   {
      updatePortraitBars();
   }

   int curXp;
   int maxXp;
   int curHp;
   int maxHp;
}

