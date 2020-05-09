///----------------------------------------------------------------------------
/// JOYSTICK
//     PRESS: đổi chiều quay thuận/nghịch kim đồng hồ
//     UP/DOWN: thay doi toc do quay
//     RIGHT (giữ tầm 2 giây): Chế độ quay lien tuc
//     LEFT (giữ tầm 2 giây): Chế độ quay rồi đợi serial nhận dược dữ liệu, hoặc là timeout trong 2 giây rồi quay tiếp
//                          : Gạt và giữ nhiều lần để thay đổi độ phân ly của góc quay 
//  Liên tục gửi ra serial số thứ tự của bức ảnh.
///----------------------------------------------------------------------------


//Thêm thư viện LiquitCrystal - nó có sẵn vì vậy bạn không cần cài thêm gì cả
#include <LiquidCrystal.h>

// Arduino stepper motor control code
#include <Stepper.h> // Include the header file
// change this to the number of steps on your motor
#define STEPS 32

// create an instance of the stepper class using the steps and pins
Stepper stepper(STEPS, 8, 10, 9, 11);  // ứng với chân IN1, IN2, IN3, In4 trên board ULN2003
int velocity; // Toc do quay

//Khởi tạo với các chân
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

//Khai báo joystick
#define joySW A0
#define joyX A1
#define joyY A2
int swValue;
int xValue;
int yValue;

// Chỉ số mỗi step
int Index = 0;

// Đợi để chụp xong ảnh rồi chạy tiếp
int WaitForAWhile = 0;
// Số bước quay mỗi khi đợi
int AngularStep = 2;

// Huong chuyen dong thuan/nghich chieu kim dong ho
byte dir = 0;

//continuous/waitforserial
byte mode= 0;
void setup() {
  Serial.begin(9600);
  //Thông báo đây là LCD 1602
  lcd.begin(16, 2);
  lcd.setCursor(0, 1);
  lcd.print("                                 ");
  // đặt con trỏ vào cột 0, dòng 1
  // Lưu ý: dòng 1 là dòng thứ 2, lòng 0 là dòng thứ 1. Nôm na, nó đếm từ 0 từ không phải từ 1
  lcd.setCursor(0, 0);
  lcd.print("Chao ban");

  lcd.setCursor(0, 1);
  lcd.print("NguyenThanhNgan");

  //Joystick
  pinMode(A0, INPUT);

  velocity = 512;
  stepper.setSpeed(velocity);

  //Thiet lap che do hoat dong
  dir = 1;
  //Thiet lap mode lien tuc
  mode =0;

  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("       Velo=" + String(velocity) + "  ");  
  lcd.setCursor(0, 1);
  lcd.print("Conti.. Sr: ");
}
void loop() {
  if (dir == 1)
  {
    stepper.step(velocity >> AngularStep);
    lcd.setCursor(0, 0);
    lcd.print("Clock  ");
  } else if (dir == 3)
  {
    stepper.step(-velocity >> AngularStep);
    lcd.setCursor(0, 0);
    lcd.print("C.Wise ");
  }
  // Gửi kết quả ra serial để điều khiển chụp ảnh
  if (dir == 1 || dir == 3)
  {
    Index++;
    lcd.setCursor(11, 1);
    lcd.print(String(Index) + "  ");
    Serial.println(Index);
  }


  // Doc phim bam Key Down
  swValue = analogRead(joySW);
  if (swValue < 10)
  {
    //Lay su kien Key Up
    while (swValue < 10)
    {
      swValue = analogRead(joySW);
    }
    dir = dir + 1;
    dir = dir % 4;
  }

  yValue = analogRead(joyY);
  if ( yValue < 10 && velocity < 1024 )
  {
    velocity = velocity + 20;
    stepper.setSpeed(velocity);
    lcd.setCursor(7, 0);
    lcd.print("Velo=" + String(velocity) + "  ");
  }
  if ( yValue > 1000 && velocity > 32 )
  {
    velocity = velocity - 20;

    stepper.setSpeed(velocity);
    lcd.setCursor(7, 0);
    lcd.print("Velo=" + String(velocity) + "  ");
  }

  xValue = analogRead(joyX);
  if ( xValue < 10 )
  {
    mode = 0; //continuous
    lcd.setCursor(0, 1);
    lcd.print("Conti.. ");
    AngularStep = 4;    
  }
  if ( xValue > 1000)
  {
    mode = 1; // Wait for serial
    // Thay doi goc quay
    AngularStep = (AngularStep + 1) % 6;
    lcd.setCursor(0, 1);
    lcd.print("Wait " + String(AngularStep)+" ");        
  }
  if (mode == 1)
  {
     WaitForAWhile=20;
     while (WaitForAWhile > 0) { 
         if (Serial.available()){
            break;
         }
         delay(100);
         WaitForAWhile--;
     }
  }
}
