/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */
 
/* This code contains sections from the Udp NTP Client example (available in
 * the Arduino 1.6.6 IDE with esp8266 board v2.0.0 installed), written by
 * Michael Margolis (2010) and modified by Tom Igoe (2012) and
 * Arturo Guadalupi (2015).
 */
 
#include "ntpclient.h"

NTPClient::NTPClient(Configuration& configuration, WifiClient& wificlient) : 
  configuration(configuration), wificlient(wificlient) {
}

void NTPClient::setup() {
  Serial.println("# NTPClient::setup: Starting UDP to receive NTP packets");
  udp.begin(configuration.getNtpLocalPort());
  Serial.print("# NTPClient::setup: Local port: ");
  Serial.println(udp.localPort());
}

void NTPClient::loop() {
  if(!timeSet && wificlient.isConnected()) {
    if(!firstPacketSent || ((millis() - lastPacketSentMillis) > 1000)) sendNTPpacket();
    receiveNTPpacket();
  }
}

boolean NTPClient::isTimeSet() {
  return timeSet;
}

void NTPClient::sendNTPpacket() {
  Serial.println("# NTPClient::sendNTPpacket: Sending packet");
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
  firstPacketSent = true;
  lastPacketSentMillis = millis();
}

void NTPClient::receiveNTPpacket() {
  int packetSize = udp.parsePacket();
  if(packetSize != 0) {
    Serial.println("# NTPClient::receiveNTPpacket: Packet received, setting time");
    udp.read(packetBuffer, ntpPacketSize);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    unsigned long epoch = secsSince1900 - 2208988800UL;      
    setTime(epoch);
    timeSet = true;
  }
}

