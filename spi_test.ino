#include <SPI.h>

//These are Arduino pin values... are they correct?
const int DRV_EN_GATE = 7;
const int DRV_CS = 10;    //slave select: when set low, the device communicates
const int DRV_MOSI = 11;
const int DRV_MISO = 12;
const int DRV_CLK = 13;

void setup() {                                                                                                                                                                                                                                           
  pinMode(DRV_EN_GATE, OUTPUT);

  pinMode(DRV_CLK, OUTPUT);
  pinMode(DRV_MOSI, OUTPUT);
  pinMode(DRV_MISO, INPUT);
  pinMode(DRV_CS, OUTPUT);
  digitalWrite(DRV_CS, HIGH); //disables 8301 on the bus

  
  SPI.begin();  //initialize SPI bus
  SPI.setClockDivider(SPI_CLOCK_DIV128);  //SPI clock = system clock/128
  SPI.setDataMode(SPI_MODE1); //CPOL = 0, CPHA = 1, rising output edge, falling data capture

  Serial.begin(115200);

  //clear
  digitalWrite(DRV_EN_GATE, LOW);
  delay(10);
  digitalWrite(DRV_EN_GATE, HIGH);
  delay(10);

  while(SPIread(0x01) != 0x01)
  {
    Serial.println("DRV init fail");
    delay(500);
  }

   Serial.println("Done with setup");

}

void loop() {

  //to communicate

  //EN_GATE must be high
  
  //call SPI.beginTransaction()
  //write the SS pin LOW
  //call SPI.transfer any number of times
  //set the SS pin HIGH
  //call SPI.endTransaction()

}

uint16_t SPIread(uint8_t addr)
{
  digitalWrite(DRV_CS, LOW);

  delayMicroseconds(50);
  uint8_t d = 1 << 7;
  d |= addr << 3;
  SPI.transfer(d);
  SPI.transfer(0);

  digitalWrite(DRV_CS, HIGH);
  delayMicroseconds(30);
  digitalWrite(DRV_CS, LOW);
  
  d = SPI.transfer(1<<7);
  uint16_t resp = d << 8;
  resp |= SPI.transfer(0);

  digitalWrite(DRV_CS, HIGH);

  return resp & 0x7FF;
}

void SPIwrite(uint8_t addr, uint16_t data)
{
  digitalWrite(DRV_CS, LOW);

  delayMicroseconds(50);
  uint8_t d = data >> 8;
  d |= addr << 3;
  SPI.transfer(d);
  d = data & 0xFF;
  SPI.transfer(d);
    
  digitalWrite(DRV_CS, HIGH);
}
