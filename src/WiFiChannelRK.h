#ifndef __WIFICHANNELRK_H
#define __WIFICHANNELRK_H

#include "Particle.h"

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * WiFiChannelRK::instance().setup();
 */
class WiFiChannelRK {
public:
    class WiFiChannelDiagnosticData: public AbstractIntegerDiagnosticData {
    public:
        WiFiChannelDiagnosticData(uint16_t id, const char* name = nullptr);
    private:
        virtual int get(IntType& val) override; // AbstractIntegerDiagnosticData
    };

    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use WiFiChannelRK::instance() to instantiate the singleton.
     */
    static WiFiChannelRK &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use WiFiChannelRK::instance().setup();
     */
    void setup();

    /**
     * @brief Locks the mutex that protects shared resources
     * 
     * This is compatible with `WITH_LOCK(*this)`.
     * 
     * The mutex is not recursive so do not lock it within a locked section.
     */
    void lock() { os_mutex_lock(mutex); };

    /**
     * @brief Attempts to lock the mutex that protects shared resources
     * 
     * @return true if the mutex was locked or false if it was busy already.
     */
    bool tryLock() { return os_mutex_trylock(mutex); };

    /**
     * @brief Unlocks the mutex that protects shared resources
     */
    void unlock() { os_mutex_unlock(mutex); };

    /**
     * @brief Returns the current Wi-Fi channel connected to
     * 
     * @return int channel number or 0 if not known
     */
    int getWiFiChannel() const { return channel; };

    /**
     * @brief Returns true if the current WiFi channel is known or false if not
     * 
     * @return true 
     * @return false 
     */
    bool hasWiFiChannel() const { return channelFound; };

    /**
     * @brief Returns true if the currently connected channel is a 2.4 GHz channel
     * 
     * @return true 
     * @return false 
     */
    bool isWiFi_2_4_GHz() const;

    /**
     * @brief Returns true if the currently connected channel is a 5 GHz channel
     * 
     * @return true 
     * @return false 
     */
    bool isWiFi_5_GHz() const;

protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use WiFiChannelRK::instance() to instantiate the singleton.
     */
    WiFiChannelRK();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~WiFiChannelRK();

    /**
     * This class is a singleton and cannot be copied
     */
    WiFiChannelRK(const WiFiChannelRK&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    WiFiChannelRK& operator=(const WiFiChannelRK&) = delete;

    /**
     * @brief Worker thread function
     * 
     * This method is called to perform operations in the worker thread.
     * 
     * You generally will not return from this method.
     */
    os_thread_return_t threadFunction(void);

    /**
     * @brief Mutex to protect shared resources
     * 
     * This is initialized in setup() so make sure you call the setup() method from the global application setup.
     */
    os_mutex_t mutex = 0;

    /**
     * @brief Worker thread instance class
     * 
     * This is initialized in setup() so make sure you call the setup() method from the global application setup.
     */
    Thread *thread = 0;

    /**
     * @brief Set to true if WiFi.ready() was most recently true. Used to check for connected state change.
     */
    bool wifiReady = false;
    
    byte bssid[6];
    int channel = 0;
    bool channelFound = false;

    unsigned long stateTime = 0;

    void stateIdle();
    void stateScanWait();

    std::function<void(WiFiChannelRK&)> stateHandler = &WiFiChannelRK::stateIdle;

    void wifiScanCallback(WiFiAccessPoint* wap);
    static void wifiScanCallbackStatic(WiFiAccessPoint* wap, void* data);

    WiFiChannelDiagnosticData *diagHandler = nullptr;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static WiFiChannelRK *_instance;

};
#endif  /* __WIFICHANNELRK_H */
