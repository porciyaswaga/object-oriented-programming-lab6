cmake_minimum_required(VERSION 3.14)
project(npc_editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем заголовочные файлы
include_directories(include)

# Исходные файлы
set(SOURCES
    src/NPC.cpp
    src/NPCFactory.cpp
    src/Observer.cpp
    src/Visitor.cpp
)

# Основная программа
add_executable(npc_editor main.cpp ${SOURCES})

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(tests tests/test_npc.cpp ${SOURCES})
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
