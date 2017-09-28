# Supreme Esthetic Shock Talent
# for 20th level ADM
script(character target)
   dice d
   int sumValue
   
   # Define the variable value to sum
   sumValue = skillPoints(SELF_CHARACTER, "BIZARRE_CLOTHES");

   # Define the base damage
   setBaseDice3i(d, 3, 12, sumValue);

   # Do the attack
   doAttackWithoutResistence(SELF_CHARACTER, target, d);

   # TODO: some effects
end
