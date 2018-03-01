#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels
#define RGB_PIN            6
#define BUTTON             5
// How many NeoPixels are attached to the Arduino
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
long r;
long g;
long b;

#define SUPPORTED_COLORS 30

byte colors[SUPPORTED_COLORS][3] = { //rgb
	//OFF
	{0,0,0},

	//White
	{ 255,255,255 },

	//Red Zone
	{255,0,0},
	{ 255,26,26 },
	{ 255,83,26 },
	{ 255,140,26 },
	{ 255,198,26 },
	{ 255,255,26 },

	//Green Zone
	{0,255,0},
	{ 198, 255, 26 },
	{ 140, 255, 26 },
	{ 83, 255, 26 },
	{ 26, 255, 26 },
	{ 26, 255, 83 },
	{ 26, 255, 140 },
	{ 26, 255, 198 },
	{ 26, 255, 255 },

	//Blue Zone
	{ 0,0,255 },
	{ 26, 198, 255 },
	{ 26, 140, 255 },
	{ 26, 83, 255 },
	{ 26, 26, 255 },
	{ 83, 26, 255 },
	{ 140, 26, 255 },
	{ 198, 26, 255 },
	{ 255, 26, 255 },
	{ 255, 26, 198 },
	{ 255, 26, 140 },
	{ 255, 26, 83 },
};


void setup() {
    pinMode(BUTTON, INPUT);

    pixels.begin(); // This initializes the NeoPixel library.

    Serial.begin(9600);

    // if analog input pin 0 is unconnected, random analog
    // noise will cause the call to randomSeed() to generate
    // different seed numbers each time the sketch runs.
    // randomSeed() will then shuffle the random function.
    randomSeed(analogRead(0));
}

void loop() {
    //randomColors();
    selectColor();
}

void randomColors() {
     
    r = random(256);
    //Serial.println(r);
    g = random(256);
    //Serial.println(g);
    b = random(256);
    //Serial.println(b);


    pixels.setPixelColor(0, pixels.Color(g,r,b)); // g, r, b
    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

}

uint8_t colorIndex = 0;
void selectColor() {
    bool buttonPressed = digitalRead(BUTTON);

    if (buttonPressed) {
        colorIndex++;
        if (colorIndex == SUPPORTED_COLORS) //reset index 
        {
            colorIndex = 0;
        }
    } 

    pixels.setPixelColor(0, pixels.Color(colors[colorIndex][1], colors[colorIndex][0], colors[colorIndex][2])); // g, r, b - apparently this is the order of params that works..
    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
}