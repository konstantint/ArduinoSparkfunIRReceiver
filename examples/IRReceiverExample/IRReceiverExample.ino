/*
  Example usage of the IR library.
  Created by Konstantin Tretyakov (http://kt.era.ee), 2012.
  MIT license.
   
  Components: 
    Arduino Uno or compatible
    Sparkfun IR receiver breakout (http://www.sparkfun.com/products/8554)
    Sparkfun IR remote (http://www.sparkfun.com/products/10280)
    
    Presumably many alternative components would work as well.
    
   Connection:
   
     Arduino pin 2 <--> IR receiver "out"
     Arduino gnd   <--> IR receiver "gnd"
     Arduino 5V    <--> IR receiver "vcc"

   Usage:
     Open the serial monitor and try using the remote on the sensor.
     The serial monitor will display commands it receives.  
     
   Notes:
     * The library currently only works with IR receiver on PIN 2. It is not too hard to change 
       it in the code, but it's not otherwise easily configurable.
     * The library uses up the PCINT2 interrupt. If any other code in the project needs this interrupt
       things probably won't work out well.
 */ 
#include <IR.h>

void setup() {
  IR.setup();  // Initialize IR receiver
  Serial.begin(9600);// Initialize serial communication
}


void loop() {
  
  // If IR module has managed to receive some commands inbetween its "available" method will become true.
  // After that, received commands can be read one-by-one using the pop() method.
  while (IR.available()) {
    command_t command_code = IR.pop(); // Receive command code
    
    // The following function will translate the 7 command codes
    // from the Sparkfun remote to strings for convenience.
    // For unknown commands it'll return NULL
    char* cmd = IR.commandToString(command_code);
    if (cmd != NULL) Serial.println(cmd);
    
    // Note: The current implementation recognizes commands sent by the Sparkfun remote
    // I have no idea what protocol this is exactly, I simply looked at the signal and wrote
    // the necessary code to recognize it 
    // (which simply treats pulses shorter than 1500us as 0, those longer than it as 1, and
    // waits until 16 bits arrive to form a command code.
    // It might not necessarily work with other remotes, but you may try looking
    // look at what bits your remote will trigger.
  }
}










