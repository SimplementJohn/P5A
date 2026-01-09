#define EN      4
#define DIR_A   16
#define STEP_A  17

#define DIR_B   15
#define STEP_B  2


void initMotors()
{
  pinMode(EN,OUTPUT);
  ledcAttachChannel(STEP_A,1000,8,0);
  pinMode(DIR_A,OUTPUT);

  ledcAttachChannel(STEP_B,1000,8,1);
  pinMode(DIR_B,OUTPUT);

  digitalWrite(EN,HIGH);

}

