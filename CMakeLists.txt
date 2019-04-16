#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

cmake_minimum_required(VERSION 3.4)
project(cozmonaut)

set(cozmo_SRC_FILES
        src/main.c
        )

set(cozmo_VERSION_MAJOR 0)
set(cozmo_VERSION_MINOR 1)
set(cozmo_VERSION_PATCH 0)

add_executable(cozmo ${cozmo_SRC_FILES})
set_target_properties(cozmo PROPERTIES C_STANDARD 99)

target_compile_definitions(cozmo PRIVATE
        -D__version_major=${cozmo_VERSION_MAJOR}
        -D__version_minor=${cozmo_VERSION_MINOR}
        -D__version_patch=${cozmo_VERSION_PATCH}
        )
