#include <Keypad.h>

// keypad configuration
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define mp3_send_cmd(x) Serial.write((x), sizeof(x))

byte CMD_PLAY_FIRST[] = {0x7E, 0x07, 0xA0, 0x30, 0x30, 0x30, 0x30, 0x31, 0x7E};
byte CMD_PLAY_PREFACE[] = {0x7E, 0x07, 0xA0, 0x30, 0x30, 0x30, 0x30};
byte CMD_PLAY_INVALID[] = {0x7E, 0x07, 0xA0, 0x39, 0x39, 0x39, 0x39, 0x39, 0x7E};

byte CMD_VOLUME_MAX[] = {0x7E, 0x03, 0xA4, 0x19, 0x7E};
byte CMD_VOLUME_MIN[] = {0x7E, 0x03, 0xA4, 0x00, 0x7E};
byte CMD_VOLUME_HALF[] = {0x7E, 0x03, 0xA4, 0x0C, 0x7E};
byte CMD_VOLUME_PREF[] = {0x7E, 0x03, 0xA4, 0x12, 0x7E};

byte CMD_LOOP_NONE[] = {0x7E, 0x02, 0xA7, 0x7E};
byte CMD_LOOP_SINGLE[] = {0x7E, 0x02, 0xA8, 0x7E};
byte CMD_LOOP_ALL[] = {0x7E, 0x02, 0xA9, 0x7E};

void setup() {
  Serial.begin(9600);

  mp3_send_cmd(CMD_VOLUME_MAX);
  /*mp3_send_cmd(CMD_LOOP_ALL);*/
  /*mp3_send_cmd(CMD_PLAY_FIRST);*/
  /*mp3_send_cmd(CMD_PLAY_INVALID);*/
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (key >= '0' && key <= '9') {
        mp3_play(key);
    }
  }
}

void mp3_play(char track_id) {
  mp3_send_cmd(CMD_PLAY_PREFACE);
  Serial.write(track_id);
  Serial.write(0x7E);
}
