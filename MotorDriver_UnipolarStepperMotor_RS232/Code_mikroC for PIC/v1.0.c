// GitHub Account:     GitHub.com/AliRezaJoodi

char Uart_Reciver;
char z1=0;
char Control_Data;

void main(){
     UART1_Init(9600);
     TRISB = 0b00000000; PORTB=0b00000000;
     //UART1_Write_Text("Stepper Motor Controller"); UART1_Write(13);
     while (1) {
           if (UART1_Data_Ready()) {
              Uart_Reciver = UART1_Read();
              //UART1_Write(Uart_Reciver);
              if (Uart_Reciver>=48 & Uart_Reciver<=57){z1= ((z1*10)+(Uart_Reciver-48));}
              if (Uart_Reciver==13) {PORTB=z1; z1=0;}
           }
     }
}

