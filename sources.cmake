# This file is the list of sources needed for dnt, dntMapEditor and 
# dntPartEditor executables

########################################################################
# Files related to the core engine
########################################################################
set(CORE_SOURCES
src/core/core.cpp
)
set(CORE_HEADERS
src/core/core.h
)
set(MAP_SOURCES
src/map/indoortexturemesh.cpp
src/map/map.cpp
)
set(MAP_HEADERS
src/map/indoortexturemesh.h
src/map/map.h
)
set(RULES_SOURCES
src/rules/bonusandsaves.cpp
src/rules/dices.cpp
src/rules/door.cpp
src/rules/thing.cpp
src/rules/object.cpp
src/rules/scenery.cpp
)
set(RULES_HEADERS
src/rules/bonusandsaves.h
src/rules/dices.h
src/rules/door.h
src/rules/character.h
src/rules/item.h
src/rules/nonplayablecharacter.h
src/rules/object.h
src/rules/playablecharacter.h
src/rules/scenery.h
src/rules/thing.h
)
set(LANG_SOURCES
src/lang/translate.cpp
)
set(LANG_HEADERS
src/lang/translate.h
)

IF(${APPLE})
   set(APPLE_CONFIG_SOURCES
       macosx/SDLMain.m)
   set(APPLE_CONFIG_HEADERS
       macosx/SDLMain.h)
ENDIF(${APPLE})

IF(${WIN32} OR ${MINGW})
   set(WIN_SOURCES
       windows/resource.rc)
ENDIF(${WIN32} OR ${MINGW})

set(DNT_LIB_HEADERS
${CORE_HEADERS}
${MAP_HEADERS}
${RULES_HEADERS}
${LANG_HEADERS}
${APPLE_CONFIG_HEADERS}
)

set(DNT_LIB_SOURCES
${CORE_SOURCES}
${MAP_SOURCES}
${RULES_SOURCES}
${LANG_SOURCES}
${APPLE_CONFIG_SOURCES}
)

set(DNT_SOURCES
src/core/main.cpp
${WIN_SOURCES}
)
set(DNT_HEADERS
)

