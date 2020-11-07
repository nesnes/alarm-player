add_library(alarm-player "")

target_sources(
    alarm-player
    PRIVATE
        Alarm.cpp
        AlarmPlayer.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Alarm.h
        ${CMAKE_CURRENT_LIST_DIR}/AlarmPlayer.h
    )

target_include_directories(
    alarm-player
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
    )

target_link_libraries(
  alarm-player
  pthread
)