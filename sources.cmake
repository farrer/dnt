# This file is the list of sources needed for dnt, dntMapEditor and 
# dntPartEditor executables

########################################################################
# Files related to the core engine
########################################################################
set(CORE_SOURCES
   src/core/commonapp.cpp
   src/core/dialog.cpp
   src/core/door.cpp
   src/core/game.cpp
   src/core/inventory.cpp
   src/core/item.cpp
   src/core/itemslot.cpp
   src/core/modstate.cpp
   src/core/money.cpp
   src/core/object.cpp
   src/core/playablecharacter.cpp
   src/core/scenery.cpp
   src/core/util.cpp
)
set(CORE_HEADERS
   src/core/commonapp.h
   src/core/dialog.h
   src/core/door.h
   src/core/game.h
   src/core/inventory.h
   src/core/item.h
   src/core/itemslot.h
   src/core/modstate.h
   src/core/money.h
   src/core/nonplayablecharacter.h
   src/core/object.h
   src/core/playablecharacter.h
   src/core/scenery.h
   src/core/util.h
)
set(MAP_SOURCES
   src/map/light.cpp
   src/map/map.cpp
   src/map/mapmesh.cpp
   src/map/npcfile.cpp
   src/map/wall.cpp
)
set(MAP_HEADERS
   src/map/map.h
   src/map/mapmesh.h
   src/map/light.h
   src/map/npcfile.h
   src/map/wall.h
)
set(RULES_SOURCES
   src/rules/character.cpp
   src/rules/dices.cpp
   src/rules/modeffect.cpp
   src/rules/ruledef.cpp
   src/rules/thing.cpp
)
set(RULES_HEADERS
   src/rules/character.h
   src/rules/dices.h
   src/rules/modeffect.h
   src/rules/ruledef.h
   src/rules/thing.h
)
set(FIGHT_SOURCES
)
set(FIGHT_HEADERS
)
set(GUI_SOURCES
   src/gui/briefing.cpp
   src/gui/dialogwindow.cpp
   src/gui/skin.cpp
)
set(GUI_HEADERS
   src/gui/briefing.h
   src/gui/dialogwindow.h
   src/gui/skin.h
)
set(AI_HEADERS
   src/ai/agent.h
   src/ai/astar.h
   src/ai/patternagent.h
)
set(AI_SOURCES
   src/ai/agent.cpp
   src/ai/astar.cpp
   src/ai/patternagent.cpp
)
set(SCRIPT_HEADERS
   src/script/mapscript.h
   src/script/pendingaction.h
   src/script/rulescript.h
   src/script/ruledefscript.h
   src/script/scriptcontroller.h
   src/script/scriptinstance.h
   src/script/scriptmanager.h
   src/script/scriptobject.h
   src/script/scriptobjectcharacter.h
   src/script/scriptobjectdice.h
   src/script/scriptobjectobject.h
   src/script/scriptobjectruledef.h
   src/script/scriptobjectrulegroup.h
   src/script/scriptstdstring.h
   src/script/skinscript.h
   src/script/widgetscript.h
)
set(SCRIPT_SOURCES
   src/script/mapscript.cpp
   src/script/pendingaction.cpp
   src/script/rulescript.cpp
   src/script/ruledefscript.cpp
   src/script/scriptcontroller.cpp
   src/script/scriptinstance.cpp
   src/script/scriptmanager.cpp
   src/script/scriptobject.cpp
   src/script/scriptobjectcharacter.cpp
   src/script/scriptobjectdice.cpp
   src/script/scriptobjectobject.cpp
   src/script/scriptobjectruledef.cpp
   src/script/scriptobjectrulegroup.cpp
   src/script/scriptstdstring.cpp
   src/script/skinscript.cpp
   src/script/widgetscript.cpp
)
set(LANG_SOURCES
   src/lang/translate.cpp
)
set(LANG_HEADERS
   src/lang/translate.h
)
set(COLLISION_SOURCES
   src/collision/collision.cpp
   src/collision/element.cpp
   src/collision/overlaps.cpp
   src/collision/square.cpp
)
set(COLLISION_HEADERS
   src/collision/collision.h
   src/collision/overlaps.h
)
set(TERRA_SOURCES
   src/terra/Terra.cpp
   src/terra/TerrainCell.cpp
   src/terra/TerraShadowMapper.cpp
   src/terra/Hlms/OgreHlmsJsonTerra.cpp
   src/terra/Hlms/OgreHlmsTerra.cpp
   src/terra/Hlms/OgreHlmsTerraDatablock.cpp
   src/terra/Hlms/PbsListener/OgreHlmsPbsTerraShadows.cpp
)
set(TERRA_HEADERS
   src/terra/Terra.h
   src/terra/TerrainCell.h
   src/terra/TerraShadowMapper.h
   src/terra/Hlms/OgreHlmsJsonTerra.h
   src/terra/Hlms/OgreHlmsTerra.h
   src/terra/Hlms/OgreHlmsTerraDatablock.h
   src/terra/Hlms/OgreHlmsTerraPrerequisites.h
   src/terra/Hlms/PbsListener/OgreHlmsPbsTerraShadows.h
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
   ${AI_HEADERS}
   ${SCRIPT_HEADERS}
   ${CORE_HEADERS}
   ${MAP_HEADERS}
   ${RULES_HEADERS}
   ${FIGHT_HEADERS}
   ${LANG_HEADERS}
   ${COLLISION_HEADERS}
   ${GUI_HEADERS}
   ${TERRA_HEADERS}
   ${APPLE_CONFIG_HEADERS}
)

set(DNT_LIB_SOURCES
   ${AI_SOURCES}
   ${SCRIPT_SOURCES}
   ${CORE_SOURCES}
   ${MAP_SOURCES}
   ${RULES_SOURCES}
   ${FIGHT_SOURCES}
   ${LANG_SOURCES}
   ${COLLISION_SOURCES}
   ${GUI_SOURCES}
   ${TERRA_SOURCES}
   ${APPLE_CONFIG_SOURCES}
)

set(DNT_SOURCES
   src/core/main.cpp
   src/core/core.cpp
   ${WIN_SOURCES}
)
set(DNT_HEADERS
   src/core/core.h
)

set(DNT_MAP_EDITOR_SOURCES
   src/apps/mapeditor/lightwindow.cpp
   src/apps/mapeditor/main.cpp
   src/apps/mapeditor/maingui.cpp
   src/apps/mapeditor/mapeditor.cpp
   src/apps/mapeditor/metadatagui.cpp
   src/apps/mapeditor/nodeswindow.cpp
   src/apps/mapeditor/positioneditor.cpp
   src/apps/mapeditor/transformwindow.cpp
   src/apps/mapeditor/attenuationtextentry.cpp
   src/apps/mapeditor/vector3textentry.cpp
   ${WIN_SOURCES}
)
set(DNT_MAP_EDITOR_HEADERS
   src/apps/mapeditor/lightwindow.h
   src/apps/mapeditor/maingui.h
   src/apps/mapeditor/mapeditor.h
   src/apps/mapeditor/metadatagui.h
   src/apps/mapeditor/nodeswindow.h
   src/apps/mapeditor/positioneditor.h
   src/apps/mapeditor/transformwindow.h
   src/apps/mapeditor/attenuationtextentry.h
   src/apps/mapeditor/vector3textentry.h
)

