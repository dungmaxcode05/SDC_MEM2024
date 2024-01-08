// khai báo thư viện 

#include <Adafruit_PWMServoDriver.h>   //thư viện điều khiển servo mạch pca9685
#include <Wire.h>                      // thư viện điều khiển servo 
#include <PS2X_lib.h>


// điều khiển servo 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver() ;     // khởi tạo đối tượng pwm với các giá trị mặc định sau : 
// số chân servo 16 - tần số pwm 50hz - điện áp đầu vao: 5v 


// định nghĩa giá trị tối thiểu và tối đa của tín hiệu pwm cho một servo 
#define servoMin 125
#define servoMax 475 


//định nghĩa các chân trên arduino sẽ được sử dụng để giao tiếp với mạch điều khiển tay cầm ps2 
#define PS2_DAT        13  
#define PS2_CMD        12
#define PS2_CLK        11
#define PS2_ATT        10

// định nghĩa các tính năng của tay cầm ps2 
#define pressures   true 
#define rumble      true
// khai báo 1 biến có tên ps2x thuộc kiểu PS2X : kiểu PS2X được định nghĩa trong thư viện PS2X_lib.h
PS2X ps2x ; 



// khai báo các biến , biến này sẽ được sử dụng để lưu trữ các giá trị liên quan đến tya cầm ps2 
int error = 0;
byte type = 0;
byte vibrate = 0;

// định nghĩa các chân từ mạch l298n gắn vào mạch arduino để điều khiển motor 
// điều khiển 2 motor 
int LPWM1 = 3 ;
int RPWM1 = 5 ;

int LPWM2 = 6 ;
int RPWM2 = 9 ;



int led1 = 0 ;  

void setup() {
  // thiết lập giao tiếp nối tiếp với tốc độ 9600 baud . Giao tiếp nối tiếp cho phép các thiết bị trao đổi dữ liệu với nhau 
  Serial.begin(9600) ;

   // điều khiển servo 
   pwm.begin() ;  // thiết lập chế độ pwm cho các chân pwm trên arduino 
    pwm.setPWMFreq(60); // thiết lập và bắt đầu chế đô rộng xung(pwm) với tần số 60hz 



  pinMode(LPWM1 , OUTPUT) ; 
  pinMode(RPWM1 , OUTPUT) ; 
  pinMode(LPWM2 , OUTPUT) ; 
  pinMode(RPWM2 , OUTPUT) ; 
  delay(300) ; 
  pinMode(led1 , OUTPUT) ;  

do {
   error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT, pressures, rumble);
   if(error == 0){
      Serial.println("\ncấu hình thành công ");

      break;
    }else{
      Serial.print(".");
      delay(100);
    }
  }while(1);  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Đã tìm thấy bộ điều khiển không các định ");
      break;
    case 1:
      Serial.println("Đã tìm thấy bộ điều khiển DualShock ");
      break;
    case 2:
      Serial.println("đã tìm thấy bộ điều khiển GuitarHero");
      break;
    case 3:
      Serial.println("Đã tìm thấy bộ điều khiển DualShock không dây ");
      break;
   }

      ps2x.read_gamepad(true, 200);  //开启震动
   delay(500);
   ps2x.read_gamepad(false, 200); //开启震动
   delay(300);
   ps2x.read_gamepad(true, 200);  //开启震动
   delay(500);

}

void loop() {
 
   ps2x.read_gamepad(false , vibrate );
    if(ps2x.Button(PSB_START)) {
       ps2x.read_gamepad(true, 200); 
      Serial.println("Bắt đầu đang được tổ chức ") ;
        
    }



    if(ps2x.Button(PSB_PAD_UP))  
    {                 
      Serial.print("hãy giữ vững điều này  ");
      Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
      forward();

    }
     else if(ps2x.ButtonReleased(PSB_PAD_UP)) {
       stop();   
     }
     if(ps2x.Button(PSB_PAD_DOWN)) 
     { 
      Serial.print("DOWN held this hard:") ; 
       Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
       back() ; 
     }
     else if(ps2x.ButtonReleased(PSB_PAD_DOWN)) {
         stop();    
     }
     if(ps2x.Button(PSB_R1)){                        
     // R_Speed = 0;      
     Serial.println("đình dũng đẹp trai ")   ;          
    }
    if(ps2x.Button(PSB_L1)){                         
     // L_Speed = 0;               
    }
    if(ps2x.Button(PSB_PAD_LEFT)) {
      Serial.print("ĐÌnh dũng ") ; 
      Serial.println(ps2x.Analog(PSB_PAD_RIGHT) , DEC);
      left() ; 
    } 
    else if(ps2x.ButtonReleased(PSB_PAD_LEFT)) 
    {
      stop() ; 
    } 
     if(ps2x.Button(PSB_PAD_RIGHT))   
    {      
      Serial.print(" quỳnh anh ") ; 
      Serial.println(ps2x.Analog(PSB_PAD_RIGHT) , DEC);
        right();
    }
    else if(ps2x.ButtonReleased(PSB_PAD_RIGHT)) 
    {   

      stop();     
    }   
 // điều khiển servo 1 
  if(ps2x.Button(PSB_R1)) {
 int pulsei = angleToPulse(180);
 int pulsej = angleToPulse(180);

  int pulseq = angleToPulse(0);
   int pulseu = angleToPulse(0);

    pwm.setPWM(0, 0, pulsej); 
      pwm.setPWM(1, 0, pulsei);
        pwm.setPWM(6, 0, pulseq);    
        pwm.setPWM(5, 0, pulseu);    

  } else if(ps2x.Button(PSB_R2)) {
    int pulsej = angleToPulse(0);
    int pulsei = angleToPulse(0);

     int pulseq = angleToPulse(180);
   int pulseu = angleToPulse(180);

    pwm.setPWM(0, 0, pulsej);
      pwm.setPWM(1, 0, pulsei);  
        pwm.setPWM(6, 0, pulseq);  
        pwm.setPWM(5, 0, pulseu);    


  } else {
     pwm.setPWM(0, 0, 0);
       pwm.setPWM(1, 0, 0);  
        pwm.setPWM(6, 0,0);    
        pwm.setPWM(5, 0,0);    


  }

  // điều khiển servo 2 

   // điều khiển servo 2 

//   if(ps2x.Button(PSB_CROSS )) {
//  int pulse = angleToPulse(0);
//     pwm.setPWM(1, 0, pulse);  
//   } else if(ps2x.Button(PSB_SQUARE )) {
//     int pulse = angleToPulse(180);
//     pwm.setPWM(1, 0, pulse);
//   } else {
//      pwm.setPWM(1, 0, 0);
//   }
    //điều khiển servo 3

  if(ps2x.Button(PSB_L1)) {
 int pulse = angleToPulse(0);
    pwm.setPWM(2, 0, pulse);  
  } else if(ps2x.Button(PSB_L2)) {
    int pulse = angleToPulse(80);
    pwm.setPWM(2, 0, pulse);
  } else {
     pwm.setPWM(2, 0, 0);
  }

   // điều khiển servo 4

//   if(ps2x.Button(PSB_R1 )) {
//  int pulse = angleToPulse(0);
//     pwm.setPWM(3, 0, pulse);  

//   } else if(ps2x.Button(PSB_R2 )) {
//     int pulse = angleToPulse(100);
//     pwm.setPWM(3, 0, pulse);
//   } else {
//      pwm.setPWM(3, 0, 0);
//   }

   // điều khiển servo 5

//   if(ps2x.Button(PSB_L3)) {
//  int pulse = angleToPulse(0);
//     pwm.setPWM(4, 0, pulse);  
//   } else if(ps2x.Button(PSB_R3)) {
//     int pulse = angleToPulse(180);
//     pwm.setPWM(4, 0, pulse);
//   } else {
//      pwm.setPWM(4, 0, 0);
//   }

  //thử nút 
  if(ps2x.Button(PSAB_PAD_RIGHT)) {
    Serial.print("PSAB_PAD_RIGHT") ; 
  }
   if(ps2x.Button(PSAB_PAD_UP)) {
    Serial.print("PSAB_PAD_UP") ; 
  }
   if(ps2x.Button( PSAB_PAD_DOWN)) {
    Serial.print(" PSAB_PAD_DOWN") ; 
  }
   if(ps2x.Button( PSAB_PAD_LEFT)) {
    Serial.print(" PSAB_PAD_LEFT") ; 
  }


  // điều khiển led
   if(ps2x.Button(PSB_SELECT))
      
      {
        digitalWrite(led1 , HIGH) ;
        delay(50);
        digitalWrite(led1 , LOW) ;
        delay(50) ;
      }   else {
        digitalWrite(led1 , LOW) ;
        delay(100) ; 
      }
}
    
 

int angleToPulse(int ang){
   int pulse = map(ang,0, 120, servoMin,servoMax);// map angle of 0 to 180 to Servo min and Servo max 
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}


//




// định hướng xe 
//xe tiến 
void forward()
{
  digitalWrite(LPWM1, LOW);
  digitalWrite(RPWM1, HIGH);
   
  digitalWrite(LPWM2, LOW); 
  digitalWrite(RPWM2, HIGH);
}
// phải 
void right()
{
   digitalWrite(LPWM1, LOW);
  digitalWrite(RPWM1, HIGH);
  
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM2, LOW);
}
//đi lùi 
void back()
{
  digitalWrite(LPWM1, HIGH);
  digitalWrite(RPWM1, LOW);
  
  digitalWrite(LPWM2, HIGH);
  digitalWrite(RPWM2, LOW);
}
// bên trái 
void left()
{
  digitalWrite(LPWM1, LOW);
  digitalWrite(RPWM1, LOW);
  
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM2, HIGH);
}
//dừng lại 
void stop()
{
  digitalWrite(LPWM1, LOW);
  digitalWrite(RPWM1, LOW);
  
  digitalWrite(LPWM2, LOW);
  digitalWrite(RPWM2, LOW); 
}




