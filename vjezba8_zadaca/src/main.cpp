 // Radila sam skupa s kolegom Antom Turalijom, to jest konzultirali smo se kada bi nam javilo greške, 
 // pa su zadaće slične.
 // Čisto da napomenem da ne biste mislili da smo jedno od drugog prepisivali. :)

#include <Arduino.h>
#include <LowPower.h>
#include "main.h"
#include "Sensors.h"
#include "Radio_nRF.h"

SENSORS sensor;
RADIO radioNRF;

SensorData dataToSend; // definirano za TX
StateType state = READ_SERIAL;

bool rstl; //nova varijabla

void setup()
{
  Serial.begin(115200);
  sensor.DHT_init();
  sensor.BH1750_init();
  /* ######################
  ovdje pozovite funkciju koja instancira radio modul
  #########################
  */
 radioNRF.nRF_init();
}

void loop()
{
  switch (state)
  {
  case READ_SERIAL:

    state = READ_SENSORS;
    break;
  case READ_SENSORS:
    dataToSend.temp = sensor.readTemp();
    dataToSend.lightLevel = sensor.readLight();
    state = RADIO_TX;
    break;
  case RADIO_TX:
    /*#######################
    ovdje pozovite funkciju "RF_send" za slanje podataka preko radio kanala te rezultat pohranite u bool varijablu
    #########################
    */
       bool rslt;
    rslt = radioNRF.RF_send(dataToSend);
    state = RADIO_RX;

    break;
  case RADIO_RX:

    /*#######################
    ovdje pozovite funkciju "RF_receive" koja ispisuje poruku je li kom preko radija bila uspješna
    #########################
    */
  
     radioNRF.RF_receive(rslt);
   

    state = SLEEP_STATE;

    break;

  case SLEEP_STATE:

    delay(50);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    delay(50);

    state = READ_SERIAL;
    break;
  }
}
