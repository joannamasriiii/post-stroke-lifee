
#include <M5Core2.h>
#include <math.h> 

float accelerationInX = 0.00F;
float accelerationInY = 0.00F;
float accelerationInZ = 0.00F;
float pitch = 0.00F;

RTC_TimeTypeDef RTCtime; //declaring object RTCtime as of type RTC_TimeTypeDef

char timeStorage[64];// an array to store time as a character

//function to get the time
void RealTimeClockTime()
{
  M5.Rtc.GetTime(&RTCtime);  // Gets the time in the real-time clock.
  // Stores real-time time data to timeStorage.
  sprintf(timeStorage, "%02d:%02d:%02d", RTCtime.Hours, RTCtime.Minutes,RTCtime.Seconds);//string print of the time
  }


//function to set the time
void setupTime()
{
  RTCtime.Hours = 0;  // Set the time and write the time set to the real time clock
  RTCtime.Minutes = 0;
  RTCtime.Seconds = 0;
  if (!M5.Rtc.SetTime(&RTCtime)) M5.Lcd.printf("wrong time set!");
  }


//main program
void setup()
{
  Serial.begin(115200);// printing on working space(background)
  M5.begin(); //TurnOn the M5Stack
  M5.IMU.Init(); //Starts the IMU 6 degree of freedom sensor ( accelerometer, angles and gyroscope).
  M5.Lcd.setTextSize(3); //Set the font size to 3.
  M5.Lcd.fillScreen(BLACK); //Set the screen color to BLACK.
  setupTime();//function to set real time 
  M5.Spk.begin();// Initialize the speaker.
  delay(100);
  }

//infinite loop to start the program
void loop()
{
  BackPostureDetection();//calling a function for posture detection
  delay(1000);
  }

//code for different vibrations
void vibration1() 
{
  M5.Axp.SetLDOEnable(3, true);  //Open the vibration.   
  delay(3000);
  M5.Axp.SetLDOEnable(3, false);  //Open the vibration.   
  delay(3000);
  }

void vibration2() 
{
  M5.Axp.SetLDOEnable(3, true);  //Open the vibration.  
  delay(2000);
  M5.Axp.SetLDOEnable(3, false);  //Open the vibration.   
  delay(2000);
  }

 void vibration3() 
 {
  M5.Axp.SetLDOEnable(3, true);  //Open the vibration.   
  delay(1000);
  M5.Axp.SetLDOEnable(3, false);  //Open the vibration.   
  delay(1000);}

//funtion for checking bad posture and inactivity time
void BackPostureDetection()
{
  
  M5.IMU.getAccelData(&accelerationInX,&accelerationInY,&accelerationInZ); //Gets accelerometer data at an interval of one millisecond
  // Formulas to convert accelormeter Data to get rotational angles (Pitch)
  pitch = 180 * atan(accelerationInX / ( sqrt (pow(accelerationInY,2)+pow(accelerationInZ,2))))*M_PI;
  
  Serial.println(pitch);//printing on the work space(background) 
  
  //keeping track of inactivity
  if (pitch <= 50 && pitch >= -50)
  {
    RealTimeClockTime();//calling real time function to keep track of inactivity 
    Serial.print("time: "); //prints on background/workspace
    Serial.println(timeStorage);//print on the background
    if (strcmp(timeStorage,"00:00:07")==0)//condition to prompt user to be active
    {
      setupTime();//resetting the clock
      Serial.println("7 secs");//printing in the background
      M5.Lcd.println("7 secs");//imforming the user how long they are inactive 
      M5.Spk.DingDong();//playing dingdong sound to alert user of inactiveness
      }
      } 



 else if  (!(pitch <= 50 && pitch >= -50))
 {
  //condition to check bad posture
  if ((pitch > 50 && pitch <= 90) || (pitch < -50 && pitch >= -90))
  {
    RealTimeClockTime();//calling time function to give user time allowance to correct posture  
    Serial.print("time: ");//printing in the background 
    Serial.println(timeStorage);//printing in the background
    //condition to check if allowance time is over
    if (strcmp(timeStorage,"00:00:05")==0)
    {
      setupTime();//reset time
      Serial.println("5 secs");
      M5.Lcd.println("5 secs");
      vibration1();//initiating vibration
      }
      }
      
      else if ((pitch > 90 && pitch <= 100) || (pitch < -90 && pitch >= -100))
      {
        RealTimeClockTime();
        Serial.print("time: ");
        Serial.println(timeStorage);
        if (strcmp(timeStorage,"00:00:05")==0)
        {
          setupTime();
          Serial.println("5 secs");
          M5.Lcd.println("5 secs");
          vibration2();
          }
          }
          
          else
          {
            RealTimeClockTime();
            Serial.print("time: ");
            Serial.println(timeStorage);
            if (strcmp(timeStorage,"00:00:05")==0)
            {
              setupTime();
              Serial.println("5 secs");
              M5.Lcd.println("5 secs");
              vibration3();
              }
              }
              
              }
              }
       
        
