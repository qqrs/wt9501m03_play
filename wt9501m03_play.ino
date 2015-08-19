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
byte CMD_PLAY_PREFACE[] = {0x7E, 0x07, 0xA0};
byte CMD_PLAY_INVALID[] = {0x7E, 0x07, 0xA0, 0x39, 0x39, 0x39, 0x39, 0x39, 0x7E};

byte CMD_VOLUME_MAX[] = {0x7E, 0x03, 0xA4, 0x19, 0x7E};
byte CMD_VOLUME_MIN[] = {0x7E, 0x03, 0xA4, 0x00, 0x7E};
byte CMD_VOLUME_HALF[] = {0x7E, 0x03, 0xA4, 0x0C, 0x7E};
byte CMD_VOLUME_PREF[] = {0x7E, 0x03, 0xA4, 0x12, 0x7E};

byte CMD_LOOP_NONE[] = {0x7E, 0x02, 0xA7, 0x7E};
byte CMD_LOOP_SINGLE[] = {0x7E, 0x02, 0xA8, 0x7E};
byte CMD_LOOP_ALL[] = {0x7E, 0x02, 0xA9, 0x7E};

enum input_state_t {
  ST_READY,
  ST_SETBANK0,
  ST_SETBANK1,
  ST_FUNC
};

input_state_t input_state;
char bank[2];


void setup() {
  Serial.begin(9600);

  input_state = ST_READY;
  bank[0] = '0';
  bank[1] = '0';

  mp3_send_cmd(CMD_VOLUME_MAX);
  /*mp3_send_cmd(CMD_LOOP_ALL);*/
  /*mp3_send_cmd(CMD_PLAY_FIRST);*/
  /*mp3_send_cmd(CMD_PLAY_INVALID);*/
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    handle_keypress(key);
  }
}

void mp3_play(char bank0, char bank1, char track) {
  mp3_send_cmd(CMD_PLAY_PREFACE);
  Serial.write('0');
  Serial.write('0');
  Serial.write(bank0);
  Serial.write(bank1);
  Serial.write(track);
  Serial.write(0x7E);
}

void handle_keypress(char key) {
  switch (input_state) {
    case ST_READY:
      if (key == '*') {
        input_state = ST_SETBANK0;
      } else if (key == '#') {
        input_state = ST_FUNC;
      } else if (key >= '0' && key <= '9') {
        mp3_play(bank[0], bank[1], key);
      }
      break;

    case ST_SETBANK0:
      if (key >= '0' && key <= '9') {
        bank[0] = key;
        input_state = ST_SETBANK1;
      }
      break;

    case ST_SETBANK1:
      if (key >= '0' && key <= '9') {
        bank[1] = key;
        input_state = ST_READY;
      }
      break;

    case ST_FUNC:
      if (key >= '0' && key <= '9') {
        handle_keypress_func(key);
        input_state = ST_READY;
      }
      break;
  }
}

void handle_keypress_func(char key) {
  switch (key) {
    case '0': mp3_send_cmd(CMD_VOLUME_MIN);   break;
    case '1': mp3_send_cmd(CMD_VOLUME_HALF);  break;
    case '2': mp3_send_cmd(CMD_VOLUME_PREF);  break;
    case '3': mp3_send_cmd(CMD_VOLUME_MAX);   break;

    case '7': mp3_send_cmd(CMD_LOOP_NONE);    break;
    case '8': mp3_send_cmd(CMD_LOOP_SINGLE);  break;
    case '9': mp3_send_cmd(CMD_LOOP_ALL);     break;
  }
}
