#include <SPI.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define FINGERPRINT_RX_PIN 2
#define FINGERPRINT_TX_PIN 3

SoftwareSerial mySerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int Interruptpin = 4;
volatile bool InterruptFlag = false;
int centerX;
int centerY;
const char* subject[][7] = {
    {"MONDAY", "EL", "VL", "VL", "EM", "IT", "HO"},
    {"TUESDAY", "VL", "IT", "EL", "MP", "MP", "MP"},
    {"WEDNESDAY", "EL", "ME", "VL", "EM", "CW", "HO"},
    {"THURSDAY", "ME", "EM", "IT", "MP", "MP", "MP"},
    {"FRIDAY", "IT", "EM", "ME", "HO", "MI", "MI"}
};

int mapping[5][6] = {
    {1, 4, 5, 6, 7, 8},    // 0th Element = Monday
    {2, 4, 6, 8, 3, 5},    // 1st Element = Tuesday
    {4, 5, 6, 2, 1, 3},
    {1, 4, 5, 6, 7, 5},
    {4, 5, 6, 2, 1, 5}
};

char attendance[5][6][20]; // Assuming each string can be up to 19 characters long
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 6; j++)
    {
        attendance[i][j][0] = '\0'; // Null-terminate the string
    }
  }

   pinMode(interruptPin, INPUT_PULLUP); // Set the interrupt pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR, FALLING);
  // Initialize the OLED object
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }// put your setup code here, to run once:
  else{
        Serial.println(F("SSD1306 allocation succeeded"));

  }


   while (!Serial); 
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1)
    {
      delay(1);
    }
  }
}
void loop() {
  // if(InterruptFlag)
  // {
  //   switch(rtc.dayOfWeek())
  //   {
  //     case 1: switch(rtc.hour())
  //             {
  //                 case 9: if(checkFingerprintID()==1)
  //                         {
  //                           strcpy(attendance[0][0], "present");
  //                         }
  //                         else
  //                         {
  //                           strcpy(attendance[0][0], "absent");
  //                         }
  //                 case 10: if(checkFingerprintID()==2)
  //                          {
  //                           strcpy(attendance[0][1], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[0][1], "absent");
  //                          }
  //                 case 11: if(checkFingerprintID()==2)
  //                          {
  //                           strcpy(attendance[0][2], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[0][2], "absent");
  //                          }
  //                 case 13: if(checkFingerprintID()==3)
  //                          {
  //                           strcpy(attendance[0][3], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[0][3], "absent");
  //                          }
  //                 case 14: if(checkFingerprintID()==4)
  //                          {
  //                           strcpy(attendance[0][4], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[0][4], "absent");
  //                          }
  //                 case 15: if(checkFingerprintID()==5)
  //                          {
  //                           strcpy(attendance[0][5], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[0][5], "absent");
  //                          }
  //                 default: InterruptFlag=false;
  //             }
  //     case 2: switch(rtc.hour())
  //             {
  //                 case 9: if(checkFingerprintID()==2)
  //                         {
  //                           strcpy(attendance[1][0], "present");
  //                         }
  //                         else
  //                         {
  //                           strcpy(attendance[1][0], "absent");
  //                         }
  //                 case 10: if(checkFingerprintID()==4)
  //                          {
  //                           strcpy(attendance[1][1], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[1][1], "absent");
  //                          }
  //                 case 11: if(checkFingerprintID()==1)
  //                          {
  //                           strcpy(attendance[1][2], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[1][2], "absent");
  //                          }
  //                 case 13: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[1][3], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[1][3], "absent");
  //                          }
  //                 case 14: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[1][4], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[1][4], "absent");
  //                          }
  //                 case 15: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[1][5], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[1][5], "absent");
  //                          }
  //                 default: InterruptFlag=false;
  //             }
  //     case 3: switch(rtc.hour())
  //             {
  //                 case 9: if(checkFingerprintID()==1)
  //                         {
  //                           strcpy(attendance[2][0], "present");
  //                         }
  //                         else
  //                         {
  //                           strcpy(attendance[2][0], "absent");
  //                         }
  //                 case 10: if(checkFingerprintID()==8)
  //                          {
  //                           strcpy(attendance[2][1], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[2][1], "absent");
  //                          }
  //                 case 11: if(checkFingerprintID()==2)
  //                          {
  //                           strcpy(attendance[2][2], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[2][2], "absent");
  //                          }
  //                 case 13: if(checkFingerprintID()==3)
  //                          {
  //                           strcpy(attendance[2][3], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[2][3], "absent");
  //                          }
  //                 case 14: if(checkFingerprintID()==7)
  //                          {
  //                           strcpy(attendance[2][4], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[2][4], "absent");
  //                          }
  //                 case 15: if(checkFingerprintID()==5)
  //                          {
  //                           strcpy(attendance[2][5], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[2][5], "absent");
  //                          }
  //                 default: InterruptFlag=false;
  //             }
  //     case 4: switch(rtc.hour())
  //             {
  //                 case 9: if(checkFingerprintID()==8)
  //                         {
  //                           strcpy(attendance[3][0], "present");
  //                         }
  //                         else
  //                         {
  //                           strcpy(attendance[3][0], "absent");
  //                         }
  //                 case 10: if(checkFingerprintID()==3)
  //                          {
  //                           strcpy(attendance[3][1], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[3][1], "absent");
  //                          }
  //                 case 11: if(checkFingerprintID()==4)
  //                          {
  //                           strcpy(attendance[3][2], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[3][2], "absent");
  //                          }
  //                 case 13: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[3][3], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[3][3], "absent");
  //                          }
  //                 case 14: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[3][4], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[3][4], "absent");
  //                          }
  //                 case 15: if(checkFingerprintID()==6)
  //                          {
  //                           strcpy(attendance[3][5], "present");
  //                          }
  //                          else
  //                          {
  //                           strcpy(attendance[3][5], "absent");
  //                          }
  //                 default: InterruptFlag=false;
  //             }
  //   }
  // }

  if (InterruptFlag) {
        int day = rtc.now().dayOfTheWeek();
        int hour = rtc.now().hour();
        if (day == 0 || day == 6) 
          return 0;    // if the day is sat or sunday come out of the if condition

        if (hour < 12) {                      // excluding 12pm
            if (checkFingerprintID()== mapping[day-1][hour-9]) {
                strcpy(attendance[day-1][hour-9], "present");
            } else {
                strcpy(attendance[day-1][hour-9], "0");

            }
        } else if (hour > 12) {
            if (checkFingerprintID()== mapping[day-1][hour-10]) {
                strcpy(attendance[day-1][hour-10], "present");
            } else {
                strcpy(attendance[day-1][hour-10], "0");
            }
        }
    }
    return 0;
    
  }
  // put your main code here, to run repeatedly:
  if(checkFingerprintID()==1)
  {
   display.drawLine(centerX - 10, centerY, centerX - 5, centerY + 5, WHITE);
   display.drawLine(centerX - 5, centerY + 5, centerX + 10, centerY - 10, WHITE);
   display.display();

  }
  
  delay(50);

}
void ISR()
{
  InterruptFlag= true;
}
void DisplayTable(const char* subject[], const char* attendance[])
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
  for (int row = 0; row < 2; row++) {
      for (int col = 0; col < 3; col++) {
      int x = col * (rectWidth);
      int y = row * (rectHeight);
      
      display.drawRect(x, y, rectWidth, rectHeight, WHITE);

      // Add tick mark pattern and 'EM' inside one rectangle
      // centerX = x + rectWidth / 2;
      // centerY = y + rectHeight / 2;
        display.setTextSize(1);  // Set text size
        display.setTextColor(WHITE); // Set text color
        display.setCursor(x + rectWidth - 13, y + rectHeight - 10);

      if ((row == 0 && col == 0))
      {       
        display.print(subject[count]);
        
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
      else if ((row == 0 && col == 1))
      {
        display.print(subject[count]);
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
      else if ((row == 0 && col == 2))
      {
        display.print(subject[count]);
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
      else if ((row == 1 && col == 0))
      {
        display.print(subject[count]);
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
      else if ((row == 1 && col == 1))
      {
        display.print(subject[count]);
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
      else if ((row == 1 && col == 2)) 
      {
        display.print(subject[count]);
        if(attendance[count-1]=="present")
        {
        display.drawLine(x + 5, y + 10, x + 10, y + 20, WHITE);
        display.drawLine(x + 10, y + 20, x + 30, y + 5, WHITE);
        }
        else
        {
        display.drawLine(x + 5, y + 4, x + 22, y + 20, WHITE);
        display.drawLine(x + 5, y + 20, x + 22, y + 4, WHITE);
        }count++;
      }
    }
  }
          display.display();

}
 
// Function to check if the scanned fingerprint matches a stored fingerprint and return the ID if a match is found
uint8_t checkFingerprintID() {
  uint8_t p = finger.getImage(); // Capture fingerprint image
  if (p != FINGERPRINT_OK) {
    // Serial.println("Failed to capture fingerprint");
    return 0; // Return 0 to indicate failure
    // Serial.println("E");
  }

  p = finger.image2Tz(); // Convert image to template
  if (p != FINGERPRINT_OK) {
    // Serial.println("Failed to convert image to template");
    return 0; // Return 0 to indicate failure
  }

  // Search for a matching fingerprint template
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    uint16_t matchedID = finger.fingerID; // Get matched fingerprint ID
    // Serial.print("Matched fingerprint ID: ");
    // Serial.println(matchedID);
    return matchedID;
  } else {
    // Serial.println("No matching fingerprint found");
    return 0; // Return 0 to indicate failure
  }
  // else {return 0;}
}








