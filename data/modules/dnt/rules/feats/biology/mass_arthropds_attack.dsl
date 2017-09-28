# Mass Arthropds Attack Talent
# Biology Student 10th level
script(float x, float y, float z)

   dice d
   int numDices
   character target

   # Set Dices
   setBaseDice(d, "2*d8+0")
   numDices = classLevel(SELF_CHARACTER, "BIOLOGY") - 10
   if(numDices > 0) 
      setAditionalDice(d, numDices, "1*d6+0")
   end

   # TODO: Call animation of insects!

   # TODO: Call insects sound!

   # Do the attack to all characters in area
   doAttackOnArea(SELF_CHARACTER, x, z, 10, d, "ATTRIBUTE", "CHARISMA", "THING", "FORTITUDE") 

end

