; dscr.a51
; Contains the Device Descriptor, Configuration(Interface, HID and Endpoint) Descriptor,
; and String descriptors.
;


DSCR_DEVICE   	 equ   1   			;; Descriptor type: Device
DSCR_CONFIG  	 equ   2   			;; Descriptor type: Configuration
DSCR_STRING   	 equ   3  		 	;; Descriptor type: String
DSCR_INTRFC   	 equ   4   			;; Descriptor type: Interface
DSCR_ENDPNT   	 equ   5   			;; Descriptor type: Endpoint
DSCR_DEVQUAL  	 equ   6   			;; Descriptor type: Device Qualifier
DSCR_OTHERSPEED  equ   7 


ET_CONTROL   equ   0   				;; Endpoint type: Control
ET_ISO       equ   1   				;; Endpoint type: Isochronous
ET_BULK      equ   2   				;; Endpoint type: Bulk
ET_INT       equ   3   				;; Endpoint type: Interrupt

public	DeviceDscr,ConfigDscr,StringDscr,HIDDscr,ReportDscr,ReportDscrEnd,StringDscr0, StringDscr1, StringDscr2
public  UserDscr
 
;EZ USB FX2 control panel Vendor ID 0h, product ID 2131h
; Lakeview Research VID is 0925h.   
VID	equ	0925h
PID	equ	1234h
DID 	equ	0000h

;cseg at 0x90
DSCR   SEGMENT   CODE PAGE

;;-----------------------------------------------------------------------------
;; Global Variables
;;-----------------------------------------------------------------------------
      rseg DSCR      ;; locate the descriptor table in on-part memory.

DeviceDscr:	
	db	DeviceDscrEnd - DeviceDscr		; Descriptor length
	db	DSCR_DEVICE			; Descriptor type = DEVICE
	db	00h,01h				; spec version (BCD) is 2.00               
	db	0,0,0				; HID class is defined in the interface descriptor
	db	64				; maxPacketSize
	db	LOW(VID),HIGH(VID)
	db	LOW(PID),HIGH(PID)
	db	LOW(DID),HIGH(DID)
	db  	 0        			; Manufacturer string index
      	db  	 0         			; Product string index
      	db  	 0         			; Serial number string index
      	db  	 1         			; Number of configurations
DeviceDscrEnd:



ConfigDscr:
	db	ConfigDscrEnd - ConfigDscr		; Descriptor length
	db	DSCR_CONFIG			; Descriptor type = OTHER SPEED CONFIG
	db	LOW(word_allignment-ConfigDscr)	; Total length (conf+interface+HID+EP's)
	db	HIGH(word_allignment-ConfigDscr)
	db	01h				; Number of interfaces
	db	01h				; Value to select this interface
	db	00h				; String index to describe this config
	db	10100000b			; b7=1; b6=self-powered; b5=Remote WU
	db	0d				; bus power = 80 ma
ConfigDscrEnd:
 
HIDIntrfcDscr:				; Interface Descriptor
	db	HIDIntrfcDscrEnd -HIDIntrfcDscr	; Descriptor length
	db	DSCR_INTRFC			; Descriptor type: INTERFACE
	db	0,0				; Interface 0, Alternate setting 0
	db	01h				; Number of endpoints
	db	03h,01,02			; Class(03)HID, Boot Interface sub class Interface sub sub class (Mouse)
	db	0h				; string index for this interface
HIDIntrfcDscrEnd:


HIDDscr:
	db	HIDDscrEnd -HIDDscr	; Descriptor length
	db	21h				; Descriptor type - HID
	db	10h,01h				; HID Spec version 1.10
	db	0				; country code(none)
	db	01h				; number of HID class descriptors
	db	22h				; class descriptor type: REPORT
	db	LOW(ReportDscrEnd - ReportDscr)
	db	HIGH(ReportDscrEnd - ReportDscr)
HIDDscrEnd:

HIDEpInDscr:	
	db	HIDEpInDscrEnd -HIDEpInDscr	; Descriptor length
	db	DSCR_ENDPNT			; Descriptor type : ENDPOINT
	db	81h				; IN-1
	db	ET_INT				; Type: INTERRUPT
	db	40h,0				; maxPacketSize = 64
	db	01h				; polling interval is 50 msec
HIDEpInDscrEnd:


word_allignment:

    	db  00h         ;Word alignment

;; usbhidio code start
ReportDscr:
        db 05h, 01h     ; 	Usage Page (Generic Desktop),
	db 09h, 02h     ; 	Usage (Mouse),
	db 0A1h, 01h    ; 	Collection (Application),
	db 09h, 01h     ;           Usage (Pointer), 
	db 0A1h, 00h	;	    Collection (Physical),
	db 95h, 03h     ;               Report Count (3),
	db 75h, 01h     ;               Report Size (1),
	db 05h, 09h	;	        Usage Page (Buttons),
	db 19h, 01h	;	        Usage minimum (1)
	db 29h, 03h	;	        Usage maximum (3)
	db 15h, 00h     ;               Logical minimum (0),
	db 25h, 01h     ;               Logical maximum (1),	
	db 81h, 02h     ;               Input (Data, Variable, Absolute), (3 button bits)
	db 95h, 01h     ;               Report Count (1),
	db 75h, 05h     ;               Report Size (5),
	db 81h, 01h     ;               Input (Constant)
	db 75h, 08h	;		Report Size (8)
	db 95h, 02h	;               Report Count (2)              
	db 05h, 01h     ;               Usage Page (Generic Desktop),
	db 09h, 30h     ;               Usage (X),
	db 09h, 31h     ;               Usage (Y),
	db 16h, 01h,80h     ;               Logical Minimum (-127),
	db 26h, 0ffh,7Fh     ;               Logical Maximum (+127),	
	db 81h, 06h     ;               Input (Data, Variable, Relative), (2 position bytes - X & Y)
	db 0C0h         ;            End Collection
	db 0C0h         ;        End Collection
ReportDscrEnd:

ReportEnd_word_allignment:
;; Alignment unneeded for usbhidio report because it has an even number of bytes.
  ;;	db  00h         ;Force word alignment



StringDscr:
StringDscr0:
		db	StringDscr0End-StringDscr0		;; String descriptor length
		db	DSCR_STRING
		db	09H,04H
StringDscr0End:

StringDscr1:	
		db	StringDscr1End-StringDscr1		;; String descriptor length
		db	DSCR_STRING
		db	'C',00
		db	'y',00
		db	'p',00
		db	'r',00
		db	'e',00
		db	's',00
		db	's',00
StringDscr1End:

StringDscr2:	
		db	StringDscr2End-StringDscr2		;; Descriptor length
		db	DSCR_STRING
		db	'E',00
		db	'Z',00
		db	'-',00
		db	'M',00
		db	'o',00
		db	'u',00
		db	's',00
		db	'e',00
StringDscr2End:

UserDscr:		
		dw	0000H
		end

