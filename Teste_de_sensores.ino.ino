//bibliotecas necessárias
#include"AirQuality.h" //Deve baixar no site que se encontra no datasheet do sensor
#include"Arduino.h"
#include "Barometer.h" //Deve baixar no site que se encontra no datasheet do sensor
#include <Wire.h>


//pinos
int pino_luz;
int pino_Temp_Humi = A1;
int pino_UV = A2;
int pino_Qualidade_Ar = A3;

// instacias
AirQuality airqualitysensor;

Barometer myBarometer;



class Barometro : public Barometer{

public:
        void ilustrar();
private:
        double temperature;
        double pressure;
};

void Barometro::ilustrar(){
            temperature = myBarometer.bmp085GetTemperature(myBarometer.bmp085ReadUT());
            pressure = myBarometer.bmp085GetPressure(myBarometer.bmp085ReadUP());
            Serial.print("Temperature: ");
            Serial.print(temperature, 2); //display 2 decimal places
            Serial.println("deg C");

            Serial.print("Pressure: ");
            Serial.print(pressure, 0); //whole number only.
            Serial.println(" Pa");
            Serial.println();  }

class UV{
public:
      void uv(int pino);
private:
      int sensorValue;
      float aux;
};
void UV::uv(int pino){
        aux = 0;
        for(int i=0;i<1024;i++)
        {
            sensorValue = analogRead(pino);
            aux = sensorValue + aux;
            delay(2);
        }
        float meanVal = aux/1024;
        Serial.print("The current UV index is:");
        Serial.print((meanVal*1000/4.3-83)/21);
        Serial.print("\n");
        delay(20);
}


class Qualidade_Ar : public AirQuality{
public:
      void ar();
private:
      int current_quality =-1;

};

void Qualidade_Ar::ar(){
      current_quality = airqualitysensor.slope();
      if (current_quality >= 0){
          if (current_quality==0)
                Serial.println("High pollution! Force signal active");
          else if (current_quality==1)
                Serial.println("High pollution!");
          else if (current_quality==2)
                Serial.println("Low pollution!");
          else if (current_quality ==3)
                Serial.println("Fresh air");  }
  }









void setup()
{
    Serial.begin(9600);
    myBarometer.init();
    airqualitysensor.init(14);
}



void loop(){
          
}
ISR(TIMER1_OVF_vect)
{
	if(airqualitysensor.counter==61)//set 2 seconds as a detected duty
	{

			airqualitysensor.last_vol=airqualitysensor.first_vol;
			airqualitysensor.first_vol=analogRead(pino_Qualidade_Ar);
			airqualitysensor.counter=0;
			airqualitysensor.timer_index=1;
			PORTB=PORTB^0x20;
	}
	else
	{
		airqualitysensor.counter++;
	}


}
