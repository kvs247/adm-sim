add_executable(makedata main.cpp)
target_link_libraries(makedata PRIVATE common)
target_link_libraries(makedata PRIVATE nlohmann_json::nlohmann_json)