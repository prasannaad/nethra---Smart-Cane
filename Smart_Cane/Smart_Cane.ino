#include <Seeed_Arduino_SSCMA.h>

int motorPin = D10; 

SSCMA AI;


TaskHandle_t Task1;
TaskHandle_t Task2;

int aiTarget = 0;

void setup()
{
    AI.begin();
    Serial.begin(9600);
    
    pinMode(motorPin, OUTPUT);

//Task 1 to read from Grove AI module.
  xTaskCreatePinnedToCore(
                    Task1code,   
                    "Task1",     
                    10000,       
                    NULL,        
                    1,           
                    &Task1,      
                    0);                           
  delay(500); 

//Task 2 to control the vibration motor.
  xTaskCreatePinnedToCore(
                    Task2code,   
                    "Task2",     
                    10000,       
                    NULL,        
                    1,           
                    &Task2,     
                    1);          
    delay(500); 

}


void aiInvoke()
{
  if (!AI.invoke())
  {
    Serial.println("invoke success");
    Serial.print("perf: prepocess=");
    Serial.print(AI.perf().prepocess);
    Serial.print(", inference=");
    Serial.print(AI.perf().inference);
    Serial.print(", postpocess=");
    Serial.println(AI.perf().postprocess);

    for (int i = 0; i < AI.boxes().size(); i++)
    {
      Serial.print("Box[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.boxes()[i].target);
      Serial.print(", score=");
      Serial.print(AI.boxes()[i].score);
      Serial.print(", x=");
      Serial.print(AI.boxes()[i].x);
      Serial.print(", y=");
      Serial.print(AI.boxes()[i].y);
      Serial.print(", w=");
      Serial.print(AI.boxes()[i].w);
      Serial.print(", h=");
      Serial.println(AI.boxes()[i].h);
      if (AI.boxes()[i].target == 0 && AI.boxes()[i].score > 80 ) {
      Serial.println("\n Humans detected \n");
      aiTarget = 1;        
     }    
      else if (AI.boxes()[i].target == 1 && AI.boxes()[i].score > 80 ) {
      Serial.println("\n Pothole detected \n"); 
      aiTarget = 2;               
     }     
      else if (AI.boxes()[i].target == 2 && AI.boxes()[i].score > 80 ) {
      Serial.println("\n Dog detected \n");
      aiTarget = 3;        
     }
     else{
      aiTarget = 0;                   
     }
    
    }
    for (int i = 0; i < AI.classes().size(); i++)
    {
      Serial.print("Class[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.classes()[i].target);
      Serial.print(", score=");
      Serial.println(AI.classes()[i].score);
    }
    for (int i = 0; i < AI.points().size(); i++)
    {
      Serial.print("Point[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.points()[i].target);
      Serial.print(", score=");
      Serial.print(AI.points()[i].score);
      Serial.print(", x=");
      Serial.print(AI.points()[i].x);
      Serial.print(", y=");
      Serial.println(AI.points()[i].y);
    }
  }   
}


//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters )
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
  aiInvoke();
  } 
}


//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters )
{
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
  isitTrue();
  }
}

void isitTrue()
{
if(aiTarget == 0)
{
digitalWrite(motorPin, LOW);          
}
if(aiTarget == 1)
{
for(int i=0;i>3;i++)
{
digitalWrite(motorPin, HIGH);          
delay(200);
digitalWrite(motorPin, LOW);          
delay(200);  
}
}
if(aiTarget == 2)
{
for(int i=0;i>5;i++)
{
digitalWrite(motorPin, HIGH);          
delay(200);
digitalWrite(motorPin, LOW);          
delay(200);  
}}
if(aiTarget == 3)
{
digitalWrite(motorPin, HIGH);          
delay(2000);  
digitalWrite(motorPin, LOW);          
}  
}



void loop()
{
}
