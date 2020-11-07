/** 
 *  @file   Alarm.h 
 *  @brief  Define the Alarm object and it associated struct/enums
 *  @author BREHMER Alexandre
 *  @date   2020-11-07 
 **/

#ifndef Alarm_h
#define Alarm_h

#include <vector>
#include <iterator>
#include <algorithm>
#include <mutex>

#include "AlarmPlayer.h"

/**
 * @brief Levels available to define an Alarm priority
 * Order is important as int-conversion is used to define priority (with lower value = lower priority)
 */
enum class AlarmLevel { LOW, MEDIUM, HIGH };

/**
 * @brief Tone played during an alarm
 */
typedef struct AlarmTone{
    /**
     *  \brief AlarmTone contructor
     *  \param _duration : initializes the \see duration attribute (in milliseconds)
     *  \param _beep : initializes the \see beep attribute, true false noise, false for silence
     *  \warning Durations below 250ms won't playback perfectly due to the \see AlarmPlayer fixed playback rate
     */
    AlarmTone(unsigned int _duration, bool _beep) : duration(_duration), beep(_beep) {};
    unsigned int duration; /**< Duration of the tone in milliseconds */
    bool beep; /**< Wether the tone should produce noise (true) or not (false) */
} AlarmTone;

/**
 * @brief Representation of an alarm with a tone sequence
 * This class plays itself in its friendly \see AlarmPlayer class using the \see start() function
 */
class Alarm {
friend AlarmPlayer;
public:
    /**
     * @brief Default constructor
     * Set the level to LOW and the sequence empty
     */
    Alarm();

    /**
     * @brief Copy constructor
     */
    Alarm (const Alarm&);

    /**
     * @brief Alarm constructor with level initialization
     * Sets the provided \see AlarmLevel and the sequence empty
     */
    Alarm(AlarmLevel level);

    /**
     * @brief Alarm constructor with lsequence and evel initialization
     * Sets the provided sequence and optionally the \see AlarmLevel (LOW by default)
     * \param sequence : The initial vector of tones to be played by an alarm
     */
    Alarm(std::vector<AlarmTone> sequence, AlarmLevel level=AlarmLevel::LOW);

    /**
     * @brief Alarm destructor
     * If being played, the alarm will \see stop() itself at destruction
     */
    ~Alarm();

    /**
     * @brief Setter of the alarm level
     * If a new level is set while being played, the Alarm will restart its playback
     * \param level : the new \see AlarmLevel of the Alarm
     */
    void setLevel(AlarmLevel level);

    /**
     * @brief Getter of the current Alarm level
     * \return the current Alarm level
     */
    AlarmLevel getLevel();

    /**
     * @brief Clears the \see AlarmTone sequence, emptying-it
     */
    void clear();

    /**
     * @brief Append a tone at the end of the \see AlarmTone sequence
     * \param tone : the new \see AlarmTone to append
     */
    void addTone(AlarmTone tone);

    /**
     * @brief Setter of the \see AlarmTone sequence
     * \param sequence : the new \see AlarmTone sequence of the Alarm
     */
    void setSequence(std::vector<AlarmTone> sequence);

    /**
     * @brief Getter of the current \see AlarmTone sequence
     * \return the current \see AlarmTone sequence of the Alarm
     */
    std::vector<AlarmTone> getSequence();

    /**
     * @brief Starts the playback of the Alarm through the \see AlarmPlayer
     * Sets the playback cursor to the begining of the sequence
     * and attaches to the \see AlarmPlayer
     * \warning If called while that alarm is already started, this function has no effect
     */
    void start();

    /**
     * @brief Stops the playback of the Alarm through the \see AlarmPlayer
     * \warning If called while that alarm has not started, this function has no effect
     */
    void stop();

    /**
     * @brief Getter of the current playback state of the Alarm
     * \return true if the Alarm started its playback, false if it didn't or has been \see stop()
     */
    bool isStarted();

protected:
    //Playback attributes
    unsigned int m_toneIndex = 0; /**< Used be \see AlarmPLayer to save the current playback index in the \see AlarmTone sequence */
    unsigned int m_toneMsElapsed = 0; /**< Used be \see AlarmPLayer to save the elapsed duration of \see AlarmTone being played */
    std::mutex m_sequence_mtx; /**< Protects the sequence vector and playback attributes from concurrent access */

private:
    AlarmLevel m_level = AlarmLevel::LOW; /**< The stored \see AlarmLevel of the Alarm */
    bool m_started = false; /**< The stored playback state of the Alarm */
    std::vector<AlarmTone> m_sequence; /**< The stored sequence of \see AlarmTone of the Alarm */
};

#endif //AlarmPlayer_h
