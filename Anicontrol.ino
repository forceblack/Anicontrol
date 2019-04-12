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

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


USB Usb;

USBH_MIDI  Midi(&Usb);

void MIDI_poll();          //Affichage de signal Midi de L'USB vers le Serial Monitor
void MIDI_sendToCable();    //On va essayer d'envoyer aussi l'info dans le Pin Tx de l'arduino


void setup()
{
  Serial.begin(115200);
  if (Usb.Init() == -1) 
    {
      while (1); //halt
    }
  delay( 200 );
}

void loop()
{
  Usb.Task();
  if ( Midi ) {
    MIDI_poll(); //On affiche (ca sert toujours)
    MIDI_sendToCable(); //On envoie (ca c'est pas encore fini)
  }
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t  rcvd;
  
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) 
  {

    //formatage d'affichage dans le Serial Monitor
    uint32_t time = (uint32_t)millis();
    sprintf(buf, "%04X%04X: ", (uint16_t)(time >> 16), (uint16_t)(time & 0xFFFF)); // Split variable to prevent warnings on the ESP8266 platform
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
  
}
