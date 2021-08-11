project(SuperJumpKickTurbo)

cmake_minimum_required(VERSION 3.5)

include(FindPkgConfig)

### CMake config

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-parentheses -pedantic -std=c++11 -fpermissive -DNETWORK_SUPPORT ${CMAKE_CXX_FLAGS}")

### Sources

set(SJKT_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/configfile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/animation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/bitmapcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/display.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/fontcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/spritesheet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/tilemap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/ttffont.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/checkbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/control.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/form.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/graphicbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/graphic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/hscrollbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/label.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/textbutton.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/textedit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/vscrollbar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/framework.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Network/download.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Network/network.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/angle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/box.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/line.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/vector2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/vector2i.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sound/audio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sound/sfxcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Stages/stagestack.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadeacross.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadein.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadeout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/strips.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/tiled.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/arena.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/boot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/fighter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/matchover.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/networkmenu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/playerselect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/roundcountin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/roundover.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/settingskeyset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/settingsmenu.cpp
)

set(SJKT_H
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/configfile.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/animation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/bitmapcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/display.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/fontcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/spritesheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/tilemap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Display/ttffont.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/event.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/checkbox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/control.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/form.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/forms_enums.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/forms.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/graphicbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/graphic.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/hscrollbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/label.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/list.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/textbutton.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Forms/vscrollbar.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/framework.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/includes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/maths.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/memory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Network/download.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Network/network.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/angle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/box.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/line.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/strings.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/vector2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Primitives/vector2i.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sound/audio.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sound/sfxcache.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Stages/stage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Stages/stagestack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadeacross.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadein.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/fadeout.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/strips.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/tiled.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Framework/Transitions/transitions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/arena.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/boot.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/fighter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/includes.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/matchover.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/menu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/networkmenu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/networkpacket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/playerselect.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/roundcountin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/roundover.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/settingskeyset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game/settingsmenu.h
)

pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 allegro_image-5)
pkg_check_modules(ENET REQUIRED libenet)

add_executable(superjumpkickturbo ${SJKT_SRCS} ${SJKT_H})

target_link_libraries(superjumpkickturbo ${ALLEGRO5_LIBRARIES} ${ENET_LIBRARIES})
target_include_directories(superjumpkickturbo PUBLIC ${ALLEGRO5_INCLUDE_DIRS} ${ENET_INCLUDE_DIRS})

