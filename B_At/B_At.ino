#include <TinyWireM.h>            //I2C library
#include <avr/interrupt.h>        //Interrupt Library
volatile bool data_requested=0;   //Set to 1 if central uC wants a datapoint
int device_address=0x0A;          //I2C address of central uC
bool new_data=0;                  //Set to 1 if attiny has new sensor reading after last transmission
uint8_t data=0;                   //Sensor reading

void setup() {
  cli();                  //desable interrupts
  GIMSK |= (1 << PCIE);   // pin change interrupt enable
  PCMSK |= (1 << PCINT4); // pin change interrupt mask
  sei();                  // enables interrupts
  TinyWireM.begin();
}

void loop() {  
  data++;
  delay(1); //Emulates sensor delay
  new_data=1;
  if(data_requested and new_data){  //transmits only if
    new_data=0;                     //  1)New data is available after last transmission 
    data_requested=0;               //  2)Central uC requested a new transmission
    ADwrite(device_address,data);
    }
}

ISR(PCINT0_vect)  //Gets interrrupted when central uC sends a data request
{
  data_requested=1;
}

void ADwrite(int address,uint8_t data) //I2C write function for one byte
{
    TinyWireM.beginTransmission(device_address);
    TinyWireM.send(address);
    TinyWireM.send(data);
    TinyWireM.endTransmission();
  }
byte ADread(int address) //I2C read function for one byte (not used)
    TinyWireM.beginTransmission(device_address);
    TinyWireM.send(address);
    TinyWireM.endTransmission();
    TinyWireM.requestFrom(device_address, 1);
    return TinyWireM.receive();
  }
