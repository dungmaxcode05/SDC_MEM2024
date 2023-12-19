// khai báo thư viện 

#include <Adafruit_PWMServoDriver.h>   //thư viện điều khiển servo mạch pca9685
#include <Wire.h>                      // thư viện điều khiển servo 
#include <PS2X_lib.h>


// điều khiển servo 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver() ;     // khởi tạo đối tượng pwm với các giá trị mặc định sau : 
// số chân servo 16 - tần số pwm 50hz - điện áp đầu vao: 5v 


// định nghĩa giá trị tối thiểu và tối đa của tín hiệu pwm cho một servo 
#define servoMin 125 
#define servoMax 575 


//
#define PS2_DAT        13
#define PS2_CMD        12
#define PS2_CLK        11
#define PS2_ATT        10


#define pressures   true 
#define rumble      true

PS2X ps2x ; 




int error = 0;
byte type = 0;
byte vibrate = 0;


int Left_motor=4;    
int Left_motor_pwm=5;    

int Right_motor_pwm=6;  
int Right_motor=7;  
int led1 = 9 ;  

void setup() {
  Serial.begin(9600) ;

   // điều khiển servo 
   pwm.begin() ;
    pwm.setPWMFreq(60); 

  pinMode(4 , OUTPUT) ; 
  pinMode(5 , OUTPUT) ; 
  pinMode(6 , OUTPUT) ; 
  pinMode(7 , OUTPUT) ; 
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
  if(ps2x.Button(PSB_TRIANGLE)) {
 int pulse = angleToPulse(0);
    pwm.setPWM(0, 0, pulse);  
  } else if(ps2x.Button(PSB_CIRCLE)) {
    int pulse = angleToPulse(180);
    pwm.setPWM(0, 0, pulse);
  } else {
     pwm.setPWM(0, 0, 0);
  }

  // điều khiển servo 2 

   // điều khiển servo 2 

  if(ps2x.Button(PSB_CROSS )) {
 int pulse = angleToPulse(0);
    pwm.setPWM(1, 0, pulse);  
  } else if(ps2x.Button(PSB_SQUARE )) {
    int pulse = angleToPulse(190);
    pwm.setPWM(1, 0, pulse);
  } else {
     pwm.setPWM(1, 0, 0);
  }
    // điều khiển servo 3

  if(ps2x.Button(PSB_L1 )) {
 int pulse = angleToPulse(0);
    pwm.setPWM(2, 0, pulse);  
  } else if(ps2x.Button( PSB_L2 )) {
    int pulse = angleToPulse(180);
    pwm.setPWM(2, 0, pulse);
  } else {
     pwm.setPWM(2, 0, 0);
  }

   // điều khiển servo 4

  if(ps2x.Button(PSB_R1 )) {
 int pulse = angleToPulse(0);
    pwm.setPWM(3, 0, pulse);  
  } else if(ps2x.Button(PSB_R2 )) {
    int pulse = angleToPulse(180);
    pwm.setPWM(3, 0, pulse);
  } else {
     pwm.setPWM(3, 0, 0);
  }

   // điều khiển servo 5

  if(ps2x.Button(PSB_L3)) {
 int pulse = angleToPulse(0);
    pwm.setPWM(4, 0, pulse);  
  } else if(ps2x.Button(PSB_R3)) {
    int pulse = angleToPulse(180);
    pwm.setPWM(4, 0, pulse);
  } else {
     pwm.setPWM(4, 0, 0);
  }

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
   int pulse = map(ang,0, 180, servoMin,servoMax);// map angle of 0 to 180 to Servo min and Servo max 
   Serial.print("Angle: ");Serial.print(ang);
   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}

// định hướng xe 

void forward()
{
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
   
  digitalWrite(6, LOW); 
  digitalWrite(7, HIGH);
}

void right()
{
   digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void back()
{
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
}

void left()
{
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
}

void stop()
{
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  digitalWrite(6, LOW);
  digitalWrite(7, LOW); 
}




