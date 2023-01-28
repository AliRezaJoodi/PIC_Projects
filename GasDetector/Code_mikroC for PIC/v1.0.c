// GitHub Account:     GitHub.com/AliRezaJoodi

// Lcd pinout settings
sbit LCD_RS at RB6_bit;
sbit LCD_EN at RB4_bit;
sbit LCD_D4 at RB3_bit;
sbit LCD_D5 at RB2_bit;
sbit LCD_D6 at RB1_bit;
sbit LCD_D7 at RB0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISB6_bit;
sbit LCD_EN_Direction at TRISB4_bit;
sbit LCD_D4_Direction at TRISB3_bit;
sbit LCD_D5_Direction at TRISB2_bit;
sbit LCD_D6_Direction at TRISB1_bit;
sbit LCD_D7_Direction at TRISB0_bit;

sbit Sound_Pin at RC4_bit;

sbit Relay at RC6_bit;

sbit Key_Up at RD2_bit;
sbit Key_Set at RD1_bit;
sbit Key_Down at RD0_bit;

unsigned int Read_the_adc(unsigned char);
void Display_LCD(unsigned int,unsigned int);
void Sound_Key_Pressing(void);
void Sound_Menu(void);
void Sound_Error(void);
void Setpoint_ADD(void);
void Setpoint_Reduce(void);
void Alarm_ON_OFF(void);
void Alarm_Enable_Disable(void);
unsigned int EEPROM_Load();
void EEPROM_Save(unsigned int);

unsigned int Input_mV=0;
unsigned int Setpoint_mV=0;
unsigned int Setpoint_mV_Max=0;
unsigned int Setpoint_mV_Min=0;
bit State_Alarm_Disable;
bit State_Alarm_ON;
bit Status_Key_Set;

void main(){
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);

   ADCON1=0x80;
   TRISA  = 0xFF;
   TRISD = 0b11111111;

    TRISC = 0b10101111; PORTC = 0b10101111;

    Relay=0;

    Status_Key_Set=0; State_Alarm_Disable=0; State_Alarm_ON=0;

    Sound_Init(&PORTC, 4);
    Sound_Menu();

    Setpoint_mV=EEPROM_Load();

     while(1){
              Input_mV=Read_the_adc(1);
              Display_LCD(Input_mV,Setpoint_mV);
              Alarm_ON_OFF();

              if(Key_Set==0 && Status_Key_Set==0){
                   Delay_ms(30);
                   if(Key_Set==0 && Status_Key_Set==0){
                        Status_Key_Set=1;
                        Sound_Menu();
                        Alarm_Enable_Disable();
                   }
              }
              if(Key_Set==1){Status_Key_Set=0;}

              do{
                 if(Key_Up==0){
                    Delay_ms(30);
                    if(Key_Up==0){
                      Sound_Key_Pressing();
                      Setpoint_ADD();
                      EEPROM_Save(Setpoint_mV);
                      Display_LCD(Input_mV,Setpoint_mV);
                      Delay_ms(50);
                    }
                 }
              }while (Key_Up==0);

              do{
                 if(Key_Down==0){
                    Delay_ms(30);
                    if(Key_Down==0){
                      Sound_Key_Pressing();
                      Setpoint_Reduce();
                      EEPROM_Save(Setpoint_mV);
                      Display_LCD(Input_mV,Setpoint_mV);
                      Delay_ms(50);
                    }
                 }
              }while (Key_Down==0);
     }
}

//********************************************************
void Alarm_Enable_Disable(){
     if(State_Alarm_Disable==0){
          State_Alarm_Disable=1;
          Relay=0;
          Lcd_Out(1,16,"X");
     }
     else if(State_Alarm_Disable==1){
          State_Alarm_Disable=0;
          Lcd_Out(1,16," ");
     }
}

//********************************************************
unsigned int EEPROM_Load(){
     unsigned int x=0;
     x = EEPROM_Read(0x01);  x=x*100;
     return x;
}

//********************************************************
void EEPROM_Save(unsigned int x){
     x=x/100;
     EEPROM_Write(0x01,x);
}
//********************************************************
void Alarm_ON_OFF(){
     Setpoint_mV_Max=Setpoint_mV;
     Setpoint_mV_Min=Setpoint_mV-200;

     if(Input_mV>Setpoint_mV_Max){State_Alarm_ON=1;}
     else if(Input_mV<Setpoint_mV_Min){State_Alarm_ON=0;}

     if(State_Alarm_ON==1 && State_Alarm_Disable==0){
          Relay=1;
          Sound_Error(); Sound_Menu();
     }
     else if(State_Alarm_ON==0){
          Relay=0;
     }
}

//********************************************************
void Setpoint_ADD(){
     Setpoint_mV=Setpoint_mV+100; if(Setpoint_mV>5000){Setpoint_mV=0;}
}

//********************************************************
void Setpoint_Reduce(){
     Setpoint_mV=Setpoint_mV-100; if(Setpoint_mV>5000){Setpoint_mV=5000;}
}

//********************************************************
unsigned int Read_the_adc(unsigned char ch){
     unsigned int x=0;
     x = ADC_Read(ch);
     x= x*4.8828125;
     return x;
}

//********************************************************
void Display_LCD(unsigned int x,unsigned int y){
     Lcd_Cmd(_LCD_FIRST_ROW); Lcd_Out_Cp("Input: ");
     x=x/100;
     Lcd_Chr_Cp(((x /10)%10)+48);
     Lcd_Chr_Cp(46);
     Lcd_Chr_Cp(((x /1)%10)+48);
     Lcd_Out_Cp("v ");
     if(State_Alarm_ON==1 && State_Alarm_Disable==0){
          Lcd_Cmd(_LCD_SECOND_ROW);
          Lcd_Out_Cp("**** Alarm ****");
     }
     else{
          Lcd_Cmd(_LCD_SECOND_ROW);
          Lcd_Out_Cp("Setpoint: ");
          y=y/100;
          Lcd_Chr_Cp(((y /10)%10)+48);
          Lcd_Chr_Cp(46);
          Lcd_Chr_Cp(((y /1)%10)+48);
          Lcd_Out_Cp("v ");
     }
}

//********************************************************
void Sound_Key_Pressing(){
     Sound_Play(4000, 100);
}

//********************************************************
void Sound_Menu(){
     Sound_Play(1000, 150);
}

//********************************************************
void Sound_Error(){
     Sound_Play(800, 30);
}