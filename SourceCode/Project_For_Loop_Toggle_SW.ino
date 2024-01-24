#include <LiquidCrystal.h>

// Pins for VT, RR, IE
#define TV_200  PB_3
#define TV_300  PC_4
#define TV_400  PC_5
#define RR_8    PC_6
#define RR_12   PC_7
#define RR_16   PD_6
#define IE_12   PD_7
#define IE_13   PA_4
#define IE_14   PA_3

// Pins for Step & Drirection
#define STEP    PD_0
#define DIR     PD_1
#define LED     PF_2

// Pin for Stop
//#define STOP    PB_5

//Pin for Limit Switch
#define LIMIT   PA_7

// Pins for LCD
#define RS      PB_0
#define RW      PB_1
#define EN      PE_4
#define D4      PE_5
#define D5      PB_4
#define D6      PA_5
#define D7      PA_6

int intermediate_delay = 0;
int forward_delay = 0;
int backward_delay = 0;
int steps = 0;
//String TV = "0";
//String RR = "0";
//String IE = "0:0";

int button_arr[27] = {100100100, 100100010, 100100001, 100010100, 100010010, 100010001, 100001100, 100001010, 100001001,
                       10100100,  10100010,  10100001,  10010100,  10010010,  10010001,  10001100,  10001010,  10001001,
                        1100100,   1100010,   1100001,   1010100,   1010010,   1010001,   1001100,   1001010,   1001001
                     };

int conditions[27][4] = {
  {300, 270,  540, 1500},   /*Condition 1*/                  {150, 270,  810, 1500},   /*Condition 2*/                  { 50, 240,  960, 1500},   /*Condition 3*/
  {300, 420,  840, 1700},   /*Condition 4*/                  {400, 270,  810, 1500},   /*Condition 5*/                  {100, 300, 1200, 1500},   /*Condition 6*/
  {500, 600, 1200, 1500},   /*Condition 7*/                  {100, 600, 1800, 1500},   /*Condition 8*/                  { 50, 475, 1900, 1500},   /*Condition 9*/
  //  {500, 750, 1500, 200},   /*Condition 10*/                 {500, 750, 1500, 200},   /*Condition 11*/                 {500, 750, 1500, 200},   /*Condition 12*/
  //  {500, 750, 1500, 200},   /*Condition 13*/                 {500, 750, 1500, 200},   /*Condition 14*/                 {500, 750, 1500, 200},   /*Condition 15*/
  //  {500, 750, 1500, 200},   /*Condition 16*/                 {500, 750, 1500, 200},   /*Condition 17*/                 {500, 750, 1500, 200},   /*Condition 18*/
  //  {500, 750, 1500, 200},   /*Condition 19*/                 {500, 750, 1500, 200},   /*Condition 20*/                 {500, 750, 1500, 200},   /*Condition 21*/
  //  {500, 750, 1500, 200},   /*Condition 22*/                 {500, 750, 1500, 200},   /*Condition 23*/                 {500, 750, 1500, 200},   /*Condition 24*/
  //  {500, 750, 1500, 200},   /*Condition 25*/                 {500, 750, 1500, 200},   /*Condition 26*/                 {500, 750, 1500, 200}    /*Condition 27*/
};

//char print_conditions[27][3][3];
//
//const char* existingArray[27][3] = {
//  {"200", "8",  "1:2"},   /*Condition 1*/                  {"300", "8",  "1:2"},   /*Condition 2*/                  {"400", "8",  "1:2"},   /*Condition 3*/
//  {"200", "8",  "1:3"},   /*Condition 4*/                  {"300", "8",  "1:3"},   /*Condition 5*/                  {"400", "8",  "1:3"},   /*Condition 6*/
  //  {500, 750, 1500, 200},   /*Condition 7*/                  {500, 750, 1500, 200},   /*Condition 8*/                  {500, 750, 1500, 200},   /*Condition 9*/
  //  {500, 750, 1500, 200},   /*Condition 10*/                 {500, 750, 1500, 200},   /*Condition 11*/                 {500, 750, 1500, 200},   /*Condition 12*/
  //  {500, 750, 1500, 200},   /*Condition 13*/                 {500, 750, 1500, 200},   /*Condition 14*/                 {500, 750, 1500, 200},   /*Condition 15*/
  //  {500, 750, 1500, 200},   /*Condition 16*/                 {500, 750, 1500, 200},   /*Condition 17*/                 {500, 750, 1500, 200},   /*Condition 18*/
  //  {500, 750, 1500, 200},   /*Condition 19*/                 {500, 750, 1500, 200},   /*Condition 20*/                 {500, 750, 1500, 200},   /*Condition 21*/
  //  {500, 750, 1500, 200},   /*Condition 22*/                 {500, 750, 1500, 200},   /*Condition 23*/                 {500, 750, 1500, 200},   /*Condition 24*/
  //  {500, 750, 1500, 200},   /*Condition 25*/                 {500, 750, 1500, 200},   /*Condition 26*/                 {500, 750, 1500, 200}    /*Condition 27*/
//};

LiquidCrystal lcd(RS, RW, EN, D4, D5, D6, D7);

void setup() {
  // Set the step and direction pins as outputs
  pinMode(STEP, OUTPUT);
  pinMode(DIR,  OUTPUT);

//  pinMode(STOP,  INPUT);
  pinMode(LIMIT, INPUT);

  //Set the bottons as inputs
  pinMode(TV_200, INPUT);
  pinMode(TV_300, INPUT);
  pinMode(TV_400, INPUT);
  pinMode(RR_8,   INPUT);
  pinMode(RR_12,  INPUT);
  pinMode(RR_16,  INPUT);
  pinMode(IE_12,  INPUT);
  pinMode(IE_13,  INPUT);
  pinMode(IE_14,  INPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Open Source BVM");
  lcd.setCursor(0, 1);
  lcd.print("Auto Ventilator");
  delay(2500);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = digitalRead(TV_200);
  int h = digitalRead(TV_300);
  int g = digitalRead(TV_400);
  int f = digitalRead(RR_8);
  int e = digitalRead(RR_12);
  int d = digitalRead(RR_16);
  int c = digitalRead(IE_12);
  int b = digitalRead(IE_13);
  int a = digitalRead(IE_14);

  int input_arr[9] = {a, b, c, d, e, f, g, h, i};
  int num[9];
  for (int i = 0; i < 9; i++) {
    if (input_arr[i] == HIGH) {
      num [i] = 1;
    }
    else {
      num[i] = 0;
    }
  }
  int p_condition = 0;
  int mul = 1;
  for (int i = 0; i < 9; i++)
  {
    p_condition += num[i] * mul;
    mul *= 10;
  }

//  for (int i = 0; i < 27; i++) {
//    for (int j = 0; j < 3; j++) {
//      strcpy(print_conditions[i][j], existingArray[i][j]);
//    }
//  }

  for (int i = 0; i < 27; i++) {
    if (p_condition == button_arr[i]) {
      // Assign variable values from the conditions array
      intermediate_delay = conditions[i][0];
      forward_delay = conditions[i][1];
      backward_delay = conditions[i][2];
      steps = conditions[i][3];
//      char* TV = print_conditions[i][0];
//      char* RR = print_conditions[i][1];
//      char* IE = print_conditions[i][2];
      break; // Exit the loop once the condition is found
    }
  }

//  lcd.setCursor(0, 0);
//  lcd.print("VT");
//  lcd.setCursor(0, 1);
//  lcd.print(TV);
//  lcd.setCursor(6, 0);
//  lcd.print("RR");
//  lcd.setCursor(6, 1);
//  lcd.print(RR);
//  lcd.setCursor(12, 0);
//  lcd.print("I:E");
//  lcd.setCursor(12, 1);
//  lcd.print(IE);
//  delay(1000);

  // Set the direction of the motor
  digitalWrite(DIR, HIGH);  // Set the direction to clockwise of Motor
  delay(intermediate_delay);                  // Wait for 1 second

  // Step the motor
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(forward_delay);     // Delay for the step pulse
    digitalWrite(STEP, LOW);
    delayMicroseconds(forward_delay);     // Delay for the step pulse
  }
  delay(intermediate_delay);                  // Wait for 1 second

  // Set the direction of the motor
  digitalWrite(DIR, LOW);  // Set the direction to clockwise of Motor
  delay(intermediate_delay);                 // Wait for 1 second

  // Step the motor
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(backward_delay);     // Delay for the step pulse
    digitalWrite(STEP, LOW);
    delayMicroseconds(backward_delay);     // Delay for the step pulse
  }
  delay(intermediate_delay);
}
