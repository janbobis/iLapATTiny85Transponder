// The code makes use of ATTiny85's internal 8MHz clock.
// No prescaler has been selected and divide counter was 
// set to 17 resulting to ~470KHz Fast PWM output to PIN4
// (by enabling PWM1B in GTCCR). Toggling was done using 
// a 26uS delay causing a 38KHz data mixing with the carrier
// signal (enabling and disabling the Fast PWM)

#define DELAY_MICROSEC  24     // This was supposed to be 26 but due to inaccuracy and some delays using 
                               // internal clock and use of delayMicroseconds function
                               // the value was adjusted to achieve the desired output
void setup() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  TCCR1 = _BV(CTC1) | _BV(CS10);
  GTCCR = _BV(PWM1B);
  OCR1C = 17;
  OCR1B = 7;  // 41% duty cycle
}

void loop() {
  // send code 1234567 (equivalent to 0E AD DC BA 98)
  // for other ILaps equivalent code please see iLapsKeyCode.txt
  sendByte(0x0E);
  sendByte(0xAD);
  sendByte(0xDC);
  sendByte(0xBA);
  sendByte(0x98);
  sendByte(0xD6);
  delay(5);
}

void sendByte(uint8_t data){
  space(DELAY_MICROSEC);   // start bit
  for (uint8_t i = 0; i<8; i++){
    if (data & 1) {
      mark(DELAY_MICROSEC);
    } else {
      space(DELAY_MICROSEC);
    }
    data>>=1;
  }
  mark(DELAY_MICROSEC);   // stop bit
}


void mark(int time){
  GTCCR &= ~(_BV(COM1B1));
  digitalWrite(4, HIGH);
  delayMicroseconds(time-5);
}

void space(int time){
  GTCCR |= _BV(COM1B1);
  delayMicroseconds(time);  
}
