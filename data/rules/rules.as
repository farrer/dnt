
class DNTRules : RuleController
{

   DNTRules()
   {
   }

   /*! Roll a rule definition check */
   int rollCheck(RuleDefinition@ def)
   {
      Dice d20(20);
      int res = 0;
      int mod = 0;
      string groupId = def.getGroupId();
      if(groupId == "skill")
      {
         /* Calculate its base attribute modifier */
         RuleDefinition@ baseAtt = def.getRelated();
         if(baseAtt !is null)
         {
            mod = (baseAtt.getValue() - 10) / 2;
         }

         res = d20.roll() + def.getValue() + mod;
      }
      else if(groupId == "attribute")
      {
         /* Calculate its modifier */
         mod = (def.getValue() - 10) / 2;
         res = d20.roll() + mod;
      }

      return res;
   }

   /*! Roll a test of a RuleDefinition against another one */
   bool roll(RuleDefinition@ test, RuleDefinition@ against)
   {
      int testRoll = rollCheck(test);
      int againstRoll = rollCheck(against);
      return testRoll >= againstRoll;
   }

   /*! Roll a test of a RuleDefinition against a fixed value */
   bool roll(RuleDefinition@ test, int against)
   {
      int testRoll = rollCheck(test);
      return testRoll >= against;
   }

}

