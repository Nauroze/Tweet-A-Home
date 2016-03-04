char val; // Data received from the serial port
// The following variables are used to assign the input and output pins
int lights = 13;
int fan = 11; 
int heater = 7;
int IRpin=8;
int IRsensor=A0;
int value=1;
int state=0;
int tempPin = 1; // Temperature sensor pin
int reading = 0;
float volts = 0;
float temp = 0;

unsigned long time;

String Str1="Current Temperature of the room: "; // Temporary string needed for temperature readings.
String Str2=""; // Temporary string needed for temperature readings.
String Str3=" degree celsius";
// The following strings are used to tweet back to the user.
String LightStr1="Lights are on";
String LightStr0="Lights are off";
String FanStr1="Fan on high mode";
String FanStrMed="Fan on medium mode";
String FanStr0="Fan is off";
String Heater1="Heater is on";
String Heater0="Heater is off";
String LightCheckStr1="Lights are still on";
String LightCheckStr0="Lights are still off";
String FanCheckStr1="Fan is still on";
String FanCheckStr0="Fan is still off";
String IRStr="Intruder at home.";
String Hashtag=" #";
String TempStr="";

void setup() {
   pinMode(lights, OUTPUT); // Set pin as OUTPUT
   pinMode(fan,OUTPUT);
   pinMode(heater, OUTPUT);  
   pinMode(IRpin,OUTPUT);
   pinMode(IRsensor,INPUT); // Set pin as INPUT
   
   Serial.begin(9600); // Start serial communication at 9600 bps 
 }
 
 void loop() {
  
   time=millis();
   time=time/1000;
   String TimeStr= String(time);
   
   if (Serial.available()) 
   { // If data is available to read,
     val = Serial.read(); // read it and store it in val
   }
  
   switch (val) {
   
    case 1:
      digitalWrite(lights, HIGH); // turn the lights on
      TempStr=LightStr1+Hashtag+TimeStr; // adding a time stamp to the string
      Serial.println(TempStr); // printing the string where it will be read by Processing
      break;
	  
    case 2:
      digitalWrite(lights, LOW); // turn lights off
      TempStr=LightStr0+Hashtag+TimeStr;
      Serial.println(TempStr);
      break;
	  
	case 3:
	  digitalWrite(fan, HIGH);
	  TempStr=FanStr1+Hashtag+TimeStr; // turn fan on at high mode
	  Serial.println(TempStr);
    break;	
	  
	case 4:
	  analogWrite(fan, 130); // turn fan on at medium mode, PWM is used here to slow down the fan
	  TempStr=FanStrMed+Hashtag+TimeStr;
	  Serial.println(TempStr);
    break;	
	  
	case 5:
	  digitalWrite(fan,LOW); // turn off the fan
      TempStr=FanStr0+Hashtag+TimeStr;
      Serial.println(TempStr);
    break;	
	
	case 6:
      digitalWrite(heater, HIGH); // turn on the heater
      TempStr=Heater1+Hashtag+TimeStr;
      Serial.println(TempStr);
    break;	
	
	case 7:
      digitalWrite(heater,LOW); // turn off the heater
      TempStr=Heater0+Hashtag+TimeStr;
      Serial.println(TempStr);
    break;
	
	case 8:
      state=digitalRead(lights); // the state of the light is assigned to the variable 'state'
      if (state==1) // if the state is high
      { TempStr=LightCheckStr1+Hashtag+TimeStr;
        Serial.println(TempStr); // string saying the lights are on in printed for Processing to read
      }
      else // if the state is low
      { TempStr=LightCheckStr0+Hashtag+TimeStr;
        Serial.println(TempStr); // string saying the lights are off in printed for Processing to read
      }
    break;
	
	case 9:
	 state=digitalRead(fan);
      if (state==0)
      { TempStr=FanCheckStr1+Hashtag+TimeStr;
        Serial.println(TempStr);
      }
      else
      { TempStr=FanCheckStr0+Hashtag+TimeStr;
        Serial.println(TempStr);
      }
    break;
	
	case 0:
	 state=digitalRead(heater);
      if (state==0)
      { TempStr=Heater1+Hashtag+TimeStr;
        Serial.println(TempStr);
      }
      else
      { TempStr=Heater0+Hashtag+TimeStr;
        Serial.println(TempStr);
      }
	break;
	
	case t:
	 Temp(); // checks the temperature
	break;
	
	IR(); // always checks if the IR signal was cut
     
    delay(1000); // Wait 10 milliseconds for next reading to avoid overloading Twitter servers
    
  }
   

 void IR()
 { 
   time=millis();
   time=time/1000;
   String TimeStr= String(time);
  
   digitalWrite(IRpin, HIGH); // IR transmitter is always on
   value= analogRead(IRsensor); // the value read by the IR receiver is assigned to the variable 'value'
   delay(100);
   
   // when there are no obstacles between the IR transmitter and receiver
   //the value read by the receiver fluctuates around 900
   // when the signal is cut, the value read by the receiver fluctuates between 300 and 400
   
   if(value>200 && value <500) // i.e. if the signal is cut
   {
      TempStr=IRStr+Hashtag+TimeStr;
      Serial.println(TempStr); // print saying that there is an intruder.
   }  
 }

 void Temp()
 {
     reading = analogRead(tempPin); // reads and stores the value from the temperature sensor to the variable 'reading'
     // converting that reading to voltage, for 3.3v arduino use 3.3
     volts = (reading * 3.3 / 1024.0);
     temp = volts/0.016;
     //after the conversion, the following string is printed along with the temperature in degrees celsius and a time stamp
     Serial.print("Current temperature of the room:");
     Serial.print(temp); 
     Serial.print(" degrees C #");
     Serial.println(String(millis()/1000));
     // the value obtained from reading is between 0 - 1023, corresponding 
     // to the analog input voltage at the AIO pin between 0V - 3.3V      
 }
