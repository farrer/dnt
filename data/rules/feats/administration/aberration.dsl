# Aberration talent
# ADM level 20
script()
   # +2 Constitution
   addEffect(SELF_CHARACTER, 2, 0, 0, "ATTRIBUTE", "CONSTITUTION")
   # +2 Strength
   addEffect(SELF_CHARACTER, 2, 0, 0, "ATTRIBUTE", "STRENGTH")
   # +5 Taylorism
   # addEffect(SELF_CHARACTER, 5, 0, 0, "SKILL", "TAYLORISM")
   # -2 Charisma
   addEffect(SELF_CHARACTER, -2, 0, 0, "ATTRIBUTE", "CHARISMA")
   # -4 Intelligence
   addEffect(SELF_CHARACTER, -4, 0, 0, "ATTRIBUTE", "INTELLIGENCE")
   # -4 I am not a fool
   addEffect(SELF_CHARACTER, -4, 0, 0, "THING" "I_AM_NOT_A_FOOL")
end
