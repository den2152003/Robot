#include "SoftwareSerial.h"
#include <Servo.h> 

SoftwareSerial HC05 (4, 3); //TX, RX
Servo servo1;  // nang
Servo servo2;  // cam
Servo servo3;  // quay

int myRelay = 5;

//low high : di toi
//high low : di lui
int ENA = 6;
//ben phai
int IN1 = 7;
int IN2 = 8;

//ben trai
int IN3 = 9;
int IN4 = 10;
int ENB = 11;
bool mode = false; // false la do line
bool exit_doline = false; // Semaphore để thoát khỏi hàm do line
// Khai báo các chân nối với cảm biến
int sensorPins[5] = {A4, A3, A2, A1, A0};  // 5 chân cảm biến

char dieu_khien;

void setup() {
  HC05.begin(9600);
  Serial.begin(9600);  // Khởi tạo Serial để in dữ liệu

  pinMode(myRelay, OUTPUT);

  servo1.attach(12);     // Gắn servo vào chân số 2
  servo2.attach(13);     // Gắn servo vào chân số 2
  servo3.attach(2);     // Gắn servo vào chân số 2

  // Thiết lập các chân cảm biến là đầu vào

  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);

}

void loop() {
  // Kiểm tra dữ liệu từ Bluetooth
  if (HC05.available() > 0) {
    dieu_khien = HC05.read();
    Serial.println(dieu_khien);  // In lệnh điều khiển nhận được
    Serial.println("------------------------------------------");  // In lệnh điều khiển nhận được
    
    Serial.println(exit_doline);  // In lệnh điều khiển nhận được

    
    if (dieu_khien == 'w') {
      mode = false;  // Chuyển sang chế độ dò line
      exit_doline = false;
    }
    
    if (dieu_khien == 'W') {
      mode = true;   // Chuyển sang chế độ Bluetooth
      exit_doline = true;
    }
  }
  
  // Chọn chế độ hoạt động
  if (!mode) {
    doline();  // Chạy dò line
  } else {
    bluetooth();  // Điều khiển qua Bluetooth
  }
}

void doline()
{
  exit_doline = false; // Reset semaphore khi vào hàm do line
  while(!exit_doline)
  {
    
      // Đọc giá trị từ các cảm biến
    int s1 = digitalRead(sensorPins[0]);  //Left Most Sensor
    int s2 = digitalRead(sensorPins[1]);  //Left Sensor
    int s3 = digitalRead(sensorPins[2]);  //Middle Sensor
    int s4 = digitalRead(sensorPins[3]);  //Right Sensor
    int s5 = digitalRead(sensorPins[4]);  //Right Most Sensor


    //thang
    if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 1) && (s5 == 1))
    {
      analogWrite(ENA,100);
      analogWrite(ENB,100);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    //cua trai it
    if((s1 == 1) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1))
    {
      analogWrite(ENA,110);
      analogWrite(ENB,80);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    //CUA TRAI NHIEU
    if((s1 == 0) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
    {
      analogWrite(ENA,120);
      analogWrite(ENB,80);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    // //CUA PHAI IT
    if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 1))
    {
    analogWrite(ENA, 80);
    analogWrite(ENB, 110);
    digitalWrite(IN1, LOW);  // Chỉnh lại thành LOW
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);  // Chỉnh lại thành HIGH
    digitalWrite(IN4, HIGH);

    }
    //CUA PHAI NHIEU
    if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 0))
    {
    analogWrite(ENA, 80);
    analogWrite(ENB, 120);
    digitalWrite(IN1, HIGH);  // Chỉnh lại thành LOW
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);  // Chỉnh lại thành HIGH
    digitalWrite(IN4, HIGH);

    }
    //CUA TRAI NHIEU X2
    if((s1 == 0) && (s2 == 0) && (s3 == 1) && (s4 == 1) && (s5 == 1))
    {
      analogWrite(ENA,130);
      analogWrite(ENB,80);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    //CUA PHAI NHIEU X2
    if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 0) && (s5 == 0))
    {
      analogWrite(ENA, 80);
      analogWrite(ENB, 130);
      digitalWrite(IN1, HIGH);  // Chỉnh lại thành LOW
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  // Chỉnh lại thành HIGH
      digitalWrite(IN4, HIGH);
    }
    if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 0) && (s5 == 0))
    {
      //stop
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    // CUA TRAI NHIEU X3
    if((s1 == 0) && (s2 == 0) && (s3 == 0) && (s4 == 1) && (s5 == 1))
    {
      analogWrite(ENA,130);
      analogWrite(ENB,0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
    //CUA PHAI NHIEU X3
      if((s1 == 1) && (s2 == 1) && (s3 == 0) && (s4 == 0) && (s5 == 0))
    {
      analogWrite(ENA, 0);
      analogWrite(ENB, 130);
      digitalWrite(IN1, LOW);  // Chỉnh lại thành LOW
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);  // Chỉnh lại thành HIGH
      digitalWrite(IN4, HIGH);
    }

    // //START
    //   if((s1 == 1) && (s2 == 1) && (s3 == 1) && (s4 == 1) && (s5 == 1))
    // {
    //   analogWrite(ENA,60);
    //   analogWrite(ENB,60);
    //   digitalWrite(IN1, LOW);
    //   digitalWrite(IN2, HIGH);
    //   digitalWrite(IN3, LOW);
    //   digitalWrite(IN4, HIGH);
    // }
    //DO LINE BIG

    if (HC05.available() > 0) {
      dieu_khien = HC05.read();
      if (dieu_khien == 'W') {
        exit_doline = true;
        mode = true;
        break;
      }
    }
  }

}

void bluetooth()
{
  analogWrite(ENA, 120);
  analogWrite(ENB, 120);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  while (exit_doline)
  {
    if (HC05.available() > 0) {
      dieu_khien = HC05.read(); // Đọc lệnh điều khiển từ Bluetooth
      if (dieu_khien == 'w') {
        exit_doline = false; // Thoát khỏi chế độ Bluetooth
        mode = false; // Chuyển về chế độ dò line
        break; // Thoát vòng lặp
      }
    }
    
    // Xử lý các lệnh điều khiển
    switch (dieu_khien)
    {
      case 'F': // TIEN
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
      case 'B': // LUI
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 'L': // TRAI
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 'R': // PHAI
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;     
      case 'S': // DUNG
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      //-----u la nag len ha xuong // back light
      case 'U': 
        servo1.write(160);
        break;
      case 'u': 
        servo1.write(35);
        break;
      //------k la gap // F1
      case 'K': 
        servo2.write(90);
        break;
      case 'k': 
        servo2.write(0);
        break;
        //------P la quay // F2
      case 'P': 
        servo3.write(180);
        delay(500);
        servo3.write(90);
        delay(500);
        break;
      case 'p': 
        servo3.write(90);
        break;
//------N la relay // F3
      case 'N': 
        digitalWrite(myRelay, HIGH);  // Bật relay
        break;
      case 'n': 
        digitalWrite(myRelay, LOW);   // Tắt relay
        break;
    }
  }
}
