include(GoogleTest)

add_executable(admsim 
  main.cpp 
  simulation/simulation.cpp
)
target_link_libraries(admsim PRIVATE common)

add_executable(simulation_test 
  simulation/simulation_test.cpp 
  simulation/simulation.cpp
)
target_include_directories(simulation_test PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(simulation_test gtest gtest_main)

enable_testing()
gtest_discover_tests(simulation_test)