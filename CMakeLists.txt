cmake_minimum_required(VERSION 2.8.4)
project(Renbro)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(RESOURCES_PATH "${PROJECT_SOURCE_DIR}/res/")
set(RESULTS_PATH "${PROJECT_SOURCE_DIR}/out/")

file(MAKE_DIRECTORY  ${RESULTS_PATH})

set(CONFIGURE_PATH "${PROJECT_SOURCE_DIR}/gen/")

configure_file (
  "${PROJECT_SOURCE_DIR}/configuration.h.in"
  "${CONFIGURE_PATH}/configuration.h"
)

include_directories("${CONFIGURE_PATH}")

set(SOURCE_FILES main.cpp model.cpp tgaimage.cpp geometry.h "${CONFIGURE_PATH}/configuration.h")
add_executable(Renbro ${SOURCE_FILES})

option(GEN_HTML "Generate html with output images" OFF)

if(GEN_HTML)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

    add_custom_command(OUTPUT "${RESULTS_PATH}/index.html"
        COMMAND Renbro
        COMMAND mogrify -format png  "${RESULTS_PATH}*.tga"
        COMMAND cd "${RESULTS_PATH}" && python -c \"import glob\;print '\\n'.join(['<img src=\\\"%s\\\"/><br/>' % (a) for a in glob.glob('*.png' ) ] ) \" > index.html
        DEPENDS Renbro
        )

    add_custom_target(
      html ALL
      DEPENDS "${RESULTS_PATH}/index.html"
    )

endif()

