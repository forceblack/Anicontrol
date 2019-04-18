/*
 *******************************************************************************
 * USB-MIDI dump utility
 * Copyright (C) 2013-2017 Yuuichi Akagawa
 *
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 *
 * This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too. ! Yeah !
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


//viens du code USB_MIDI_converter
#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif
//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////

USB Usb;

USBH_MIDI  Midi(&Usb);

void MIDI_poll();          //Affichage de signal Midi de L'USB vers le Serial Monitor
void MIDI_sendToCable();    //On va essayer d'envoyer aussi l'info dans le Pin Tx de l'arduino
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime); //ajout delay

void setup()
{

  _MIDI_SERIAL_PORT.begin(31250);
  //attention, je dois choisir si je veux afficher ou si je veux envoyer dans le cable donc soit la ligne du dessus, soit celle du dessous
//  Serial.begin(115200);
  if (Usb.Init() == -1) 
    {
      while (1); //halt
    }
  delay( 200 );
}

void loop()
{
  Usb.Task();
   uint32_t t1 = (uint32_t)micros(); //ajout delay
  if ( Midi ) {
 //   MIDI_poll();        //On affiche (ca sert toujours)
    MIDI_sendToCable(); //On envoie (ca c'est pas encore fini)
  }

    //delay(1ms) a priori, sans ca marche aussi...
  doDelay(t1, (uint32_t)micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI ! Yeah !
void MIDI_poll()
{
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t  rcvd;
  
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) 
  {

    //formatage d'affichage dans le Serial Monitor
    uint32_t time = (uint32_t)millis();
    sprintf(buf, "%04X%04X: ", (uint16_t)(time >> 16), (uint16_t)(time & 0xFFFF)); // Split variable to prevent warnings on the ESP8266 platform ! Yeah ! 
    Serial.print(buf);
    Serial.print(rcvd);
    Serial.print(':');
    for (int i = 0; i < 64; i++) 
      {
        sprintf(buf, " %02X", bufMidi[i]);
        Serial.print(buf);
      }
    Serial.println("");
    //c'etait vraiment tres interessant
    
  }
}

void MIDI_sendToCable()
{
//  Serial.println("Je suis la");
  uint8_t outBuf[ 3 ];
  uint8_t size;

  do {
    if ( (size = Midi.RecvData(outBuf)) > 0 ) {
      //MIDI Output
      _MIDI_SERIAL_PORT.write(outBuf, size);
 //     Serial.println("Je suis dans le do");
    }
  } while (size > 0);
  
}

// Delay time (max 16383 us)
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  t3 = t2 - t1;
  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
