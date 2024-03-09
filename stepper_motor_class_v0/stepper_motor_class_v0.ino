// This object is used to control stepper motor, using stepper motor driver
// Validated the code with Arduino Mega 2560 + Ramps 1.4 + HR4988 Motor Driver + Nema 17 motor + 24 Volt DC power supply
// Start Copy .................................................................................................
class stepper_shah{
  public:
    stepper_shah(int pin_ss, int pin_ee, int pin_dd, int mt_0); // variables step, enable, direction pins & step time
    // functions .................................
    void mtime(int mt_0);
    void mdir(bool d);
    void menable();
    void mdisable();
    void mmove(int step);

  private:
    int pin_s; // Ramps 1.4 ==>>// X 38E 54S 55D;  Y 56E 60S 61D // 1500=180 degree
    int pin_e;
    int pin_d;   
    int mt;
};

stepper_shah::stepper_shah(int pin_ss, int pin_ee, int pin_dd, int mt_0){
  pin_s = pin_ss;
  pin_e = pin_ee;
  pin_d = pin_dd;
  mt = mt_0;

  pinMode(pin_s,OUTPUT);
  pinMode(pin_e,OUTPUT);
  pinMode(pin_d,OUTPUT);

  digitalWrite(pin_e, false);
  digitalWrite(pin_e, false);}

void stepper_shah::mtime(int mt_0){ mt = mt_0;} // set step time
void stepper_shah::mdir(bool d){digitalWrite(pin_d, d);} // select direction
void stepper_shah::menable(){digitalWrite(pin_e, false);} // motor power on
void stepper_shah::mdisable(){digitalWrite(pin_e, true);} // motor power off

void stepper_shah::mmove(int step){ // start motor rotation
      for (int i=0; i<step;i++)   // pull pin HIGH-LOW
      {digitalWrite(pin_s, HIGH);
      delayMicroseconds(mt);
      digitalWrite(pin_s, LOW);
      delayMicroseconds(mt);
        }}

// Select motor pins
stepper_shah motor_1 (60,56,61,700); // pul=60, enable=56, dir=61, step time = 700 higher time-->slower speed // create motor_1 object to control
// Ready to use motor
// You can make many motors: motor_1, motor_2, motor_3, motor_4, motor_5, motor_6 etc.
// END Copy ...................................................................................................................................


void setup(){}

void loop(){
   motor_1.menable(); // motor power on ==> you can not move motor with hand
   motor_1.mdir(false); // change direction
   motor_1.mmove(200); // move 200 step
   delay(200); // wait 2 sec
   motor_1.mdir(true); // change direction
   motor_1.mtime(2500); // change time- slower speed
   motor_1.mmove(200); // move 200 step
   motor_1.mtime(700); // change time- faster speed 
   motor_1.mdisable(); // motor power off ==> you can move the motor with hand
   delay(5000); // wait 5 sec
}