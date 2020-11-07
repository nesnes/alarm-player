#include "AlarmPlayer.h"
#include "Alarm.h"

AlarmPlayer AlarmPlayer::m_instance=AlarmPlayer();

AlarmPlayer::AlarmPlayer() : m_playerThread(&AlarmPlayer::run, this){
}

AlarmPlayer::~AlarmPlayer() {
    m_alive = false; // Contact the playback thread for termination
    m_playerThread.join(); // Wait for the playback thread
    while(m_alarmList.size()) m_alarmList.at(0)->stop(); // Detach every Alarm
    this->beep(false); // Depending on hardware, make sure we stop any noise
}

AlarmPlayer& AlarmPlayer::Instance() {
    return m_instance; // AlarmPlayer is a singleton, return its only instance
}

void AlarmPlayer::attach(Alarm* alarm){
    if(!m_alive) return; // Do not attach while destroying
    m_alarmList_mtx.lock();
    if(std::find(m_alarmList.begin(), m_alarmList.end(), alarm) == m_alarmList.end()) // Make sure alarm isn't yet attached
        m_alarmList.push_back(alarm);
    // Sort alarms by level, higest priority first
    std::sort(m_alarmList.begin(), m_alarmList.end(), [](Alarm* a, Alarm* b) {return (int)a->getLevel() > (int)b->getLevel(); });
    m_alarmList_mtx.unlock();
}

void AlarmPlayer::detach(Alarm* alarm){
    m_alarmList_mtx.lock();
    m_alarmList.erase(std::remove(m_alarmList.begin(), m_alarmList.end(), alarm), m_alarmList.end());
    // Sort alarms by level, higest priority first, not mandatory on detach but future-proof
    std::sort(m_alarmList.begin(), m_alarmList.end(), [](Alarm* a, Alarm* b) {return (int)a->getLevel() > (int)b->getLevel(); });
    m_alarmList_mtx.unlock();
}

void AlarmPlayer::run() {
    while(m_alive){
        //Get highest priority alarm
        m_alarmList_mtx.lock();
        if(m_alarmList.size()){
            // Highest priority alarm is always first in vector (thanks to sorting on insert)
            Alarm* alarm = m_alarmList.at(0);
            this->updateAlarm(alarm); // Play the alarm tone
        }
        else this->beep(false); // Turn beep off when no alarm to playback
        m_alarmList_mtx.unlock();

        //Sleep until next turn. The sleep precision is well enough for this audio alarm.
        std::this_thread::sleep_for(std::chrono::milliseconds(m_interval_ms));
    }
}

void AlarmPlayer::updateAlarm(Alarm* alarm){
    alarm->m_sequence_mtx.lock();
    std::vector<AlarmTone> sequence = alarm->getSequence();
    if(sequence.size()){
        if(alarm->m_toneIndex>sequence.size()-1) alarm->m_toneIndex = 0; // Reset playback to begining of sequence
        AlarmTone tone = sequence.at(alarm->m_toneIndex);
        alarm->m_toneMsElapsed += m_interval_ms;
        if(alarm->m_toneMsElapsed>=tone.duration){ // Switch to next tone in sequence
            alarm->m_toneIndex++;
            alarm->m_toneMsElapsed = 0;
        }
        this->beep(tone.beep);
    }
    else this->beep(false); // Turn beep off when alarm has no tone
    alarm->m_sequence_mtx.unlock();
}

void AlarmPlayer::beep(bool noisy){
    m_noisy = noisy;
    std::cout << (noisy?"X\a":"_") << std::flush;
}

bool AlarmPlayer::isPlaying(){
    return m_alarmList.size();
}

bool AlarmPlayer::isNoisy(){
    return m_noisy;
}