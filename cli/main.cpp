#include <iostream>
#include <Alarm.h>

//Create custom alarms
Alarm alarm_low = Alarm({
    AlarmTone(1*1000, true),
    AlarmTone(29*1000, false)} ,
    AlarmLevel::LOW
);
Alarm alarm_medium = Alarm({
    AlarmTone(250, true),
    AlarmTone(750, false)} ,
    AlarmLevel::MEDIUM
);
Alarm alarm_high = Alarm({
    AlarmTone(250, true),// Beep 1
    AlarmTone(500, false),
    AlarmTone(250, true),// Beep 2
    AlarmTone(500, false),
    AlarmTone(250, true),// Beep 3
    AlarmTone(500, false),
    AlarmTone(250, true),// Beep 4
    AlarmTone(500, false),
    AlarmTone(250, true),// Beep 5
    AlarmTone(2*1000, false)} ,
    AlarmLevel::HIGH
);

void printHelp(){
    std::cout << "Sample CLI interface for Alarm player" << std::endl << std::endl;
    std::cout << "\t Press 'h' followed by ENTER to toggle High-level alarm" << std::endl;
    std::cout << "\t Press 'm' followed by ENTER to toggle Medium-level alarm" << std::endl;
    std::cout << "\t Press 'l' followed by ENTER to toggle Low-level alarm" << std::endl;
    std::cout << "\t Press 'q' followed by ENTER to exit the program" << std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv) {
    if(argc>1 && std::string(argv[1]) == "-h") printHelp();
    char c = ' ';
    while (c != 'q'){
        std::cin >> c;
        switch(c){
            case 'l':
                alarm_low.isStarted() ? alarm_low.stop() : alarm_low.start();
                break;
            case 'm':
                alarm_medium.isStarted() ? alarm_medium.stop() : alarm_medium.start();
                break;
            case 'h':
                alarm_high.isStarted() ? alarm_high.stop() : alarm_high.start();
                break;
        }
    }

    return EXIT_SUCCESS;
}
