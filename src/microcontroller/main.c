#include <Servo.h>

// 1. Set up
#define BITS_CHARACTER 6
#define LOW_ANGLE 90 
#define HIGH_ANGLE 60 // Servos are set inversely (downwards), thus a decreasement when HIGH

// 1.1 Binary representation of each ASCII character
int binary[][BITS_CHARACTER] = {
  {1,0,0,0,0,0},  // 'a' 1
  {1,1,0,0,0,0},  // 'b' 12
  {1,0,0,1,0,0},  // 'c' 14
  {1,0,0,1,1,0},  // 'd' 145
  {1,0,0,0,1,0},  // 'e' 15
  {1,1,0,1,0,0},  // 'f' 124
  {1,1,0,1,1,0},  // 'g' 1245
  {1,1,0,0,1,0},  // 'h' 125
  {0,1,0,1,0,0},  // 'i' 24
  {0,1,0,1,1,0},  // 'j' 245
  {1,0,1,0,0,0},  // 'k' 13
  {1,1,1,0,0,0},  // 'l' 123
  {1,0,1,1,0,0},  // 'm' 134
  {1,0,1,1,1,0},  // 'n' 1345
  {1,0,1,0,1,0},  // 'o' 135
  {1,1,1,1,0,0},  // 'p' 1234
  {1,1,1,1,1,0},  // 'q' 12345
  {1,1,1,0,1,0},  // 'r' 1235
  {0,1,1,1,0,0},  // 's' 234
  {0,1,1,1,1,0},  // 't' 2345
  {1,0,1,0,0,1},  // 'u' 136
  {1,1,1,0,0,1},  // 'v' 1236
  {0,1,0,1,1,1},  // 'w' 2456
  {1,0,1,1,0,1},  // 'x' 1346
  {1,0,1,1,1,1},  // 'y' 13456
  {1,0,1,0,1,1},  // 'z' 1356

  {1,0,0,0,0,0},  // '1' 1
  {1,1,0,0,0,0},  // '2' 12
  {1,0,0,1,0,0},  // '3' 14
  {1,0,0,1,1,0},  // '4' 145
  {1,0,0,0,1,0},  // '5' 15
  {1,1,0,1,0,0},  // '6' 124
  {1,1,0,1,1,0},  // '7' 1245
  {1,1,0,0,1,0},  // '8' 125
  {0,1,0,1,0,0},  // '9' 24
  {0,1,0,1,1,0},  // '0' 245
  {1,1,1,1,1,1}   // ' ' All pins up
};

// 1.2 ASCII dictionary (array really)
char ascii[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z',
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ' '
};

// 1.2 Setting up the digital pins
const int servoPins[6] = {3, 5, 6, 9, 10, 11}; 
Servo servos[6];

// 1.3 Attach servos to the new pins
void setup() {  

  Serial.begin(9600);

  for (int i = 1; i <= 6; i++) {

    // Starting from 1 to be consistent with the servos enumaration (i.e: a total of 6 servos used)
    servos[i].attach(servoPins[i-1]); 
    Serial.print("Attached servo ");
    Serial.print(i);
    Serial.print(" to pin ");
    Serial.println(servoPins[i-1]);

  }

  Serial.println("Enter text to display in Braille:");

  // Initial state of the pad is all servos up (in order for the user to self-calibrate)
  set_all_servos_to_high();

}



// 2. Codification system represenation
void loop() {

      /*
      
      This function will map the input its corresponding bit sequence. Then, represent it by
      setting 1s to HIGH and 0s to LOW for the corresponding digital motors (attached to a given servomotor).

      It contains statements used for debuggin purposes.
      
      */


  if (Serial.available() > 0) {

    Serial.println("\nPlease provide your input: "); // Input will really be received from python itself.
    String data = Serial.readString();

    // 2.1 Clean input
    data.toLowerCase();
    data.trim(); 

    Serial.print("\n\n\nSTARTING TO REPRESENT ");     
    Serial.print(data);
    Serial.print(" ...\n");    
    Serial.println("-----------------------------------------------------");

    for(int i = 0; i < data.length(); i++) { // 2.2 For each character in the received string loop and...

      char input = data[i];

      if (i != 0) {
        // 2.3 If not the first character, set them to LOW to identify the change between characters    
        set_all_servos_to_low(); 
      }

      Serial.print("\n\tCharacter is ");
      Serial.print(input);

      for (int j = 0; j < sizeof(ascii); j++) { // Mapping slice of string input to ASCII character

        if (input == ascii[j]) {
          Serial.println("\n\tCharacter has been mapped successfully.");
          Serial.println("\tI will now represent it...\n");

          // 2.4 Pointer to the given character array in the binary dictionary
          int *output = binary[j];
          
          for (int k = 0; k < BITS_CHARACTER; k++) { // Mapping to each bit of the given character

            Serial.print(output[k]);
            Serial.print(" ==> ");
            Serial.print("\tPIN: ");
            Serial.print(servoPins[k]);
            Serial.print(", Servo: ");
            Serial.print(k +1);

            if (output[k] == 1) {
              Serial.println(", Set to HIGH");
              servos[k+1].write(HIGH_ANGLE);
            }else{
              Serial.println(", Set to LOW");
              servos[k+1].write(LOW_ANGLE); // 
            }
          }
          
          // 2.5 Seven seconds to read the input
          delay(7000); 

          Serial.println("-----------------------------------------------------");
          break;

        }
      }
    } 
  // Once all pins have been represented, left the pad with all pins upwards   
  set_all_servos_to_high();
  }
}


// 3. Other functions 
void set_all_servos_to_low() {


  /*

  This function set all pins to rest (LOW)

  */


  Serial.println("'set_all_servos_to_low' has been called");
  for (int i = 0; i < 6; i++) {
    servos[i].write(LOW_ANGLE);   // 60 is HIGH, 80 is LOW
  }
  Serial.println("All servos are now at 'LOW' position");
  delay(2000);   
}

void set_all_servos_to_high() {


  /*

  This function set all pins to rest (HIGH)

  */


  Serial.println("'set_all_servos_to_low' has been called");
  for (int i = 0; i < 6; i++) {
    servos[i].write(HIGH_ANGLE);   // 60 is HIGH, 80 is LOW
  }
  Serial.println("All servos are now at 'HIGH' position");
  delay(2000);  
}