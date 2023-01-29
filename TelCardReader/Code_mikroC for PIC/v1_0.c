// GitHub Account:     GitHub.com/AliRezaJoodi

#define CHKBIT(ADDRESS,BIT)  (ADDRESS &(1<<BIT))
#define SETBIT(ADDRESS,BIT)  (ADDRESS|=1<<BIT)
#define CLRBIT(ADDRESS,BIT)  (ADDRESS &=~(1<<BIT))

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

sbit Sound_Pin at RA0_bit;

sbit Data_pin at RD4_bit;
sbit Reset_pin at RD2_bit;
sbit Clock_pin at RD3_bit;

unsigned char data1[64];
unsigned char oldstate;
int credit;
unsigned int x1[10];
char s1[11];

void Display_Start_Text(){
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "TelCard Reader");
     Lcd_Out(2, 1, "Press Key");
}

void RED(){
     unsigned char i,j;
     for (j=0;j<64;j++)data1[j]=0;
     delay_ms(150);
     SETBIT(PORTC,2);
     SETBIT(PORTC,3);
     delay_us(5);
     CLRBIT(PORTC,3);
     CLRBIT(PORTC,2);
     for(j=0;j<64;j++){
             for(i=0;i<8;i++){
                    SETBIT(PORTC,3); delay_us(5);
                    data1[j]=data1[j]<<1;
                    if(CHKBIT(PORTC,1)) data1[j]=data1[j]|0b00000001;
                    CLRBIT(PORTC,3); delay_us(5);
             }
     }
     //for (i=0;i<13;i++){
         //Lcd_Cmd(_LCD_CLEAR);
         //IntToStr(i, s1); Lcd_Out(1,1,s1);
         //IntToStr(data1[i], s1); Lcd_Out(2,1,s1);
         //delay_ms(1000);
     //}
}

void CRD(){
     unsigned char i,j,n;
     credit=0; x1[3]=0; x1[2]=0; x1[1]=0; x1[0]=0; n=3;
     for (j=9;j<13;j++){
         for (i=0;i<8;i++){
             if (CHKBIT(data1[j],0))x1[n]++;
             data1[j]=data1[j]>>1;
         }
         n--;
     }
     credit=((x1[3]*512)+(x1[2]*64)+(x1[1]*8)+x1[0])*50;
}

void Display_credit(){
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "TelCard Reader");
  Lcd_Out(2, 1, "Credit: ");
  if (((credit / 1000) % 10)>0) {
     Lcd_Chr(2,9, ((credit / 1000)  % 10) + 48);
     Lcd_Chr(2,10, ((credit / 100)  % 10) + 48);
     Lcd_Chr(2,11, ((credit / 10)   % 10) + 48);
     Lcd_Chr(2,12, (credit % 10) + 48);
  }
  if (((credit / 1000) % 10)==0) {
     if (((credit / 100) % 10)>0) {
        Lcd_Chr(2,9, ((credit / 100)  % 10) + 48);
        Lcd_Chr(2,10, ((credit / 10)   % 10) + 48);
        Lcd_Chr(2,11, (credit % 10) + 48);
     }
     if(((credit / 100) % 10)==0) {
                 if (((credit / 10) % 10)>0) {
                    Lcd_Chr(2,9, ((credit / 10)  % 10) + 48);
                    Lcd_Chr(2,10, (credit % 10) + 48);
                    }
                 if (((credit / 10) % 10)==0) {
                    Lcd_Chr(2,9, (credit % 10) + 48);
                 }
     }
  }
  Lcd_Out_CP("Rial");
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
     //TRISA = 0b11111111;
     TRISC = 0b11110011;
     TRISD = 0b11111110;
     Lcd_Init(); Lcd_Cmd(_LCD_CLEAR); Lcd_Cmd(_LCD_CURSOR_OFF);
     PORTD = 0b11111110; Sound_Init(&PORTD, 0);
     Sound_Menu(); Display_Start_Text();
     while(1) {
              CLRBIT(PORTC,3);
              CLRBIT(PORTC,2);
              if (Button(&PORTD, 1, 1, 1)) oldstate = 1;
              if (oldstate && Button(&PORTD, 1, 1, 0)) {
                 Sound_Key_Pressing();
                 RED();
                 CRD();
                 Display_credit();
                 oldstate = 0;
              }
     }
}
