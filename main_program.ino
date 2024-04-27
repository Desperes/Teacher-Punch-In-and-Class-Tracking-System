#include <avr/pgmspace.h>
#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define FINGERPRINT_RX_PIN 2
#define FINGERPRINT_TX_PIN 3
#define LEFT_BUTTON_PIN 4
#define RIGHT_BUTTON_PIN 5
#define INTERRUPT_PIN 8

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// SoftwareSerial mySerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
volatile bool InterruptFlag = false;
const char PROGMEM subject[][11] = {
    "MONDAY", "EL", "VL", "VL", "EM", "IT", "HO",
    "TUESDAY", "VL", "IT", "EL", "MP", "MP", "MP",
    "WEDNESDAY", "EL", "ME", "VL", "EM", "CW", "HO",
    "THURSDAY", "ME", "EM", "IT", "MP", "MP", "MP",
    "FRIDAY", "IT", "EM", "ME", "HO", "MI", "MI"
};

// const char subject[][11] PROGMEM= {
//     {"MONDAY", "EL", "VL", "VL", "EM", "IT", "HO"},
//     {"TUESDAY", "VL", "IT", "EL", "MP", "MP", "MP"},
//     {"WEDNESDAY", "EL", "ME", "VL", "EM", "CW", "HO"},
//     {"THURSDAY", "ME", "EM", "IT", "MP", "MP", "MP"},
//     {"FRIDAY", "IT", "EM", "ME", "HO", "MI", "MI"}
// };
const int mapping[5][6] PROGMEM= {
    {1, 4, 5, 6, 7, 8},    // 0th Element = Monday
    {2, 4, 6, 8, 3, 5},    // 1st Element = Tuesday
    {4, 5, 6, 2, 1, 3},
    {1, 4, 5, 6, 7, 5},
    {4, 5, 6, 2, 1, 5}
};
char attendance[5][6][5];
RTC_DS1307 rtc;
int currentDay = rtc.now().dayOfTheWeek();
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 6; j++)
    {
        attendance[i][j][0] = '\0'; // Null-terminate the string
    }
  }
  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON_PIN), leftButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON_PIN), rightButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN),isr, FALLING);

  // Initialize the OLED object
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }// put your setup code here, to run once:
  else{
        Serial.println(F("SSD1306 allocation succeeded"));

  }


   while (!Serial); 
  delay(100);
  // Serial.println(F("\n\nAdafruit finger detect test"));

  // set the data rate for the sensor serial port
  //finger.begin(57600);
  // delay(5);
  // if (finger.verifyPassword())
  // {
  //   Serial.println("Found fingerprint sensor!");
  // }
  // else
  // {
  //   Serial.println("Did not find fingerprint sensor :(");
  //   while (1)
  //   {
  //     delay(1);
  //   }
  // }
  
  DisplayTable((const char **)subject,attendance,currentDay);

}
void loop() {
  if(InterruptFlag)
  {
    // int mapping = pgm_read_word(&mapping[]);
        int day = rtc.now().dayOfTheWeek();
        int hour= rtc.now().hour();
        int mappingVal;

        if (day == 0 || day == 6) 
          return 0;    // if the day is sat or sunday come out of the if condition

        if (hour < 12) {  
            mappingVal= pgm_read_word(&(mapping[day-1][hour-9]));                    // excluding 12pm
            if (checkFingerprintID()== mappingVal) {
                strcpy(attendance[day-1][hour-9], "pres");
            } else {
                strcpy(attendance[day-1][hour-9], "abs");

            }
        } else if (hour > 12) {
            mappingVal= pgm_read_word(&(mapping[day-1][hour-10])); 
            if (checkFingerprintID()== mappingVal) {
                strcpy(attendance[day-1][hour-10], "pres");
            } else {
                strcpy(attendance[day-1][hour-10], "abs");
            }
        }    
    InterruptFlag=false;
    DisplayTable((const char **)subject,attendance,day);
  }

}
void isr()
{
  InterruptFlag= true;
}
void DisplayTable(const char* subject[],  char attendance[][6][5], int day)
{
  display.clearDisplay();
  //draw arrows 
  int arrowSize = 5; // Smaller arrow size
  int arrowX = 0; // Initial position for left arrow
  int arrowY = SCREEN_HEIGHT - (2 * arrowSize); // Leave enough space for two arrows
  display.fillTriangle(arrowX, arrowY + arrowSize, arrowX + arrowSize, arrowY, arrowX + arrowSize, arrowY + (2 * arrowSize), WHITE); 
  arrowX = SCREEN_WIDTH - arrowSize; // Rightmost position for right arrow
  display.fillTriangle(arrowX + arrowSize, arrowY + arrowSize, arrowX, arrowY, arrowX, arrowY + (2 * arrowSize), WHITE);

  // Draw 3x2 table of rectangles
  int rectWidth = 42; // Adjusted width to fit within the screen
  int rectHeight = 27; // Adjusted height to fit within the screen
  int count=1;
  for (int row = 0; row < 2; row++)
  {
     for (int col = 0; col < 3; col++)
    {
      int x = col * (rectWidth);
      int y = row * (rectHeight);
      
      display.drawRect(x, y, rectWidth, rectHeight, WHITE);

      // Add tick mark pattern and 'EM' inside one rectangle
      // centerX = x + rectWidth / 2;
      // centerY = y + rectHeight / 2;
        display.setTextSize(1);  // Set text size
        display.setTextColor(WHITE); // Set text color
        display.setCursor(x + rectWidth - 13, y + rectHeight - 10);     
        display.print(subject[day-2][count]);        
        if(attendance[day-2][count-1]=="pres")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else if(attendance[day-2][count-1]=="abs")
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }
        else{}
        count++;
      
    }
  }
  // Calculate the position for "Monday" text
  int textWidth = 7 * 6;                       // Assuming each character occupies 6 pixels
  int textX = (SCREEN_WIDTH - textWidth) / 2;  // Center text horizontally
  int textY = SCREEN_HEIGHT - 8;               // Bottom of the screen with some margin

  // Set text properties and display "Monday" text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(textX, textY);
  display.print(subject[day-2][0]);
  display.display();

}
 
// Function to check if the scanned fingerprint matches a stored fingerprint and return the ID if a match is found
uint8_t checkFingerprintID() {
  // uint8_t p = finger.getImage(); // Capture fingerprint image
  // if (p != FINGERPRINT_OK) {
  //   // Serial.println("Failed to capture fingerprint");
  //   return 0; // Return 0 to indicate failure
  //   // Serial.println("E");
  // }

  // p = finger.image2Tz(); // Convert image to template
  // if (p != FINGERPRINT_OK) {
  //   // Serial.println("Failed to convert image to template");
  //   return 0; // Return 0 to indicate failure
  // }

  // // Search for a matching fingerprint template
  // p = finger.fingerFastSearch();
  // if (p == FINGERPRINT_OK) {
  //   uint16_t matchedID = finger.fingerID; // Get matched fingerprint ID
  //   // Serial.print("Matched fingerprint ID: ");
  //   // Serial.println(matchedID);
  //   return matchedID;
  // } else {
  //   // Serial.println("No matching fingerprint found");
  //   return 0; // Return 0 to indicate failure
  // }
  // else {return 0;}
}
void leftButtonISR()
{
  

  currentDay--;
    if (currentDay < 2) {
      currentDay = 6; // Wrap around to Sunday if currentDay becomes negative
    }
    // Redraw the schedule for the new day
    DisplayTable((const char **)subject,attendance,currentDay);
      delay(200); // Debounce delay
  // Code to execute when the left button is pressed

}

void rightButtonISR()
{   
    currentDay++;
    if (currentDay > 4) {
      currentDay = 0; // Wrap around to Monday if currentDay exceeds 6
    }
    // Redraw the schedule for the new day
    DisplayTable((const char **)subject,attendance,currentDay);
    delay(200);
}




