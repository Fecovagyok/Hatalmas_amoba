cmake_minimum_required(VERSION 3.23.2)

project(Hatalmas C)
include(CMakePrintHelpers)

if(WIN32)

set(SDL2_INCLUDE_DIRS "C:/myVClibs/SDL2-2.26.0/include")
set(SDl2_GFX_INCLUDE_DIRS "C:/mylibs/sdl2/include")
set(SDL2_LIBRARIES C:/myVClibs/SDL2-2.26.0/lib/x86/SDL2.lib)
set(SDL2_GFX_LIBRARIES C:/mylibs/sdl2/SDL2_gfx.lib)
set(SDL2_MIXER_LIBRARIES C:/myVClibs/SDL2_mixer-2.6.2/lib/x86/SDL2_mixer.lib)
set(SDL2_TTF_LIBRARIES C:/myVClibs/SDL2_ttf-2.20.1/lib/x86/SDL2_ttf.lib)
set(SDL2_IMAGE_LIBRARIES C:/myVClibs/SDL2_image-2.6.2/lib/x86/SDL2_image.lib)


ELSE()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules")
find_package(SDL2 REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
ENDIF()

add_executable(hatalmas_hazi WIN32
    main.c
    uj.c
    uj_beolvas.c
    fo_megjelen.c
    esemenyhalo.c
    fomenu.c
    jatek.c
    jatek_megjelen.c
    benagep.c
    audio/play.c
        palya_manage.c)

cmake_print_variables(SDL2_IMAGE_LIBRARIES)
include_directories(${SDL2_INCLUDE_DIRS} ${SDl2_GFX_INCLUDE_DIRS})
target_link_libraries(hatalmas_hazi
    ${SDL2_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

if(WIN32)
	target_link_libraries(hatalmas_hazi C:/myVClibs/SDL2-2.26.0/lib/x86/SDL2main.lib)
endif()

