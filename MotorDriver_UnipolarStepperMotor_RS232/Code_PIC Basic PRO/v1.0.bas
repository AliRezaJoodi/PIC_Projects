'GitHub Account: GitHub.com/AliRezaJoodi

DEFINE     OSC		 8		' Set the Xtal frequency to 8mHz

TRISC=%10111111
SPBRG=12                         'Set Baud Rate to 9600
RCSTA=%10010000
RCSTA=%10010000
TXSTA=%00100000

TRISB=%00000000
PORTB=%00000000
       
UART_Reciver var byte
Buffer var byte[17]
k var byte
Control_Data VAR BYTE
Z VAR BYTE

Pause 1000  : LCDOUT $FE,$1
for k=1 to 16   
    Buffer[k]=32
next k
K=0
                          
Main:
    gosub charin
    if UART_Reciver=0 then main
    gosub charout
goto main 
     
charin:
    UART_Reciver=0
    IF PIR1.5=1 THEN
        UART_Reciver=RCREG
    ENDIF
RETURN

charout:
    if PIR1.4=0 then charout
    TXREG=UART_Reciver
    if (UART_Reciver>=48)and(UART_Reciver<=57) then
        'Buffer[k]=UART_Reciver
        'k=k+1
        Z=(Z*10)+(UART_Reciver-48)
        'PORTB=Z
    endif
    IF UART_Reciver=13 then
        PORTB=Z
        Z=0    
    endif 
RETURN

END
    
