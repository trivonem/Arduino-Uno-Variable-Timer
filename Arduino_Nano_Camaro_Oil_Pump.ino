//Digital pin 13 is the input pin from Mosfet to let Arduino know to turn the pump on
//Digital Pin 2 is the Output PIn from Arduino to Mosfet to turn on Relay for Pumps
int trigger= A0; //Analog input pin to determine if pump needs to be on
#define Psignal 2 // GPIO TO turn pump on and off
int Pump= LOW;//Pump Status for Arduino
unsigned long Main_time; //Keep tracks of system time
unsigned long timer1; //Variable to track of time with the pumps
unsigned long timer2; //2nd Time keeping Variable
unsigned long timer3; //3rd Time keeping Variable
unsigned long timer4; //4th Time keeping Variable
int delay_off; //Time length of how long turbo oil pump will run
int Status= LOW; //Status of Delay function if Delay is active ongoing value equals HIGH
void setup() {
  //Setting up pins for functions
  pinMode(trigger, INPUT); //Creating the input Trigger pin from the Car to Arduino
  pinMode(Psignal, OUTPUT);//Creating the Output Signal wire to turn pump on
  digitalWrite(Psignal,LOW); //Intially turning Pump off
  Pump=LOW; //Letting Arduino know pump is off in beginining
  Serial.begin(9600);
  Serial.println("Starting up");
}
void loop() {
  Main_time=millis();
  Serial.println(Main_time);
  //Turn Pumps on When Car is on
  if (digitalRead(trigger)== HIGH&&Pump==LOW){
    pumpon();
    timer1 = millis();
    Serial.println("Task1: PUMP ON");
  }
  //Adds the delay to pump if Pump is already on and running
  if (analogRead(trigger)==0&&Pump==HIGH&&Status==LOW){
    timer2 = millis();
    delay_off=time_checker(timer1,timer2);
    turbotimer(delay_off);
    Serial.println("Task 2 Setting up Delay");   
  }
  //Turns the Pump off after Delay condition met
  if (Main_time>=timer4&&Status==HIGH){
    pumpoff();
    Status=LOW;
    Serial.println("Task 3 Delay Over Pump OFf");
  }
/*  if (Main_time>=timer4){
    if(Status==HIGH){
      pumpoff();
      Status=LOW;
      Serial.println("Task 3 Delay over Pump turn off");
    }
    else if(Status=LOW){
      
    }
  }*/

}
//Turn Pump on function
void pumpon(){
  digitalWrite(Psignal,HIGH);
  Pump=HIGH; //Letting Arduino Know Pump is on
  Status=LOW;
  Serial.println("Pump on");
}
//Turn pump off function
void pumpoff(){
  digitalWrite(Psignal,LOW);
  Pump=LOW; //Letting Arduin know pump if off
  Serial.println("Pump off");
}
//Functions to determine how long the delay is
int time_checker(unsigned long t, unsigned long e){
  unsigned long value =e-t;
  int change;
  Serial.println("Time checker value:");
  Serial.print(value);
  if (value<= 15000){//ENgine for 15 seconds
      change =0;
    }
  else if(value>15001&&value<=60000){//Engine on between 15s and 1 minute 
      change=1;
    }
  else if(value>60000&&value<=300000){//Engine on between 1m and 5m
      change=2;
    }
  else if(value>300000&&value<=1200000){//Engine on between 5m and 20m
      change=3;
  }else{
      change=4;
  }
  Serial.println("Time Checker:");
  Serial.println(change);
  return change;
}
void turbotimer(int m){
   timer3=millis();
   switch(m){
    case 0://Immediately turns off Pumps
      timer4=0;
      Status=HIGH;
      break;
    case 1: //Turns pumps off after 30 seconds
      timer4=timer3+30000;
      Status=HIGH;
      break;
    case 2: //Turns pumps off after 2 minutes
      timer4=timer3+120000;
      Status=HIGH;
      break;
    case 3: //Turns pumps off after 5 minutes
      timer4=timer3+300000;
      Status=HIGH;
      break;
    case 4: //Turns pumps off after 10 minutes
      timer4=timer3+600000;
      Status=HIGH;
      break;
   }
   Serial.println("Turbo timer: Timer active");
}
