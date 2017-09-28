# Esthetic Shock II Talent
# for 1st level ADM
script(character target)
   dice d
   int sumValue
   
   # Define the variable value to sum
   sumValue = skillPoints(SELF_CHARACTER, "BIZARRE_CLOTHES") / 2;

   # Define the base damage
   setBaseDice3i(d, 1, 6, sumValue);

   # Do the attack
   doAttackAgainst(SELF_CHARACTER, target, d, "SKILL", "BIZARRE_CLOTHES", "THING", "I_AM_NOT_A_FOOL");

   # TODO: some effects
end
