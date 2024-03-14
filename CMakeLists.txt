cmake_minimum_required(VERSION 3.1...3.27)

project(
  Magnify-ATAR
  VERSION 0.1
  LANGUAGES CXX)

# ROOT dependency
find_package(ROOT 6.20 CONFIG REQUIRED)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# shared library for reading data
add_library(AtarEventViewer SHARED
  ${PROJECT_SOURCE_DIR}/event/Data.cc
  ${PROJECT_SOURCE_DIR}/event/Data.h
  ${PROJECT_SOURCE_DIR}/viewer/GuiController.cc
  ${PROJECT_SOURCE_DIR}/viewer/GuiController.h
  ${PROJECT_SOURCE_DIR}/viewer/ViewWindow.cc
  ${PROJECT_SOURCE_DIR}/viewer/ViewWindow.h
  ${PROJECT_SOURCE_DIR}/viewer/ControlWindow.cc
  ${PROJECT_SOURCE_DIR}/viewer/ControlWindow.h
  ${PROJECT_SOURCE_DIR}/viewer/MainWindow.cc
  ${PROJECT_SOURCE_DIR}/viewer/MainWindow.h)
target_include_directories(AtarEventViewer PUBLIC
  ${PROJECT_SOURCE_DIR}/event
  ${PROJECT_SOURCE_DIR}/viewer)
target_link_libraries(AtarEventViewer PUBLIC
 ROOT::Core ROOT::Gui ROOT::Tree ROOT::RIO ROOT::Graf3d)

add_executable(magnify-atar main.cc)
target_link_libraries(magnify-atar AtarEventViewer
  ROOT::Graf3d ROOT::Gui
  ROOT::Core ROOT::Tree ROOT::RIO)


root_generate_dictionary(G__AtarEventViewer
  ${PROJECT_SOURCE_DIR}/viewer/GuiController.h
  ${PROJECT_SOURCE_DIR}/viewer/ViewWindow.h
  ${PROJECT_SOURCE_DIR}/viewer/ControlWindow.h
  ${PROJECT_SOURCE_DIR}/viewer/MainWindow.h
  ${PROJECT_SOURCE_DIR}/event/Data.h
  MODULE AtarEventViewer LINKDEF ${PROJECT_SOURCE_DIR}/Linkdef.h)

install(TARGETS AtarEventViewer DESTINATION lib)
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/libAtarEventViewer.rootmap
        ${CMAKE_CURRENT_BINARY_DIR}/libAtarEventViewer_rdict.pcm
        DESTINATION lib)
