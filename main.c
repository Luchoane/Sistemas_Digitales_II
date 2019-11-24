// Proyecto ENTREGA: INTERRUPT SERIAL - copy
// Clock de CPU por defecto 12 MHz

// GPIOs y LEDs
#define DIR1            (*((int*) 0xA0002004U)) // Registro de direccion de GPIO
#define PIN             (*((int*) 0xA0002104U)) // Registro PIN (data) GPIO
#define LED_AZUL        (1U<<1)                 // Desplazamiento para P1.1
#define LED_VERDE       (1U<<0)                // Desplazamiento para P1.0
#define LED_ROJO        (1U<<2)                 // Desplazamiento para P1.2

// NVIC
#define NVIC_ISER0      (*((int*) 0xE000E100U)) // Registro ISER0 de NVIC

// INTENSET
#define INTENSET        (*((int*) 0x4006400CU)) // INTENSET
#define RXRDYEN         (1U<<0)                // Enable interrupcion de RX de INTENSET
#define TXRDYEN         (1U<<2)                // Enable interrupcion de TX de INTENSET
#define PINASSIGN0      (*((int*) 0x4000C000U)) // Dir de PINASSING0. 15:8 es donde lee RX. 7:0 es TX

#define SYSAHBCLKCTRL0  (*((int*) 0x40048080U)) // Registro de control de clock de periféricos
#define UART0_ENABLE  (1U<<14) // Habilitacion de clk bit 14, en=1
#define PRESETCTRL0     (*((int*) 0x40048088U)) // Registro 0 de reset de periféricos
#define UART0_RST_N     (1U << 14) // Reset UART0 desde PRESETCTRL0 de perifericos, bit 14
#define CTIMER_BASE     (0x40038000U)             // Direccion base de CTIMER
#define CTIMER_IR       (*((int*) (CTIMER_BASE + 0x000))) // Registro de interrupcion de CTIMER
#define MCR             (*((int*) (CTIMER_BASE + 0x014))) // Registro de control de MATCH 
#define MR0             (*((int*) (CTIMER_BASE + 0x018))) // Registro MATCH 0
#define TCR             (*((int*) (CTIMER_BASE + 0x004))) // Registro de contorl de CTIMER

// USART0
#define USART0          (*((int*) (0x40064000U))) // Base de USART0
#define BRG_USART0      (*((int*) (USART0 + 0x020))) // Offset para BRG - Reset value 0x0005
#define RXDAT_USART0    (*((int*) (USART0 + 0x014))) // Offset para RXDAT - Aca se guarda el last char
#define TXDAT_USART0    (*((int*) (USART0 + 0x014))) // Offset para TXDAT - Aca se guarda el last char
#define CFG_USART0      (*((int*) (USART0 + 0X000))) // Offset para CFG - Para activar el periferico

#define SYS_CONF        (*((int*) (0x40048000)))
#define UART0CLKSEL     (*((int*) (SYS_CONF + 0x090))) // Offset para UART0CLKSEL - Reset value 0x7

// PIO0_25 RDX en hexa 19
// PIO0_24 TRX en hexa 18

int main()
{
  
  // Conf. de pines para LEDs 
  SYSAHBCLKCTRL0 |= (1U<<20);    // Habilita clock en GPIO
  DIR1           |= (LED_AZUL);
  //DIR1           |= (LED_AZUL | LED_VERDE | LED_ROJO) ;    // P0.8, P0.9, y P0.12 como salida
  
  //Conf. de USART0
  SYSAHBCLKCTRL0 |= UART0_ENABLE; // Habilitamos clk en UART, bit 14 en=1
  PRESETCTRL0    |= UART0_RST_N; // Sacamos de reset al UART0, bit 14 en=1
  SYSAHBCLKCTRL0 |= (1U << 7);  // Habilitamos clock en Switch matrix
  INTENSET       |= RXRDYEN; // Se va a generar una interrupcion y guardar el last charater en RXDAT
  INTENSET       |= TXRDYEN; // Se va a generar una interrupcion y guardar el last charater en RXDAT
  
  //PINASSIGN0     |= ((0x8)<<0); // Borro los bits menos significativos de 15:8
  //PINASSIGN0     |= ((0x18)<<0); // Escribo la direccion del PIN0_19 en los bits de 15:8 de PINASSING0
  PINASSIGN0  &= ~(0xFFFF);   // Aca borramos los bits 15:0 de PINASSIGN0
  PINASSIGN0  |= 24;                // Escribimos 24 en los bits menos significativos
  PINASSIGN0  |= (25 << 8) ;   // Escribimos 25 en los bits 15:8
  
  SYSAHBCLKCTRL0 &= ~(1U << 7);  // Des-habilitamos clock en Switch matrix
  
  BRG_USART0     |= ((0x4D) << 0);
  UART0CLKSEL    &= ~(0x7); // Elegimos fuente de clock para USART0
  CFG_USART0     &= (1U<<0); // Se activa el periferico
  
  // Conf. de CTimer
  // SYSAHBCLKCTRL0 |= (1U << 25); // Habilitamos clock en CTIMER
  // PRESETCTRL0    |= (1U << 25); // Sacamos de reset al timer
  // MCR |= (1U << 0)|(1U << 1);   // Reset de TC(bit 1) e interrupcion (bit 0) en Match0
  // MR0  = 6000000U;              // Match cada 1/2 segundo (para 12 MHz)
  // TCR |= (1U << 0);             // Habilita timer (comienza conteo)
  
  // NVIC_ISER0 |= (1U << 23);     // Habilitamos interrupciones de timer en NVIC
  NVIC_ISER0 |= (1U << 3);     // Habilitamos interrupciones de USART0 en NVIC    
  
  // Apagamos todos los LEDS
  PIN |=  (LED_AZUL | LED_VERDE | LED_ROJO);      // Se apagan con 1
  
  while(1){
    printf(RXDAT_USART0);
    TXDAT_USART0 = RXDAT_USART0;
    printf("TXDAT_USART0");
  };     // Loop infinito vacio (no hace nada)
}

void UART0_IRQHandler()
{

  printf(RXDAT_USART0);
  TXDAT_USART0 = RXDAT_USART0;
  printf("TXDAT_USART0");

}

// No utilizar Timer Clock
/*void CTIMER0_IRQHandler()
{
   int* p_Pin            = (int *) 0xA0002100U; // Declaracion de puntero a registro PIN, reemplazado por MACRO PIN
  CTIMER_IR |= (1U << 0);  // borramos flag de interrupcion escribiendo un 1 en bit 0 de CTIMER_IR
  
  PIN    ^= ( LED_VERDE ) ; // Conmuta estado de LEDs
  printf("=)\n");
} */
 
// Definicion vacia de SystemInit ya que es llamada por el codigo STARTUP
void SystemInit()
{
  // vacio
};
