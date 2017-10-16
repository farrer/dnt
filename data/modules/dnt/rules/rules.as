
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

   /*! Add the roll check text to the briefieng */
   void briefCheckText(string testName, int testRoll, int againstRoll)
   {
      bool res = testRoll >= againstRoll;
      brief(testName + ": " + testRoll + " x " + againstRoll + ": ");
      if(res)
      {
         brief(gettext("Success."), 0, 250, 0);
      }
      else
      {
         brief(gettext("Failure."), 250, 0, 0);
      }
      briefLineBreak();
   }

   /*! Roll a test of a RuleDefinition against another one */
   bool roll(RuleDefinition@ test, RuleDefinition@ against)
   {
      int testRoll = rollCheck(test);
      int againstRoll = rollCheck(against);
      briefCheckText(test.getName(), testRoll, againstRoll);
      return testRoll >= againstRoll;
   }

   /*! Roll a test of a RuleDefinition against a fixed value */
   bool roll(RuleDefinition@ test, int against)
   {
      int testRoll = rollCheck(test);
      briefCheckText(test.getName(), testRoll, against);
      return testRoll >= against;
   }

   /*! Check if a Character can interact with a target Object.
    * Note that the 'at sight' verification is already made by the engine
    * before calling this function. */
   bool canInteract(Character@ actor, Object@ target)
   {
      //TODO: check if at battle mode.
      if(actor.distance(target) > actor.getDisplacement())
      {
         brief(gettext("The object is too far away!"), 250, 0, 0);
         briefLineBreak();
         return false;
      }

      return true;
   }

}

