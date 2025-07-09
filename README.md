# WiFiChannelRK

*Library to expose Wi-Fi channel information*

- Github: https://github.com/rickkas7/WiFiChannelRK
- License: MIT

The Device OS API does not expose the current Wi-Fi channel that it is connected to. This library provides a hacky workaround to get it, along with convenience methods for determining if the channel is 2.4 GHz or 5 GHz based on its channel number.

When you've connected or reconnected to Wi-Fi, the library waits a short period of time (default: 4 seconds) and then does a WiFi.scan() to find the BSSID that we're connected to. The scan results include the channel, and this is saved if found.

```
void setup() {
    WiFiChannelRK::instance()
        .withUpdateCallback(updateCallback)
        .setup();

    Particle.connect();
}

void loop() {
}

void updateCallback() {
    Log.info("updateCallback channel=%d, 2.4GHz=%d, 5GHz=%d",
        WiFiChannelRK::instance().getWiFiChannel(),
        (int) WiFiChannelRK::instance().isWiFi_2_4_GHz(),
        (int) WiFiChannelRK::instance().isWiFi_5_GHz());
}
```

Example output:

```
0000026432 [app] INFO: updateCallback channel=6, 2.4GHz=1, 5GHz=0
```
