// GitHub Account:     GitHub.com/AliRezaJoodi

// LCD module connections
sbit LCD_RS at RB6_bit;
sbit LCD_EN at RB4_bit;
sbit LCD_D4 at RB3_bit;
sbit LCD_D5 at RB2_bit;
sbit LCD_D6 at RB1_bit;
sbit LCD_D7 at RB0_bit;

sbit LCD_RS_Direction at TRISB6_bit;
sbit LCD_EN_Direction at TRISB4_bit;
sbit LCD_D4_Direction at TRISB3_bit;
sbit LCD_D5_Direction at TRISB2_bit;
sbit LCD_D6_Direction at TRISB1_bit;
sbit LCD_D7_Direction at TRISB0_bit;
// End LCD module connections

sbit Sound_Pin at RA0_bit;

char txt1[] = "M32.ir";
unsigned char RS02w = 0xE0;
unsigned char RS02r = 0xE1;
unsigned char Lentgh_MSB = 0;
unsigned char Lentgh_LSB = 0;
int Lentgh= 0;
unsigned char oldstate;
bit Auto_State;
char buffer[7];

void Display_Start_Text(){
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "Ultrasonic");
     Lcd_Out(2, 1, "20CM - 600CM");
     Delay_ms(2000);
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "Press Key");
}

void Read_Lentgh_SRF02(){
     Lentgh = 0; Lentgh_MSB = 0; Lentgh_LSB = 0;
     I2C1_Start();
     I2C1_Wr(RS02w);
     I2C1_Wr(0);
     I2C1_Wr(0x51);
     I2C1_Stop();
     //Soft_I2C_Break();
     Delay_ms(70);
     I2C1_Start();
     I2C1_Wr(RS02w);
     I2C1_Wr(2);
     I2C1_Stop();
     //Soft_I2C_Break();
     Delay_ms(70);
     I2C1_Start();
     I2C1_Wr(RS02r);
     Lentgh_Msb = I2C1_Rd(1);
     Lentgh_Lsb = I2C1_Rd(0);
     I2C1_Stop();
     //Soft_I2C_Break();
     Delay_ms(70);
     Lentgh=(Lentgh_MSB*256)+Lentgh_LSB;
     IntToStr(Lentgh, buffer);
     //Lentgh=21;
}

void Display_Lentgh(){
  Lcd_Cmd(_LCD_CLEAR);
  //Lcd_Cmd(_LCD_CURSOR_OFF);
  //Lentgh=Lentgh+1;
  //Lentgh=123;
  Lcd_Out(1, 1, "Lentgh= ");
  //Lcd_Out(1, 12, " CM ");
  if (((Lentgh / 100) % 10)>0) {
     Lcd_Chr(1,9, ((Lentgh / 100)  % 10) + 48);
     //if (((Lentgh / 10) % 10)>0) {
        Lcd_Chr(1,10, ((Lentgh / 10)   % 10) + 48);
        Lcd_Chr(1,11, (Lentgh % 10) + 48);
     //}
  }
  if(((Lentgh / 100) % 10)==0) {
     if (((Lentgh / 10) % 10)>0) {
        Lcd_Chr(1,9, ((Lentgh / 10)  % 10) + 48);
        Lcd_Chr(1,10, (Lentgh % 10) + 48);
     }
     if (((Lentgh / 10) % 10)==0) {
        Lcd_Chr(1,9, (Lentgh % 10) + 48);
     }
  }
  //Lcd_Chr(1,9, ((Lentgh / 100)  % 10) + 48);
  //Lcd_Chr(1,10, ((Lentgh / 10)   % 10) + 48);
  //Lcd_Chr(1,11, (Lentgh % 10) + 48);
  Lcd_Out_CP("CM");
}


void Sound_Key_Pressing() {
  Sound_Play(4000, 100);
}

void Sound_Menu() {
  Sound_Play(2000, 100);
}

void Sound_Error() {
  Sound_Play(800, 30);
}


void main(){
  TRISD = 0b11111111;

  Auto_state=0; if (Button(&PORTD, 0, 1, 0)) Auto_state=1;

     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     //Lcd_Out(1,1,txt1);
     I2C1_Init(100000);

     //TRISA = 0xFE; PORTA = 0;
     TRISA = 0b11111110; PORTA = 0b11111110;
     Sound_Init(&PORTA, 0);
     Sound_Menu();

     if (Auto_state==0) Display_Start_Text();

     //Lentgh=12;
     //Display_Lentgh();
     while(1) {
//do {
              if (Button(&PORTD, 0, 1, 1)) oldstate = 1;
              if (oldstate && Button(&PORTD, 0, 1, 0)) {
                 Read_Lentgh_SRF02();
                 Sound_Key_Pressing();
                 Display_Lentgh();
                 oldstate = 0;
              }

              if (Auto_state==1){
                 Read_Lentgh_SRF02();
                 Display_Lentgh();
                 Delay_ms(500);
              }
//} while(1);
              //Read_Lentgh_SRF02();
              //Display_Lentgh();
              //Delay_ms(1000);
              //if (Button(&PORTA, 0, 1, 0)) Lcd_Out_CP("**************");

     }
}