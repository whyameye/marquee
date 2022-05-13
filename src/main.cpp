#include <Arduino.h>
#include "NeoPixelBus.h"

#define PIXEL_PIN 2
#define PIXEL_COUNT 200
#define LED_ENABLE_PIN 4
#define SPACE_BETWEEN_LEDS 4

NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1800KbpsMethod> strip(PIXEL_COUNT, PIXEL_PIN);

RgbColor wheelColor(byte WheelPos, byte iBrightness) {
    float R, G, B;
    float brightness = iBrightness / 255.0;

    if (WheelPos < 85) {
        R = (float) (WheelPos * 3);
        G = (float) (255 - WheelPos * 3);
        B = 0;
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        R = (float) (255 - WheelPos * 3);
        G = 0;
        B = (float) (WheelPos * 3);
    } else {
        WheelPos -= 170;
        R = 0;
        G = (float) (WheelPos * 3);
        B = (float) (255 - WheelPos * 3);
    }
    R = (float) (R * brightness + .5);
    G = (float) (G * brightness + .5);
    B = (float) (B * brightness + .5);
    return {(byte) R, (byte) G, (byte) B};
}

void colorWipe(RgbColor c) {
    for (uint16_t i = 0; i < PIXEL_COUNT; i++) {
        strip.SetPixelColor(i, c);
    }
    strip.Show();
}

void setup() {
    pinMode(LED_ENABLE_PIN, OUTPUT);
    digitalWrite(LED_ENABLE_PIN, HIGH);
    strip.Begin();
    delay(1);
    colorWipe(RgbColor(0, 0, 0)); // shut off the LEDs}
}

void loop() {
    static uint8_t ledModToLight = 0;

    ledModToLight = (++ledModToLight) % SPACE_BETWEEN_LEDS;
    for (int i = 0; i < PIXEL_COUNT; i++) {
        if (!((i + ledModToLight) % SPACE_BETWEEN_LEDS))
            strip.SetPixelColor(i, wheelColor(25, 255));
        else
            strip.SetPixelColor(i, wheelColor(25, 0));
    }
    strip.Show();
    delay(100);
}