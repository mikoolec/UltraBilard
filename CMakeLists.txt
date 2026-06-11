cmake_minimum_required(VERSION 3.16)

project(UltraBilard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# To jest ta część, która "zastępuje" ręczne pobieranie.
# CMake sam połączy się z serwerami SFML i pobierze właściwą wersję.
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(UltraBilard main.cpp

    audiomanager.h audiomanager.cpp
    bilardhole.h bilardhole.cpp
    gameobject.h)

target_link_libraries(UltraBilard PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

include(GNUInstallDirs)
install(TARGETS UltraBilard
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

 file(COPY "${SFML_SOURCE_DIR}/extlibs/bin/x64/openal32.dll"
      DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")