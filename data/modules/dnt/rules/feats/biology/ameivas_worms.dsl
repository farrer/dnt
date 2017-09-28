# Ameiva's Worms
# Talent of a Biology Student 4th level

script(character target)

   dice d
   int sumValue

   sumValue = classLevel(SELF_CHARACTER, "BIOLOGY")
   setBaseDice3i(d, 1, 6, sumValue)

   doAttackAgainst(SELF_CHARACTER, target, d, "ATTRIBUTE", "CHARISMA", "ATTRIBUTE", "CONSTITUTION")

end

