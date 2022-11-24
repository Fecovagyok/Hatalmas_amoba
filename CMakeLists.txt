cmake_minimum_required(VERSION 3.25)

project(Hatalmas C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules")
find_package(SDL2 REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include(CMakePrintHelpers)

add_executable(hatalmas_hazi
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
)

cmake_print_variables(SDL2_IMAGE_LIBRARIES)
include_directories(${SDL2_INCLUDE_DIRS} )
target_link_libraries(hatalmas_hazi
    ${SDL2_LIBRARIES}
    ${SDL2_GFX_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

