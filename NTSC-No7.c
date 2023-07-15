#include <xc.h>
#include <sys/attribs.h>
#include <string.h>


#include "configuration.h"
#include "definitions.h"
#include "device.h"


//	NTSC.c     initial
//	NTSC-No2.c ram 縮小
//	NTSC-No3.c interlace(同期信号のみ)
//	NTSC-No4.c 水平解像度も2倍にする


//#define PSET1(x,y) {if((x)>=0 && (x)<=232 && (y)>=0 && (y)<=160)video[(y)+56][(x)/32+2] |= (0x80000000>>((x)%32));}
//#define PSET1(x,y) {if((x)>=0 && (x)<=232 && (y)>=0 && (y)<=160)video[(y)][(x)/32+2] |= (0x80000000>>((x)%32));}

uint32_t v_sync1[10];
uint32_t v_sync21[10];
uint32_t v_sync23[10];
uint32_t h_sync[10];
//uint32_t sync[262][10];
//uint32_t video[262][10];                // 10,480 byte
uint32_t video[321][20];                // 25,680 byte
const uint32_t video_0[20];

/********************************************************************************/
/*		PSET1																	*/
//		y=0    偶数行 0    video[0]
//		y=1    奇数行 0    video[161]
//		y=2    偶数行 1    video[1]
//		y=3    奇数行 1    video[162]
//
//		y=317  奇数行 158  video[319]
//		y=318  偶数行 159  video[159]
//		y=319  奇数行 159  video[320]
//		y=320  偶数行 160  video[160]
/********************************************************************************/
void PSET1(int x, int y, int c)
{
	uint32_t bit;
	int byte;
	
	if(x<0 || x>464 || y<0 || y>320)
		return;
	if((y & 1)==0){
		y /=2;
		byte = x/32+4;
		bit = 0x80000000 >> x%32;
		if(c)
			video[y][byte] |= bit;
		else
			video[y][byte] &= ~bit;
	}
	else{
		y = y/2;
		byte = x/32+4;
		bit = 0x80000000 >> x%32;
		if(c)
			video[y+161][byte] |= bit;
		else
			video[y+161][byte] &= ~bit;
	}
}

int PGET1(int x, int y)
{
	uint32_t bit;
	int byte;
	
	if(x<0 || x>464 || y<0 || y>320)
		return 0;
	if((y & 1)==0){			// y:0,2,4...,320 → video[0],[1]...[160]
		y /=2;
		byte = x/32+4;
		bit = 0x80000000 >> x%32;
		return (video[y][byte] & bit) ? 1: 0;
	}
	else{					// y:1,3,5...319 → video[161],[162]...[320]
		y = y/2;
		byte = x/32+4;
		bit = 0x80000000 >> x%32;
		return (video[y+161][byte] & bit) ? 1: 0;
	}
}

/********************************************************************************/
/*		sync_data_set															*/
//
//      IO clock = 20MHz
//      shift clock = 20MHz/2/(1+1) = 5MHz  1bit:0.2μs
//      同期信号 10転送 = 32x0.2x10 = 64μs
//      h_sync: 4.8μs(24bit) low, 59.2μs(296bit) high
//      v_sync: 59.2μs(296bit) high, 4.8μs(24bit) low
//      同期信号は v_sync:3cycle + h_sync:259cycle
//
//		v_sync1  x3
//		h_sync   x259
//		v_sync21 x1
//		v_sync1  x2
//		v_sync23 x1
//		h_sync   x259
/********************************************************************************/
void sync_data_set()
{
    int i, byte;
    uint32_t bit;

	bit = 0x80000000;
	byte = 0;
	for(i=0; i<10*32; i++){
		if(i<296)
			v_sync1[byte] &= ~bit;
		else
			v_sync1[byte] |= bit;
		
		if(i<24)
			v_sync21[byte] &= ~bit;
		else if(i<160)
			v_sync21[byte] |= bit;
		else if(i<296)
			v_sync21[byte] &= ~bit;
		else
			v_sync21[byte] |= bit;
		
		if(i<160)
			v_sync23[byte] &= ~bit;
		else
			v_sync23[byte] |= bit;
		
		if(i<24)
			h_sync[byte] &= ~bit;
		else
			h_sync[byte] |= bit;
		if((bit>>=1)==0){
			bit = 0x80000000;
			++byte;
		}
	}

#if 0
    bit = 0x80000000;
    byte = 0;
    for(i=0; i<10*32; i++){
		if(i<296)
			v_sync[byte] &= ~bit;
		else
			v_sync[byte] |= bit;
		
		if(i<24)
			h_sync[byte] &= ~bit;
		else
			h_sync[byte] |= bit;
		if((bit>>=1)==0){
			bit = 0x80000000;
			++byte;
		}
	}
#endif
}

/********************************************************************************/
/*		spi_init																*/
/********************************************************************************/
void spi_init()
{
//	int i;

	sync_data_set();

	/*** SPI setting	***/
	RPB2R = 4;								// SDO2
	SPI2CON = 0x1083C;						// 32bit
	SPI2CONSET = 0x8000;					// Enable
	SPI2BRG = 0;							// 20MHz/2/(0+1)=10MHz
	
	RPB13R = 3;								// SDO1
	SPI1CON = 0x1083C;						// 32bit
	SPI1CONSET = 0x8000;					// Enable
	SPI1BRG = 1;							// 20MHz/2/(1+1)=5MHz
//	IPC7bits.SPI1IP = 1;
//	IFS1bits.SPI1TXIF = 0;
//	IEC1bits.SPI1TXIE = 1;
	
//    for(;;){
//        SPI1BUF = 0x55555555;//AAAAA
//        SPI2BUF = 0x55555555;
//
//        volatile int cnt;
//       	for(cnt=0; cnt<100; cnt++)
//       		;
//    }
        
    
	/*** DMA setting	***/
	DMACON = 0x8000;						// DMA Enable
	
	//channel 2
	DCH2CON = 0;
	DCH2CONbits.CHAEN = 0;					// channel automatic off
	DCH2ECONbits.CHSIRQ = _SPI1_TX_IRQ;
	DCH2ECONbits.SIRQEN = 1;				// channel start IRQ enable bit
	DCH2SSA = ((uint32_t) v_sync1)&0x1FFFFFFF;
	DCH2DSA = ((uint32_t) (&SPI1BUF))&0x1FFFFFFF;
	DCH2SSIZ = sizeof(v_sync1);
	DCH2DSIZ = 4;
	DCH2CSIZ = 4;							// 1回の転送バイト数
	DCH2INTbits.CHBCIE = 1;					// Block transfer complete interrupt
	DCH2INTbits.CHBCIF = 0;					// Block transfer complete interrupt flag clear
	IPC10bits.DMA2IP = 5;
	IFS1bits.DMA2IF = 0;
	IEC1bits.DMA2IE = 1;
	
	//channel 3
	DCH3CON = 0;
	DCH3CONbits.CHAEN = 0;					// channel automatic off
	DCH3ECONbits.CHSIRQ = _SPI2_TX_IRQ;
	DCH3ECONbits.SIRQEN = 1;				// channel start IRQ enable bit
	DCH3SSA = ((uint32_t) video_0)&0x1FFFFFFF;
	DCH3DSA = ((uint32_t) (&SPI2BUF))&0x1FFFFFFF;
	DCH3SSIZ = sizeof(video_0);
	DCH3DSIZ = 4;
	DCH3CSIZ = 4;							// 1回の転送バイト数
	DCH3INTbits.CHBCIE = 1;					// Block transfer complete interrupt
	DCH3INTbits.CHBCIF = 0;					// Block transfer complete interrupt flag clear
	IPC10bits.DMA3IP = 5;
	IFS1bits.DMA3IF = 0;
	IEC1bits.DMA3IE = 1;

	__builtin_disable_interrupts();
	DCH2CONbits.CHEN = 1;					// DMA ch2 enable
	DCH3CONbits.CHEN = 1;					// DMA ch3 enable
	__builtin_enable_interrupts();
	
	/*** timer2 setting for voice ***/
//	T2CON = 0;
//	PR2 = 255;
//	TMR2 = 0;
//    T2CONbits.ON = 1;
////	IPC2bits.T2IP = 4;
////	IFS0bits.T2IF = 0;
////	IEC0bits.T2IE = 1;
    
//    /*** timer4 setting for voice ***/
//    T4CON = 0;
//    PR4 = 1250-1;                 // 16KHz
//    TMR4 = 0;
//    T4CONbits.ON = 1;
//    IPC4bits.T4IP = 4;
//    IFS0bits.T4IF = 0;
////    IEC0bits.T4IE = 1; //AAAAA

//    /*** OC1 setting for voice ***/
//    RPB3Rbits.RPB3R = 5;    // OC1 output
//    OC1CON = 0x0000; // Turn off the OC1 when performing the setup
//    OC1R = 0x0064; // Initialize primary Compare register
//    OC1RS = 0x0064; // Initialize secondary Compare register
//    OC1CON = 0x0006; // Configure for PWM mode without Fault pin enabled
//    OC1CONSET = 0x8000; // Enable OC1
}
/********************************************************************************/
/*		interrupt																*/
/********************************************************************************/
void __ISR(_DMA2_VECTOR, IPL5AUTO) _DMA2Interrupt ()
{
	static int cnt;
	
	IFS1bits.DMA2IF = 0;					// Clear DMA ch2 IF
	DCH2INTbits.CHBCIF = 0;

	if(cnt < 263){
		if(cnt < 3+7)
			DCH2SSA = ((uint32_t) v_sync1)&0x1FFFFFFF;
		else if(cnt < 262)
			DCH2SSA = ((uint32_t) h_sync)&0x1FFFFFFF;
		else
			DCH2SSA = ((uint32_t) v_sync21)&0x1FFFFFFF;
	}
	else{
		if(cnt < 265+7)
			DCH2SSA = ((uint32_t) v_sync1)&0x1FFFFFFF;
		else if(cnt < 266+7)
			DCH2SSA = ((uint32_t) v_sync23)&0x1FFFFFFF;
		else
			DCH2SSA = ((uint32_t) h_sync)&0x1FFFFFFF;
	}
	DCH2CONbits.CHEN = 1;
    if(++cnt >= (262*2+1))
        cnt = 0;
}

void __ISR(_DMA3_VECTOR, IPL5AUTO) _DMA3Interrupt ()
{
	static int cnt= 469;
	
	IFS1bits.DMA3IF = 0;					// Clear DMA ch3 IF
	DCH3INTbits.CHBCIF = 0;

	if(cnt < 161)               // 偶数行 161
		DCH3SSA = ((uint32_t) video[cnt])&0x1FFFFFFF;
	else if(cnt < 262+1)
		DCH3SSA = ((uint32_t) video_0)&0x1FFFFFFF;
	else if(cnt < 262+1+160)    // 奇数行 160
		DCH3SSA = ((uint32_t) video[cnt-102])&0x1FFFFFFF;
	else
		DCH3SSA = ((uint32_t) video_0)&0x1FFFFFFF;
	
	DCH3CONbits.CHEN = 1;
    if(++cnt >= (262*2+1))
        cnt = 0;
}

unsigned char *timer4data;
long timer4cnt, timer4end;

//void __ISR(_TIMER_4_VECTOR, IPL4AUTO) T2Interrupt()
//{
//    OC1RS = timer4data[timer4cnt];
//    if(++timer4cnt >= timer4end)
//        IEC0bits.T4IE = 0;      // 割り込み処理終了
//	IFS0bits.T4IF = 0;
//}

//void __ISR(_SPI_1_VECTOR, IPL5AUTO) SPI1Interrupt()
//{
//    static int word_cnt;
//    static int line_cnt;
//    static int video_cnt;
//    
//    if(line_cnt < 3)
//        SPI1BUF = v_sync[word_cnt];
//    else
//        SPI1BUF = h_sync[word_cnt];
//    if(++word_cnt==10){
//        word_cnt = 0;
//        if(++line_cnt==262)
//            line_cnt = 0;
//    }
//    SPI2BUF = ((uint32_t *)video)[video_cnt];
//    if(++video_cnt==10*262)
//        video_cnt = 0;
//	IFS1bits.SPI1TXIF = 0;
//}
