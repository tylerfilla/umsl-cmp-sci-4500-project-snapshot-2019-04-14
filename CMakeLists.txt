#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

cmake_minimum_required(VERSION 3.4)
project(cozmonaut)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(PythonInterp 3.5 REQUIRED)
find_package(PythonLibs 3.5 REQUIRED)

include(GetGitRevisionDescription)

get_git_head_revision(GIT_REFSPEC GIT_HASH)

set(cozmo_SRC_FILES
        src/op/friend_list.c
        src/op/friend_remove.c
        src/op/go.c
        src/args.c
        src/global.c
        src/main.c
        )

set(cozmo_VERSION_MAJOR 0)
set(cozmo_VERSION_MINOR 1)
set(cozmo_VERSION_PATCH 0)

add_executable(cozmo ${cozmo_SRC_FILES})
set_target_properties(cozmo PROPERTIES C_STANDARD 99)
target_include_directories(cozmo PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(cozmo PRIVATE ${PYTHON_LIBRARY})

target_compile_definitions(cozmo PRIVATE
        -D__git_hash=${GIT_HASH}
        -D__git_refspec=${GIT_REFSPEC}
        -D__version_major=${cozmo_VERSION_MAJOR}
        -D__version_minor=${cozmo_VERSION_MINOR}
        -D__version_patch=${cozmo_VERSION_PATCH}
        )
