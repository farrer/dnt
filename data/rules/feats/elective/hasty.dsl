# Hasty Elective Talent
script()

   int curDis
   int sumV

   # Get the current displacement
   curDis = thingValue(SELF_CHARACTER, "DISPLACEMENT") 

   # Calculate its sum value
   sumV = ( (20 * curDis) / 100 );

   if(sumV > 0)
      addEffect(SELF_CHARACTER, sumV, 0, 0, "THING", "DISPLACEMENT")
   else
      addEffect(SELF_CHARACTER, sumV, 0, 0, "THING", "DISPLACEMENT")
   end
end
