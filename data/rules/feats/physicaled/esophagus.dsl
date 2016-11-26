# Script defining the use of a powerfull esophagus
script(character target)

   dice d;
   int level;
   int numDices;
   int range;

   # Set the range and base dice
   range = 7;
   setBaseDice(d, "6*d6+0");

   # Get the class level
   level = classLevel(SELF_CHARACTER, "PHYSICAL_EDUCATION");

   # Verify if is a 15+ Physical education character
   if(level >= 15)

      # Calculate the aditional dice, based on class level
      numDices = level-15;
      setAditionalDice(d, numDices, "2*d6+0"); 
      
      # TODO: call esophagus animation

      # Roll the attack against the target
      if(doAttack(SELF_CHARACTER, target, d, "CONSTITUTION")
         # Dec the number of powerfull esophagus
         featDec("POWERFUL_ESOPHAGUS", -1);
      end

   end

end

