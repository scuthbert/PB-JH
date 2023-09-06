int prevR, prevG, prevB = 0;

void Flame() {
  // Flicker, based on our initial RGB values
  for(int i=0; i<strip.numPixels()-1; i++) {
    strip.setPixelColor(i, strip.getPixelColor(i+1));
  }

  // Regular (orange) flame:
  int r = 255, g = 75, b = 10;

  int flickerR = random(0,75);
  int flickerG = random(0,20);
  int flickerB = random(0,5);

  int r1 = (prevR+r)/1.5;
  int g1 = (prevG+g)/1.5;
  int b1 = (prevB+b)/1.5;
  int w1 = max(max(r1, g1), b1)-100;

  prevR = (r1-flickerR)*0.5;
  prevG = (g1-flickerG)*0.5;
  prevB = (b1-flickerB)*0.5;

  strip.setPixelColor(strip.numPixels()-1, r1, g1, b1, w1);
}