/*
 * Edit by Vincent
 * 2017/4/15
 * PWM I/O test
 */
#define ch1_rc_InputPin A0
#define ch2_rc_InputPin A1
#define ch3_rc_InputPin A2
#define ch4_rc_InputPin A3
#define ch5_rc_InputPin A4
#define ch6_rc_InputPin A5
#define test_channel A0
//how much input channeds to be used
uint8_t maximum_channels = 1;

//table of channel status
// 0 -- unused
// 1 -- setup
// 2 -- timer started (HIGH)
// 3 -- timer finished (LOW)
uint8_t rc_input_channel_status = 1;
uint32_t rc_input_pwm_value_raw;



void setup()
{
  Serial.begin(115200);
  rc_input_channel_status = 1;



}
void loop()
{
  read_rc_input();
  for(uint8_t channel; channel <= maximum_channels; channel++){
    Serial.print("channel ");
    Serial.print(channel);
    Serial.print(":  ");
    Serial.println(rc_input_pwm_value_raw);
  }
  delay(500);
}


void read_rc_input(){
  //time start pwm read
  uint64_t pwm_read_begin;
  //time finish pwm read 
  uint64_t pwm_read_finish;
  //timeout watchdog
  uint64_t timeout_timer = millis();
  //how many channel has not been read
  uint8_t channel_to_read = maximum_channels;
  //whether to quit
  while(channel_to_read > 0){
    if(timeout_timer > (millis()+100)){
      Serial.println("timeout");
    }
    for(uint8_t channel; channel <= maximum_channels; channel++){
      //skip the unused channel
      if(rc_input_channel_status == 0){
        continue;
      }
      //skip the already setup channel
      if(rc_input_channel_status == 2){
        continue;
      }
      /*
      reference table for analog pin
      https://electronics.stackexchange.com/questions/67087/analogread0-or-analogreada0
      static const uint8_t A0 = 14;
      static const uint8_t A1 = 15;
      static const uint8_t A2 = 16;
      static const uint8_t A3 = 17;
      static const uint8_t A4 = 18;
      static const uint8_t A5 = 19;
      static const uint8_t A6 = 20;
      static const uint8_t A7 = 21;
      */
      if(digitalRead(test_channel)== HIGH){
        //timer start!!!!!!!!
        pwm_read_begin = micros();
        //update status to reading
        rc_input_channel_status = 2;
        channel_to_read--;
      }
    }

  }
  //all usable channels are setup

  channel_to_read = maximum_channels;
  while(channel_to_read > 0){
    if(timeout_timer > (millis()+100)){
      Serial.println("timeout");

    }
    for(uint8_t channel; channel <= maximum_channels; channel++){
      //skip the unused channel
      if(rc_input_channel_status == 0){
        continue;
      }
      //skip the already read channel
      if(rc_input_channel_status == 3){
        continue;
      }

      if(digitalRead(test_channel)== LOW){
        //timer start!!!!!!!!
        pwm_read_finish = micros();
        //update status to already read
        rc_input_channel_status = 3;
        channel_to_read--;
      }
    }

  }
    rc_input_pwm_value_raw = pwm_read_begin - pwm_read_finish;


}

