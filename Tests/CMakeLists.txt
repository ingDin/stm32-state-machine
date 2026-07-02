cmake_minimum_required(VERSION 3.16)

project(UnitTests C)

enable_testing()

# Debug symbols
add_compile_options(-g)

include_directories(
    Unity/Inc
    Unit/Inc
    Fakes/Inc
    ../App/Inc
)

# Coverage flags (GNU only)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# ============================================
#  Extract Unity tests automatically
# ============================================

execute_process(
    COMMAND python ${CMAKE_SOURCE_DIR}/Tests/extract_tests.py
            ${CMAKE_SOURCE_DIR}/Tests/Unit/Src
    OUTPUT_VARIABLE UNITY_TEST_LIST
)

string(REPLACE "\n" ";" UNITY_TEST_LIST "${UNITY_TEST_LIST}")

# ============================================
#  GLOB for Unity / Unit / Fakes
# ============================================

file(GLOB UNITY_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/Unity/Src/*.c"
)

file(GLOB UNIT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/Unit/Src/*.c"
)

file(GLOB FAKES_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/Fakes/Src/*.c"
)

# ============================================
#  Generate executables for each test
# ============================================

foreach(EXE_NAME ${UNITY_TEST_LIST})

    add_executable(${EXE_NAME}
        ${UNITY_SRC}
        ${UNIT_SRC}
        ${FAKES_SRC}
        ${CMAKE_CURRENT_SOURCE_DIR}/test_runner.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../App/Src/state_machine.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../App/Src/button.c
    )

    target_include_directories(${EXE_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Unity/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Unit/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Fakes/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/../App/Inc
    )

    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})

endforeach()

# ============================================
#  Save output to file (one test per line)
# ============================================

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Test_results/TEST)

string(REPLACE "\r\n" "\n" UNITY_TEST_LIST "${UNITY_TEST_LIST}")
string(REPLACE "\r" "\n" UNITY_TEST_LIST "${UNITY_TEST_LIST}")

file(WRITE ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt "")
foreach(TEST_NAME ${UNITY_TEST_LIST})
    file(APPEND ${CMAKE_SOURCE_DIR}/Test_results/TEST/test_list.txt "${TEST_NAME}\n")
endforeach()

# ============================================
#  Coverage generation using gcovr
# ============================================

find_program(GCOVR gcovr)
if(NOT GCOVR)
    message(WARNING "gcovr not found, coverage targets will be disabled.")
    return()
endif()

set(COVERAGE_DIR ${CMAKE_SOURCE_DIR}/Test_results/COVERAGE)
set(COVERAGE_HTML ${COVERAGE_DIR}/html)

add_custom_target(coverage_clean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${COVERAGE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${COVERAGE_HTML}
    COMMENT "Cleaning coverage output folders..."
)

add_custom_target(coverage_xml
    DEPENDS coverage_clean
    COMMAND ${GCOVR}
        -r ${CMAKE_SOURCE_DIR}
        --object-directory ${CMAKE_BINARY_DIR}
        --xml-pretty
        -o ${COVERAGE_DIR}/coverage.xml
    COMMENT "Generating XML coverage report..."
)

add_custom_target(coverage_html
    DEPENDS coverage_clean
    COMMAND ${GCOVR}
        -r ${CMAKE_SOURCE_DIR}
        --object-directory ${CMAKE_BINARY_DIR}
        --html --html-details
        -o ${COVERAGE_HTML}/index.html
    COMMENT "Generating HTML coverage report..."
)

add_custom_target(coverage
    DEPENDS coverage_xml coverage_html
    COMMENT "Coverage generation complete."
)
