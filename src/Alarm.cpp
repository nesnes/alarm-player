#include "Alarm.h"


Alarm::Alarm() {

}

Alarm::Alarm(AlarmLevel level) : m_level(level){

}

Alarm::Alarm(std::vector<AlarmTone> sequence, AlarmLevel level) 
 : m_sequence(sequence)
 , m_level(level) {
}

Alarm::~Alarm() {
    this->stop(); // Make sure the alarm gets detached from the AlaramPlayer
}

void Alarm::setLevel(AlarmLevel level){
    if(level == m_level) return;
    m_level = level;
    // Notify the AlarmPlayer on level update by restarting the playback
    // This avoids a constantly-running level lookup in the AlarmPlayback playlist
    if(this->isStarted()){
        this->stop();
        this->start();
    }
}

AlarmLevel Alarm::getLevel(){
    return m_level;
}

void Alarm::clear(){
    m_sequence_mtx.lock();
    m_sequence.clear();
    m_toneIndex = 0;
    m_toneMsElapsed = 0;
    m_sequence_mtx.unlock();
}

void Alarm::addTone(AlarmTone tone){
    m_sequence_mtx.lock();
    m_sequence.push_back(tone);
    m_sequence_mtx.unlock();
}

void Alarm::setSequence(std::vector<AlarmTone> sequence){
    this->clear();
    m_sequence_mtx.lock();
    m_sequence = sequence;
    m_sequence_mtx.unlock();
}

std::vector<AlarmTone> Alarm::getSequence(){
    return m_sequence;
}

void Alarm::start(){
    if(this->isStarted()) return;
    AlarmPlayer& player = AlarmPlayer::Instance();
    m_sequence_mtx.lock();
    m_toneIndex = 0;
    m_toneMsElapsed = 0;
    m_sequence_mtx.unlock();
    player.attach(this);
    m_started = true;
}

void Alarm::stop(){
    if(!this->isStarted()) return;
    AlarmPlayer& player = AlarmPlayer::Instance();
    player.detach(this);
    m_started = false;
}

bool Alarm::isStarted(){
    return m_started;
}