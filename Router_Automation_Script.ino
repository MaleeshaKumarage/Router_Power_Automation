/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLlaEy8OxN"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "XIjWEMUB7g32j3eY8MJtvbVhCL8kKaEf"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
const char *ssid     = "Redmi Note 8 Pro";
const char *password = "mal33sha@123";
char *source = "";
int user_Value = 0;
int currentMinute =0;
BlynkTimer timer;


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  user_Value = param.asInt();
  Serial.print(user_Value);
  // Update state
  Blynk.virtualWrite(V1, user_Value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
   Serial.print("Connected");
  digitalWrite(LED_BUILTIN, HIGH);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, password);
  pinMode(5, 1);
  pinMode(4, 0);
  pinMode(12, 0);
  pinMode(14, 0);
  pinMode(13, 0);
  // Connect to Wi-Fi
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(19800);
  
}

void loop()
{
  int curr_Hour = getCurrentDateTime();
  int curr_Volt = getVoltage();
  
//  Serial.print(curr_Hour);
//  Serial.print("   ---    ");
  Serial.println(curr_Volt);
  bool reading1 = digitalRead(5);
  bool reading2 = digitalRead(4);

 
    if(curr_Hour >5 && curr_Hour <16 ){//6,7,8.....14,15
      source = "Solar";
      Blynk.virtualWrite(V6, source);
       //Solar
       pinMode(4, 0);
       pinMode(5, 1);
   
    }else{//16,17,18......3,4,5
        if(curr_Volt<11){
          Serial.print("No Power on Power Pack");
          source = "Using Solar Power due to powercut";
          Blynk.virtualWrite(V6, source);
      //Powerpack
            pinMode(4, 0);
            pinMode(5, 1);  
        }else{
          if(source == "Using Solar Power due to powercut"){
            pinMode(4, 0);
            pinMode(5, 1); 
            if(currentMinute == 00 || currentMinute == 0){
              source = "Solar";
            } 
          }else{
            source = "Powerpack";
            Blynk.virtualWrite(V6, source);
            Serial.print("Power Pack");
            //Powerpack
            pinMode(4, 1);
            pinMode(5, 0);
          }
          
        }
    }

  
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketchres.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

int getCurrentDateTime(){
  timeClient.update();

  time_t epochTime = timeClient.getEpochTime();
  //Serial.print("Epoch Time: ");
  //Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  
  Blynk.virtualWrite(V3, formattedTime);
  int currentHour = timeClient.getHours();
  //Serial.print("Hour: ");
  //Serial.println(currentHour);  

  currentMinute = timeClient.getMinutes();
  //Serial.print("Minutes: ");
  //Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
  //Serial.print("Seconds: ");
  //Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
  //Serial.print("Week Day: ");
  //Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
  //Serial.print("Month day: ");
  //Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  //Serial.print("Month: ");
  //Serial.println(currentMonth);

  String currentMonthName = months[currentMonth-1];
  //Serial.print("Month name: ");
  //Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
  //Serial.print("Year: ");
  //Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  //Serial.print("Current date: ");
  //Serial.println(currentDate);

  //Serial.println("");
  
  return currentHour;
  
  
}

int getVoltage(){
  float R1 = 7501.0;
  float R2 =3002.0;
  float vIn = 0;
  float vOut = 0;
  int volt = analogRead(A0);// read the input
  vOut = ((volt)*5)/1024.0;
  vIn = vOut/(R2/(R1+R2));
  
  
  double voltage = map(volt,0,1023, 0, 1260) + 0;// map 0-1023 to 0-2500 and add correction offset
  Blynk.virtualWrite(V2, vIn);
  
  voltage /=100;// divide by 100 to get the decimal values
  //Serial.print("Voltage: ");
  //Serial.print(voltage);//print the voltge
  //Serial.println("V");
  if(voltage < 11.9 || volt < 780){
    //Serial.println("Low Voltage");
  Serial.print(vIn);//print the voltge
  Serial.println("V");//print the voltge
  }
  delay(500);
  return vIn;
}
