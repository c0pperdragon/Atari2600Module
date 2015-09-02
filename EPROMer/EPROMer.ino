#include <avr/pgmspace.h>

const unsigned int address = 0*4096;
const byte code[] PROGMEM = {
#include "yars_revenge_pal.h"
#include "venture_pal.h"
#include "space_invaders_pal.h"
#include "seaquest_pal.h"
};
//const unsigned int address = 4*4096;
//const byte code[] PROGMEM = {
//#include "river_raid_pal.h"
//#include "q-bert_pal.h"
//#include "pitfall_pal.h"
//#include "pac-man_pal.h"
//};
//const unsigned int address = 8*4096;
//const byte code[] PROGMEM = {
//#include "megamania_pal.h"
//#include "keystone_kapers_pal.h"
//#include "enduro_pal.h"
//#include "donkey_kong_pal.h"
//};
//const unsigned int address = 12*4096;
//const byte code[] PROGMEM = {
//#include "demon_attack_pal.h"
//#include "berzerk_pal.h"
//#include "atlantis_pal.h"
//#include "adventure_pal.h"
//};

#define E   2
#define CLK 11
#define AB  12

void setup() 
{
  pinMode(E,OUTPUT);
  digitalWrite(E,HIGH);
  pinMode(CLK,OUTPUT);
  digitalWrite(CLK,HIGH);
  pinMode(AB,OUTPUT);
  digitalWrite(AB,HIGH);

  pinMode(0,OUTPUT);
  digitalWrite(0,HIGH);
  pinMode(1,OUTPUT);
  digitalWrite(1,HIGH);
  int i;
  for (i=0; i<6; i++)
  {  pinMode(A0+i,OUTPUT);
     digitalWrite(A0+i,HIGH);
  }
  
  // let program wait until serial monitor is connected
  Serial.begin(9600);  
}


void loop() 
{
  Serial.print("EPROMer ready with ");
  Serial.print(sizeof(code));
  Serial.print(" bytes for address ");
  Serial.println(address, HEX);
  Serial.println("Valid commands: r(ead), w(rite), z(ero find) c(ompare) t(est adressing)");
  
  int c = Serial.read();
  while (c<0) c = Serial.read();
  if (c=='r')
  { 
    readeprom(false);
  }
  else if (c=='w')  
  {
    writeeprom();
  }  
  else if (c=='z')  
  {
    readeprom(true);
  }  
  else if (c=='c')  
  {
    compareeprom();
  }  
  else if (c=='t')
  { 
    testaddressanddata();
  }
}




void testaddressanddata()
{
  configureoutput();
  writeaddress(0b1000000000000000);
  writedata(0b10101010);    
}

void readeprom(bool showonlyzero)
{
  Serial.print("Reading from address: ");
  Serial.println(address);

  configureinput();
 
  long int totalzeroes = 0;
  long int i;
  for (i=0; i<0x10000; i++)
  {
    if ((i&15)==0 && !showonlyzero) {
        Serial.print(i, HEX);
        Serial.print("  ");
    }

    writeaddress(address+i);
    delayMicroseconds(1);
    digitalWrite(E, LOW);
    delayMicroseconds(1);
    
    byte d = readdata();
    digitalWrite(E, HIGH);
    
    int bit;
    for (bit=0; bit<8; bit++)
    {  
      if ((d&(1<<bit))==0) totalzeroes++;
    }
    
    if (showonlyzero)
    {
      if (d!=0xff)
      {
        Serial.print(i,HEX);  
        Serial.print(" ");
        Serial.println(d,HEX);
      }
    }
    else
    {  if (d<16) Serial.print("0");
      Serial.print(d,HEX);  
      Serial.print(" ");
      if ((i&15)==15)  Serial.print("\n");
    }
  }
  
  Serial.print("Total number of zero bits: ");
  Serial.println(totalzeroes);
}


void writeeprom()
{
  configureoutput();
  
  int len = sizeof(code);
  
  Serial.print("Programming ");
  Serial.print(len);
  Serial.print(" bytes at address ");
  Serial.println(address);

  unsigned int i;
  for (i=0; i<len; i++)
  {
    byte d = pgm_read_byte_near(code+i);

    unsigned int a = address+i;

    writeaddress(a);
    writedata(d);
    delayMicroseconds(10);
    
    int tries;
    for (tries=0; tries<50; tries++)
    { digitalWrite(E,LOW);
      delayMicroseconds(100);
      digitalWrite(E,HIGH);
      delayMicroseconds(50);
    }
    
    if (d<16) Serial.print("0");
    Serial.print(d,HEX);  
    Serial.print(" ");
    if ((i&15)==15)  Serial.print("\n");
  }
  
}


void compareeprom()
{
  configureinput();
  
  int len = sizeof(code);
  
  Serial.print("Comparing ");
  Serial.print(len);
  Serial.print(" bytes at address ");
  Serial.println(address);

  unsigned counterrors=0;
  unsigned int i;
  for (i=0; i<len; i++)
  {
    byte d = pgm_read_byte_near(code+i);

    unsigned int a = address+i;

    writeaddress(a);
    delayMicroseconds(1);
    digitalWrite(E, LOW);
    delayMicroseconds(1);    
    byte x = readdata();
    digitalWrite(E, HIGH);
    
    if (d!=x)
    {
      Serial.print("Difference at position ");      
      Serial.print(i);
      Serial.print(" (address:");
      Serial.print(a,HEX);
      Serial.print(") ");
      Serial.print(" found ");
      Serial.print(x,HEX);
      Serial.print(" instead of ");
      Serial.println(d,HEX);
      counterrors++;
    }
  }
  Serial.print("Found ");
  Serial.print(counterrors);
  Serial.println(" differences");
}




void configureoutput()
{
  int i;
  for (i=0; i<8; i++)
  {   pinMode(10-i,OUTPUT);
      digitalWrite(10-i,HIGH);
  }  
}

void configureinput()
{
  int i;
  for (i=0; i<8; i++)
  {   pinMode(10-i,INPUT);
  }  
}


// pump out bits of address serially into the shift
// registers (lowest bit comes last)
void writeaddress(unsigned int a)
{  
  int i;
  
  // two highest bits of address  
  for (i=0; i<2; i++)  
  {
    digitalWrite(1-i, ((a&0x8000) == 0) ? LOW:HIGH);  
    a = a<<1;   // continue with next bit
  }
  // next 6 bits bits of address  
  for (i=0; i<6; i++)
  {
    digitalWrite(A5-i, ((a&0x8000) == 0) ? LOW:HIGH);  
    a = a<<1;   // continue with next bit
  }
  
  // lowest 8 bit of address will be clocked into shift register
  for (i=0; i<8; i++)  
  { digitalWrite(CLK, LOW);           // prepare low-to-high transition of clock  
    digitalWrite(AB, ((a&0x8000) == 0) ? LOW:HIGH);  // put next bit to shifter input
    delayMicroseconds(1);            // let output stabilize
    digitalWrite(CLK, HIGH);          // issue clock  to shifter
    delayMicroseconds(1);            // let shifter to their work  
    a = a<<1;   // continue with next bit
  }
  digitalWrite(AB, HIGH);  // remove data from pin    
}

// put output data to the arduino pins 10 - 3 (which map to EPROM Q0 - Q7)
void writedata(unsigned int d)
{
  for (int i=0; i<8; i++)
  {  digitalWrite(10-i,((d&(1<<i))==0) ? LOW:HIGH);
  }
}

// read data from the arduino pins 10 - 3 (which map to EPROM Q0 - Q7)
byte readdata()
{
  byte d = 0;
  int i;
  for (i=0; i<8; i++)
  {
    if (digitalRead(10-i)==HIGH)
    {
      d = d | (1<<i);
    }
  }
  return d;
}






