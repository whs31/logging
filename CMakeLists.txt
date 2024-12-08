cmake_minimum_required(VERSION 3.15)

include_guard(GLOBAL)

project(log
  VERSION 1.0.0
  DESCRIPTION "Rolly extension: simple enough logging facade library based on spdlog"
  HOMEPAGE_URL "whs31.github.io/rlog"
  LANGUAGES C CXX
)
set(PROJECT_NAMESPACE rolly::)

option(ROLLY_LOG_TEST "Enable tests" OFF)
option(ROLLY_LOG_PYBIND "Enable python bindings" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
  set(CMAKE_GENERATOR_PLATFORM "x64" CACHE STRING "" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PYBIND11_FINDPYTHON ON)
set(ROLLY_LOG_FULL_NAME ${PROJECT_NAMESPACE}${PROJECT_NAME})

# -- dependencies --
find_package(rolly REQUIRED)
find_package(spdlog REQUIRED)
if(ROLLY_LOG_PYBIND)
  find_package(pybind11 REQUIRED)
endif()

add_library(${PROJECT_NAME})
add_library(${ROLLY_LOG_FULL_NAME} ALIAS ${PROJECT_NAME})

if(ROLLY_LOG_COMPAT)
  set(ROLLY_LOG_CXX_STANDARD 17)
else()
  set(ROLLY_LOG_CXX_STANDARD 20)
endif()

message(STATUS "[${ROLLY_LOG_FULL_NAME}] c++ standard: ${ROLLY_LOG_CXX_STANDARD}")

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD ${ROLLY_LOG_CXX_STANDARD}
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
)

target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/log.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/progress_bar.cc
)

if(ROLLY_LOG_PYBIND)
  message(STATUS "[${ROLLY_LOG_FULL_NAME}] adding python bindings")
  target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/pybind.pybind.cc)
endif()

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    fmt::fmt
    spdlog::spdlog
    rolly::rolly
)
if(ROLLY_LOG_PYBIND)
  target_link_libraries(${PROJECT_NAME} PRIVATE pybind11::pybind11)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LOG_PYBIND=1)
endif()

if(ROLLY_LOG_COMPAT)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_COMPAT=1)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LOG_COMPAT=1)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>$<INSTALL_INTERFACE:include> PRIVATE src)

if(BUILD_SHARED_LIBS)
  message(STATUS "[${ROLLY_LOG_FULL_NAME}] adding shared export symbols: ROLLY_LOG_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LOG_LIBRARY=1)
else()
  message(STATUS "[${ROLLY_LOG_FULL_NAME}] adding static export symbols: ROLLY_LOG_STATIC_LIBRARY")
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DROLLY_LOG_STATIC_LIBRARY=1)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "[${ROLLY_LOG_FULL_NAME}] adding compiler flags: -Wall -Wextra -Wpedantic -Werror")
  target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
    $<$<NOT:$<CXX_COMPILER_ID:Clang>>:-Werror>
  )
endif()

if(ROLLY_LOG_TEST)
  add_subdirectory(bin)
endif()

# -- installation --
message(STATUS "[${ROLLY_LOG_FULL_NAME}] tests: ${INTEGRATION_TESTS}")
message(STATUS "[${ROLLY_LOG_FULL_NAME}] installing ${PROJECT_NAME} in namespace ${PROJECT_NAMESPACE}")
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
message(STATUS "[${ROLLY_LOG_FULL_NAME}] installing in prefix: ${CMAKE_INSTALL_PREFIX}")

set(PROJECT_CONFIG_FILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
set(PROJECT_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(CONFIG_TARGETS_FILE "${PROJECT_NAME}-config-targets.cmake")
set(CONFIG_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(INSTALL_EXPORT_DESTINATION_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/rolly> $<INSTALL_INTERFACE:include/rolly> ; DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAMESPACE} FILE "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_TARGETS_FILE}")
install(EXPORT ${PROJECT_NAME}
  DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR}
  NAMESPACE ${PROJECT_NAMESPACE}
  FILE ${CONFIG_TARGETS_FILE}
)
configure_package_config_file(${PROJECT_CONFIG_FILE_IN} ${PROJECT_CONFIG_FILE} INSTALL_DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})
write_basic_package_version_file(${CONFIG_VERSION_FILE} COMPATIBILITY AnyNewerVersion)
install(FILES ${PROJECT_CONFIG_FILE} ${CONFIG_VERSION_FILE} DESTINATION ${INSTALL_EXPORT_DESTINATION_DIR})

message(STATUS "[${ROLLY_LOG_FULL_NAME}] configuring ${PROJECT_NAME} done!")
unset(ROLLY_LOG_TEST CACHE)
unset(ROLLY_LOG_PYBIND CACHE)
