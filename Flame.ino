int currentR, currentG, currentB, currentW = 0;
int baseR = 230, baseG = 65, baseB = 10;

void Flame() {
  // Move all the pixels 'down' by one to scan the random colors across the matrix
  for (int i=0; i<strip.numPixels()-1; i++) {
    strip.setPixelColor(i, strip.getPixelColor(i+1));
  }
  float brightness = rKnob/255;

  // For the last pixel, avg the current color with our 'base' color and flicker a random amount from that value
  currentR = (((currentR + baseR) / 2.0) - random(0, 70))*brightness;
  currentG = (((currentG + baseG) / 2.0) - random(0, 30))*brightness;
  currentB = (((currentB + baseB) / 2.0) - random(0, 5))*brightness;
  currentW = (random(1, 20)); // Pure random white seems to look "flickery" enough

  strip.setPixelColor(strip.numPixels()-1, currentR, currentG, currentB, currentW);
}
