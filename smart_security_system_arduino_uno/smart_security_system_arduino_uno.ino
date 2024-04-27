#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


#define MAX_PASS_LEN 10
#define LOCK_KEY '*'
#define CLEAR_KEY '0'
#define BACK_KEY '#'
#define SUBMIT_KEY 'D'
#define SECOND 1000
#define MINUTE 60*SECOND
#define DANGER "Danger!!"

#define RLED_PIN A2
#define GLED_PIN A1
#define BLED_PIN A0

bool locked;
int len;
int remaining_tries;
static char pass[MAX_PASS_LEN+100]; // extra size for buffer overflow failsafe
static const char PASSWORD[MAX_PASS_LEN+100] = "156B";

void try_pass();
bool is_pswd();
void toggle_lock();
void send_warning();
void glow_red();
void glow_green();
void glow_blue();

void setup(){
   Serial.begin(115200);
   locked = true;
   len = 0;
   remaining_tries = 3;
   pinMode(RLED_PIN, OUTPUT);
   pinMode(GLED_PIN, OUTPUT);
   pinMode(BLED_PIN, OUTPUT);
   glow_blue();
}
  
void loop(){
  char key = keypad.getKey();

  if (key) {
    if (!locked && key != LOCK_KEY) {
      //skip
    }
    else if (key == LOCK_KEY) {
//      Serial.println("locking..");
      if (!locked)
        toggle_lock();
    } else if (key == CLEAR_KEY) {
      len = 0;
      memset(pass, 0, sizeof(pass));
    } else if (key == BACK_KEY) {
      len = max(0, len-1);
      memset(pass+len, 0, sizeof(pass)-len);
    } else if (key == SUBMIT_KEY) {
      try_pass();
    } else {
//        Serial.println(key);
      pass[len++] = key;
      if (len == MAX_PASS_LEN) {
          try_pass();
      }
    }
  }
  
  delay(50); // saving processor power
}

void try_pass() {
//  Serial.println(pass);
  if (!is_pswd()) {
    if (remaining_tries == 0) {
      send_warning();
    } else {
      remaining_tries--;
    }
  } else {
      toggle_lock();
  }
  len = 0;
  memset(pass, 0, sizeof(pass));
}

bool is_pswd() {
  if (len == 0)
    return false;
//  Serial.print("ispaswd: ");
//  Serial.println(pass);
  for (int i=0; i<len; i++) {
//    Serial.print(i);
    if (pass[i] != PASSWORD[i])
      return false;
  }
  return true;
}

void toggle_lock() {
//  Serial.print("\ntogglecalled: ");
//  Serial.println(pass);
  locked = !locked;
  if (locked) {
    glow_blue();
  } else {
    glow_green(); 
  }
  memset(pass, 0, sizeof(pass));
  len = 0;
  remaining_tries = 3;
  if (locked) {
    delay(1000);
  }
}

void send_warning() {
  Serial.println(DANGER);
  remaining_tries = 3;
  glow_red();  
  delay(10000);
}

void glow_red() {
  analogWrite(RLED_PIN, 255);
  analogWrite(GLED_PIN, 0);
  analogWrite(BLED_PIN, 0);  
}

void glow_green() {
  analogWrite(RLED_PIN, 0);
  analogWrite(GLED_PIN, 255);
  analogWrite(BLED_PIN, 0);  
}

void glow_blue() {
  analogWrite(RLED_PIN, 0);
  analogWrite(GLED_PIN, 0);
  analogWrite(BLED_PIN, 255);  
}
