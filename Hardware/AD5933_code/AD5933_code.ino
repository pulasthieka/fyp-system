#include <Wire.h>
const int device_address = 0x0D;
const unsigned int numberOfIncremrnts = 5;
const unsigned int incrementFrequency = 10000;
const unsigned int settlingCycles = 100; //Must be less than 512
const unsigned int startFrequency = 5000;
unsigned int currentFrequency; // variable to keep track of current frequency after frequency increment
long img = 0;
long real = 0;
void setup()
{
  Wire.begin();
  //  Wire.setClock(400000);
  Serial.begin(115200);
  while (!Serial);         
  Wire.beginTransmission(device_address);
  if (!Wire.endTransmission()) {
    Serial.println("AD5933 found");
  }
  else {
    Serial.println("Error - AD5933 not found");
    while (1) {
      delay(1);
      //will not pass this point if AD5933 is not connected
    }
  }

}


void loop()
{
  //  Give all the frequencies in Hz
  //after executing requestImp(), printImp() can be executed multiple times (as long as you don't exceed numberOfIncrements)
  //Can use checkFrequencySweepOver() function to verify if frequency sweep is over
  requestImp(startFrequency, numberOfIncremrnts, incrementFrequency, settlingCycles);
  waitTillImpReady(); //can also use checkImpReady() to check if measurement is ready
  printImp("W","w"); //Getting the sample from AD5933 and serial printing
  waitTillImpReady();
  printImp("Y","y");
  waitTillImpReady();
  printImp("Z","z");
  waitTillImpReady();
  printImp("A","a");
  waitTillImpReady();
  printImp("C","c");
}

long convertToMachineFrequency(unsigned int frequencyValue) {
  //this function converts Hz to AD5933 compatible frequency unit
  //frequencyValue shound be in Hz
  return ((long) ((frequencyValue / ((16.776 / 4) * 1000000.0)) * pow(2, 27)));
}

void requestImp(unsigned int startFrequency,unsigned int numberOfIncremrnts, unsigned int incrementFrequency, unsigned int settlingCycles) {
  //Input frequencyValue in Hz
  //This function will request AD5933 to sample data
  long machineStartFrequency = convertToMachineFrequency(startFrequency);
  //Splitting to one byte chunks
  int frequencyThree = machineStartFrequency >> 16;
  int frequencyTwo   = (machineStartFrequency >> 8) & 0xFF;
  int frequencyOne   = machineStartFrequency & 0xFF;
  ADwrite(0x80, 0b0); //setting control reg
  ADwrite(0x81, 0b10000); //setting control reg (reset=1, using internal clock)
  //Setting start frequency
  ADwrite(0x82, frequencyThree);
  ADwrite(0x83, frequencyTwo);
  ADwrite(0x84, frequencyOne);
  //Setting increment frequency
  long machineIncrementFrequency = convertToMachineFrequency(incrementFrequency);
  ADwrite(0x85, (machineIncrementFrequency >> 16));
  ADwrite(0x86, ((machineIncrementFrequency >> 8) & 0xFF));
  ADwrite(0x87, (machineIncrementFrequency & 0xFF));
  //Settingnumber of incriments
  ADwrite(0x88, (numberOfIncremrnts >> 8));
  ADwrite(0x89, (numberOfIncremrnts & 0xFF));
  //settling cycles
  ADwrite(0x8A, (settlingCycles >> 8));
  ADwrite(0x8B, (settlingCycles & 0xFF));
  //Taking measurement
  ADwrite(0x80, 0b10110001); //standby
  ADwrite(0x81, 0b00010000); //reset
  ADwrite(0x80, 0b00010001); //initialize
  delayMicroseconds(2); //wait for 2ms to settle down
  currentFrequency=startFrequency;
  ADwrite(0x80, 0b00100001); //start swipe
}

bool checkImpReady(void) {
  //returns True if measurement is ready
  return ((ADread(0x8F) & 0b00000010) == 2);
}
bool checkFrequencySweepOver(void){
  //returns True if sweep is over
  return ((ADread(0x8F) & 0b00000100) == 4);
  }
void waitTillImpReady(void) {
  //waits till AD5933 samples one measurement
  while ((ADread(0x8F) & 0b00000010) != 2 ) {
    delayMicroseconds(1);
  }
}

void printImp(char realChar, char imgChar){
  waitTillImpReady(); //Making sure we don't get invalid data
  real = ADread(0x94) << 7; //looks kinda funny but simple way didn't work (at least in my arduino
  real = real << 1 | ADread(0x95);
  img = ADread(0x96) << 7;
  img = img << 1 | ADread(0x97);
  ADwrite(0x80, 0b00110001); //Increment frequency
//  Serial.println("Frequncy : "+String(currentFrequency)+ "Hz, Real : "+String(real) + ", Imaginary : " + String(img));
Serial.print(realChar+ String(real) + "X"+ imgChar + String(img) + "X");
  currentFrequency+=incrementFrequency;
  }

void ADwrite(int address, int data)
{
  Wire.beginTransmission(device_address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}
byte ADread(int address) //read one byte from address
{
  Wire.beginTransmission(device_address);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(device_address, 1);
  return Wire.read();
}
void CheckStatus() //Ckeck status of circuit(page 26)
{
  Wire.beginTransmission(device_address);
  Wire.write(0x8F);
  Wire.endTransmission();
  Wire.requestFrom(device_address, 1);
  Serial.print("Status = ");
  Serial.println(Wire.read(), BIN);
}
