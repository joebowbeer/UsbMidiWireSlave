/* UsbMidiWireSlave
 * 
 * Reads MIDI messages from USB Host Shield and forwards them to I2C master.
 *  
 * Install this sketch on an Arduino board connected to a USB Host Shield,
 * and install a Wire master on a second device.
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
  Wire.write((size == 3) ? message : emptyMessage, 3);
}

