'GitHub Account: GitHub.com/AliRezaJoodi

DEFINE OSC 8
  
Sensor_left  var portb.4
Sensor_riht  var portd.4
Relay var portd.1
low relay

loop:
    if Sensor_left = 1 & Sensor_riht = 0 then
        high relay                                             
    endif 
    if Sensor_left = 0 & Sensor_riht = 1 then
        low relay                                            
    endif      
goto loop 
   
end
