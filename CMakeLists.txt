cmake_minimum_required(VERSION 3.16)
project(Alien)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(Alien
        Menu.cpp
        GameMenu.cpp
        Creature.cpp
        Player.cpp
        Monster.cpp
        RandomNumberGenerator.cpp
        StartingMenu.cpp
        MainMenu.cpp
        Game.cpp
        AudioEngine.cpp
        Level.cpp
        Bullet.cpp
        Health.cpp
        json_handling.cpp
        main.cpp)


include_directories(./images)
include_directories(./audio)
include_directories(./fonts)

if (APPLE)
    set(SFML_DIR "/opt/homebrew/Cellar/sfml@2/2.6.2_1/lib/cmake/SFML")
endif (APPLE)

find_package(SFML 2.6 COMPONENTS system window graphics audio network REQUIRED)

target_link_libraries(Alien sfml-graphics sfml-window sfml-system sfml-audio)

add_custom_command(TARGET Alien POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:Alien>/config)

add_custom_command(TARGET Alien POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/audio $<TARGET_FILE_DIR:Alien>/audio)

add_custom_command(TARGET Alien POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/fonts $<TARGET_FILE_DIR:Alien>/fonts)

add_custom_command(TARGET Alien POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:Alien>/images)