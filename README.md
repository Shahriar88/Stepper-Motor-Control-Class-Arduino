<<< Ramps 1.4 >>>>
// This object is used to control stepper motor, using stepper motor driver
// Validated the code with Arduino Mega 2560 + Ramps 1.4 + HR4988 Motor Driver + Nema 17 motor + 12 Volt DC power supply
// The RAMPS_1-4manual.pdf, RAMPS_1_4_dossier.pdf, Stepper_Driver.jpg files are downloded from different sources
<<>>



<<<Stepper Motor Driver Connection>>>

suppose you use pins as below:

// Aruduino Mega 250 to stepper motor driver
// stepper_shah motor_1(step, enable, dir, step_time_us);


<<>>
stepper_shah motor_1(24, 22, 23, 100);

// pul + to +5Volt
// dir + to +5 VOlt
// Enable - to ground

// pul - controls rotation- step pin Arduino - pin 24
// dir - controls direction- dir pin Arduino - pin 23
//Enable +  inverted Enable- enable pin Arduino - pin 22

<<>>



// Learn how to and set the Dip switches of Stepper Motor driver according to the table on the device

// I supplied 12 Volt DC to the stepper motor driver

