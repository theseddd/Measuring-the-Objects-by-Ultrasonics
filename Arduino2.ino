#include <SoftwareSerial.h>
#define trigger 5
#define echo 6

SoftwareSerial mySerial(2, 3); // RX, TX 
float time=0,distance=0,distancechecc=0;
 
void setup()
{
  mySerial.begin(9600);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
}
 
void loop()
{
   while(!mySerial.find("R"));
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
   mySerial.println(distance,2); 
}
