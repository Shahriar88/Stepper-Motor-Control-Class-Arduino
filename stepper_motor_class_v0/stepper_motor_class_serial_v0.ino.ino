// =====================================================
// =====================================================
// Examples:
// EN
// DIR 0
// TIME 700
// MOVE 200
// DIR 1
// TIME 2500
// MOVE 200
// DIS
// =====================================================
// =====================================================
// __deviceid__ = '4d6420536861687269617220466f72686164'
// __portid__ = '53686168726961723838'

// This object is used to control stepper motor, using stepper motor driver
// Validated the code with Arduino Mega 2560 + Ramps 1.4 + HR4988 Motor Driver + Nema 17 motor + 12 Volt DC power supply
// The RAMPS_1-4manual.pdf, RAMPS_1_4_dossier.pdf, Stepper_Driver.jpg files are downloded from different sources

class stepper_shah{
  public:
    stepper_shah(int pin_ss, int pin_ee, int pin_dd, int mt_0);
    void mtime(int mt_0);
    void mdir(bool d);
    void menable();
    void mdisable();
    void mmove(int step);

  private:
    int pin_s;
    int pin_e;
    int pin_d;
    int mt;
};

stepper_shah::stepper_shah(int pin_ss, int pin_ee, int pin_dd, int mt_0){
  pin_s = pin_ss;
  pin_e = pin_ee;
  pin_d = pin_dd;
  mt = mt_0;

  pinMode(pin_s, OUTPUT);
  pinMode(pin_e, OUTPUT);
  pinMode(pin_d, OUTPUT);

  // NOTE: Your original code wrote pin_e twice.
  // This is likely a typo; typically you'd set enable and direction default states.
  digitalWrite(pin_e, true);   // disabled by default for A4988/HR4988 style (EN=HIGH disables)
  digitalWrite(pin_d, false);  // default direction
}

void stepper_shah::mtime(int mt_0){ mt = mt_0; }
void stepper_shah::mdir(bool d){ digitalWrite(pin_d, d); }
void stepper_shah::menable(){ digitalWrite(pin_e, false); }
void stepper_shah::mdisable(){ digitalWrite(pin_e, true); }

void stepper_shah::mmove(int step){
  for (int i = 0; i < step; i++){
    digitalWrite(pin_s, HIGH);
    delayMicroseconds(mt);
    digitalWrite(pin_s, LOW);
    delayMicroseconds(mt);
  }
}



// =====================================================
// Motor object (your pins)
// Ramps 1.4 example: enable=56, step=60, dir=61
// Your constructor order: (step, enable, dir, step_time)
// =====================================================
// stepper_shah motor_1(step, enable, dir, step_time_us);
// stepper_shah motor_1(24, 22, 23, 100); // Aruduino Mega 250 to stepper motor driver
stepper_shah motor_1(60, 56, 61, 700);


// =====================================================
// Simple Serial command interface
// =====================================================
String line;
long pos_steps = 0;          // software-tracked position
bool dir_state = false;      // last direction we set (0/1)
int step_time_us = 700;

void printHelp(){
  Serial.println(F("\nCommands:"));
  Serial.println(F("  EN                -> enable motor"));
  Serial.println(F("  DIS               -> disable motor"));
  Serial.println(F("  DIR 0|1           -> set direction"));
  Serial.println(F("  TIME <us>         -> set step pulse half-period in microseconds (e.g., 700, 2500)"));
  Serial.println(F("  MOVE <steps>      -> move N steps in current DIR (positive integer)"));
  Serial.println(F("  GOTO <abs_steps>  -> go to absolute position (software)"));
  Serial.println(F("  HOME              -> set software position to 0 (no motion)"));
  Serial.println(F("  STATUS            -> print current settings"));
  Serial.println(F("  HELP              -> print this help\n"));
}

void printStatus(){
  Serial.print(F("EN: "));
  // We can’t directly read enable state from class, so just show intended state messages.
  Serial.print(F("(use EN/DIS to change) | DIR="));
  Serial.print(dir_state ? 1 : 0);
  Serial.print(F(" | TIME(us)="));
  Serial.print(step_time_us);
  Serial.print(F(" | POS(steps)="));
  Serial.println(pos_steps);
}

static inline String upcaseTrim(String s){
  s.trim();
  s.toUpperCase();
  return s;
}

void setup(){
  Serial.begin(115200);
  while(!Serial) { /* for boards with native USB */ }

  Serial.println(F("\nStepper Serial Control Ready."));
  Serial.println(F("Set Serial Monitor: 'Newline' (recommended) and 115200 baud."));
  printHelp();

  // Start disabled by default (already set in constructor)
  motor_1.mdisable();
  motor_1.mdir(dir_state);
  motor_1.mtime(step_time_us);
}

void handleCommand(String cmd){
  cmd = upcaseTrim(cmd);
  if (cmd.length() == 0) return;

  // Split into keyword + rest
  int sp = cmd.indexOf(' ');
  String key = (sp == -1) ? cmd : cmd.substring(0, sp);
  String arg = (sp == -1) ? ""  : cmd.substring(sp + 1);
  arg.trim();

  if (key == "HELP"){
    printHelp();
    return;
  }

  if (key == "EN"){
    motor_1.menable();
    Serial.println(F("Motor enabled."));
    return;
  }

  if (key == "DIS"){
    motor_1.mdisable();
    Serial.println(F("Motor disabled."));
    return;
  }

  if (key == "DIR"){
    if (arg.length() == 0){
      Serial.println(F("Usage: DIR 0 or DIR 1"));
      return;
    }
    int v = arg.toInt();
    dir_state = (v != 0);
    motor_1.mdir(dir_state);
    Serial.print(F("Direction set to "));
    Serial.println(dir_state ? 1 : 0);
    return;
  }

  if (key == "TIME"){
    if (arg.length() == 0){
      Serial.println(F("Usage: TIME <microseconds> (e.g., TIME 700)"));
      return;
    }
    int t = arg.toInt();
    if (t < 50) {
      Serial.println(F("TIME too small. Try >= 50 us (start with 700)."));
      return;
    }
    step_time_us = t;
    motor_1.mtime(step_time_us);
    Serial.print(F("Step time set to "));
    Serial.print(step_time_us);
    Serial.println(F(" us."));
    return;
  }

  if (key == "MOVE"){
    if (arg.length() == 0){
      Serial.println(F("Usage: MOVE <steps> (positive integer)"));
      return;
    }
    long steps = arg.toInt();
    if (steps <= 0){
      Serial.println(F("MOVE needs a positive step count."));
      return;
    }

    Serial.print(F("Moving "));
    Serial.print(steps);
    Serial.println(F(" steps..."));

    motor_1.mmove((int)steps);
    pos_steps += dir_state ? steps : -steps;

    Serial.print(F("Done. POS="));
    Serial.println(pos_steps);
    return;
  }

  if (key == "GOTO"){
    if (arg.length() == 0){
      Serial.println(F("Usage: GOTO <abs_steps>"));
      return;
    }
    long target = arg.toInt();
    long delta = target - pos_steps;
    if (delta == 0){
      Serial.println(F("Already at target (software position)."));
      return;
    }

    bool newDir = (delta > 0);
    long steps = labs(delta);

    dir_state = newDir;
    motor_1.mdir(dir_state);

    Serial.print(F("Going to "));
    Serial.print(target);
    Serial.print(F(" ("));
    Serial.print(steps);
    Serial.println(F(" steps)..."));

    motor_1.mmove((int)steps);
    pos_steps = target;

    Serial.println(F("Done."));
    return;
  }

  if (key == "HOME"){
    pos_steps = 0;
    Serial.println(F("Software position reset to 0 (no motion)."));
    return;
  }

  if (key == "STATUS"){
    printStatus();
    return;
  }

  Serial.print(F("Unknown command: "));
  Serial.println(cmd);
  Serial.println(F("Type HELP"));
}

void loop(){
  while (Serial.available()){
    char c = (char)Serial.read();

    // Build line until newline
    if (c == '\n' || c == '\r'){
      if (line.length() > 0){
        handleCommand(line);
        line = "";
      }
    } else {
      line += c;
      // Basic protection
      if (line.length() > 80) line = "";
    }
  }
}

