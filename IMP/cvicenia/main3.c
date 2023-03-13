/***********************************************************************************/
/*                                                                                 */
/* Laboratorni uloha c. 3 z pRmetu IMP                                           */
/*                                                                                 */
/* Demonstrace rizeni jasu/barvy RGB LED pomoci PWM signalu generovaneho casovacem */
/*                                                                                 */
/* Reseni vytvoril a odevzdava: (Samuel Kuchta, xkucht11)                          */
/*                                                                                 */
/***********************************************************************************/

#include "MKL05Z4.h"
#define OVERFLOW 0xFF  // Doporucena hodnota preteceni casovace

void delay(long long bound) {  // Funkce zpozdeni - funkce skonci po nastavenem poctu cyklu
  long long i;
  for(i=0; i<bound; i++);
}

void MCUInit(void) { // Initialize hodin MCU, zastaveni Watchdogu (COP)
	MCG->C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
	SIM->COPC = SIM_COPC_COPT(0x00);
}

void SysInit(void) { // Povoleni hodin do modulu, ktere budou pouzivany:
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(0x01); // zdroj hodin do casovace TPM
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // povoleni hodin do portu B (pro RGB LED)
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // povoleni hodin do casovace TPM0
}

// Inicializace portu s RGB LED. Jednotlive barevne slozky LED
// budou rizeni prislusnymi kanalovymi vystupy casovace TPM0,
// a to konkretne signalem PWM, ktery bude casovacem generovan.
void PortInit(void) {
	PORTB->PCR[9] = PORT_PCR_MUX(0x02);  //  PTB9: TPM0_CH2 -> R
	PORTB->PCR[10] = PORT_PCR_MUX(0x02); // PTB10: TPM0_CH1 -> G
	PORTB->PCR[11] = PORT_PCR_MUX(0x02); // PTB11: TPM0_CH0 -> B
}

void Timer0Init(void) {
    TPM0_CNT = 0;  // 1. Vynulujte registr citace (Counter) casovace TPM0
    TPM0_MOD = OVERFLOW; // 2. Nastavte hodnotu preteceni

    // 3. Nastavte rezim generovani PWM na zvolenem kanalu (n) casovace v ridicim
    //    registru TPM0_CnSC tohoto kanalu, konkretne:
    //    Edge-aligned PWM: High-true pulses (clear Output on match, set Output on reload),
    //    preruseni ani DMA requests nebudou vyuzivany.
    TPM0_C0SC = 0b00101000;
    TPM0_C1SC = 0b00101000;
    TPM0_C2SC = 0b00101000;

    // 4. Nastavte konfiguraci casovace v jeho stavovem a ridicim registru (SC):
    //    (up counting mode pro Edge-aligned PWM, Clock Mode Selection (01),
    //    Prescale Factor Selection (Divide by 8), bez vyuziti preruseni ci DMA.
    //    POZN.: !!!!! Budete-li masky v SC registru nastavovat postupne, je NUTNO
    //    toto provadet pri Clock Mode Selection = 00 (tj. v rezimu TPM disabled).
    TPM0_SC = 0b00001011;
}

int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

int max(int a, int b) {
  if (a >= b) {
    return a;
  } else {
    return b;
  }
}

int main(void) {
  int increment = 1; // Priznak zvysovani (1) ci snizovani (0) hodnoty compare
  unsigned int compare = 0; // Hodnota pro komparacni registr (urcujici stridu PWM).
  unsigned int compare_multi = 0; // prechod medzi farbami
  signed int R = OVERFLOW, G = 0, B = 0;

  MCUInit();
  SysInit();
  PortInit();
  Timer0Init();

  R = OVERFLOW;

  while (1) {
	/*
    if (increment) {
      compare++;  // Zvysuj stridu
      increment = !(compare >= OVERFLOW);
    } else {
      compare--;  // Snizuj stridu
      increment = (compare == 0x00);
    }
      // 5. Priradte aktualni hodnotu compare do komparacniho registru zvoleneho kanalu casovace TPM0
    TPM0_C0V = compare;
    TPM0_C1V = compare;
    TPM0_C2V = compare;
	*/


    if (compare_multi >= 1280 || compare_multi < 512) {  // R 255	255	0	  0	  0	  255
      R = min(++R, OVERFLOW);
    } else {
      R = max(--R, 0);
    }

    if (compare_multi >= 256 && compare_multi < 1024) {  // G 0	  255	255	255	0		0
      G = min(++G, OVERFLOW);
    } else {
      G = max(--G, 0);
    }

    if (compare_multi >= 768) {  // B 0	  0	  0	  255	255	255
      B = min(++B, OVERFLOW);
    } else {
      B = max(--B, 0);
    }

    compare_multi = (compare_multi + 1) % 1536;

    TPM0_C0V = R;
    TPM0_C1V = G;
    TPM0_C2V = B;

    delay(4000);  // 6. LEDku nechte urcity cas svitit dle aktualni hodnoty stridy
  }
}
