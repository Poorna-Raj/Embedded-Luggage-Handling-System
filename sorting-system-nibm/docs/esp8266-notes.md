- The **GPIO 10 and 9** are not useable. Eventho they are acceptable GPIO pins to the chip they are reserved by the ESP8266 module for FLASH memory pins.

- The GPIO 0,2,15 are boot sensitive pins. They are usable but need to be in a specific condition at the start as follows,

| Pin    | NodeMCU | GPIO | Required level at boot | Meaning          |
| ------ | ------- | ---- | ---------------------- | ---------------- |
| GPIO0  | D3      | 0    | **HIGH**               | LOW = flash mode |
| GPIO2  | D4      | 2    | **HIGH**               | Must be HIGH     |
| GPIO15 | D8      | 15   | **LOW**                | Must be LOW      |
