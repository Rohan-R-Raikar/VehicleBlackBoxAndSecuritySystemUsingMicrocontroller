#include <Wire.h>
#include <LiquidCrystal.h>

const int rs = 18, en = 19, d4 = 20, d5 = 21, d6 = 22, d7 = 23;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//------------------------------------Sound & SD card ---------------------

//#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
//------------------------------------------------------------------------------------
//TMRpcm audio;
int file_number = 0;
char filePrefixname[50] = "Blackbox";
char exten[10] = ".wav";

const int recordLed = 17;
const int mic_pin = A2;
const int sample_rate = 16000;
#define SD_CSPin 4
// delay function for with serial log.

//---------------------------gps-------------------------------
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial Serialgsm(10, 11);

static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

String dataMessage;

String Lat,Long;
//------------------------IRsensor-----------------------

#define doorswitch 3
int d;
int flag1 = 0;

//------------------------Alcohol----------------------------------------


const int ledPin = 16;
const int GLED = 17;
const int threshold = 30;
float percentageA;
//-----------------------Ultrasonic --------------------------------
const int trigger = 12;
const int echo = 13;
int distance, duration;
const int trigger1 = 14;
const int echo1 = 15;
int distance1, duration1;

int flag = 0;
//------------------ vibration-----------------------------
int value_vab;
float percentage;
const int vib = A1;

//=============================================================
int analogInputin = A3;
float vout = 0.0;
float Pin = 0.0;
float Pout = 0.0;
float vin = 0.0;
float R1 = 4700.0; // resistance of R1
float R2 = 3000.0; // resistance of R2
int value = 0;
//--------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  Serialgsm.begin(9600);
  ss.begin(GPSBaud);

  pinMode(A0, INPUT);
  pinMode(doorswitch, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(GLED, OUTPUT);

  //pinMode(enA, OUTPUT);
//  pinMode(in1, OUTPUT);
//  pinMode(in2, OUTPUT);

  pinMode (trigger, OUTPUT); //ultrasonic
  pinMode (echo, INPUT);
    pinMode (trigger1, OUTPUT);// ultrasonic
  pinMode (echo1, INPUT);
  pinMode (vib, INPUT);// vibration

  digitalWrite(ledPin, LOW);
  digitalWrite(GLED, HIGH);

 // digitalWrite(enA, LOW);
//  digitalWrite(in2, LOW);
//  digitalWrite(in1, HIGH);
  //----------------------------------------------
  pinMode(mic_pin, INPUT);
  pinMode(recordLed, OUTPUT);

  Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin))
  {
    Serial.println("An Error has occurred while mounting SD");
  }

  while (!SD.begin(SD_CSPin))
  {
    Serial.print(".");
    delay(500);
  }
 // audio.CSPin = SD_CSPin;
  //----------------------------------------------
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Blackbox for ");
  lcd.setCursor(0, 1);
  lcd.print(" Vehicle Safety &");
  lcd.setCursor(0, 2);
  lcd.print("Accident Alert System");
  delay(3000);
}
//=====================================================================================
//void Batteryvoltage()
//{
//
//  value = analogRead(analogInputin);
//  vout = (value * 5.0) / 1024.0;
//  vin = vout / (R2 / (R1 + R2));
//  Serial.print("Battery Voltage=");
//  Serial.println(vin);
//  
//  if (vin < 0.09)
//  {
//    vin = 0.0;  //statement to quash undesired reading !
//  }
//  Pout = (vin * vin) / (R1 + R2);
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Battery Vtg = ");
//  lcd.setCursor(0, 1);
//  lcd.print(vin);
//  lcd.print(" V");
//  delay(4000);
//
//  if (vin >= 12)
//  {
//   
//    Serial.println("Battery Full");
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("  Battery FULL");
//   
//    delay(500);
//    flag=1;
//  } 
//  if (vin < 11)
//  {
//      Serial.println("Battery LOW");
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("  Battery LOW");
//    lcd.setCursor(0, 1);
//    lcd.print("  Please charge");
//    delay(500);
//    alert();
//  }
//
//}
//================================================================
void wait_min(int mins)
{
  int count = 0;
  int secs = mins * 60;
  while (1)
  {
    Serial.print('.');
    delay(1000);
    count++;
    if (count == secs)
    {
      count = 0;
      break;
    }
  }
  Serial.println();
  return ;
}
//===================Alert===========================
void alert()
{
  flag1 = 1;
  digitalWrite(GLED, LOW);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
//============================================================================
void door()
{
  int d = digitalRead(doorswitch);
  Serial.print("d= ");
  Serial.println(d);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Please Wait");
  lcd.setCursor(0, 1);
  lcd.print("Checking Door");
  delay(2000);

  if (d == HIGH)
  {
    Serial.println("Door Open Alert!!!");
    alert();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Please Close or");
    lcd.setCursor(0, 1);
    lcd.print("Check Door");
    delay(2000);
  }
  else
  {
    flag1==0;
    flag=1;
  }
  
}
//===============================================================================
void alcohol()
{
  int a = analogRead(A0);
  Serial.print("a= ");
  Serial.println(a);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Please Wait");
  lcd.setCursor(0, 1);
  lcd.print("Testing Alcohol");
  delay(2000);
  percentageA = map(a, 2, 1023, 0,100); // map the 10-bit data to 8-bit data
  if (percentageA > threshold)
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Alcohol Detect ");
    lcd.setCursor(0, 2);
    lcd.print("   is HIGH");
    delay(1500);

      Serial.println("Alcohol Detected Engine OFF ");
    alert();

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" NOT Safe for ");
    lcd.setCursor(0, 2);
    lcd.print("  Driving ");
    delay(2500);

  }
  if (percentageA < threshold && flag1 != HIGH)
  {
    digitalWrite(GLED, HIGH);
    Serial.println("Door close & Alcohol NOT Detected Engine ON ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Door close");
    lcd.setCursor(0, 1);
    lcd.print("  NO Alcohol");
    lcd.setCursor(0, 2);
    lcd.print("  Detected");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Safe to Drive");
      delay(2500);
    flag = 1;
    
  }
}
//--------------------------------Ultrasonic------------------------------------------

void obstacle() // ultrasonic
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(echo, HIGH);
  distance = duration / 58.2;
  Serial.println("Check distance");
  Serial.println(distance);
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Please Wait");
  lcd.setCursor(0, 1);
  lcd.print("  Checking for ");
  lcd.setCursor(0, 2);
  lcd.print(" vehicle distance");
  delay(3000);
  reverse_obstacle();
  if (distance >= 30  && distance1>=30)
  {
  //  analogWrite(enA, HIGH);
    Serial.println("distance OK");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Distance OK");
    lcd.setCursor(0, 2);
    lcd.print("  Engine ON");
    delay(3000);
    flag = 1;

  }

  else if (( distance < 30 && distance >= 15)  &&( distance1< 30 && distance1 >= 15))
  {
   
    Serial.println("Distance is Medium");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Distance Medium");
    lcd.setCursor(0, 2);
    lcd.print(" Engine Slow");
    delay(3000);
    flag = 1;
  }

  else if (15 > distance && 15>distance1)
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Obstcale Detect");
    lcd.setCursor(0, 2);
    lcd.print("  Engine OFF");
    delay(3000);
    alert();
  }
}
void reverse_obstacle() // ultrasonic
{
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(echo1, HIGH);
  distance1 = duration1 / 58.2;
  Serial.println("Check distance1");
  Serial.println(distance1);
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Please Wait");
  lcd.setCursor(0, 1);
  lcd.print("  Checking for ");
  lcd.setCursor(0, 2);
  lcd.print(" reverse distance");
  delay(3000);
 }
//--------------------------------------vibration------------------------------------------

void vibration()
{
  int value_vab = analogRead(vib);
  float  percentage = map(value_vab, 270, 530, 0, 100);
  Serial.print("value_vab");
  Serial.println(value_vab);
  Serial.print("percentage");
  Serial.println(percentage);

  if (percentage <= 30)
  {
    Serial.println("Vibration Level OK");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Vibration Level ");
    lcd.setCursor(0, 2);
    lcd.print("   is  LOW");
    delay(3000);
    flag = 1;
  }

  else if (30 < percentage && percentage < 60)
  {
   
    Serial.println("Vibration Level medium");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(" Vibration Detect ");
    lcd.setCursor(0, 2);
    lcd.print(" Vehicle Speed LOW");
    delay(3000);
    flag = 1;
  }

  else if (60 <= percentage)
  {
    Serial.println("Vibration Level high");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  Vibration HIGH ");
    lcd.setCursor(0, 2);
    lcd.print("  Accident Alart");
    delay(3000);
    alert();
    gsm();
  }
}
//-----------------------------------------------GPS-------------------------------------------------
void gps_location()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.location.isUpdated())
    {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);

      Lat = (gps.location.lat(), 6);
      Long = (gps.location.lng(), 6);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Latitude = ");
      lcd.setCursor(0, 1);
      lcd.print(Lat);
      lcd.setCursor(0, 2);
      lcd.print(" Longitude = ");
      lcd.setCursor(2, 3);
      lcd.print(Long);
      delay(3000);
    }
  }
}
//----------------------------------------------GSM------------------------------------------------------

void gsm()
{
  gps_location();

  Serialgsm.print("AT");  //Start Configuring GSM Module
  delay(1000);         //One second delay
  Serialgsm.println();
  Serialgsm.println("AT+CMGF=1");  // Set GSM in text mode
  delay(1000);                  // One second delay
  Serialgsm.println();
  Serialgsm.print("AT+CMGS=");     // Enter the receiver number
  Serialgsm.print("\"+91**********\"");
  Serialgsm.println();
  delay(1000);
  Serialgsm.print("Accident Alart SMS"); // SMS body - Sms Text
  dataMessage = ("Latitute: " + String(Lat) + "  " + "Longitute: " + String(Long) + " Accident Alert !, Need Help ");
  // Send the SMS text message
  Serialgsm.print(dataMessage);
  delay(1000);
  Serialgsm.println();
  Serialgsm.write(26);                //CTRL+Z Command to send text and end session
  //while(1);                        //Just send the text ones and halt
}
//----------------------------------------Record Sound---------------------------------------
//void record()
//{
//  Serial.println("############################################");
//  char fileSlNum[20] = "";
//  itoa(file_number, fileSlNum, 10);
//  char file_name[50] = "";
//  strcat(file_name, filePrefixname);
//  strcat(file_name, fileSlNum);
//  strcat(file_name, exten);
//  Serial.print("New File Name: ");
//  Serial.println(file_name);
//  digitalWrite(recordLed, HIGH);
//  audio.startRecording(file_name, sample_rate, mic_pin);
  //Serial.println("startRecording ");
  // record audio for 2mins. means , in this loop process record 2mins of audio.
  // if you need more time duration recording audio then
  // pass higher value into the wait_min(int mins) function.
//  wait_min(2);
//  digitalWrite(recordLed, LOW);
//  audio.stopRecording(file_name);
//  Serial.println("stopRecording");
//  file_number++;
//  Serial.println("#########################################################");
//}

//void startRecording() {
//  char fileName[] = "recording.wav";
//
//  recordingFile = SD.open(fileName, FILE_WRITE);
//
//  if (recordingFile) {
//    Serial.println("Recording started.");
//  } else {
//    Serial.println("Error starting recording.");
//  }
//}
//
//void stopRecording() {
//  if (recordingFile) {
//    recordingFile.close();
//    Serial.println("Recording stopped.");
//  }
//}
//=====================================================================================================

void loop()
{
  if (flag1 == 0)
  {
    //Batteryvoltage();
    door();
    alcohol();
    obstacle();
    reverse_obstacle();
    vibration();
    if(flag==1)
  {
   // record();
  }
  }
  if (flag1 == 1)
  {
    Serial.println("Alert !!!");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  Alert Please ");
    lcd.setCursor(0, 2);
    lcd.print("  Check All Sensors");
    delay(3000);
  }

}
