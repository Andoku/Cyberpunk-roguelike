Project ( rog )
cmake_minimum_required(VERSION 2.8)

set ( SOURCES CApp.cpp CApp.h CEvent.h CInterface.h CMap.h CSurface.h CTile.h CUnit.h Define.h )

set(CMAKE_C_FLAGS "-std=c99")

Find_Package ( SDL REQUIRED )
Find_Package ( SDL_image REQUIRED )
Find_Package ( SDL_ttf REQUIRED )

link_libraries (
   ${SDL_LIBRARY}
   SDLmain
   SDL_image
   SDL_ttf
)

add_executable (
   rog
   ${SOURCES}
)
