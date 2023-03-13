#include "MK60D10.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

  // Frequencies of notes.
#define A_2 110
#define As_2 116
#define B_2 123
#define C_3 130
#define Cs_3 138
#define D_3 146
#define Ds_3 155
#define E_3 164
#define F_3 174
#define Fs_3 185
#define G_3 196
#define Gs_3 207
#define A_3 220
#define As_3 233
#define B_3 246
#define C_4 261
#define Cs_4 277
#define D_4 293
#define Ds_4 311
#define E_4 329
#define F_4 349
#define Fs_4 369
#define G_4 392
#define Gs_4 415
#define A_4 440
#define As_4 466
#define B_4 493
#define C_5 523
#define Cs_5 554
#define D_5 587
#define Ds_5 622
#define E_5 659
#define F_5 698
#define Fs_5 739
#define G_5 783
#define Gs_5 830
#define A_5 880
#define As_5 932
#define B_5 987
#define C_6 1046
#define Cs_6 1108
#define D_6 1174
#define Ds_6 1244
#define E_6 1318
#define F_6 1396
#define Fs_6 1479

#define LED_D9  0x20 // Port B, bit 5
#define LED_D10 0x10 // Port B, bit 4
#define LED_D11 0x8  // Port B, bit 3
#define LED_D12 0x4  // Port B, bit 2

#define BTN_SW6 0x800  // Port E, bit 11

#define BUFF_SIZE 256
char buffer[BUFF_SIZE];

static RCM_Type *rcm = RCM;  // rst srcs.

void delay(uint64_t bound) {
	for (uint64_t i=0; i < bound; i++) { __NOP(); }
}

void beep(uint32_t freq, float len_multiplier) {
  int len = len_multiplier * 100 * freq / 440;
  freq = 2875 * A_4 / freq;
  for (uint32_t q = 0; q < len; q++) {
    PTA->PDOR = GPIO_PDOR_PDO(0x0010); delay(freq);
    PTA->PDOR = GPIO_PDOR_PDO(0x0000); delay(freq);
  }
}

void str_send(char* str) {
  uint16_t i = 0;
  while (str[i] != '\0') {
    while((UART5->S1 & UART_S1_TDRE_MASK || UART5->S1 & UART_S1_TC_MASK) == 0);
    UART5->D = str[i++];
  }
}

void stat_send(uint32_t rst_src) {
  sprintf(buffer, "\r\nReset number: %d ", WDOG_RSTCNT); str_send(buffer);
  if (WDOG_RSTCNT % 2 == 0) {  // Periodic mode.
    sprintf(buffer, "(Periodic)\r\n"); str_send(buffer);
  } else {
    sprintf(buffer, "(Windowed)\r\n"); str_send(buffer);
  }

  sprintf(buffer, "Reset source: "); str_send(buffer);
  if (rst_src & 0x1) {
		sprintf(buffer, "Low Leakage Wakeup Reset\r\n");
  } else if (rst_src & 0x2) {
		sprintf(buffer, "Low-Voltage Detect Reset\r\n");
  } else if (rst_src & 0x4) {
		sprintf(buffer, "Loss-of-Clock Reset\r\n");
	} else if (rst_src & 0x8) {
		sprintf(buffer, "Loss-of-Lock Reset\r\n");
	} else if (rst_src & 0x20) {
		sprintf(buffer, "Watchdog\r\n");
	} else if (rst_src & 0x40) {
		sprintf(buffer, "External Reset Pin\r\n");
	} else if (rst_src & 0x80) {
		sprintf(buffer, "Power-On Reset\r\n");
	} else if (rst_src & 0x100) {
		sprintf(buffer, "JTAG Generated Reset\r\n");
	} else if (rst_src & 0x200) {
		sprintf(buffer, "Core Lockup\r\n");
	} else if (rst_src & 0x400) {
		sprintf(buffer, "Software\r\n");
	} else if (rst_src & 0x800) {
		sprintf(buffer, "MDM-AP System Reset Request\r\n");
	} else if (rst_src & 0x1000) {
		sprintf(buffer, "EzPort Reset\r\n");
	} else if (rst_src & 0x2000) {
		sprintf(buffer, "Stop Mode Acknowledge Error Reset\r\n");
	} else if (rst_src & 0xffffffff) {
		sprintf(buffer, "External Reset source (Probably debugger)\r\n");
  } else {
    sprintf(buffer, "Unknown\r\n");
  }
  str_send(buffer);
}

void MCU_Init() {
  MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
  SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
}

void UART5_Init() {
    UART5->C2  &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    UART5->BDH =  0x00;
    UART5->BDL =  0x1A;  // 115 200 Bd, 1 stop bit
    UART5->C4  =  0x0F;  // Oversampling ratio 16, match address mode disabled
    UART5->C1  =  0x00;  // 8 data bits, no parity
    UART5->C3  =  0x00;
    UART5->MA1 =  0x00;  // no match address
    UART5->MA2 =  0x00;  // no match address
    UART5->S2  |= 0xC0;
    UART5->C2  |= UART_C2_TE_MASK;  // turn on transmitter.
}

// Port INIT
void Ports_Init() {
  SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
  SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;

  PORTA->PCR[4] = PORT_PCR_MUX(0x01);

  PORTB->PCR[5] = PORT_PCR_MUX(0x01);  // D9  LED
  PORTB->PCR[4] = PORT_PCR_MUX(0x01);  // D10 LED
  PORTB->PCR[3] = PORT_PCR_MUX(0x01);  // D11 LED
  PORTB->PCR[2] = PORT_PCR_MUX(0x01);  // D12 LED

  PORTE->PCR[8]  = PORT_PCR_MUX(0x03);  // UART0_TX
  PORTE->PCR[11] = PORT_PCR_MUX(0x01);  // SW6

  // set ports as output
  PTA->PDDR =  GPIO_PDDR_PDD(0x10);
  PTB->PDDR =  GPIO_PDDR_PDD(0x3C);  // LEDs as output
  PTB->PDOR |= GPIO_PDOR_PDO(0x3C);  // turn all LEDs OFF
}

void WDOG_Init() {
  WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xC520);
  WDOG_UNLOCK = WDOG_UNLOCK_WDOGUNLOCK(0xD928);

  WDOG_TOVALH = WDOG_TOVALH_TOVALHIGH(0);
  WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(3000);

    // WDOG enabled, CLK source LPO, enabled in CPU Debug mode, periodic mode
  if (WDOG_RSTCNT % 2 == 0) {  // Periodic mode
    WDOG_STCTRLH = 0b0101;
  } else {  // Windowed mode
    WDOG_STCTRLH = 0b1101;
    WDOG_WINH = WDOG_WINH_WINHIGH(0);
    WDOG_WINL = WDOG_WINL_WINLOW(2000);
    WDOG_TOVALL = WDOG_TOVALL_TOVALLOW(4000);
  }
}

int main() {
  MCU_Init();
  Ports_Init();
  UART5_Init();

  int error_cnt = 0;
  int led_interval = 300000;
  bool add = false;

  beep(A_2, 0.2);  // boot-up sound
  beep(Fs_6, 0.2);
  delay(2000000);

  WDOG_Init();

  uint32_t rst_src = rcm->SRS0 | rcm->SRS1 << 8;  // Power-On Reset or External Reset Pin
  if (!(rst_src & 32)) {
    WDOG_RSTCNT |= UINT16_MAX;
    beep(Ds_5, 1.2);  // win-xp start-up sound
    beep(Ds_4, 0.6);
    beep(As_4, 2);
    beep(Gs_4, 1.4);
    beep(Ds_4, 1.4);
    beep(Ds_5, 1.4);
    beep(As_4, 5);
    delay(2000000);
  }
  stat_send(rst_src);

  while (1) {  // fur elise -Beethoven.
    beep(E_4, 1);
    beep(Ds_4, 1);
    beep(E_4, 1);
    beep(Ds_4, 1);
    beep(E_4, 1);
    beep(B_3, 1);
    beep(D_4, 1);
    beep(C_4, 1);
    beep(A_3, 2);

    delay(500000);

    beep(C_3, 1);
    beep(E_3, 1);
    beep(A_3, 1);
    beep(B_3, 2);

    delay(500000);

    beep(E_3, 1);
    beep(A_3, 1);
    beep(B_3, 1);
    beep(C_4, 2);

    delay(1500000);

    beep(E_4, 1);
    beep(Ds_4, 1);
    beep(E_4, 1);
    beep(Ds_4, 1);
    beep(E_4, 1);
    if (error_cnt == 1) {
      if (WDOG_RSTCNT % 2 == 0) {  // simulate error in program (periodic mode).
        while(1) {  // program in infinite loop (LED lights up). watchdog time going to run out.
          GPIOB_PDOR ^= LED_D12;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          GPIOB_PDOR ^= LED_D11;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          GPIOB_PDOR ^= LED_D10;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          GPIOB_PDOR ^= LED_D9;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          GPIOB_PDOR ^= LED_D10;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          GPIOB_PDOR ^= LED_D11;
          PTB->PDOR &= ~GPIO_PDOR_PDO(0x1);
          delay(led_interval);
          PTB->PDOR |= GPIO_PDOR_PDO(0x3C);

          if (led_interval < 3) {
            add = true;
          }
          if (led_interval > 300000) {
            add = false;
          }
          if (add) {
            led_interval *= 1.15;
            led_interval++;  // to prevent 3 * 1.15 = 3.45 = 3
          } else {
            led_interval /= 1.15;
          }
        };
      } else {  // windowed refresh too soon
        sprintf(buffer, "WDOG automatically refreshed too soon!\r\n"); str_send(buffer);
        delay(500000);  // delayed so the mcu can ssend data before interrupt.
        WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
        WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);
      }
    }

    beep(B_3, 1);
    beep(D_4, 1);
    beep(C_4, 1);
    beep(A_3, 2);

    delay(500000);

    beep(C_3, 1);
    beep(E_3, 1);
    beep(A_3, 1);
    beep(B_3, 2);

    delay(500000);

    beep(E_3, 1);
    beep(C_4, 1);
    beep(B_3, 1);
    beep(A_3, 3);

    delay(2000000);  // fur-elise end pause.

    if (WDOG_RSTCNT % 2 == 0) {  // Periodic mode.
      sprintf(buffer, "WDOG automatically refreshed in time!\r\n"); str_send(buffer);
      WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
      WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);
    } else {  // Windowed mode.
      while(1) {
        if (!(GPIOE_PDIR & BTN_SW6)) {  // if button SW6 pushed.
          sprintf(buffer, "WDOG manually refreshed in time!\r\n"); str_send(buffer);
          WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xA602);
          WDOG_REFRESH = WDOG_REFRESH_WDOGREFRESH(0xB480);
          break;
        }
      }
    }

    error_cnt++;
  }
  return 0;
}
