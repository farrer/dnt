" Vim syntax file
" Language: DNT Script Language
" Maintainer: Farrer <farrer@dnteam.org>
" Last Change: 2012 Aug 11

syn clear

" Key Words
syn keyword dntStatement      script end
syn keyword dntConditional    if else
syn keyword dntRepeat         while

" String
syn region dntString         start=+"+   end=+"+
syn match dntNumber          "-\=\<\d\+\>"
syn match dntByte             "\$[0-9a-fA-F]\+\>"

" Comments
syn match   dntComment       "#.*$" contains=dntTodo
syn keyword dntTodo          contained TODO FIXME XXX NOTE


" Operators and Types
syn keyword dntOperator       SELF_OBJECT SELF_CHARACTER 
syn keyword dntOperator       ACTIVE_CHARACTER TARGET_CHARACTER
syn keyword dntOperator       ACTUAL_MAP SELF_MISSION
syn keyword dntOperator       OWNER_HEIGHT OWNER_POSX OWNER_POSZ
syn keyword dntOperator       void bool int float string dice character
syn keyword dntOperator       object weapon
syn keyword dntOperator       true false

" Functions
syn keyword dntFunction       wait print briefing briefingColor exit
syn keyword dntFunction       gettext
syn keyword dntFunction       moveToPosition moveToCharacter moveFromCharacter
syn keyword dntFunction       moveToObject setIdle
syn keyword dntFunction       missionAdd missionComplete missionIsActive
syn keyword dntFunction       missionAbort missionCompletionValue
syn keyword dntFunction       missionSetTemp missionGetTemp missionSetXp
syn keyword dntFunction       featActualQuantity featCost featQuantityPerDay
syn keyword dntFunction       getRandomAttackFeat getPowerfullAttackFeat
syn keyword dntFunction       getRandomHealFeat getPowerfullHealFeat
syn keyword dntFunction       skillPoints attModifier thingValue
syn keyword dntFunction       combatEnter getNearestEnemy setTargetCharacter
syn keyword dntFunction       getNPCByName isAlive
syn keyword dntFunction       getPsycho setPsycho getActualLife getMaxLife
syn keyword dntFunction       characterAtRange canAttack
syn keyword dntFunction       callAnimation setAnimation
syn keyword dntFunction       featUseAtCharacter runAwayFromBattle
syn keyword dntFunction       inventoryHave inventoryGetItemByInfo
syn keyword dntFunction       inventoryGetItemValue inventoryGiveItem
syn keyword dntFunction       inventoryAddMoney inventoryCountItemByInfo
syn keyword dntFunction       dialogSetInitial
syn keyword dntFunction       getObject getObjectState
syn keyword dntFunction       isAllDead mapTravel
syn keyword dntFunction       addFead haveFeat
syn keyword dntFunction       openCharacterDialog
syn keyword dntFunction       classLevel roll rollDice addEffect
syn keyword dntFunction       particleAddToCharacter
syn keyword dntFunction       setBaseDice setAditionalDice getBaredHandsDice
syn keyword dntFunction       setBaseDice3i showText showTextStatic
syn keyword dntFunction       doAttack doAttackAgainst featDec doAttackOnArea
syn keyword dntFunction       doAttackWithoutResistence
syn keyword dntFunction       weaponEquiped getEquipedWeapon
syn keyword dntFunction       weaponGetAmmoType weaponGetAmmo weaponDecAmmo
syn keyword dntFunction       weaponGetRangeType weaponGetRange weaponGetDice
syn keyword dntFunction       weaponSound weaponSoundAtCharacter
syn keyword dntFunction       playSound

syn sync lines=250

if !exists("did_dnt_syntax_inits")
   let did_dnt_syntax_inits = 1
   " The default methods for highlighting.  Can be overridden later
   hi link dntStatement       Statement
   hi link dntConditional     Conditional
   hi link dntRepeat          Repeat
   hi link dntString          String
   hi link dntNumber          Number
   hi link dntByte            Number
   hi link dntComment         Comment
   hi link dntTodo            Todo
   hi link dntOperator        Operator
   hi link dntFunction        Function
endif


let b:current_syntax = "dnt"



