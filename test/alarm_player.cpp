#include "gtest/gtest.h"
#include <Alarm.h>
#include <AlarmPlayer.h>

TEST(alarm_player, no_alarm){
    AlarmPlayer& player = AlarmPlayer::Instance();
    ASSERT_EQ(player.isPlaying(), false);
    ASSERT_EQ(player.isNoisy(), false);
}

TEST(alarm_player, play_alarm){
    std::cout << '\r'; //prepare test output for cleanup
    Alarm alarm = Alarm({
        AlarmTone(1*1000, true),
        AlarmTone(1*1000, false)} ,
        AlarmLevel::LOW
    );
    AlarmPlayer& player = AlarmPlayer::Instance();
    ASSERT_EQ(player.isPlaying(), false);

    // Start the alarm
    alarm.start();
    ASSERT_EQ(player.isPlaying(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarm started to ring

    // Monitor noise for 2 cycles
    for(unsigned int i=0;i<2;i++){
        ASSERT_EQ(player.isNoisy(), true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        ASSERT_EQ(player.isNoisy(), false);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // Stop the alarm
    ASSERT_EQ(player.isPlaying(), true);
    alarm.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarm stopped
    ASSERT_EQ(player.isPlaying(), false);
    ASSERT_EQ(player.isNoisy(), false);
    std::cout << '\r'; // Clean test output
}



TEST(alarm_player, play_empty_alarm){
    std::cout << '\r'; //prepare test output for cleanup
    Alarm alarm = Alarm(AlarmLevel::LOW);
    AlarmPlayer& player = AlarmPlayer::Instance();
    ASSERT_EQ(player.isPlaying(), false);

    // Start the alarm
    alarm.start();
    ASSERT_EQ(player.isPlaying(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarm started to ring
    ASSERT_EQ(player.isNoisy(), false);
    ASSERT_EQ(player.isPlaying(), true);
    alarm.stop();
    ASSERT_EQ(player.isPlaying(), false);
    std::cout << '\r'; // Clean test output
}



TEST(alarm_player, alarm_level){
    std::cout << '\r'; //prepare test output for cleanup
    Alarm alarm_low = Alarm({
        AlarmTone(5*1000, false)} ,
        AlarmLevel::LOW
    );
    Alarm alarm_high = Alarm({
        AlarmTone(5*1000, true)} ,
        AlarmLevel::HIGH
    );
    AlarmPlayer& player = AlarmPlayer::Instance();
    ASSERT_EQ(player.isPlaying(), false);

    // Start the alarm, HIGH first
    alarm_low.start();
    alarm_high.start();
    ASSERT_EQ(player.isPlaying(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarms started to ring
    //If priority level is working, noise should be emitted
    ASSERT_EQ(player.isNoisy(), true);
    ASSERT_EQ(player.isPlaying(), true);
    alarm_low.stop();
    alarm_high.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarms stopped
    ASSERT_EQ(player.isPlaying(), false);
    ASSERT_EQ(player.isNoisy(), false);

    // Start the alarm, LOW first
    alarm_low.start();
    alarm_high.start();
    ASSERT_EQ(player.isPlaying(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarms started to ring
    //If priority level is working, noise should be emitted
    ASSERT_EQ(player.isNoisy(), true);
    ASSERT_EQ(player.isPlaying(), true);
    alarm_low.stop();
    alarm_high.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //Make sure alarms stopped
    ASSERT_EQ(player.isPlaying(), false);
    ASSERT_EQ(player.isNoisy(), false);
    std::cout << '\r'; // Clean test output
}