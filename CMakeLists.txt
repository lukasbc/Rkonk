# --- Configuration du projet ---

cmake_minimum_required(VERSION 3.16)
project(rkonK LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# --- Sources et exécutable ---
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})


# --- Détection et linkage de SFML ---
find_package(SFML 2.6.2 CONFIG COMPONENTS Graphics Window Audio QUIET)

if(NOT SFML_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.2
        GIT_SHALLOW    ON
    )
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(sfml)
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        sfml-audio    
        sfml-graphics
        sfml-main
        sfml-network
        sfml-system
        sfml-window
        )


# --- Link spécifique Windows ---
if (WIN32)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    opengl32
    -static-libgcc 
    -static-libstdc++
    winpthread
    wsock32 
    ws2_32
    )
endif()


# --- Retirer cette ligne si on veut le terminal ---
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

# --- Support MSVC Visual Studio ---
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                 PROPERTY  VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# --- Cibles pour clang-tidy,clang-format ---
add_custom_target(tidy-fix
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} -fix -fix-errors ${SOURCES} --quiet -- -I"${CMAKE_CURRENT_SOURCE_DIR}/src"
    COMMENT "Applying clang-tidy fixes...")

add_custom_target(tidy
    COMMAND clang-tidy -p ${CMAKE_BINARY_DIR} ${SOURCES} --quiet -- -I"${CMAKE_CURRENT_SOURCE_DIR}/src"
    COMMENT "Running clang-tidy...")

add_custom_target(format
    COMMAND clang-format -i ${SOURCES}
    COMMENT "Formatting code with clang-format...")


# --- Génération de la documentation (Doxygen) ---

find_package(Doxygen QUIET)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else()
    message(WARNING "Doxygen not found. Documentation will not be generated.")
endif()