#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define I2C_ADDR 0x27 // <
#define BACKLIGHT_PIN 3
#define trigger 5
#define echo 6
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
SoftwareSerial mySerial(2, 3); // RX, TX 
EthernetServer server(80);  //server port  
float time=0,distance=0,distance2=0,distancechecc=0,sumdistance=52.00;
int pos = 0; 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 2, 100 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 2, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
String readString,maintext="Stand by";

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  server.begin();  
  lcd.begin (20,4); 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);  
  lcd.home ();
  lcd.setCursor(5,1); 
  lcd.print(maintext); 
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
// Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
            if (readString.indexOf("?check") >0){
               sensor();
               if(sumdistance-1<(distance + distance2)){
                Serial.println("No objects to measure the length.");
                maintext = "No objects to measure the length.";
                lcd.clear();
                lcd.setCursor(0,1); // ไปทตัวอักษรที่ 3 แถวที่ 2
                lcd.print("No objects to"); 
              
                lcd.setCursor(1,2); // ไปที่ตัวอักษรที่ 5 แถวที่ 3
                lcd.print("measure the length.");
                  
               }else{
                maintext = "Width : "+String(sumdistance - (distance + distance2))+" cm";
                Serial.print("sensor1 : ");
                Serial.print(distance);
                Serial.print(" cm  sensor2 : ");
                Serial.print(distance2);
                Serial.print(" cm  Width : ");
                Serial.print(sumdistance - (distance + distance2));
                Serial.println(" cm");
                lcd.clear();
                lcd.setCursor(3,1); // ไปทตัวอักษรที่ 3 แถวที่ 2
                lcd.print("Width : "); 
              
                lcd.setCursor(5,2); // ไปที่ตัวอักษรที่ 5 แถวที่ 3
                lcd.print(String(sumdistance - (distance + distance2))+" cm");  
               }
           }
           if (readString.indexOf("?clear") >0){
                maintext="Stand by";
                lcd.clear();
                lcd.setCursor(5,1); 
                lcd.print(maintext); 
           }
           //clearing string for next read
            readString="";             
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>System measures</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>The system measures the width of objects by ultrasonic.</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>"+maintext+"</H2>");
           client.println("<br />");  
           client.println("<a href=\"/?check\"\">Check</a>");
           client.println("<a href=\"/?clear\"\">Clear</a><br />");   
           client.println("<br />");     
           client.println("<br />");            
           client.println("<p>Created by Engineer MFU for more projects!</p>");  
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           
         }
       }
    }
}

}
void sensor(){
  Serial.print("Check sensor1 :");
  Black:
   for(int i=1;i<=2;i++){ 
     delay(50);    
     digitalWrite(trigger,LOW);
     delayMicroseconds(2);
     digitalWrite(trigger,HIGH);
     delayMicroseconds(10);
     digitalWrite(trigger,LOW);
     delayMicroseconds(2);
     time=pulseIn(echo,HIGH);
     distance=time*340/20000;
     if(i==1){
      distancechecc = distance; 
     }  
     delay(50);
   }
   if(distancechecc-distance>=1||distancechecc-distance<=-1){
    goto Black;
   }
   Serial.println("OK");
   Serial.print("Check sensor2 :");
   Re:
   mySerial.write("R");
   long time_start = millis();
   while(true){
    if(mySerial.available()){
      break;
    }
    if(millis()-time_start>=1000){
      goto Re;
    }
   }
   Serial.println("OK");
   String in = mySerial.readString();
   distance2 = in.toFloat();   
}
