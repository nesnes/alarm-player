#include "gtest/gtest.h"
#include <Alarm.h>

TEST(alarm, constructor_default) {
    Alarm alarm = Alarm();
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::LOW);
    ASSERT_EQ(alarm.getSequence().size(), 0);
    ASSERT_EQ(alarm.isStarted(), false);
}

TEST(alarm, constructor_level) {
    Alarm alarm = Alarm(AlarmLevel::MEDIUM);
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::MEDIUM);
    ASSERT_EQ(alarm.getSequence().size(), 0);
    ASSERT_EQ(alarm.isStarted(), false);
}

TEST(alarm, constructor_sequence) {
    Alarm alarm = Alarm({
        AlarmTone(1*1000, true),
        AlarmTone(2*1000, false)} ,
        AlarmLevel::HIGH
    );
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::HIGH);
    ASSERT_EQ(alarm.getSequence().size(), 2);
    ASSERT_EQ(alarm.isStarted(), false);
    //Check first tone
    ASSERT_EQ(alarm.getSequence().at(0).beep, true);
    ASSERT_EQ(alarm.getSequence().at(0).duration, 1*1000);
    //Check second tone
    ASSERT_EQ(alarm.getSequence().at(1).beep, false);
    ASSERT_EQ(alarm.getSequence().at(1).duration, 2*1000);
}

TEST(alarm, setters_level) {
    Alarm alarm = Alarm();

    alarm.setLevel(AlarmLevel::HIGH);
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::HIGH);

    alarm.setLevel(AlarmLevel::MEDIUM);
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::MEDIUM);

    alarm.setLevel(AlarmLevel::LOW);
    ASSERT_EQ(alarm.getLevel(), AlarmLevel::LOW);
}

TEST(alarm, setters_sequence) {
    Alarm alarm = Alarm({
        AlarmTone(1*1000, true),
        AlarmTone(2*1000, false)} ,
        AlarmLevel::LOW
    );
    //Add a tone
    ASSERT_EQ(alarm.getSequence().size(), 2);
    alarm.addTone(AlarmTone(3*1000, false));
    ASSERT_EQ(alarm.getSequence().size(), 3);

    //Clear sequence
    alarm.clear();
    ASSERT_EQ(alarm.getSequence().size(), 0);

    //Set sequence
    std::vector<AlarmTone> sequence = {AlarmTone(4*1000, true), AlarmTone(5*1000, false)};
    alarm.setSequence(sequence);
    ASSERT_EQ(alarm.getSequence().size(), 2);
    ASSERT_EQ(alarm.getSequence().at(0).beep, true); // First tone
    ASSERT_EQ(alarm.getSequence().at(0).duration, 4*1000);
    ASSERT_EQ(alarm.getSequence().at(1).beep, false); // Second tone
    ASSERT_EQ(alarm.getSequence().at(1).duration, 5*1000);

    //Reset sequence without clear
    std::vector<AlarmTone> sequence2 = {AlarmTone(6*1000, true)};
    alarm.setSequence(sequence2);
    ASSERT_EQ(alarm.getSequence().size(), 1);
    ASSERT_EQ(alarm.getSequence().at(0).beep, true); // First tone
    ASSERT_EQ(alarm.getSequence().at(0).duration, 6*1000);

    //Clear sequence
    alarm.clear();
    ASSERT_EQ(alarm.getSequence().size(), 0);
}



TEST(alarm, start_stop) {
    std::cout << '\r'; //prepare test output for cleanup
    Alarm alarm = Alarm();
    ASSERT_EQ(alarm.isStarted(), false);

    //First cycle
    alarm.start();
    ASSERT_EQ(alarm.isStarted(), true);
    alarm.stop();
    ASSERT_EQ(alarm.isStarted(), false);

    //Second cycle, with sleep
    alarm.start();
    ASSERT_EQ(alarm.isStarted(), true);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    alarm.stop();
    ASSERT_EQ(alarm.isStarted(), false);
    std::cout << '\r'; // Clean test output
}