/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */
 
/* This code contains sections from the Udp NTP Client example (available in
 * the Arduino 1.6.6 IDE with esp8266 board v2.0.0 installed), written by
 * Michael Margolis (2010) and modified by Tom Igoe (2012) and
 * Arturo Guadalupi (2015).
 */
 
#include "ntpclient.h"

NTPClient::NTPClient(Configuration& configuration) :
  configuration(configuration), lastPacketSentMillis(0) {
}

void NTPClient::setup() {
  Serial.println(F("# NTPClient::setup: Starting UDP to receive NTP packets"));
  udp.begin(configuration.getNtpLocalPort());
  Serial.printf_P(PSTR("# NTPClient::setup: Local port: %d\n"), udp.localPort());
}

void NTPClient::loop() {
  if((millis() - lastPacketSentMillis) > sendDelay) sendNTPpacket();
  if(packetSent && (millis() - lastPacketSentMillis) < 1000) receiveNTPpacket();
}

boolean NTPClient::isTimeSet() {
  return timeSet;
}

void NTPClient::sendNTPpacket() {
  Serial.println(F("# NTPClient::sendNTPpacket: Sending packet"));
  /* Make sure there are no stale packets in the receive queue*/
  int packetSize;
  do {
    packetSize = udp.parsePacket();
    if (packetSize != 0) {
      udp.read(packetBuffer, ntpPacketSize);
    }
  } while (packetSize);

  /* Get the IP address. */
  IPAddress timeServerIP;
  WiFi.hostByName(configuration.getNtpServerName(), timeServerIP);   
  
  /* Prepare the packet. */
  memset(packetBuffer, 0, ntpPacketSize);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  /* Send the packet. */
  udp.beginPacket(timeServerIP, 123);
  udp.write(packetBuffer, ntpPacketSize);
  udp.endPacket();
  packetSent = true;
  lastPacketSentMillis = millis();
}

void NTPClient::receiveNTPpacket() {
  int packetSize = udp.parsePacket();
  if(packetSize != 0) {
    packetSent = false;
    Serial.println(F("# NTPClient::receiveNTPpacket: Packet received, setting time"));
    udp.read(packetBuffer, ntpPacketSize);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    unsigned long epoch = secsSince1900 - 2208988800UL;      
    setTime(epoch);
    timeSet = true;
    sendDelay = 1000 * 60 * 10;  // every ten minutes
  }
}
