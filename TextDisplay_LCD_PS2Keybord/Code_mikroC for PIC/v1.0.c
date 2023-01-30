// GitHub Account:     GitHub.com/AliRezaJoodi

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

sbit PS2_Data at RC3_bit;
sbit PS2_Clock at RC2_bit;
sbit PS2_Data_Direction at TRISC3_bit;
sbit PS2_Clock_Direction at TRISC2_bit;

sbit LED_Caps_Lock at RD0_bit;

unsigned short keydata = 0, special = 0, down = 0;
bit state_Caps_Lock;
unsigned char state_Line=1;

void Display_Start_Text(){
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "Press Key");
     Delay_ms(1000);
     Lcd_Cmd(_LCD_CLEAR);
}

void Display_keydata(){
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "keydata= ");
  if (((keydata / 100) % 10)>0) {
     Lcd_Chr(1,9, ((keydata / 100)  % 10) + 48);
     Lcd_Chr(1,10, ((keydata / 10)   % 10) + 48);
     Lcd_Chr(1,11, (keydata % 10) + 48);
  }
  if(((keydata / 100) % 10)==0) {
     if (((keydata / 10) % 10)>0) {
        Lcd_Chr(1,9, ((keydata / 10)  % 10) + 48);
        Lcd_Chr(1,10, (keydata % 10) + 48);
     }
     if (((keydata / 10) % 10)==0) {
        Lcd_Chr(1,9, (keydata % 10) + 48);
     }
  }
}

void main(){
     TRISD = 0b11111110; LED_Caps_Lock=0;
     Lcd_Init(); Lcd_Cmd(_LCD_CLEAR); Lcd_Cmd(_LCD_UNDERLINE_ON); Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
     Ps2_Config();
     Display_Start_Text();
     //state_Line2=~state_Line2;
     while(1) {
              if (Ps2_Key_Read(&keydata, &special, &down)) {

                 if (down && !special && keydata) {
                 //if (down  && keydata) {
                    //Lcd_Cmd(_LCD_CLEAR);
                    //Display_keydata();
                    Lcd_Chr_CP(keydata);
                 }
                 else if (down && (keydata == 13)) {
                      //state_Line2=~state_Line2;
                      if (state_Line==1) {
                         Lcd_Cmd(_LCD_SECOND_ROW);
                         state_Line=2;
                      }
                      //if (state_Line==1)state_Line=2;
                 }
                 if (down && (keydata == 18)) {
                    Lcd_Cmd(_LCD_CLEAR);
                    Lcd_Cmd(_LCD_FIRST_ROW);
                    state_Line=1;
                 }
                 if (down && (keydata == 16)) {
                    Lcd_Cmd(_LCD_MOVE_CURSOR_LEFT);
                    Lcd_Chr_CP(' ');
                    Lcd_Cmd(_LCD_MOVE_CURSOR_LEFT);
                 }
                 if (down && (keydata == 25)) {
                    //if (state_Caps_Lock==0) state_Caps_Lock=1; else state_Caps_Lock=0;
                    //LED_Caps_Lock= state_Caps_Lock;
                    LED_Caps_Lock=~LED_Caps_Lock;
                 }
              }
     }
}