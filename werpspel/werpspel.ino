
#include <WS2812FX.h>

#define LED_COUNT 120
#define LED_PIN 12
#define LED_COUNT_SCORE 28
#define LED_PIN_SCORE 13
int buttonPins[10] = {2,3,4,5,6,7,8,9,10,11};
bool buttonState[10] = {false,false,false,false,false,false,false,false,false,false};
//bool buttonPrevState[10] = {false,false,false,false,false,false,false,false,false,false};
int ledSegmentsEffect[10] = {0,0,0,0,0,0,0,0,0,0};
uint32_t ledcolors[10] = {GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN};
int ledSegementSpeed[10] = {500,500,500,500,500,500,500,500,500,500};
int scoreValues[10] = {3,1,1,3,1,5,1,1,1,3};



WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WS2812FX ws2812fx_score = WS2812FX(LED_COUNT_SCORE, LED_PIN_SCORE, NEO_GRB + NEO_KHZ800);

bool cijferOFF_R[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool cijfer0_R[14] = {1,1,1,1,1,1,1,1,1,1,1,1,0,0};
bool cijfer1_R[14] = {0,0,1,1,1,1,0,0,0,0,0,0,0,0};
bool cijfer2_R[14] = {1,1,0,0,1,1,1,1,0,0,1,1,1,1};
bool cijfer3_R[14] = {1,1,1,1,1,1,1,1,0,0,0,0,1,1};
bool cijfer4_R[14] = {0,0,1,1,1,1,0,0,1,1,0,0,1,1};
bool cijfer5_R[14] = {1,1,1,1,0,0,1,1,1,1,0,0,1,1};
bool cijfer6_R[14] = {1,1,1,1,0,0,1,1,1,1,1,1,1,1};
bool cijfer7_R[14] = {0,0,1,1,1,1,1,1,0,0,0,0,0,0};
bool cijfer8_R[14] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool cijfer9_R[14] = {1,1,1,1,1,1,1,1,1,1,0,0,1,1};

bool cijferOFF_L[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool cijfer0_L[14] = {1,1,1,1,1,1,1,1,1,1,1,1,0,0};
bool cijfer1_L[14] = {1,1,1,1,0,0,0,0,0,0,0,0,0,0};
bool cijfer2_L[14] = {0,0,1,1,1,1,0,0,1,1,1,1,1,1};
bool cijfer3_L[14] = {1,1,1,1,1,1,0,0,0,0,1,1,1,1};
bool cijfer4_L[14] = {1,1,1,1,0,0,1,1,0,0,0,0,1,1};
bool cijfer5_L[14] = {1,1,0,0,1,1,1,1,0,0,1,1,1,1};
bool cijfer6_L[14] = {1,1,0,0,1,1,1,1,1,1,1,1,1,1};
bool cijfer7_L[14] = {1,1,1,1,1,1,0,0,0,0,0,0,0,0};
bool cijfer8_L[14] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
bool cijfer9_L[14] = {1,1,1,1,1,1,1,1,0,0,1,1,1,1};

bool cijfer_R[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool cijfer_L[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int score;
uint32_t scoreColor = BLUE;
unsigned long lastButtonPress;
int debounce = 500;

int btn_pin = A1;
int btn_led = A0;

  int pinsPerButton = 12;

void setup() {
  for(int i = 0; i<10;i++)
  {
    pinMode(buttonPins[i], INPUT);
  }

  pinMode(btn_pin, INPUT);
  pinMode(btn_led, OUTPUT);

  digitalWrite(btn_led, HIGH);
  

  Serial.begin(9600);

  ws2812fx.init();
  ws2812fx.setBrightness(100);
  ws2812fx.setColor(0x00FF00);
  ws2812fx.setSpeed(5000);
  ws2812fx.setMode(0);
  ws2812fx.start();

  ws2812fx_score.init();
  ws2812fx_score.setBrightness(100);
  ws2812fx_score.setColor(0xFF0000);
  ws2812fx_score.setSpeed(5000);
  ws2812fx_score.setMode(0);
  ws2812fx_score.start();

  score = 0;

  




  //set segments

  setSegments();
  //define segments for scorebord
  ws2812fx_score.setCustomMode(showGetal);
  ws2812fx_score.setSegment(0,  0, 27, FX_MODE_CUSTOM, RED, 1000, false);
  lastButtonPress = millis();
}

void loop() {
  if((millis() - debounce) > lastButtonPress)
  {
    checkButtons();
  }
  showScore();
  setSegments();
  ws2812fx.service();
  ws2812fx_score.service();
}
void checkButtons()
{
  //check normal buttons
  for(int i = 0; i<10;i++)
  {
    int val = digitalRead(buttonPins[i]);
    if(val == 1 && val != buttonState[i])
    {
      //button is pressed
      buttonState[i] = val;
      ledSegementSpeed[i] = 1000;
      lastButtonPress = millis();
      score = score + scoreValues[i];
      ledSegmentsEffect[i] = 1;
      ledcolors[i] = RED;
    }
  }
  //check reset button
  if(digitalRead(btn_pin) == HIGH)
  {
    resetGame();
  }
}

void resetGame()
{
  score = 0;
  for(int i = 0; i<10;i++)
  {
    buttonState[i] = false;
  }
  for(int i = 0; i<10;i++)
  {
    ledSegmentsEffect[i] = 0;
  }
  for(int i = 0; i<10;i++)
  {
    ledcolors[i] = GREEN;
  }
  for(int i = 0; i<10;i++)
  {
    ledSegementSpeed[i] = 500;
  }
  lastButtonPress = millis();
}

void setSegments()
{
  for(int i = 0; i<10;i++)
  {
    ws2812fx.setSegment(i,  (i*pinsPerButton), ((i*pinsPerButton)+pinsPerButton)-1, ledSegmentsEffect[i], ledcolors[i], ledSegementSpeed[i], false);
  }
 }

void showScore()
 {
   switch(score)
   {
     case 0:
      setLeft(cijferOFF_L);
      setRight(cijfer0_R);      
      break;
     case 1:
      setLeft(cijferOFF_L);
      setRight(cijfer1_R);
      break;
     case 2:
      setLeft(cijferOFF_L);
      setRight(cijfer2_R);
      break;
     case 3:
      setLeft(cijferOFF_L);
      setRight(cijfer3_R);
      break;
     case 4:
      setLeft(cijferOFF_L);
      setRight(cijfer4_R);
      break;
     case 5:
      setLeft(cijferOFF_L);
      setRight(cijfer5_R);
      break;
     case 6:
      setLeft(cijferOFF_L);
      setRight(cijfer6_R);
      break;
     case 7:
      setLeft(cijferOFF_L);
      setRight(cijfer7_R);
      break;
     case 8:
      setLeft(cijferOFF_L);
      setRight(cijfer8_R);
      break;
     case 9:
      setLeft(cijferOFF_L);
      setRight(cijfer9_R);
      break;
     case 10:
      setLeft(cijfer1_L);
      setRight(cijfer0_R);
      break;
     case 11:
      setLeft(cijfer1_L);
      setRight(cijfer1_R);
      break;
     case 12:
      setLeft(cijfer1_L);
      setRight(cijfer2_R);
      break;
     case 13:
      setLeft(cijfer1_L);
      setRight(cijfer3_R);
      break;
     case 14:
      setLeft(cijfer1_L);
      setRight(cijfer4_R);
      break;
     case 15:
      setLeft(cijfer1_L);
      setRight(cijfer5_R);
      break;
     case 16:
      setLeft(cijfer1_L);
      setRight(cijfer6_R);
      break;
     case 17:
      setLeft(cijfer1_L);
      setRight(cijfer7_R);
      break;
     case 18:
      setLeft(cijfer1_L);
      setRight(cijfer8_R);
      break;
     case 19:
      setLeft(cijfer1_L);
      setRight(cijfer9_R);
      break;
     case 20:
      setLeft(cijfer2_L);
      setRight(cijfer0_R);
      break;
     case 21:
      setLeft(cijfer2_L);
      setRight(cijfer1_R);
      break;
     case 22:
      setLeft(cijfer2_L);
      setRight(cijfer2_R);
      break;
     case 23:
      setLeft(cijfer2_L);
      setRight(cijfer3_R);
      break;
     case 24:
      setLeft(cijfer2_L);
      setRight(cijfer4_R);
      break;
     case 25:
      setLeft(cijfer2_L);
      setRight(cijfer5_R);
      break;
     case 26:
      setLeft(cijfer2_L);
      setRight(cijfer6_R);
      break;
     case 27:
      setLeft(cijfer2_L);
      setRight(cijfer7_R);
      break;
     case 28:
      setLeft(cijfer2_L);
      setRight(cijfer8_R);
      break;
     case 29:
      setLeft(cijfer2_L);
      setRight(cijfer9_R);
      break;
     case 30:
      setLeft(cijfer3_L);
      setRight(cijfer0_R);
      break;
     case 31:
      setLeft(cijfer3_L);
      setRight(cijfer1_R);
      break;
     case 32:
      setLeft(cijfer3_L);
      setRight(cijfer2_R);
      break;
     case 33:
      setLeft(cijfer3_L);
      setRight(cijfer3_R);
      break;
     case 34:
      setLeft(cijfer3_L);
      setRight(cijfer4_R);
      break;
     case 35:
      setLeft(cijfer3_L);
      setRight(cijfer5_R);
      break;
     case 36:
      setLeft(cijfer3_L);
      setRight(cijfer6_R);
      break;
     case 37:
      setLeft(cijfer3_L);
      setRight(cijfer7_R);
      break;
     case 38:
      setLeft(cijfer3_L);
      setRight(cijfer8_R);
      break;
     case 39:
      setLeft(cijfer3_L);
      setRight(cijfer9_R);
      break;
     case 40:
      setLeft(cijfer4_L);
      setRight(cijfer0_R);
      break;
     case 41:
      setLeft(cijfer4_L);
      setRight(cijfer1_R);
      break;
     case 42:
      setLeft(cijfer4_L);
      setRight(cijfer2_R);
      break;
     case 43:
      setLeft(cijfer4_L);
      setRight(cijfer3_R);
      break;
     case 44:
      setLeft(cijfer4_L);
      setRight(cijfer4_R);
      break;
     case 45:
      setLeft(cijfer4_L);
      setRight(cijfer5_R);
      break;
     case 46:
      setLeft(cijfer4_L);
      setRight(cijfer6_R);
      break;
     case 47:
      setLeft(cijfer4_L);
      setRight(cijfer7_R);
      break;
     case 48:
      setLeft(cijfer4_L);
      setRight(cijfer8_R);
      break;
     case 49:
      setLeft(cijfer4_L);
      setRight(cijfer9_R);
      break;
     case 50:
      setLeft(cijfer5_L);
      setRight(cijfer0_R);
      break;
     case 51:
      setLeft(cijfer5_L);
      setRight(cijfer1_R);
      break;
     case 52:
      setLeft(cijfer5_L);
      setRight(cijfer2_R);
      break;
     case 53:
      setLeft(cijfer5_L);
      setRight(cijfer3_R);
      break;
     case 54:
      setLeft(cijfer5_L);
      setRight(cijfer4_R);
      break;
     case 55:
      setLeft(cijfer5_L);
      setRight(cijfer5_R);
      break;
     case 56:
      setLeft(cijfer5_L);
      setRight(cijfer6_R);
      break;
     case 57:
      setLeft(cijfer5_L);
      setRight(cijfer7_R);
      break;
     case 58:
      setLeft(cijfer5_L);
      setRight(cijfer8_R);
      break;
     case 59:
      setLeft(cijfer5_L);
      setRight(cijfer9_R);
      break;
     case 60:
      setLeft(cijfer6_L);
      setRight(cijfer0_R);
      break;
     case 61:
      setLeft(cijfer6_L);
      setRight(cijfer1_R);
      break;
     case 62:
      setLeft(cijfer6_L);
      setRight(cijfer2_R);
      break;
     case 63:
      setLeft(cijfer6_L);
      setRight(cijfer3_R);
      break;
     case 64:
      setLeft(cijfer6_L);
      setRight(cijfer4_R);
      break;
     case 65:
      setLeft(cijfer6_L);
      setRight(cijfer5_R);
      break;
     case 66:
      setLeft(cijfer6_L);
      setRight(cijfer6_R);
      break;
     case 67:
      setLeft(cijfer6_L);
      setRight(cijfer7_R);
      break;
     case 68:
      setLeft(cijfer6_L);
      setRight(cijfer8_R);
      break;
     case 69:
      setLeft(cijfer6_L);
      setRight(cijfer9_R);
      break;
     case 70:
      setLeft(cijfer7_L);
      setRight(cijfer0_R);
      break;
     case 71:
      setLeft(cijfer7_L);
      setRight(cijfer1_R);
      break;
     case 72:
      setLeft(cijfer7_L);
      setRight(cijfer2_R);
      break;
     case 73:
      setLeft(cijfer7_L);
      setRight(cijfer3_R);
      break;
     case 74:
      setLeft(cijfer7_L);
      setRight(cijfer4_R);
      break;
     case 75:
      setLeft(cijfer7_L);
      setRight(cijfer5_R);
      break;
     case 76:
      setLeft(cijfer7_L);
      setRight(cijfer6_R);
      break;
     case 77:
      setLeft(cijfer7_L);
      setRight(cijfer7_R);
      break;
     case 78:
      setLeft(cijfer7_L);
      setRight(cijfer8_R);
      break;
     case 79:
      setLeft(cijfer7_L);
      setRight(cijfer9_R);
      break;
     case 80:
      setLeft(cijfer8_L);
      setRight(cijfer0_R);
      break;
     case 81:
      setLeft(cijfer8_L);
      setRight(cijfer1_R);
      break;
     case 82:
      setLeft(cijfer8_L);
      setRight(cijfer2_R);
      break;
     case 83:
      setLeft(cijfer8_L);
      setRight(cijfer3_R);
      break;
     case 84:
      setLeft(cijfer8_L);
      setRight(cijfer4_R);
      break;
     case 85:
      setLeft(cijfer8_L);
      setRight(cijfer5_R);
      break;
     case 86:
      setLeft(cijfer8_L);
      setRight(cijfer6_R);
      break;
     case 87:
      setLeft(cijfer8_L);
      setRight(cijfer7_R);
      break;
     case 88:
      setLeft(cijfer8_L);
      setRight(cijfer8_R);
      break;
     case 89:
      setLeft(cijfer8_L);
      setRight(cijfer9_R);
      break;
     case 90:
      setLeft(cijfer9_L);
      setRight(cijfer0_R);
      break;
     case 91:
      setLeft(cijfer9_L);
      setRight(cijfer1_R);
      break;
     case 92:
      setLeft(cijfer9_L);
      setRight(cijfer2_R);
      break;
     case 93:
      setLeft(cijfer9_L);
      setRight(cijfer3_R);
      break;
     case 94:
      setLeft(cijfer9_L);
      setRight(cijfer4_R);
      break;
     case 95:
      setLeft(cijfer9_L);
      setRight(cijfer5_R);
      break;
     case 96:
      setLeft(cijfer9_L);
      setRight(cijfer6_R);
      break;
     case 97:
      setLeft(cijfer9_L);
      setRight(cijfer7_R);
      break;
     case 98:
      setLeft(cijfer9_L);
      setRight(cijfer8_R);
      break;
     case 99:
      setLeft(cijfer9_L);
      setRight(cijfer9_R);
      break;
      default:
      setLeft(cijferOFF_L);
      setRight(cijferOFF_R);
      break;
   }
 }

 void setLeft(bool* src) {
    memcpy(cijfer_L, src, sizeof(src[0])*14);
}
 void setRight(bool* src) {
    memcpy(cijfer_R, src, sizeof(src[0])*14);
}

uint16_t showGetal(void) {
  // get the current segment
  WS2812FX::Segment* seg = ws2812fx_score.getSegment();
  //seg->start
  //seg->stop

  //set right digit
  for(int i = 0; i<14;i++)
  {
    if(cijfer_R[i] == 1)
    {
      ws2812fx_score.setPixelColor(i, scoreColor);
    }else{
      ws2812fx_score.setPixelColor(i, BLACK);
    }
  }

  //set left digit
  for(int i = 14; i<28;i++)
  {
    if(cijfer_L[i-14] == 1)
    {
      ws2812fx_score.setPixelColor(i, scoreColor);
    }else{
      ws2812fx_score.setPixelColor(i, BLACK);
    }
  }
    



  return seg->speed; // return the segment's speed
}

