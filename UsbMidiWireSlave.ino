/* UsbMidiWireSlave
 * 
 * Functions as an I2C slave, reading MIDI messages from a USB Host and
 * writing them to the I2C master.
 *  
 * Install this sketch on an Arduino attached to a USB Host Shield, or an 
 * Arduino Mega ADK, and install a Wire master sketch on a second device.
 */

#include <usbh_midi.h>
//#include <usbhub.h>
#include <Wire.h>

USB Usb;
//USBHub Hub(&Usb);
USBH_MIDI Midi(&Usb);

const uint8_t SLAVE_ADDRESS = 0x42;
const uint8_t emptyMessage[] = {0, 0, 0};

void setup() {
  if (Usb.Init() == -1) {
    while (1); //halt
  }

  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
}

void loop() { /* empty */ }

void requestEvent() {
  uint8_t message[3];
  uint8_t size = 0;

  Usb.Task();
  if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
    size = Midi.RecvData(message);
  }
  Wire.write((size != 0) ? message : emptyMessage, 3);
}

