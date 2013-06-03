//-----------------------------------------------------------------------------
//   File:      FW.c
//-----------------------------------------------------------------------------
#include "lp.h"
#include "lpregs.h"
#include "syncdly.h"            // ͬ����ʱ��

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------
#define DELAY_COUNT   0x9248*8L  // ��ʱ����
#define _IFREQ  48000            
#define _CFREQ  48000           

//-----------------------------------------------------------------------------
// �ȽϺ���
//-----------------------------------------------------------------------------
#define   min(a,b) (((a)<(b))?(a):(b))
#define   max(a,b) (((a)>(b))?(a):(b))

//-----------------------------------------------------------------------------
// ȫ�ֱ���
//-----------------------------------------------------------------------------
volatile BOOL   GotSUD;
BOOL      Rwuen;
BOOL      Selfpwr;
volatile BOOL   Sleep;                  // ����ģʽʹ���ź�

WORD   pDeviceDscr;   
WORD   pDeviceQualDscr;
WORD   pHighSpeedConfigDscr;
WORD   pFullSpeedConfigDscr;   
WORD   pConfigDscr;
WORD   pOtherConfigDscr;   
WORD   pStringDscr;   

//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
void SetupCommand(void);
void TD_Init(void);
void TD_Poll(void);
BOOL TD_Suspend(void);
BOOL TD_Resume(void);

BOOL DR_GetDescriptor(void);
BOOL DR_SetConfiguration(void);
BOOL DR_GetConfiguration(void);
BOOL DR_SetInterface(void);
BOOL DR_GetInterface(void);
BOOL DR_GetStatus(void);
BOOL DR_ClearFeature(void);
BOOL DR_SetFeature(void);
BOOL DR_VendorCmnd(void);
BOOL DR_LED1(void);						//��ʾ����0
BOOL DR_LED2(void);						//��ʾ����1
BOOL DR_LED3(void);						//��ʾ����2
BOOL DR_LED4(void);					   	//��ʾ�ַ�A
BOOL DR_LED5(void);						//��ʾ����F

// �˵�ӳ��
const char code  EPCS_Offset_Lookup_Table[] =
{
	0,    // EP1OUT
		1,    // EP1IN
		2,    // EP2OUT
		2,    // EP2IN
		3,    // EP4OUT
		3,    // EP4IN
		4,    // EP6OUT
		4,    // EP6IN
		5,    // EP8OUT
		5,    // EP8IN
};

#define epcs(EP) (EPCS_Offset_Lookup_Table[(EP & 0x7E) | (EP > 128)] + 0xE6A1)

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------

//�����߳�
void main(void)
{
	DWORD   i;
//	DWORD j;
	WORD   offset;
	DWORD   DevDescrLen;
	DWORD   j=0;
	WORD   IntDescrAddr;
	WORD   ExtDescrAddr;
		 DWORD ii;
	
	// ��ʼ��
	Sleep = FALSE;               // ��ֹ����ģʽ
	Rwuen = FALSE;               // ��ֹԶ�̻���
	Selfpwr = FALSE;            // ��ֹ�Թ���
	GotSUD = FALSE;       
	
	// ��ʼ���û��豸
	TD_Init();
	
	//����USB������
	pDeviceDscr = (WORD)&DeviceDscr;
	pDeviceQualDscr = (WORD)&DeviceQualDscr;
	pHighSpeedConfigDscr = (WORD)&HighSpeedConfigDscr;
	pFullSpeedConfigDscr = (WORD)&FullSpeedConfigDscr;
	pStringDscr = (WORD)&StringDscr;
	
    if ((WORD)&DeviceDscr & 0xC000)
	{
		// �ض���������
		IntDescrAddr = INTERNAL_DSCR_ADDR;
		ExtDescrAddr = (WORD)&DeviceDscr;
		DevDescrLen = (WORD)&UserDscr - (WORD)&DeviceDscr + 2;
		for (i = 0; i < DevDescrLen; i++)
			*((BYTE xdata *)IntDescrAddr+i) = *((BYTE xdata *)ExtDescrAddr+i);
		
		// ����������ָ��
		pDeviceDscr = IntDescrAddr;
		offset = (WORD)&DeviceDscr - INTERNAL_DSCR_ADDR;
		pDeviceQualDscr -= offset;
		pConfigDscr -= offset;
		pOtherConfigDscr -= offset;
		pHighSpeedConfigDscr -= offset;
		pFullSpeedConfigDscr -= offset;
		pStringDscr -= offset;
	}
	
	EZUSB_IRQ_ENABLE();            // ʹ��USB�ж�
	EZUSB_ENABLE_RSMIRQ();            //Զ�̻����ж�
	
	INTSETUP |= (bmAV2EN | bmAV4EN);     // ʹ��INT 2 & 4 �Զ�����
	
	USBIE |= bmSUDAV | bmSUTOK | bmSUSP | bmURES | bmHSGRANT;   // ʹ��ѡ����ж�
	EA = 1;                  // ʹ��8051ȫ���ж�
	
#ifndef NO_RENUM
	// ������о�
	if(!(USBCS & bmRENUM))
	{
		EZUSB_Discon(TRUE);   //���о�
	}
#endif
	
	// ����
	USBCS &=~bmDISCON;
	
	CKCON = (CKCON&(~bmSTRETCH)) | FW_STRETCH_VALUE;
	
	//��Sleep���
	Sleep = FALSE;
	
	//�����س��߳�
	while(TRUE)               //��ѭ��
	{
		//�о��û��豸
		TD_Poll();
		
		if(GotSUD)
		{
			SetupCommand();          
			GotSUD = FALSE;          // ��SETUP���
		}
		
		//��鲢����
		if (Sleep)
		{
			if(TD_Suspend())
			{ 
				Sleep = FALSE;     //��Sleep���
				do
				{
					EZUSB_Susp();         //����״̬����
				}
				while(!Rwuen && EZUSB_EXTWAKEUP());
				EZUSB_Resume();   
				TD_Resume();
			}   
		}
		

		
        for(ii=0;ii<=18;ii+=1) //�ɰ�����
		{			
			for(j=1;j<5;j++)
			{ 
				IOA=0xff;				
				EZUSB_Delay(ii);
				
				IOA=0x00;
				EZUSB_Delay(18-ii);				
			}			
		}

		EZUSB_Delay(1000);

		for(ii=0;ii<=18;ii+=1) //�����䰵
		{			
			for(j=1;j<5;j++)
			{ 
				IOA=0x00;				
				EZUSB_Delay(ii);
				
				IOA=0xFF;
				EZUSB_Delay(18-ii);				
			}			
		}	
		
	}
}

BOOL HighSpeedCapable()
{
	// ����USB����
	
	if (REVID & 0xF0)    //�����fx2lp/fx1����fx2.  
	{
		
		// fx2lp �� fx1 �Ĵ���
		if (GPCR2 & bmHIGHSPEEDCAPABLE)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		// fx2 �Ĵ���
		return TRUE;
	}
}   

// �豸����
void SetupCommand(void)
{
	void   *dscr_ptr;
	
	switch(SETUPDAT[1])
	{
	case SC_GET_DESCRIPTOR:                  //���������
		if(DR_GetDescriptor())
            switch(SETUPDAT[3])         
		{
	case GD_DEVICE:            //�豸
		SUDPTRH = MSB(pDeviceDscr);
		SUDPTRL = LSB(pDeviceDscr);
		break;
	case GD_DEVICE_QUALIFIER:            //�豸�޶�
		if (HighSpeedCapable())
		{
			SUDPTRH = MSB(pDeviceQualDscr);
			SUDPTRL = LSB(pDeviceQualDscr);
		}
		else
		{
			EZUSB_STALL_EP0();
		}
		break;
	case GD_CONFIGURATION:         // ����
		SUDPTRH = MSB(pConfigDscr);
		SUDPTRL = LSB(pConfigDscr);
		break;
	case GD_OTHER_SPEED_CONFIGURATION:  // ������������
		SUDPTRH = MSB(pOtherConfigDscr);
		SUDPTRL = LSB(pOtherConfigDscr);
		break;
	case GD_STRING:            // �ַ���
		if(dscr_ptr = (void *)EZUSB_GetStringDscr(SETUPDAT[2]))
		{
			SUDPTRH = MSB(dscr_ptr);
			SUDPTRL = LSB(dscr_ptr);
		}
		else 
			EZUSB_STALL_EP0();   // ��ֹ�˵�0
		break;
	default:            // ��Ч����
		EZUSB_STALL_EP0();      // ��ֹ�˵�0
		}
		break;
	case SC_GET_INTERFACE:                  // ��ýӿ�
		DR_GetInterface();
		break;
	case SC_SET_INTERFACE:                  // ���ýӿ�
		DR_SetInterface();
		break;
	case SC_SET_CONFIGURATION:               //��������
		DR_SetConfiguration();
		break;
	case SC_GET_CONFIGURATION:               //�������
		DR_GetConfiguration();
		break;
	case SC_GET_STATUS:                  // ���״̬
		if(DR_GetStatus())
            switch(SETUPDAT[0])
		{
	case GS_DEVICE:            // �豸
		EP0BUF[0] = ((BYTE)Rwuen << 1) | (BYTE)Selfpwr;
		EP0BUF[1] = 0;
		EP0BCH = 0;
		EP0BCL = 2;
		break;
	case GS_INTERFACE:         // �ӿ�
		EP0BUF[0] = 0;
		EP0BUF[1] = 0;
		EP0BCH = 0;
		EP0BCL = 2;
		break;
	case GS_ENDPOINT:         // �˵�
		EP0BUF[0] = *(BYTE xdata *) epcs(SETUPDAT[4]) & bmEPSTALL;
		EP0BUF[1] = 0;
		EP0BCH = 0;
		EP0BCL = 2;
		break;
	default:            //��Ч����
		EZUSB_STALL_EP0();    
		}
		break;
	case SC_CLEAR_FEATURE:                  // �������
		if(DR_ClearFeature())
            switch(SETUPDAT[0])
		{
	case FT_DEVICE:            // �豸
		if(SETUPDAT[2] == 1)
			Rwuen = FALSE;       // ��ֹԶ�̻���
		else
			EZUSB_STALL_EP0(); 
		break;
	case FT_ENDPOINT:     
		if(SETUPDAT[2] == 0)
		{
			*(BYTE xdata *) epcs(SETUPDAT[4]) &= ~bmEPSTALL;
			EZUSB_RESET_DATA_TOGGLE( SETUPDAT[4] );
		}
		else
			EZUSB_STALL_EP0();  
		break;
		}
		break;
	case SC_SET_FEATURE:                  // ��������
		if(DR_SetFeature())
            switch(SETUPDAT[0])
		{
	case FT_DEVICE:        
		if(SETUPDAT[2] == 1)
			Rwuen = TRUE;     
		else if(SETUPDAT[2] == 2)
			break;
		else
			EZUSB_STALL_EP0();  
		break;
	case FT_ENDPOINT:     
		*(BYTE xdata *) epcs(SETUPDAT[4]) |= bmEPSTALL;
		break;
	default:
		EZUSB_STALL_EP0();     
		}
		break;
	case SC_LED1:						  //��ʾ����0
		DR_LED1();
		break;
	default:                     		// ��Ч����
		if(DR_VendorCmnd())
            EZUSB_STALL_EP0();            // ֹͣ�ϵ�0
   }
   
   //����
   EP0CS |= bmHSNAK;
}

// �����ж�
void resume_isr(void) interrupt WKUP_VECT
{
	EZUSB_CLEAR_RSMIRQ();
}


