#ifndef __INC_DELAY_H_
#define __INC_DELAY_H_

/* Header platform.h musi obsahovat konstantu CPU_DELAY_CALIB, ktera funkci zhruba kalibruje
*/

/* --------------------------------------------------------
 Vstup    :  -
 Vystup   :  -
 Navrat   :  -
 Popis    :  Delay, velmi priblizna doba cekani v uS
 ------------------------------------------------------- */
void DelayCPU_uS(U32 u32Us);

/* =========================== Konec ====================================== */

#endif  /* __INC_DELAY_H_ */
