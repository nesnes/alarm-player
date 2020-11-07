add_executable(
  alarm_cli
  main.cpp
)

target_link_libraries(
  alarm_cli
  alarm-player
)

