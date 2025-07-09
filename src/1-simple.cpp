#include "Particle.h"

#include "WiFiChannelRK.h"

// System thread defaults to on in 6.2.0 and later and this line is not required
#ifndef SYSTEM_VERSION_v620
SYSTEM_THREAD(ENABLED);
#endif

SYSTEM_MODE(SEMI_AUTOMATIC);

SerialLogHandler logHandler(LOG_LEVEL_TRACE);

// This is optional to add the Wi-Fi channel to device diagnostics
static WiFiChannelRK::WiFiChannelDiagnosticData _wifiChannelDiagData(DIAG_ID_USER+10, "net:channel");


void setup() {
    // Uncomment the following to wait for USB serial to be connected for debugging
    waitFor(Serial.isConnected, 8000); delay(2000);

    // From global application setup you must call:    
    WiFiChannelRK::instance().setup();

    Particle.connect();
}


void loop() {

}

