//======================================================
// Function and global variables definition
//======================================================

void port_init();             	// initialize ports
void clock_init();            	// initialize operation clock
void ADC_init();              	// initialize A/D convertor
void ADC_start(unsigned char ch);	// start A/D convertor
unsigned int ADC_read();      	// read A/D convertor
void LCD_init();              	// initialize LCD
void WDT_init();
void WDT_clear();
void Timer0_init();           	// initialize Timer0
void Timer2_init();           	// initialize Timer2
void Timer3_init();           	// initialize Timer3
void Timer4_init();
void UART0_init();             	// initialize UART interface
void UART1_init();             	// initialize UART interface
//void UART0_def_init();
void UART1_def_init();
void WT_init();               	// initialize Watch timer




