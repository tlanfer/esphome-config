#include "esphome.h"

#define POLLING_INTERVAL 30
#define AVG_OVER 10
#define DEBOUNCE_DURATION 150
#define ONE_HOUR 3600000

#define BLINK_TIME 1

#define LED_PIN D1
#define LED_PIN_GND D2

#define SENSOR_PIN A0

class MyPowerSensor : public Component, public Sensor {

    private:
        unsigned long lastRead;
        unsigned long lastPublishTime;

        unsigned long risingEdgeTimeout;
        long threshold = 0;
        bool wasHigh = false;

        long measureValue() {
            long val = 0;

            for(int i=0; i<AVG_OVER; i++) {

                // turn on
                digitalWrite(LED_PIN, HIGH);
                delay(BLINK_TIME);
                int hiVal = analogRead(SENSOR_PIN);

                // turn off
                digitalWrite(LED_PIN, LOW);
                delay(BLINK_TIME);
                int loVal = analogRead(SENSOR_PIN);

                val += (hiVal - loVal);
            }

            val /= AVG_OVER;
            return val;
        }

    public:
        MyPowerSensor(long _threshold) {
            threshold = _threshold;
        }

        void setup() override {
            lastPublishTime = 0;

            pinMode(SENSOR_PIN, INPUT);
            pinMode(LED_PIN, OUTPUT);
            pinMode(LED_PIN_GND, OUTPUT);
            digitalWrite(LED_PIN_GND,LOW);
        }

        void loop() override {
            unsigned long now = millis();

            if( now - lastRead > POLLING_INTERVAL) {
                lastRead = now;
                long val = measureValue();
//                ESP_LOGD("custom", "Measured: %d", val);
                bool isHigh = val > threshold;

                if( val > 200) {
                    return;
                }

                // rising edge
                if( isHigh && !wasHigh){
                    ESP_LOGD("custom", "Rising edge");
                    risingEdgeTimeout = now + DEBOUNCE_DURATION;
                }

                // falling edge
                if( !isHigh && wasHigh) {
                    ESP_LOGD("custom", "Falling edge");
                    risingEdgeTimeout = 0;
                }

                // check if high for long enough
                if( risingEdgeTimeout > 0 && risingEdgeTimeout > now) {
                    if( lastPublishTime > 0) {
                        unsigned long timeBetween = now - lastPublishTime;
                        float result = ((1.0*ONE_HOUR)/timeBetween)/75.0;

                        ESP_LOGI("custom", "Publish %f kW over %d ms", result, timeBetween);

                        publish_state(result);
                    }

                    risingEdgeTimeout = 0;
                    lastPublishTime = now;
                }

                wasHigh = isHigh;
            }
        }
};