add_executable(
  unit_tests
  alarm_player.cpp
  alarm_test.cpp
)

target_link_libraries(
  unit_tests
  gtest_main
  alarm-player
)

add_test(
  NAME alarm_tests
  COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/unit_tests
)