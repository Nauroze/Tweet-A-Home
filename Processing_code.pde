import twitter4j.conf.*;
import twitter4j.*;
import twitter4j.auth.*;
import twitter4j.api.*;
import java.util.*;
import processing.serial.*; 
 
List<Status>statuses = null;
 
TwitterFactory twitterFactory;
Twitter twitter;
Serial myPort;
String LastTweet="";
String ArduinoMsg="x"; // Response from arduino 
String ArduinoTemp="x";

void setup() {     
  size(800, 600);    
  background(0); 
  connectTwitter(); 
  String arduinoPort = Serial.list()[1];
  myPort = new Serial(this, arduinoPort, 9600);
  }  

//displays the last posted tweet in a window 
void draw() {     
  background(0);
  getTweets();
  responseCheck();
  delay(5000);  // Checks twitter every 10 seconds, to avoid exceeding the frame limit , the delay is not made shorter.
  }  
 
 
// Initial connection
void connectTwitter() {  
 
    ConfigurationBuilder cb = new ConfigurationBuilder();  
    cb.setOAuthConsumerKey("npdCfxKDBfS8wNd473pJw");  //Consumer Key
    cb.setOAuthConsumerSecret("TP6MhaSP82nc17TfR8wTUlFm7LOJd5Pq1CoklAnQc0"); //Consumer Secret
    cb.setOAuthAccessToken("2307092941-qpZBBH8camPy33hPYGVDbbG5b2Wy8DIZCzAhU0F");  // Access Token
    cb.setOAuthAccessTokenSecret("A9dqQxbkiL3fPk7qE4LETacI78XRKzyp2Hbi6mV4vg1Ja");  // Access Token Secret
 
    twitterFactory = new TwitterFactory(cb.build());    
    twitter = twitterFactory.getInstance();  
 
    println("connected");
} 

// Method to get tweets
void getTweets() {      
  try {        
    statuses = twitter.getUserTimeline();
      }   
  catch(TwitterException error) {         
    println("Get timeline: " + error + " Status code: " + error.getStatusCode());
  }    
  
   text(statuses.get(0).getUser().getName() + ": " + statuses.get(0).getText(), random(width), random(height), 300, 200);
   LastTweet= statuses.get(0).getText();
 
 
   if (LastTweet.equals("Turn on the lights"))
   { 
     myPort.write('1');         //sends a 1 as a character to compare, as a result turn lights on.
     println("lights is on");   
   } 
   else if (LastTweet.equals("Turn off the lights"))
   {                          
    myPort.write('2');          //sends a 2 as a character to compare, as a result turn lights off.
    println("lights is off");  
   } 
   else if( LastTweet.equals("Turn on the fan"))
   {
     myPort.write('3'); //sends a 3 as a character to compare, as a result turn the fan on high speed.
     println("fan is on");
   }
   else if( LastTweet.equals("Fan on medium"))
   {
     myPort.write('4'); //sends a 4 as a character to compare, as a result turn the fan on medium speed.
     println("fan is on");
   }
   else if( LastTweet.equals("Turn off the fan"))
   {
     myPort.write('5'); //sends a 5 as a character to compare, as a result turn the fan off.
     println("fan is off");
   }
   else if( LastTweet.equals("Turn on the heater"))
   
   {
     myPort.write('6'); //sends a 6 as a character to compare, as a result turn the heater on.
     println("heater is on");
   }
   else if( LastTweet.equals("Turn off the heater"))
   {
     myPort.write('7'); //sends a 7 as a character to compare, as a result turn the heater off.
     println("heater is off");
   }
    else if( LastTweet.equals("Status of the lights"))
   {
     myPort.write('8'); //sends a 8 as a character to compare, as a result checks lights.
     println("light check");
   }
     else if( LastTweet.equals("Status of the fan"))
   {
     myPort.write('9'); //sends a 9 as a character to compare, as a result checks fan.
     println("fan check");
   }
     else if( LastTweet.equals("Status of the heater"))
   {
     myPort.write('0'); //sends a 0 as a character to compare, as a result checks heater.
     println("heater check");
   }
   else if( LastTweet.equals("What is the current temperature?"))
   {
     myPort.write('t'); //sends a 't' as a character to compare, as a result check temperature.
     println("temp check");
   }
   
   delay(1000);
   myPort.write('x');
 
   }  
  
 // Method to post tweets 
 void postTweet(String S)
 {
   try
   {
    Status status = twitter.updateStatus(S);
    println("new tweet --:{ " + status.getText() + " }:--");
   }
    catch(TwitterException e) {
    println("Status Error: " + e + "; statusCode: " + e.getStatusCode());
  } 
 }
 
 // Checks for response from Arduino and posts the relevant tweet
 void responseCheck()
 {
   
    delay(200);   
    ArduinoMsg=myPort.readStringUntil('\n');
    if(ArduinoMsg!=null){
       
        if(!ArduinoMsg.equals(ArduinoTemp)){
          postTweet(ArduinoMsg);
        }
      
        ArduinoTemp=ArduinoMsg;
    }   
}