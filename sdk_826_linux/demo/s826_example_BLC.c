////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SENSORAY MODEL 826 PROGRAMMING EXAMPLES
// This file contains simple functions that show how to program the 826.
// Copyright (C) 2012 Sensoray
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FYP project setup - R0 
// J.L. Dantanarayana & U.G.S. Kashmira
// 22.04.2023
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Header functions declarations

#ifndef _LINUX
#include <windows.h>
#include <conio.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef _LINUX
#include "..\826api.h"
#else
#include "826api.h"
#endif


// Helpful macros for DIOs
#define DIO(C)                  ((uint64)1 << (C))                          			// convert dio channel number to uint64 bit mask
#define DIOMASK(N)              {(uint)(N) & 0xFFFFFF, (uint)((N) >> 24)}   			// convert uint64 bit mask to uint[2] array
#define DIOSTATE(STATES,CHAN)   ((STATES[CHAN / 24] >> (CHAN % 24)) & 1)    			// extract dio channel's boolean state from uint[2] array

////////////////////////////////////////////////////////////////////////////////////////////////
// ERROR HANDLING
// These examples employ very simple error handling: if an error is detected, the example 
// functions will immediately return an error code. This behavior may not be suitable for some 
// real-world applications but it makes the code easier to read and understand. In a real
// application, it's likely that additional actions would need to be performed. The examples use
// the following X826 macro to handle API function errors; it calls an API function and stores
// the returned value in errcode, then returns immediately if an error was detected.

#define X826(FUNC)   if ((errcode = FUNC) != S826_ERR_OK) { printf("\nERROR: %d\n", errcode); return errcode;}

// end of hearder fuction declarations


////////////////////////////////////////////////////////////////////////////////////////////////
// Globale veriable declaration

// Counter ports initialization
uint counter_platform_motor = 3;			
uint counter_rotory_motor = 5;
uint counter_master_motor = 4;


// Encoder count 
uint counts_plotform_motor = 0;							// encoder counts when the snapshot was captured in platform motor
uint counts_rotory_motor = 0;
uint counts_master_motor = 0;

// 
uint timestamp;											// time the snapshot was captured
uint timestamp_pltform;
uint timestamp_rotory;
uint timestamp_start;
uint timestamp_previous;								// time the previous snapshot was captured 
uint aout_m= 0;
uint aout_r=0;// output duplicate waveform on this dac channel
int errcode = S826_ERR_OK;
int steps_m = 0;
int steps_r = 0;

//for loop oparation
long long int i ;

float function_exicution_time = 0.0;
float dt = 0.0;
float F_ref_m = 0.0;
float F_ref_max_m = 0.0;
float F_ref_min_m = 0.0;
float F_ref_r = 0.0;

// motor drive operation
unsigned int analog_out_motor_m;
float I_a_ref_m = 0.0;
float I_motor_m = 0.0;

unsigned int analog_out_motor_r;
float I_a_ref_r = 0.0;
float I_motor_r = 0.0;

// motor parameters
float K_tn_m = 24.0;										// Master motor force constent
float M_n_m = 0.655;										// Mass : (Motor shaft, 2 x linera bearings, 1 x Long green bearing and shft connector 1 x short green bearing and shft connector)=600g and (whight hammer head)=55g .
float friction_m = 0.0;

float K_tn_r = 24.0;										// Replica motor force constent
float M_n_r = 0.655;										// Mass : (Motor shaft, 2 x linera bearings, 1 x Long green bearing and shft connector 1 x short green bearing and shft connector)=600g and (whight hammer head)=55g .
float friction_r = 0.0;

// Velocity mesurment
float velocity_sum_m = 0.0;
float velocity_m = 0.0;										// motor velocity
float position_x_m = 0.0;										// motor physical position
float position_x_m_prev = 0.0;
float position_dx_m = 0.0;

float velocity_sum_r = 0.0;
float velocity_r = 0.0;										// motor velocity	// velocity filter constent
float position_x_r = 0.0;										// motor physical position
float position_x_r_prev = 0.0;
float position_dx_r = 0.0;

float g_velocity = 300.0;									// velocity filter constent

// DOb
float dob_input_m = 0.0;
float dob_filter_input_m = 0.0;
float dob_filter_output_m = 0.0;
float dob_force_m = 0.0;

float dob_input_r = 0.0;
float dob_filter_input_r = 0.0;
float dob_filter_output_r = 0.0;
float dob_force_r = 0.0;
int const g_dis = 200;


// RTOB
float rtob_filter_input_m = 0.0;
float rtob_filter_output_m = 0.0;
float rtob_force_m = 0.0;

float rtob_filter_input_r = 0.0;
float rtob_filter_output_r = 0.0;
float rtob_force_r = 0.0;

// PID
float const C_f = 1.0;

float const K_p = 900;
float const K_d = 60;

// end of veriable declarations

////////////////////////////////////////////////////////////////////////////////////////////////
// Function declaration

static float VelocityCalculation();
static int ControlLoop(uint board);
static int DemoWatchdog(uint board);
static int MotorOutDAC(uint board);
static int DoB();
static int RToB();
static int dtCalculation();

// end of function declaration



////////////////////////////////////////////////////////////////////////////////////////////////
// Main function.
int main(int argc, char **argv)
{
	uint board      = 0;	// change this if you want to use other than board number 0
	int errcode     = S826_ERR_OK;	
	int boardflags  = S826_SystemOpen();	// open 826 driver and find all 826 boards

	if (argc > 1)
        board = atoi(argv[1]);

	if (boardflags < 0)
        	errcode = boardflags;                       // problem during open
    	else if ((boardflags & (1 << board)) == 0) 
	{
		int i;
        	printf("TARGET BOARD of index %d NOT FOUND\n",board);// driver didn't find board you want to use
        	for (i = 0; i < 8; i++)
		{
			if (boardflags & (1 << i))
			{
				printf("board %d detected. try \"./s826demo %d\"\n", i, i);
            		}
        	}
	} else  
	{	// running functions
		X826( ControlLoop(board)); // read the endocer value, output anlog value and file write
		X826( DemoWatchdog(board)); // watchdog timer
	}

	switch (errcode)
	{
		case S826_ERR_OK:           break;
		case S826_ERR_BOARD:        printf("Illegal board number"); break;
		case S826_ERR_VALUE:        printf("Illegal argument"); break;
		case S826_ERR_NOTREADY:     printf("Device not ready or timeout"); break;
		case S826_ERR_CANCELLED:    printf("Wait cancelled"); break;
		case S826_ERR_DRIVER:       printf("Driver call failed"); break;
		case S826_ERR_MISSEDTRIG:   printf("Missed adc trigger"); break;
		case S826_ERR_DUPADDR:      printf("Two boards have same number"); break;S826_SafeWrenWrite(board, 0x02);
		case S826_ERR_BOARDCLOSED:  printf("Board not open"); break;
		case S826_ERR_CREATEMUTEX:  printf("Can't create mutex"); break;
		case S826_ERR_MEMORYMAP:    printf("Can't map board"); break;
		default:                    printf("Unknown error"); break;
	}
    
#ifndef _LINUX	
	printf("\nKeypress to exit ...\n\n");
	while (!_kbhit());
	_getch();
#endif

	S826_SystemClose();
	return 0;
}
// end of main function


////////////////////////////////////////////////////////////////////////////////////////////////
// Bilateral Control with Excess force Reduction

static int ControlLoop(uint board)
{	// ***Configure interfaces and start them running.
	//X826( S826_DacRangeWrite(board, aout, S826_DAC_SPAN_10_10, 0)); // program dac output range: -10V to +10V
	X826( S826_DacRangeWrite(board, aout, S826_DAC_SPAN_0_5, 0));// program dac output range: -0V to +5V , motor 0 value is 2.5V / for motor drive operation
	
	
	X826( S826_CounterModeWrite(board, counts_master_motor, S826_CM_K_QUADX4)); // Configure counter as incremental encoder interface. quadrature
	X826( S826_CounterStateWrite(board, counter_master_motor, 1) ); // Start tracking encoder position.
	X826( S826_CounterModeWrite(board, counter_rotory_motor, S826_CM_K_QUADX4)); // Configure counter as incremental encoder interface. quadrature
	X826( S826_CounterStateWrite(board, counter_rotory_motor, 1) ); // Start tracking encoder position.	

	I_motor_m = 0.0;	//stop the motor input current
    I_motor_r = 0.0;	//stop the motor input current
	MotorOutDAC(board);
	
	X826( S826_CounterSnapshot(board, counter_master_motor)); // Trigger snapshot on counter 0.
	X826( S826_CounterSnapshotRead(board, counter_master_motor, &counts_master_motor, &timestamp_start, NULL, 0));// Read the snapshot:
	printf("Timestamp:%d \n",timestamp_start);
	printf("counts:%d \n",counts_plotform_motor);

	X826( S826_CounterSnapshot(board, counter_rotory_motor)); // Trigger snapshot on counter 0.
	X826( S826_CounterSnapshotRead(board, counter_rotory_motor, &counts_rotory_motor, &timestamp, NULL, 0));// Read the snapshot:
	printf("Timestamp:%d \n",timestamp);
	printf("counts:%d \n",counts_plotform_motor);

	dtCalculation();

	// ****file write operation
	FILE *fp;// file pointer file file operation
	fp = fopen("/home/linux/Documents/texture_haptic/BLC_001.txt", "a+");

	// 1
	for(i=0;i<400000;i++)
	{	
	    X826( S826_CounterSnapshot(board, counter_platform_motor));     // Trigger snapshot on counter 0.
		X826( S826_CounterSnapshot(board, counter_rotory_motor));       // Trigger snapshot on counter 0.

		X826( S826_CounterSnapshotRead(board, counter_platform_motor, &counts_plotform_motor, &timestamp_pltform, NULL, 0));// Read the snapshot:
		X826( S826_CounterSnapshotRead(board, counter_rotory_motor, &counts_rotory_motor, &timestamp, NULL, 0));// Read the snapshot:

        dtCalculation();
		VelocityCalculation();
        DoB();
        RToB();
//
//		if (F_ref < F_ref_max)
//		{
//			F_ref = F_ref + dt * ramp_rate;
//		}else
//		{
//			F_ref = F_ref;
//		}
//		I_a_ref = K_p*(F_ref - rtob_torque)*(M_n/K_tn);
//		I_motor = I_a_ref + (dob_torque/K_tn);
//
//		I_motor = 0.175;
	
		MotorOutDAC(board);	// the motor input current
		
		fprintf(fp,"%f,%d,%d\n",function_exicution_time,counts_plotform_motor,counts_rotory_motor);
	}

	fclose(fp);

	// Injecting nevagive current, stopping cycle
	F_ref = 0.0;
	I_motor = 0.0;
	I_a_ref = 0.0;//stop the mortor input current
	MotorOutDAC(board);
	X826( S826_CounterStateWrite(board, counter_platform_motor, 0)); // Stop tracking encoder position.
	X826( S826_CounterStateWrite(board, counter_rotory_motor, 0)); // Stop tracking encoder position.

	return errcode;
} // end of main function

////////////////////////////////////////////////////////////////////////////////////////////////
// Analog Voltage generation for Motor Driver (in Current control mode)
static int MotorOutDAC(uint board)
{	
	if (I_motor_m < 0)        { I_motor_m = 0;
	}else if(I_motor_m > 1.0)	{ I_motor_m = 1.0;    }

    if (I_motor_r < 0)        { I_motor_r = 0;
    }else if(I_motor_r > 1.0)	{ I_motor_r = 1.0;    }

//	analog_out_motor = (unsigned int)(32768 + (I_motor/1.5)*32768);			// DAC senstivity 16 bit (2**16)movo zero vlaue 0 -->  2.5V out in dac
	analog_out_motor_m = (unsigned int)(32768 + (I_motor_m/1.5)*32768);					// DAC senstivity 16 bit (2**16)movo zero vlaue 0 -->  0V out in dac
    analog_out_motor_r = (unsigned int)(I_motor_r*65635);					// DAC senstivity 16 bit (2**16)movo zero vlaue 0 -->  0V out in dac

    X826( S826_DacDataWrite(board, aout_m, analog_out_motor_m, 0) );		// Analog value out from dac
    X826( S826_DacDataWrite(board, aout_r, analog_out_motor_r, 0) );		// Analog value out from dac

    return 0;															// negativa current - negative cont, Positive current positive counter incrimet
}

// Sampling time calculator
static int dtCalculation()
{
    function_exicution_time = (float)(timestamp - timestamp_start)/1000000;
    dt =(float)(timestamp - timestamp_previous)/1000000;
    timestamp_previous = timestamp;
    return 0;
}

// Velocity Calculation for Master and Replica
static float VelocityCalculation()
{
	steps_m = counts_master_motor;
    steps_r = counts_rotory_motor;

    position_x_m = steps_m*0.000005;
	velocity_sum_m = velocity_sum_m + velocity_m*dt;
	velocity_m = g_velocity * (position_x_m - velocity_sum_m);

    position_x_r = steps_r*0.000005;
    velocity_sum_r = velocity_sum_r + velocity_r*dt;
    velocity_r = g_velocity * (position_x_r - velocity_sum_r);
	return 0;
}

// DOB
static int DoB()
{
	dob_input_m = velocity_m * g_dis * M_n_m;
	dob_filter_input_m = K_tn_m * I_motor_m + dob_input_m;
	dob_filter_output_m = dob_filter_output_m + g_dis*(dob_filter_input_m - dob_filter_output_m)*dt;
	dob_force_m = dob_filter_output_m - dob_input_m;

    dob_input_r = velocity_r * g_dis * M_n_r;
    dob_filter_input_r = K_tn_r * I_motor_r + dob_input_r;
    dob_filter_output_r = dob_filter_output_r + g_dis*(dob_filter_input_r - dob_filter_output_r)*dt;
    dob_force_r = dob_filter_output_r - dob_input_r;
	return 0;
}

// RTOB
static int RToB()
{
	rtob_filter_input_m = dob_filter_input_m - friction_m;
	rtob_filter_output_m = rtob_filter_output_m + g_dis*(rtob_filter_input_m - rtob_filter_output_m)*dt;
	rtob_force_m = rtob_filter_output_m - dob_input_m;

    rtob_filter_input_r = dob_filter_input_r - friction_r;
    rtob_filter_output_r = rtob_filter_output_r + g_dis*(rtob_filter_input_r - rtob_filter_output_r)*dt;
    rtob_force_r = rtob_filter_output_r - dob_input_r;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// The demo.
static int DemoWatchdog(uint board)
{
	int rc;
	uint timing[5];
	// set timer 1 time-out to around 1 second
	timing[0] = 55000000;
	printf("\nDemoWatchdog\n");
	// enable writing to watchdog
	rc = S826_SafeWrenWrite(board, 0x02);
	if (rc != 0)
	{
		printf("failed to enable wren for watchdog\n");
		return rc;
	}
	rc = S826_WatchdogConfigWrite(board, 0x00, timing);
	
	if (rc != 0)
	{
		printf("failed to configure WD\n");
		return rc;
	}

	rc = S826_WatchdogEnableWrite(board, 1);
	if (rc != 0)
	{
		printf("failed to enable WD\n");
		return rc;
	}
    
	// commented out.  for testing watchdog cancel
	// CreateThread(NULL, 4096, testThread, board, 0, NULL);

	// watch indefinitely for watchdog
	rc = S826_WatchdogEventWait(board, S826_WAIT_INFINITE);

	switch (rc)
	{
		case 0:
			printf("watchdog successfully expired\n");
			break;
		case S826_ERR_NOTREADY:
			printf("WD wait timed out\n");
			break;
		case S826_ERR_CANCELLED:
			printf("WD wait cancelled\n");
			break;
		default:
		printf("error %d\n", rc);
        
	}
	// disable watchdog
	rc = S826_WatchdogEnableWrite(board, 0);
	if (rc != 0)
	{
		printf("failed to disable WD\n");
		return rc;
	}
	return 0;
}
