/** 
 *  @file   AlarmPlayer.h 
 *  @brief  Define the AlarmPlayer object
 *  @author BREHMER Alexandre
 *  @date   2020-11-07 
 **/

#ifndef AlarmPlayer_h
#define AlarmPlayer_h

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <thread>
#include <mutex>

class Alarm; //Forward-declaration of the Alarm class

/**
 * @brief Representation of an alarm player that emit with a tone sequence
 * This class plays attached \see Alarms depending on their priority level
 * \warning This class is a singleton, access to its single-instance is availbale through the Instance() static function
 */
class AlarmPlayer  {
friend Alarm;
public:

    /**
     * @brief Getter of signle Instance of the AlarmPlayer (Singleton)
     * \return the instance of the AlarmPlayer
     */
    static AlarmPlayer& Instance();

    /**
     * @brief Wether an Alarm is currently being played
     * This can be used for sync with any other user-feedback system, and useful for tests
     * \return true if an alarm is currently being played, false otherwise
     */
    bool isPlaying();

    /**
     * @brief Wether the AlarmPlayer is currently eitting noise
     * This can be used for sync with any other user-feedback system, and useful for tests
     * \return true if a noise is currently being emitted, false otherwise
     */
    bool isNoisy();

protected:

    /**
     * @brief Attaches a new alarm to the AlarmPlayer for playback
     * \param alarm : the alarm to be attached
     * \warning if the alarm has already been attached, this function has no effect
     */
    void attach(Alarm* alarm);

    /**
     * @brief Detaches an alarm from the AlarmPlayer
     * \param alarm : the alarm to be detached
     * \warning if the alarm has already been detached, or has not been attached, this function has no effect
     */
    void detach(Alarm* alarm);

private:

    /**
     * @brief Playback thread of the AlarmPlayer
     */
    void run();
    bool m_alive = true; /**< Used during destruction to stop the thread and prevent attachments  */
    std::thread m_playerThread; /**< Stores the playback thread instance */

    /**
     * @brief Update playback position of an alarm
     * This function will select the tone in the Alarm's sequence, and call the \see beep() function
     * \param alarm : the alarm to playback
     */
    void updateAlarm(Alarm* alarm);

    /**
     * @brief Emits noise depending on \see noisy parameter
     * \param noisy : true for a sound to be played, false for silence
     * \warning This function is a mockup and doesn't actually communicates with noise-emitting hardware
     *          An 'X' will be printed to the standard output when noisy is true
     *          An '_' will be printed to the standard output when noisy is false
     *          As an attempt to produce sound, a '\a' is also printed when noisy is true
     */
    void beep(bool noisy);
    volatile bool m_noisy = false; /**< Stores the latest request to the \see beep() function */

    unsigned int m_interval_ms = 250; /**< Fixed duration of a single \see beep() */

    std::vector<Alarm*> m_alarmList; /**< List of attached alarm, sorted by priority level, highest first */
    std::mutex m_alarmList_mtx; /**< Protects the alarmList from concurrent access */

    //AlarmPlayer is a singleton, so its constructors are private
    AlarmPlayer& operator= (const AlarmPlayer&) = delete;
    AlarmPlayer (const AlarmPlayer&);
    static AlarmPlayer m_instance;
    AlarmPlayer(); /**< Constructor of the AlarmPlayer, privatized by Singleton */
    ~AlarmPlayer(); /**< Destructor of the AlarmPlayer, privatized by Singleton. Detaches every alarm and stops the thread. */
};

#endif //AlarmPlayer_h
