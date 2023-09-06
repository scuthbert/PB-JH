#include <Adafruit_NeoPixel.h>

#define MATRIX_PIN 13
#define BUTTON_R 12
#define BUTTON_B 27
#define BUTTON_G 33
#define BUTTON 15
#define SWITCH_UP 32
#define SWITCH_DOWN 14
#define R_POT A2
#define G_POT A3
#define B_POT A4
#define LIGHT_LEVEL A5
#define MAX_BRIGHTNESS 75 // Safety measure - when we turned it up to 100% the expoy cracked in ~15mins 🥴

// To add a new mode, copy+rename one of the existing files and add your new method below
#define STATE_LENGTH 5
void RGBWKnobs();
void RGBKnobs();
void Rainbow();
void FlowyRainbow();
void Flame();
void (*states[STATE_LENGTH])() = { RGBWKnobs, RGBKnobs, Rainbow, FlowyRainbow, Flame };

Adafruit_NeoPixel strip(64, MATRIX_PIN, NEO_GRBW);
int stateCount, switchUp, switchDown, lastDebounceTime = 0;
float rKnob, gKnob, bKnob, lightLevel, newLightLevel, smoothedLightLevel = 0.0; 

void setup() {
  Serial.begin(115200);

  strip.begin();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(SWITCH_UP, INPUT_PULLUP);
  pinMode(SWITCH_DOWN, INPUT_PULLUP);
  pinMode(LIGHT_LEVEL, INPUT);
  pinMode(R_POT, INPUT);
  pinMode(G_POT, INPUT);
  pinMode(B_POT, INPUT);

  pinMode(BUTTON_R, OUTPUT);
  pinMode(BUTTON_G, OUTPUT);
  pinMode(BUTTON_B, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON), iterateState, FALLING);
}

void iterateState() {
  if ((millis() - lastDebounceTime) > 250) {
    lastDebounceTime = millis();
    stateCount++;
    if (stateCount >= STATE_LENGTH) {
      stateCount = 0;
    }
  }
}

void loop() {
  rKnob = 255 - analogRead(R_POT) / 18;
  gKnob = 255 - analogRead(G_POT) / 18;
  bKnob = 255 - analogRead(B_POT) / 18;

  newLightLevel = analogRead(LIGHT_LEVEL) / 400.0; // To make the light level more sensitive to low light adjustments, decrease 400.0 
  smoothedLightLevel = (lightLevel + (newLightLevel * 0.1)) / 1.1; // To make the light level adjustment more twitch, increase the .1s here
  lightLevel = constrain(smoothedLightLevel, 0.05, 1); // To make the darkest dark darker, decrease 0.05

  switchUp = !digitalRead(SWITCH_UP);
  switchDown = !digitalRead(SWITCH_DOWN);

  if (switchDown) { // Switch down === turn off the light
    strip.setBrightness(0);
  } else if (switchUp) { // Switch up === turn up the light
    strip.setBrightness(MAX_BRIGHTNESS);
  } else { // Switch middle === attempt to match room brightness
    strip.setBrightness(MAX_BRIGHTNESS * lightLevel);
  }

  // Allow the current state to update the matrix
  states[stateCount]();
  strip.show();

  // Set the button color to match the middlemost pixel - as the panel is 
  uint32_t avgPixelColor = strip.getPixelColor(32);
  analogWrite(BUTTON_R, 255 - (uint8_t) (avgPixelColor >> 16) * 0.3); // Red and Green are both waaaay brighter than blue,
  analogWrite(BUTTON_G, 255 - (uint8_t) (avgPixelColor >> 8) * 0.6); //   so we reduce their relative values here
  analogWrite(BUTTON_B, 255 - (uint8_t) avgPixelColor);
}
