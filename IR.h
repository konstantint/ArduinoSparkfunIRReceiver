/*
  IR.cpp - Library for receiving data from a particular IR remote.
  Created by Konstantin Tretyakov (http://kt.era.ee/), 2012.
  MIT license.
  
  NB: 
    * The IR detector must be connected to PIN 2 of Arduino. Only one detector is supported.
    * The code uses PCINT2_vect, so it probably wont link with other code that wants to use this interrupt.

  Usage:
    void setup() {
      ...
      IR.setup();
      ...
    }
    
    void loop() {
      while (IR.available()) {
        command_t command = IR.pop(); // Read next queued IR command.
        char* cmd = IR.commandToString(command);  // Translate to string (only translates several known commands, returns NULL for others)
        if (cmd != NULL) Serial.println(cmd);     // Show
      }
    }
*/
#ifndef IR_H
#define IR_H

#ifndef NULL
#define NULL 0
#endif

typedef unsigned int command_t;

// Ring buffer for storing unsigned ints
class RingBuffer {
private:
  static const int BUF_SIZE_BITS = 7;
  static const int BUF_SIZE = (1 << BUF_SIZE_BITS);
  static const int BUF_SIZE_MASK = (BUF_SIZE - 1);
  int head;
  int tail;
  command_t buffer[BUF_SIZE];
  
public:
  inline void init() {
    head = tail = 0;
  }
  inline void push(command_t val) {
    buffer[head++] = val;
    head &= BUF_SIZE_MASK;
    if (head == tail) { // Overwriting tail
      tail++;
      tail &= BUF_SIZE_MASK;
    }
  }
  inline command_t pop() {  // Don't do this unless available == true
    unsigned int result = buffer[tail++];
    tail &= BUF_SIZE_MASK;
    return result;
  }
  inline int available() {
    return (head != tail);
  }
};

// --------------------------- IR Detector lib -------------------------- //
// IR Detector data
class IRDetector {
public:
  // Codes
  //    onoff   1110001001000000    57920
  //    up      1110001001001000    57928
  //    dn      1110001001010000    57936
  //    mt      1110001001001100    57932
  //    left    1110001001010100    57940
  //    right   1110001001000100    57924
  //    av      1110001001011100    57948
  static const command_t ON_OFF = 57920;
  static const command_t UP = 57928;
  static const command_t DOWN = 57936;
  static const command_t MUTE = 57932;
  static const command_t LEFT = 57940;
  static const command_t RIGHT = 57924;
  static const command_t AV = 57948;
  
  // Simple helper for translating commands to their names
  // If unknown returns NULL
  inline char* commandToString(command_t cmd) {
    if (cmd == ON_OFF) return "ON-OFF";
    else if (cmd == UP) return "UP";
    else if (cmd == DOWN) return "DOWN";
    else if (cmd == MUTE) return "MUTE";
    else if (cmd == LEFT) return "LEFT";
    else if (cmd == RIGHT) return "RIGHT";
    else if (cmd == AV) return "AV";
    else return NULL;
  }
    
private:
  unsigned long lastBitTimestamp;  
  unsigned long curTime;
  unsigned long elapsed;
  unsigned int numBitsAvailable;
  command_t command;
  RingBuffer buffer;
  
public:
  void setup();  
  void interrupt();
  inline int available() { return buffer.available(); }
  inline command_t pop() { return buffer.pop(); }
};
extern IRDetector IR;

#endif
