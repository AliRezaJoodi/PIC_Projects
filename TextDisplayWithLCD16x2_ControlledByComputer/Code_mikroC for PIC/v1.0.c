// GitHub Account:     GitHub.com/AliRezaJoodi

sbit LCD_RS at RB2_bit;
sbit LCD_EN at RB0_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISB2_bit;
sbit LCD_EN_Direction at TRISB0_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;

char uart_rd;
unsigned char i;
char Buffer[17];

void Display_Start_Text(){
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Out(1, 1, "Typing With USB");
     Delay_ms(1000); Lcd_Cmd(_LCD_CLEAR);
}

void main(){
     UART1_Init(9600);

     TRISD = 0b00001111; TRISB = 0b11010111; Lcd_Init();
     Lcd_Cmd(_LCD_CURSOR_OFF); Lcd_Cmd(_LCD_CLEAR);

     Display_Start_Text();
     UART1_Write_Text("Typing With USB"); UART1_Write(13);

     while (1) {
           if (UART1_Data_Ready()) {
              uart_rd = UART1_Read();
              UART1_Write(uart_rd);
              if (uart_rd>=32 & uart_rd<=126){Buffer[i++]=uart_rd;}
              if (uart_rd==13) {
                 if (Buffer[0]=='A') {for (i=1;i<=16;i++) Lcd_Chr(1, i,Buffer[i]);}
                 if (Buffer[0]=='B') {for (i=1;i<=16;i++) Lcd_Chr(2, i,Buffer[i]);}
                 for (i=0;i<=16;i++) Buffer[i]=32; i=0;
              }
           }
     }
}