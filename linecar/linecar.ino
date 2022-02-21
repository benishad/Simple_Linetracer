int Lmotor1 = 5;                //왼쪽 모터 핀 5,6번
int Lmotor2 = 6;
int Rmotor1 = 9;                //오른쪽 모터 핀 9,10번
int Rmotor2 = 10;
int LeftIR = 7;                 //IR센서 핀 왼쪽7, 오른쪽8 번
int RightIR = 8;

int Side_trig = 4;              //측면 초음파센서 핀 4,3 번
int Side_echo = 3;

int Front_trig = 12;            //전면 초음파센서 핀 12,11 번
int Front_echo = 11;

int Motor_speed = 200;          //기본 모터 속도 200          0~255까지------ 수정하는 부분
int Motor_speed2 = 100;         //느려지는 모터 속도 100               ------ 수정하는 부분

int Fdis = 20;                  //전면 거리를 20cm                    ------ 수정하는 부분
int Sdis = 10;                  //측면 거리를 10cm                    ------ 수정하는 부분

int CDS = A0;

float duration;
float distance;
float Fduration;
float Fdistance;

boolean state = false;
boolean state_two = false;

void setup(){
  Serial.begin(9600);
  pinMode(Lmotor1, OUTPUT);
  pinMode(Lmotor2, OUTPUT);
  pinMode(Rmotor1, OUTPUT);
  pinMode(Rmotor2, OUTPUT);
  pinMode(LeftIR, INPUT);
  pinMode(RightIR, INPUT);

  pinMode(Side_trig, OUTPUT);
  pinMode(Side_echo, INPUT);
  pinMode(Front_trig, OUTPUT);
  pinMode(Front_echo, INPUT);

  pinMode(CDS, INPUT);
}

void loop(){
  CDS = analogRead(A0);
  //Serial.print("CDS = ");
  //Serial.println(CDS);                //조도센서값 모니터창에 표시 센서를 가리고 열고 하여 센서값에 최대 변화를 확인
                                      //0에 가까울수록 주변이 밝음  
  frontdistance();
  sidedistance();
  if( CDS < 600){                         //주변이 밝으면 기본 움직임
    Serial.print("주변 밝음 ");
    modeset();
  }
  else{
    Serial.print("어두움 ");
    slow();
  }
  delay(200);
}

void modeset(){
  if(state == true && state_two == true){
    nomove();
    Serial.println("nomove_again");
  }
  else if(state == true){
    nomove();                               //모터정지
    Serial.println("nomove_FOW");
  }
  else if(state_two == true){
    slow();
    Serial.println("SLOW_CAR");
    delay(3000);                            //3초후 기본 속도로 동작
    go();
    Serial.println("Go_FowardCAR");
  }
  else{
    go();
  }
}

void sidedistance(){                       //측면 초음파센서로 거리 측정 코드
  digitalWrite(Side_trig, HIGH);
  delay(10);
  digitalWrite(Side_trig, LOW);
  duration = pulseIn(Side_echo, HIGH);        //pulseIn함수는 (핀, 상태) 이고 핀상태가 바뀌면 그때까지 경과된 시간을 ms단위로 바꿔줌.
  distance = ((float)(340 * duration) / 10000) / 2;    //계산한 거리를 다시 cm로 바꾸는 코드
  
  Serial.print("Sdis = ");
  Serial.println(distance);
  if(distance < Sdis){
    state_two = true;
  }
  else{
    state_two = false;                      //장애물 비감지시 기본 속도로 동작
  }
  delay(100);
}

void frontdistance(){                      //전면 초음파 센서로 거리 측정 코드
  digitalWrite(Front_trig, HIGH);
  delay(10);
  digitalWrite(Front_trig, LOW);
  Fduration = pulseIn(Front_echo, HIGH);        //pulseIn함수는 (핀, 상태) 이고 핀상태가 바뀌면 그때까지 경과된 시간을 ms단위로 바꿔줌.
  Fdistance = ((float)(340 * Fduration) / 10000) / 2;    //계산한 거리를 다시 cm로 바꾸는 코드
  
  Serial.print("Fdis = ");
  Serial.println(Fdistance);
  if(Fdistance < Fdis){
    state = true;
  }
  else{
    state = false;
  }
  delay(100);
}

void go(){
  if(!digitalRead(LeftIR) && !digitalRead(RightIR)){            //왼쪽 오른쪽 둘다 신호 없을때 전진
    forward();
  }
  else if(!digitalRead(LeftIR) && digitalRead(RightIR)){        //오른쪽에 신호 들어오면 좌회전
    turn_left();
  }
  else if(digitalRead(LeftIR) && !digitalRead(RightIR)){        //왼쪽에 신호 들어오면 우회전
    turn_right();
  }
  else if(digitalRead(LeftIR) && digitalRead(RightIR)){         //둘다 신호 들어오면 정지
    nomove();
  }
}

void slow(){
  if(!digitalRead(LeftIR) && !digitalRead(RightIR)){            //둘다 신호 없을때 느린 전진
    forwardslow();
  }
  else if(!digitalRead(LeftIR) && digitalRead(RightIR)){        //오른쪽만 신호있으면 느린 좌회전
    turn_leftslow();
  }
  else if(digitalRead(LeftIR) && !digitalRead(RightIR)){        //왼쪽만 신호 들어오면 느린 우회전
    turn_rightslow();
  }
  else if(digitalRead(LeftIR) && digitalRead(RightIR)){         //둘다 신호 들어오면 정지
    nomove();
  }
}

void forward(){                               //기본 전진
  analogWrite(Lmotor1, Motor_speed);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, Motor_speed);
  analogWrite(Rmotor2, 0);
  Serial.print("빠른전진");
}

void forwardslow(){                             //느린 전진
  analogWrite(Lmotor1, Motor_speed2);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, Motor_speed2);
  analogWrite(Rmotor2, 0);
  Serial.print("느린전진");
}

void back(){                                    //후진 (사용안하지만 일단 넣어놨습니다.)
  analogWrite(Lmotor1, 0);
  analogWrite(Lmotor2, Motor_speed);
  analogWrite(Rmotor1, 0);
  analogWrite(Rmotor2, Motor_speed);
}

void turn_left(){                               //기본 좌회전
  analogWrite(Lmotor1, 0);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, Motor_speed);
  analogWrite(Rmotor2, 0);
  Serial.print("LEFT");
}

void turn_leftslow(){                           //느린 좌회전
  analogWrite(Lmotor1, 0);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, Motor_speed2);
  analogWrite(Rmotor2, 0);
  Serial.print("slow_LEft");
}

void turn_right(){                              //기본 우회전
  analogWrite(Lmotor1, Motor_speed);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, 0);
  analogWrite(Rmotor2, 0);
  Serial.print("RIGHT");
}

void turn_rightslow(){                          //느린 우회전
  analogWrite(Lmotor1, Motor_speed2);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, 0);
  analogWrite(Rmotor2, 0);
  Serial.print("slow_RIGHT");
}

void nomove(){                        //모터 정지
  analogWrite(Lmotor1, 0);
  analogWrite(Lmotor2, 0);
  analogWrite(Rmotor1, 0);
  analogWrite(Rmotor2, 0);
}
