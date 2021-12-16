cmake_minimum_required(VERSION 3.17)
project(TP3)

SET(SDL2_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        ${SDL2_PATH}
        )

FIND_PATH(SDL2_INCLUDE_DIR SDL.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PATH(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
        HINTS
        $ENV {SDL2IMAGEDIR}
        $ENV {SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_LIBRARY_TEMP
        NAMES SDL2
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PACKAGE(Threads)
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

IF(NOT SDL2_BUILDING_LIBRARY)
    IF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
        FIND_LIBRARY(SDL2MAIN_LIBRARY
                NAMES SDL2main
                HINTS
                $ENV{SDL2DIR}
                PATH_SUFFIXES lib64 lib
                PATHS ${SDL2_SEARCH_PATHS}
                )
    ENDIF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
ENDIF(NOT SDL2_BUILDING_LIBRARY)

IF(NOT APPLE)
    FIND_PACKAGE(Threads)
ENDIF(NOT APPLE)

FIND_LIBRARY(420C35JO_TP3_LIBRARY ./lib420C35JOTP3.a)

IF(MINGW)
    SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
ENDIF(MINGW)

IF(SDL2_LIBRARY_TEMP)
    IF(NOT SDL2_BUILDING_LIBRARY)
        IF(SDL2MAIN_LIBRARY)
            SET(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
        ENDIF(SDL2MAIN_LIBRARY)
    ENDIF(NOT SDL2_BUILDING_LIBRARY)

    IF(APPLE)
        SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
    ENDIF(APPLE)

    IF(NOT APPLE)
        SET(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
    ENDIF(NOT APPLE)

    IF(MINGW)
        SET(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2_LIBRARY_TEMP})
    ENDIF(MINGW)

    SET(SDL2_LIBRARY ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
    SET(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")
ENDIF(SDL2_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

include_directories(Optional)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SOURCE_FILES LIST_DIRECTORIES false "*.cpp" "*.hpp" "*.h")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#SECTION DE TESTS
enable_testing()
add_executable(BSTest MyTests/testFile.cpp)

    #BSTREE REMOVE

#Feuille Unique
add_test(Unique_Leaf_ROOT BSTest 0)
add_test(Unique_Leaf_LEFT BSTest 1)
add_test(Unique_Leaf_RIGHT BSTest 2)

#1 Branche a gauche
add_test(One_Branch_G_ROOT BSTest 3)
add_test(One_Branch_G_LEFT BSTest 4)
add_test(One_Branch_G_RIGHT BSTest 5)

#1 Branche a droite
add_test(One_Branch_D_ROOT BSTest 6)
add_test(One_Branch_D_LEFT BSTest 7)
add_test(One_Branch_D_RIGHT BSTest 8)

#Contient 2 branches
add_test(Double_Branch_ROOT BSTest 9)
add_test(Double_Branch_LEFT BSTest 10)
add_test(Double_Branch_RIGHT BSTest 11)

#Cas particulier
add_test(Cas_Particulier_1 BSTest 12)
add_test(Cas_Particulier_2 BSTest 13)

#Breath First
#add_test(BreathFirst BSTest 14)

#Test d'ajout de doublons
#add_test(AjoutDoublons BSTest 15)

#BSTree Search test
add_test(RechercheClassique BSTest 16)
add_test(RechercheElemSupp BSTest 17)
add_test(RechercheElemPointeParElemSupp BSTest 18)

#Remise en contexte de l'erreur
add_test(ErreurFrequente BSTest 19)

target_link_libraries(${PROJECT_NAME} ${420C35JO_TP3_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY_TEMP})
target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARY})