#pragma NOIV               // Do not generate interrupt vectors
/*
-----------------------------------------------------------------------------
   File:      usbhidio_fx2.c
   Contents:   Hooks required to implement USB peripheral function.

   This is an adaptation of the Cypress example FX2_Hid_Keyboard.c,
   which was in turn adapted from Cypress's bulkloop.c 
   (and is available on request from Cypress).

   The application communicates with the usbhdio host applications available from:
 
   www.Lvr.com/hidpage.htm

   This code requires the full (not evaluation) version of the Keil compiler.
   Additions to the keyboard code are labeled with "usbhidio start" and "usbhidio end"
   Unneeded keyboard code is commented out.
  
   Copyright (c) 2000 Cypress Semiconductor All rights reserved
   with some changes by Jan Axelson (jan@Lvr.com) 
-----------------------------------------------------------------------------
*/

#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"            // SYNCDELAY macro

extern BOOL GotSUD;             // Received setup data flag
extern BOOL Sleep;
extern BOOL Rwuen;
extern BOOL Selfpwr;

#define	min(a,b) (((a)<(b))?(a):(b))

#define GD_HID	0x21
#define GD_REPORT	0x22
#define CR_SET_REPORT 0x09
#define HID_OUTPUT_REPORT 2

#define BTN_ADDR		0x71
#define LED_ADDR		0x72

#define PF_IDLE			0
#define PF_GETKEYS		1

#define KEY_WAKEUP		0
#define KEY_F1			1
#define KEY_F2			2
#define KEY_F3			3

WORD	pHIDDscr;
WORD	pReportDscr;
WORD	pReportDscrEnd;
extern code HIDDscr;
extern code  ReportDscr; 
extern code ReportDscrEnd;

BYTE Configuration;             // Current configuration
BYTE AlternateSetting;          // Alternate settings

BYTE	Configuration;		// Current configuration
BYTE	AlternateSetting;	// Alternate settings

BYTE buttons;
BYTE oldbuttons;
BYTE leds = 0xFF;

BYTE read_buttons (void);
void write_leds (BYTE d);

#define VR_NAKALL_ON    0xD0
#define VR_NAKALL_OFF   0xD1
//-----------------------------------------------------------------------------
// Task Dispatcher hooks
//   The following hooks are called by the task dispatcher.
//-----------------------------------------------------------------------------

// read_buttons and write_leds are unused by usbhidio

BYTE read_buttons (void)
{
	BYTE d;

    d = IOA & 0x70;
	d >>= 4;

	switch (d)
	{
	    case 0x06:   //PA4=0
		    d = ~0x01;
			break;
		
		case 0x05:   //PA5=0
		    d = ~0x02;
			break;		
		
		default:
		    d = 0x0F;
		    break;
	}

	d &= 0x0F; 
	return d;
}


void TD_Init(void)             // Called once at startup
{
   // set the CPU clock to 48MHz
   CPUCS = ((CPUCS & ~bmCLKSPD) | bmCLKSPD1) ;

   // set the slave FIFO interface to 48MHz
   IFCONFIG |= 0x40;

   OEA &= ~(0x30); //PA4 5 ÉèÖÃÎªÊäÈë

  // Registers which require a synchronization delay, see section 15.14
  // FIFORESET        FIFOPINPOLAR
  // INPKTEND         OUTPKTEND
  // EPxBCH:L         REVCTL
  // GPIFTCB3         GPIFTCB2
  // GPIFTCB1         GPIFTCB0
  // EPxFIFOPFH:L     EPxAUTOINLENH:L
  // EPxFIFOCFG       EPxGPIFFLGSEL
  // PINFLAGSxx       EPxFIFOIRQ
  // EPxFIFOIE        GPIFIRQ
  // GPIFIE           GPIFADRH:L
  // UDMACRCH:L       EPxGPIFTRIG
  // GPIFTRIG
  
  // Note: The pre-REVE EPxGPIFTCH/L register are affected, as well...
  //      ...these have been replaced by GPIFTC[B3:B0] registers

  // default: all endpoints have their VALID bit set
  // default: TYPE1 = 1 and TYPE0 = 0 --> BULK  
  // default: EP2 and EP4 DIR bits are 0 (OUT direction)
  // default: EP6 and EP8 DIR bits are 1 (IN direction)
  // default: EP2, EP4, EP6, and EP8 are double buffered

  // we are just using the default values, yes this is not necessary...
  EP1OUTCFG = 0xA0;
  EP1INCFG = 0xA0;
  SYNCDELAY;                    // see TRM section 15.14
  EP2CFG = 0xA2;
  SYNCDELAY;                    
  EP4CFG = 0xA0;
  SYNCDELAY;                    
  EP6CFG = 0xE2;
  SYNCDELAY;                    
  EP8CFG = 0xE0;

  // out endpoints do not come up armed
  
  // since the defaults are double buffered we must write dummy byte counts twice
  SYNCDELAY;                    
  EP2BCL = 0x80;                // arm EP2OUT by writing byte count w/skip.
  SYNCDELAY;                    
  EP2BCL = 0x80;
  SYNCDELAY;                    
  EP4BCL = 0x80;                // arm EP4OUT by writing byte count w/skip.
  SYNCDELAY;                    
  EP4BCL = 0x80;    

  // enable dual autopointer feature
  AUTOPTRSETUP |= 0x01;

  Rwuen = TRUE;                 // Enable remote-wakeup

}


void TD_Poll(void)              // Called repeatedly while the device is idle
{
	if( !(EP1INCS & 0x02) )	// Is the EP1INBUF available,
	{
	    buttons = read_buttons();
		if (buttons == read_buttons())	//·À¶¶¶¯
		{
          	EP1INBUF[0] = 0;
			if ((oldbuttons - buttons) != 0)	//Change in button state
			{
				if (!(buttons & 1))	//// Ä£Äâ×ó¼ü
				{
					EP1INBUF[0]|= 0x01;
			     }
				if (!(buttons & 2))	//// Ä£ÄâÓÒ¼ü
				{
					EP1INBUF[0] |= 0x02;
				 }
				if (buttons & 4)	//// Advance in x direction
					EP1INBUF[1] = 0;
				else
					EP1INBUF[1] = 0x0A;
				if (buttons & 8)	//// Advance in y direction
					EP1INBUF[2] = 0;
				else
					EP1INBUF[2] =0x0A;
			
				EP1INBC = 3;
			}
			oldbuttons = buttons;
		}

	}

}

BOOL TD_Suspend(void)          // Called before the device goes into suspend mode
{
   return(TRUE);
}

BOOL TD_Resume(void)          // Called after the device resumes
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// Device Request hooks
//   The following hooks are called by the end point 0 device request parser.
//-----------------------------------------------------------------------------

BOOL DR_GetDescriptor(void)
{
		return(TRUE);
}

BOOL DR_SetConfiguration(void)   // Called when a Set Configuration command is received
{
   Configuration = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetConfiguration(void)   // Called when a Get Configuration command is received
{
   EP0BUF[0] = Configuration;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_SetInterface(void)       // Called when a Set Interface command is received
{
   AlternateSetting = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetInterface(void)       // Called when a Set Interface command is received
{
   EP0BUF[0] = AlternateSetting;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_GetStatus(void)
{
   return(TRUE);
}

BOOL DR_ClearFeature(void)
{
   return(TRUE);
}

BOOL DR_SetFeature(void)
{
   return(TRUE);
}

BOOL DR_VendorCmnd(void)
{
  BYTE tmp;
  
  switch (SETUPDAT[1])
  {
     case VR_NAKALL_ON:
        tmp = FIFORESET;
        tmp |= bmNAKALL;      
        SYNCDELAY;                    
        FIFORESET = tmp;
        break;
     case VR_NAKALL_OFF:
        tmp = FIFORESET;
        tmp &= ~bmNAKALL;      
        SYNCDELAY;                    
        FIFORESET = tmp;
        break;
     default:
        return(TRUE);
  }

  return(FALSE);
}

//-----------------------------------------------------------------------------
// USB Interrupt Handlers
//   The following functions are called by the USB interrupt jump table.
//-----------------------------------------------------------------------------

// Setup Data Available Interrupt Handler
void ISR_Sudav(void) interrupt 0
{
   GotSUD = TRUE;            // Set flag
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUDAV;         // Clear SUDAV IRQ
}

// Setup Token Interrupt Handler
void ISR_Sutok(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUTOK;         // Clear SUTOK IRQ
}

void ISR_Sof(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSOF;            // Clear SOF IRQ
}

void ISR_Ures(void) interrupt 0
{
   // whenever we get a USB reset, we should revert to full speed mode
   
   ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
 //  pOtherConfigDscr = pHighSpeedConfigDscr;
//   ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmURES;         // Clear URES IRQ
}

void ISR_Susp(void) interrupt 0
{
   Sleep = TRUE;
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUSP;
}

void ISR_Highspeed(void) interrupt 0
{
 //  if (EZUSB_HIGHSPEED())
 //  {
 //     pConfigDscr = pHighSpeedConfigDscr;
 //     ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
 //     pOtherConfigDscr = pFullSpeedConfigDscr;
  //    ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
 //  }

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmHSGRANT;
}



void ISR_Ep0ack(void) interrupt 0
{
}
void ISR_Stub(void) interrupt 0
{
}
void ISR_Ep0in(void) interrupt 0
{
}
void ISR_Ep0out(void) interrupt 0
{
}
void ISR_Ep1in(void) interrupt 0
{
}
void ISR_Ep1out(void) interrupt 0
{
}
void ISR_Ep2inout(void) interrupt 0
{
}
void ISR_Ep4inout(void) interrupt 0
{
}
void ISR_Ep6inout(void) interrupt 0
{
}
void ISR_Ep8inout(void) interrupt 0
{
}
void ISR_Ibn(void) interrupt 0
{
}
void ISR_Ep0pingnak(void) interrupt 0
{
}
void ISR_Ep1pingnak(void) interrupt 0
{
}
void ISR_Ep2pingnak(void) interrupt 0
{
}
void ISR_Ep4pingnak(void) interrupt 0
{
}
void ISR_Ep6pingnak(void) interrupt 0
{
}
void ISR_Ep8pingnak(void) interrupt 0
{
}
void ISR_Errorlimit(void) interrupt 0
{
}
void ISR_Ep2piderror(void) interrupt 0
{
}
void ISR_Ep4piderror(void) interrupt 0
{
}
void ISR_Ep6piderror(void) interrupt 0
{
}
void ISR_Ep8piderror(void) interrupt 0
{
}
void ISR_Ep2pflag(void) interrupt 0
{
}
void ISR_Ep4pflag(void) interrupt 0
{
}
void ISR_Ep6pflag(void) interrupt 0
{
}
void ISR_Ep8pflag(void) interrupt 0
{
}
void ISR_Ep2eflag(void) interrupt 0
{
}
void ISR_Ep4eflag(void) interrupt 0
{
}
void ISR_Ep6eflag(void) interrupt 0
{
}
void ISR_Ep8eflag(void) interrupt 0
{
}
void ISR_Ep2fflag(void) interrupt 0
{
}
void ISR_Ep4fflag(void) interrupt 0
{
}
void ISR_Ep6fflag(void) interrupt 0
{
}
void ISR_Ep8fflag(void) interrupt 0
{
}
void ISR_GpifComplete(void) interrupt 0
{
}

void ISR_GpifWaveform(void) interrupt 0
{
}
