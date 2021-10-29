int timer_flag = 0, timer_counter = 0, TIMER_CYCLE = 10;

void setTimer(int duration)
{
  timer_counter = duration / TIMER_CYCLE;
  timer_flag = 0;
}

void timer_run()
{
  timer_counter--;
  if (timer_counter == 0)
    timer_flag = 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  setTimer(10);
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  /*
    We have initial frequency is 16 * 10^6 Hz
    We need time = 10ms --> Frequency = 100Hz
    We use prescaler = 256 and counter = 625.
    Since counter begin at 0-based index so OCR1A = 624
    Prove:
      We have Freq_out = Freq_init / (prescaler * (counter + 1)) = (16 * 10^6) / (256 * (624 + 1)) = 100Hz
  */
  OCR1A = 624;// = (16 * 10^6) / (100 * 256) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 bits for 256 prescaler
  TCCR1B |= 1 << CS12;  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect){
  button_reading();
  timer_run();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (timer_flag == 1)
  {
    digitalWrite(3, !digitalRead(3));
    Serial.println(millis());
    setTimer(1000);
  }
  //delay(10);            // waits 10ms
}
