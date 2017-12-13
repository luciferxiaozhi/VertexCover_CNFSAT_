cmake_minimum_required(VERSION 2.8)
project(VertexCover_CNFSAT_)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()

find_package (Threads)

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/run)


# create the main executable
## add additional .cpp files if needed
add_executable(SPandVC SPandVC.cpp )
add_executable(Driver Driver.cpp )
add_executable(rgen rgen.cpp )

# link MiniSAT libraries
target_link_libraries(SPandVC minisat-lib-static)
target_link_libraries(SPandVC ${CMAKE_THREAD_LIBS_INIT})

# install
install(PROGRAMS GenVEFromRgen.py intersection.py compareStreet.py DESTINATION bin)
install(TARGETS SPandVC Driver rgen RUNTIME DESTINATION bin)