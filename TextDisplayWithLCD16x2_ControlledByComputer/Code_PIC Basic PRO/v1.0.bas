'GitHub Account: GitHub.com/AliRezaJoodi

'INCLUDE "Modedefs.Bas"
'Define Loader_Used 1

DEFINE     OSC		11.059200		' Set the Xtal frequency to 11.0592mHz
  
DEFINE	LCD_DREG	PORTD		' Set Data to PortB
DEFINE 	LCD_DBIT	4			' Set starting Data to Bit0
DEFINE 	LCD_RSREG   PORTB	    ' Set Register Select to PortD
DEFINE 	LCD_RSBIT   2			' Set RS line to PORTD.2
DEFINE 	LCD_EREG	PORTB		' Set Enable to PortD
DEFINE 	LCD_EBIT  	0			' Set Enable line to PortD.0
DEFINE 	LCD_BITS	4			' Set for 4 bit Bus
DEFINE 	LCD_LINES	2			' Set number of lines to 2
LCDOUT $FE, 1, "Typing With USB"
   

TRISC=%10111111
SPBRG=12                         'Set Baud Rate to 9600
RCSTA=%10010000
RCSTA=%10010000
TXSTA=%00100000
       
UART_Reciver var byte
Buffer var byte[17]
k var byte

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
    'Lcdout UART_Reciver
    if (UART_Reciver>=32)and(UART_Reciver<=127) then
        Buffer[k]=UART_Reciver
        k=k+1
    endif
    IF UART_Reciver=13 then
        IF (Buffer[0]="A")or(Buffer[0]="a") THEN
            LCDOUT $FE, $2
            for k=1 to 16   
                Lcdout Buffer[k]
                Buffer[k]=32
            next k
        ENDIF
        IF (Buffer[0]="B")or(Buffer[0]="b") THEN
            LCDOUT $FE, $C0
            for k=1 to 16   
                Lcdout Buffer[k]
                Buffer[k]=32
            next k
        ENDIF
        k=0
    endif 
RETURN

END
    
