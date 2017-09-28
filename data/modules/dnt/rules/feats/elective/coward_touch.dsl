# Coward Touch Elective Talent
script(character target)
   dice d
   int sumValue
   
   # Define the variable value to sum
   sumValue = thingValue(SELF_CHARACTER, "LEVEL");

   # Define the base damage
   setBaseDice3i(d, 1, 3, sumValue);

   # Do the attack
   doAttack(SELF_CHARACTER, target, d, "ATTRIBUTE", "STRENGTH");

   # TODO: some effects
end
