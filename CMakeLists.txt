# ########## Project setup ##########
PROJECT(Cyberpunk-roguelike)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

Find_Package ( SDL REQUIRED )
Find_Package ( SDL_image REQUIRED )
Find_Package ( SDL_ttf REQUIRED )

# ########## Cyberpunk-roguelike executable ##########
# Sources:
SET(Cyberpunk-roguelike_executable_SRCS
    CMap.cpp
    CApp.cpp
    CTile.cpp
    CInterface.cpp
    CUnit.cpp
    CEvent.cpp
    CSurface.cpp
)

# Headers:
SET(Cyberpunk-roguelike_executable_HDRS
    CMap.h
    Define.h
    CInterface.h
    CTile.h
    CUnit.h
    CSurface.h
    CApp.h
    CEvent.h
)

link_libraries (
   ${SDL_LIBRARY}
   SDLmain
   SDL_image
   SDL_ttf
)

# actual target:
ADD_EXECUTABLE(Cyberpunk-roguelike ${Cyberpunk-roguelike_executable_SRCS})

# add install target:
INSTALL(TARGETS Cyberpunk-roguelike DESTINATION bin)
