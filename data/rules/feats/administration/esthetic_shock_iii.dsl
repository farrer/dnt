# Esthetic Shock III Talent
# for 15th level ADM
script(character target)
   dice d
   int sumValue
   
   # Define the variable value to sum
   sumValue = skillPoints(SELF_CHARACTER, "BIZARRE_CLOTHES");

   # Define the base damage
   setBaseDice3i(d, 1, 4, sumValue);

   # Do the attack
   doAttackWithoutResistence(SELF_CHARACTER, target, d);

   # TODO: some effects
end
