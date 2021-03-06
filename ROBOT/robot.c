#include <16F877A.h>

#device adc=10                         // Usa resoluci�n de 10 bits
#fuses XT,NOWDT,NOPROTECT,NOLVP,NOPUT,NODEBUG,NOCPD,NOBROWNOUT 
#use delay(clock=4000000)
#define lcd_rs_pin pin_d3
#define lcd_rw_pin pin_A4 
#define lcd_enable_pin pin_d2
#include <lcd.c>

#use RS232(BAUD=9600,BITS=8,PARITY=N,XMIT=PIN_C6,RCV=PIN_C7,stream=PORT1) 
#USE fast_io(d)
#USE fast_io(b)
#USE fast_io(c)

char data;
float voltaje,temper,medicion,batery,l1;
int16 maximo1, minimo1,media1,maximo2, minimo2,media2,S1,S2,duty=0;
int Timer2,Poscaler;


void ini_puertos(){
set_tris_b(0b0000000);     // pines del puerto b son salidas
set_tris_d(0b11);
set_tris_c(0b10000000);
return;
}

void ini_voltaje(){
SETUP_ADC (ADC_CLOCK_INTERNAL);
SETUP_ADC_PORTS (ALL_ANALOG);
lcd_init();
lcd_putc("\fVoltaje");
SET_ADC_CHANNEL(0);
delay_ms(20);
batery=READ_ADC();
voltaje=5.0*batery/1023.0;
lcd_gotoxy(3,2);
printf(lcd_putc, "Valor:%fV   ",voltaje);
delay_ms(1500);
}




void ini_robot(){
 
lcd_putc("\f");                                    // Inicia LCD
lcd_putc(" Calibrar \n");
lcd_putc(" sensores");       
delay_ms(2000);
lcd_putc("\f");                             // Limpia pantalllcd_init(); 
lcd_putc("maxS1:\n");
lcd_putc("maxS2:");
delay_ms(1000); 
lcd_gotoxy(8,1);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,1);     // Acomoda cursor LCD
setup_adc (adc_clock_internal);
setup_adc_ports (all_analog);
set_adc_channel (6);               // Elige canal a medir RA1
delay_us (20);
maximo1=read_adc ();   // Hace conversi�n AD 
setup_adc (adc_off);                 // Apaga ADC
printf(lcd_putc,"%4Ld ",maximo1);   // xxx.x �C
delay_ms (2000); 
lcd_gotoxy(8,2);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,2);     // Acomoda cursor LCD
setup_adc (adc_clock_internal);
setup_adc_ports (all_analog);
set_adc_channel (1);               // Elige canal a medir RA2
delay_us (20);
maximo2=read_adc ();   // Hace conversi�n AD 
setup_adc (adc_off);                 // Apaga ADC
printf(lcd_putc,"%4Ld ",maximo2);   // xxx.x �C
delay_ms (2000);
lcd_putc("\f"); 


////////minimossss


lcd_putc("\f");                             // Limpia pantalllcd_init(); 
lcd_putc("minS1:\n");
lcd_putc("minS2:");
delay_ms(1000); 
lcd_gotoxy(8,1);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,1);     // Acomoda cursor LCD
setup_adc (adc_clock_internal);
setup_adc_ports (all_analog);
set_adc_channel (1);               // Elige canal a medir RA1
delay_us (20);
minimo1=read_adc ();   // Hace conversi�n AD 
setup_adc (adc_off);                 // Apaga ADC
printf(lcd_putc,"%4Ld ",minimo1);   // xxx.x �C
delay_ms (2000); 
lcd_gotoxy(8,2);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,2);     // Acomoda cursor LCD
setup_adc (adc_clock_internal);
setup_adc_ports (all_analog);
set_adc_channel (1);               // Elige canal a medir RA2
delay_us (20);
minimo2=read_adc ();   // Hace conversi�n AD 
setup_adc (adc_off);                 // Apaga ADC
printf(lcd_putc,"%4Ld ",minimo2);   // xxx.x �C
delay_ms (2000);
lcd_putc("\f");

//////7 promedio


lcd_putc("media1:\n");
lcd_putc("media2:");
delay_ms(1000); 
lcd_gotoxy(8,1);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,1);     // Acomoda cursor LCD
media1=(maximo1-minimo1)/2 ;   // Hace conversi�n AD 

printf(lcd_putc,"%4Ld ",media1);   // xxx.x �C
delay_ms (2000); 
lcd_gotoxy(8,2);      // Acomoda cursor LCD
lcd_putc("   ");       // Limpia ese sector de pantalla
lcd_gotoxy(8,2);     // Acomoda cursor LCD
delay_us (20);
media2=(maximo2-minimo2)/2;   // Hace conversi�n AD 
printf(lcd_putc,"%4Ld ",media2);
delay_ms(2000);
lcd_putc("\f");








return;
}
void ini_lcd(){
lcd_init(); // Inicialzac�n de la LCD
lcd_gotoxy(1,1);
lcd_putc(" bienvenidos");
delay_ms(1000);
lcd_putc("\f"); 
lcd_putc("PRESIONE\n");
lcd_putc("UN BOTON");
delay_ms(2000);
lcd_putc("\f");                             // Limpia pantalla
return;
}


void ini_temperatura(){
 lcd_init();
lcd_putc("\f");                                    // Inicia LCD
lcd_putc(" Termometro\n");          // Saca texto
lcd_putc(" con LM35");               // Saca texto
delay_ms(2000);
lcd_putc("\f");                             // Limpia pantalla
lcd_putc("Temperatura\n");          // Saca texto
lcd_putc("actual");                      // Saca texto
delay_ms(1000); 
lcd_gotoxy(14,2);                       // Acomoda cursor LCD
lcd_putc("oC");
lcd_gotoxy(8,2);      // Acomoda cursor LCD
lcd_putc(" ");          // Limpia ese sector de pantalla
lcd_gotoxy(8,2);     // Acomoda cursor LCD
setup_adc (adc_clock_internal);
setup_adc_ports (all_analog);
set_adc_channel (4);                // Elige canal a medir RA0
delay_us (20);
medicion=read_adc ();              // Hace conversi�n AD 
setup_adc (adc_off);                 // Apaga ADC
temper=medicion*(0.48875);     // Pasa binario a �C 
printf(lcd_putc,"%02.1f",temper);   // xxx.x �C
delay_ms (3000);
lcd_putc("\f"); 
return;
}


void main(){
ini_puertos();
lcd_putc("puertos ok"); 
delay_ms(2000);
lcd_putc("\f");
output_b(0b00000000);                              ///PWM




ini_lcd();
delay_ms(20);
lcd_gotoxy(1,1);
lcd_putc("Temp(1) VOLT(3)\n");
lcd_putc("SENSORES(2)");        // Saca texto
while(true){
if(INPUT(PIN_D0)==1){
ini_temperatura();
}
if (INPUT(PIN_E2)==1){
ini_voltaje();
}
if (INPUT(PIN_D1)==1){
ini_robot();
data=getc();

   while(1){
   Timer2=255;
   Poscaler=1;
   setup_timer_2(t2_div_by_4,Timer2,Poscaler);      //100% FULL HD
   setup_ccp1(ccp_pwm);
   duty=800;
   set_pwm1_duty(duty);
   set_pwm2_duty(duty);
  data=getc(); 

 if(data=='Z'){
 Timer2=255;
   delay_ms(20);
   Poscaler=1;
   setup_timer_2(t2_div_by_16,Timer2,Poscaler);      //100% FULL HD
   setup_ccp1(ccp_pwm);
   duty=1023;
   set_pwm1_duty(duty);
 
 }
 if(data=='X'){
 Timer2=255;
   Poscaler=1;
   setup_timer_2(t2_div_by_4,Timer2,Poscaler);      //100% FULL HD
   setup_ccp1(ccp_pwm);
   duty=1023;
   set_pwm1_duty(duty);
 
 }
  
if(data=='W'){                                    //SE ALMACENA EL VALOR EN DATA, DEL MODULO Y SE INICIA EL MOTOR DEPENDIENDO DE SU VALOR W,S,A,D.
   output_high(PIN_B0);
   output_low(PIN_B1);
   output_high(PIN_B2);
   output_low(PIN_B3);
   output_high(PIN_B4);
   output_low(PIN_B5);
   output_high(PIN_B6);
   output_low(PIN_B7);
   lcd_init(); // Inicialzac�n de la LCD
   lcd_gotoxy(1,1);
   lcd_putc(" adelante");
   
  

 
  
   
}
if(data=='S'){                                    //SE ALMACENA EL VALOR EN DATA, DEL MODULO Y SE INICIA EL MOTOR DEPENDIENDO DE SU VALOR W,S,A,D.
   output_low(PIN_B0);
   output_high(PIN_B1);
   output_low(PIN_B2);
   output_high(PIN_B3);
   output_low(PIN_B4);
   output_high(PIN_B5);
   output_low(PIN_B6);
   output_high(PIN_B7);
   lcd_init(); // Inicialzac�n de la LCD
   lcd_gotoxy(1,1);
   lcd_putc(" atras");
 ;
   }
if(data=='A'){                                    //SE ALMACENA EL VALOR EN DATA, DEL MODULO Y SE INICIA EL MOTOR DEPENDIENDO DE SU VALOR W,S,A,D.
   output_low(PIN_B0);
   output_high(PIN_B1);
   output_high(PIN_B2);
   output_low(PIN_B3);
   output_low(PIN_B4);
   output_high(PIN_B5);
   output_high(PIN_B6);
   output_low(PIN_B7);
   lcd_init(); // Inicialzac�n de la LCD
   lcd_gotoxy(1,1);
   lcd_putc(" IZQUIERDA"); 

}
if(data=='D'){                                    //SE ALMACENA EL VALOR EN DATA, DEL MODULO Y SE INICIA EL MOTOR DEPENDIENDO DE SU VALOR W,S,A,D.
   output_high(PIN_B0);
   output_low(PIN_B1);
   output_low(PIN_B2);
   output_high(PIN_B3);
   output_high(PIN_B4);
   output_low(PIN_B5);
   output_low(PIN_B6);
   output_high(PIN_B7);
   lcd_init(); // Inicialzac�n de la LCD
   lcd_gotoxy(1,1);
   lcd_putc(" DERECHA");   
   ;
}
if(data=='F'){                                    //SE ALMACENA EL VALOR EN DATA, DEL MODULO Y SE INICIA EL MOTOR DEPENDIENDO DE SU VALOR W,S,A,D.
   output_low(PIN_B0);
   output_low(PIN_B1);
   output_low(PIN_B2);
   output_low(PIN_B3);
  output_low(PIN_B4);
   output_low(PIN_B5);
   output_low(PIN_B6);
   output_low(PIN_B7);
   lcd_init(); // Inicialzac�n de la LCD
   lcd_gotoxy(1,1);
   lcd_putc(" STOP"); 
  
}
}
}
}}
