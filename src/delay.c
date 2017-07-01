#include "system.h"

/* --------------------------------------------------------
 Vstup    :  -
 Vystup   :  -
 Navrat   :  -
 Popis    :  funkce pro vytvoreni spozdeni 1us
             parametr je nastaven v platforme
 ------------------------------------------------------- */
void DelayCPU_Body() {
   VU32 dl;
   dl = CPU_DELAY_CALIB;
   while (--dl);
}


/* --------------------------------------------------------
 Vstup    :  -
 Vystup   :  -
 Navrat   :  -
 Popis    :  Delay, velmi priblizna doba cekani v uS
 ------------------------------------------------------- */
void DelayCPU_uS(U32 u32Us){
   int i;
   for(i=0;i<u32Us; i++) {
      DelayCPU_Body();
   }
}

/* =========================== Konec ====================================== */

