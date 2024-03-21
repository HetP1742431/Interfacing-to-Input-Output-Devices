// Include FreeRTOS Libraries
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Include xilinx Libraries
#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"

// Other miscellaneous libraries
#include "pmodkypd.h"
#include "sleep.h"
#include "xil_cache.h"

/* Parameter definitions */
// Device ID declarations
#define KYPD_DEVICE_ID   	XPAR_AXI_KEYPAD_DEVICE_ID

/*************************** Enter your code here ****************************/
// TODO: Define the seven-segment display (SSD) device id here. to do this
// look into xparameters.h
#define SSD_DEVICE_ID 		XPAR_AXI_SSD_DEVICE_ID
/*****************************************************************************/

// keypad key table
#define DEFAULT_KEYTABLE 	"0FED789C456B123A"

// Declaring the devices
PmodKYPD 	KYPDInst;

/*************************** Enter your code here ****************************/
// TODO: Declare the seven-segment display (SSD) device here
XGpio SSDInst;
/*****************************************************************************/

// Function prototypes
void InitializeKeypad();
static void keypadTask( void *pvParameters );
u32 SSD_decode(u8 key_value, u8 cathode);

int main(void)
{
	int status;

	// Initialize keypad
	InitializeKeypad();

/*************************** Enter your code here ****************************/
	// TODO: Initialize SSD
	XGpio_Initialize(&SSDInst, SSD_DEVICE_ID);
	// TODO: Set SSD GPIO direction to output after initialization
	XGpio_SetDataDirection(&SSDInst, 1, 0x00);
/*****************************************************************************/

	xil_printf("Initialization Complete, System Ready!\n");


	xTaskCreate(keypadTask,					/* The function that implements the task. */
				"main task", 				/* Text name for the task, provided to assist debugging only. */
				configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
				NULL, 						/* The task parameter is not used, so set to NULL. */
				tskIDLE_PRIORITY,			/* The task runs at the idle priority. */
				NULL);

	vTaskStartScheduler();
	while(1);
	return 0;
}

static void keypadTask( void *pvParameters )
{
   u16 keystate;
   XStatus status, last_status = KYPD_NO_KEY;
   u8 new_key, current_key = 'x', previous_key = 'x';
   u32 ssd_value=0;

/*************************** Enter your code here ****************************/
   // TODO: Define a constant of type TickType_t named 'xDelay' and initialize
   // it with a value of 200 divided by portTICK_RATE_MS for the SSD delay
   const TickType_t xDelay = 10 / portTICK_RATE_MS;
/*****************************************************************************/

   xil_printf("Pmod KYPD app started. Press any key on the Keypad.\r\n");
   while (1) {
	  // Capture state of the keypad
	  keystate = KYPD_getKeyStates(&KYPDInst);

	  // Determine which single key is pressed, if any
	  // if a key is pressed, store the value of the new key in new_key
	  status = KYPD_getKeyPressed(&KYPDInst, keystate, &new_key);

	  // Print key detect if a new key is pressed or if status has changed
	  if (status == KYPD_SINGLE_KEY && last_status == KYPD_NO_KEY){
		 xil_printf("Key Pressed: %c\r\n", (char) new_key);
		 // pass the value of current_key to previous_key
		 previous_key = current_key;
		 // store the new key pressed by the user
		 current_key = new_key;
	  } else if (status == KYPD_MULTI_KEY && status != last_status){
		 xil_printf("Error: Multiple keys pressed\r\n");
	  }

	  if (status != last_status) {
	  		  xil_printf("Status Changed: %d\r\n", status);
	  	  }

	  last_status = status;

/*************************** Enter your code here ****************************/
	  // TODO: Use the SSD_decode function to convert 'current_key' to its
	  // binary representation and store it in 'ssd_value'.
	  ssd_value = SSD_decode(current_key, 0);
	  // TODO: Write 'ssd_value' to the right digit of the SSD.
	  XGpio_DiscreteWrite(&SSDInst, 1, ssd_value);
	  // TODO: Modify the above code to display 'ssd_value' on the left SSD digit instead.

	  // TODO: Implement alternating display logic.
	  // Display 'current_key' on the right SSD, then pause. Next, display
	  // 'previous_key' on the left SSD and pause again.
	  // Experiment with delay timings to reduce flickering.
	  ssd_value = SSD_decode(current_key, 1);
	  XGpio_DiscreteWrite(&SSDInst, 1, ssd_value);
	  vTaskDelay(xDelay);
	  ssd_value = SSD_decode(previous_key, 0);
	  XGpio_DiscreteWrite(&SSDInst, 1, ssd_value);
	  vTaskDelay(xDelay);
	  // TODO: Add a xil_printf statement to output the value of 'status'
	  // whenever it changes.

/*****************************************************************************/
   }
}

void InitializeKeypad()
{
   KYPD_begin(&KYPDInst, XPAR_AXI_KEYPAD_BASEADDR);
   KYPD_loadKeyTable(&KYPDInst, (u8*) DEFAULT_KEYTABLE);
}

// This function is hard coded to translate key value codes to their binary representation
u32 SSD_decode(u8 key_value, u8 cathode)
{
    u32 result;

	// key_value represents the code of the pressed key
    switch(key_value){ // Handles the coding of the 7-seg display
        case 48: result = 0b00111111; break; // 0
        case 49: result = 0b00110000; break; // 1
        case 50: result = 0b01011011; break; // 2
        case 51: result = 0b01111001; break; // 3
        case 52: result = 0b01110100; break; // 4
        case 53: result = 0b01101101; break; // 5
        case 54: result = 0b01101111; break; // 6
        case 55: result = 0b00111000; break; // 7
        case 56: result = 0b01111111; break; // 8
        case 57: result = 0b01111100; break; // 9
        case 65: result = 0b01111110; break; // A
        case 66: result = 0b01100111; break; // B
        case 67: result = 0b00001111; break; // C
        case 68: result = 0b01110011; break; // D
        case 69: result = 0b01001111; break; // E
        case 70: result = 0b01001110; break; // F
        default: result = 0b00000000; break; // default case - all seven segments are OFF
    }

	// cathode handles which display is active (left or right)
	// by setting the MSB to 1 or 0
    if(cathode==0)
            return result;
        else
            return result | 0b10000000;
}