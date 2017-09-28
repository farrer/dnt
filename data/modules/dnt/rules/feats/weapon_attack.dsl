# weapon_attack.dsl : This script implements the basic attack
# (unarmed or with the equiped weapon).

script(character target)

   dice d
   int curAmmo
   weapon curWeapon
   string concept
   
   # Initialize to some defaults
   concept = "STRENGTH"

   # Verify if is an weapon attack or unarmed
   if(weaponEquiped(SELF_CHARACTER) == true)

      # With an weapon. let's see if it uses ammo or not
      curWeapon = getEquipedWeapon(SELF_CHARACTER)

      if(weaponGetAmmoType(curWeapon) != "none")
         # Verify if have ammo to use
         if(weaponGetAmmo(curWeapon) <= 0)
            briefingColor(gettext("Out of ammo!"), 255, 10, 10)
            exit()
         end
      end

      # Verify if is ranged (ranged == use DEXTERITY)
      if(weaponGetRangeType(curWeapon) == "Ranged")
         concept = "DEXTERITY"
      end

      # Get weapon's dice
      d = weaponGetDice(curWeapon)

      # Do the attack
      if(doAttack(SELF_CHARACTER, target, d, "ATTRIBUTE", concept))

         # Make the weapon's sound
         weaponSoundAtCharacter(curWeapon, SELF_CHARACTER)
         
         # Apply the ammo - 1, if needed
         if(weaponGetAmmoType(curWeapon) != "none")
            
            if(weaponDecAmmo(curWeapon) <= 0)
               # Show a message if no more ammo, to allow pc reload
               briefingColor(gettext("No more ammo. Reload your weapon!"), 235, 80, 10)
            end
         end
      end

   else
      # Unarmed attack, get the bared hands dice
      d = getBaredHandsDice(SELF_CHARACTER)
      
      # Call the bare hands sound
      playBareHandsSound(SELF_CHARACTER)

      # Do the attack
      doAttack(SELF_CHARACTER, target, d, "ATTRIBUTE", concept)

   end

end


