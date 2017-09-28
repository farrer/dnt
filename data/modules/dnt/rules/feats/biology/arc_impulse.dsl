# Arc Impulse Talent
# 7th level Biology Student
script(character target)
   dice d
   int sumValue
   int confusionRoll
   
   # Define the variable value to sum
   sumValue = classLevel(SELF_CHARACTER, "BIOLOGY");

   # Define the base damage
   setBaseDice3i(d, 1, 6, sumValue);

   # Do the attack
   doAttackAgainst(SELF_CHARACTER, target, d, "ATTRIBUTE", "STRENGTH", "THING", "ARMATURE_CLASS");

   # Verify if inflicted Confusion to the target
   confusionRoll = rollDice(6) + sumValue
   if(confusionRoll > thingValue(target, "FORTITUDE"))
      # Add Confusion
      addEffect(SELF_CHARACTER, -4, 20, 0, "THING", "I_AM_NOT_A_FOOL")
      addEffect(SELF_CHARACTER, -2, 20, 0, "THING", "REFLEXES")
   end

end
