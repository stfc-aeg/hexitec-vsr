////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file VSR.h
//! \brief Header file - Hexitec VSR DLL
////////////////////////////////////////////////////////////////////////////////////////////////////

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the VSR_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// VSR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef VSR_EXPORTS
#define VSR_API __declspec(dllexport)
#else
#define VSR_API __declspec(dllimport)
#endif

//#pragma once

#include "aS_Pleora_NTx.h"
#include "aS_InstanceManager.h"

using namespace aS;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	A macro that defines the hexitec adresses, calculation constants and registers. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
#define AS_MODULE_ADDRESS_CONTROLLER (u8)0xC0
#define AS_MODULE_ADDRESS_VSR1 (u8)0x90
#define AS_MODULE_ADDRESS_VSR2 (u8)0x91
#define AS_VSR_POWER_ON_DELAY (u32)10000
#define AS_HEXITEC_CLOCK_SPEED (dbl)42500000
#define AS_DAC_REF_VOLTAGE (dbl)3
#define AS_INTERNAL_REF_VOLTAGE (dbl)2.048
#define AS_HEXITEC_SETUP_REGISTER_SIZE 10
#define AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR1 (u8)0x2f
#define AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR2 (u8)0x90
#define AS_HEXITEC_MIN_FRAME_TIMEOUT (u32)25
#define AS_HEXITEC_FRAME_TIMEOUT_MULTIPLIER (dbl)2.5
#define AS_HEXITEC_DARK_CORRECTION_FRAME_COUNT 8192
#define AS_HEXITEC_MAX_STREAM_REGISTER_COUNT 60
#define AS_HEXITEC_SERIAL_TIMEOUT 1000
#define AS_HEXITEC_TARGET_TEMPERATURE_LL 10
#define AS_HEXITEC_TARGET_TEMPERATURE_LL_0x00 -10
#define AS_HEXITEC_TARGET_TEMPERATURE_UL 40
#define AS_HEXITEC_HEIGHT_FACTOR 1
#define AS_HEXITEC_WIDTH_FACTOR 16
#define AS_NTC_MATERIAL_CONST_B (dbl)3625

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the u8. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum HexitecGain : u8 {
	/// <summary>	An enum constant representing the Hexitec gain high option. </summary>
	AS_HEXITEC_GAIN_HIGH = 0,
	/// <summary>	An enum constant representing the Hexitec gain low option. </summary>
	AS_HEXITEC_GAIN_LOW = 1
} HexitecGain;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the u8. </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum HexitecAdcSample : u8 {
	/// <summary>	An enum constant representing the ADC sample at rising edge option. </summary>
	AS_HEXITEC_ADC_SAMPLE_RISING_EDGE = 0,
	/// <summary>	An enum constant representing the ADC sample at falling edge option. </summary>
	AS_HEXITEC_ADC_SAMPLE_FALLING_EDGE = 1
} HexitecAdcSample;

#ifndef _M_X64
#pragma pack(push)
#pragma pack(1)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the VSR device operating values. </summary>
/// 
/// <remarks>
/// The function ReadModuleOperatingValues() sets the values within this structure if a pointer to
/// a generated structure is passed. This struct can be defined by each VSR module.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecVsrOperatingValues {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 3.3V rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v3_3;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the high voltage monitor rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		hvMon;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the calculated high voltage of the hv rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		hvOut;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 1.2V rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v1_2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 1.8V rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v1_8;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 2.5V rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v2_5;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 3.3V low noise rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v3_3ln;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 1.65V low noise rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v1_65ln;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 1.8V analog rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v1_8ana;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the 3.8V analog rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		v3_8ana;
} HexitecVsrOperatingValues, *HexitecVsrOperatingValuesPtr, **HexitecVsrOperatingValuesHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the controller operating values. </summary>
/// 
/// <remarks>
/// The function ReadOperatingValues() sets the values within this structure if a pointer to
/// a generated structure is passed.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecControllerOperatingValues {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured peltier current. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		peltierCurrent;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured ntc temperature. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		ntcTemperature;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured high voltage current. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		hvCurrent;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the measured voltage at the high voltage rail. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl		hvVoltage;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the 1.8V rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v1_8_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the 2.5V rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v2_5_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the 3.3V rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v3_3_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the 5.0V rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v5_0_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the 5.5V rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v5_5_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the spare power rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	v_spare_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the PL power rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	vPL_pwrGood;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Returns whether the PS power rail is up or not. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Control	vPS_pwrGood;
} HexitecControllerOperatingValues, *HexitecControllerOperatingValuesPtr, **HexitecControllerOperatingValuesHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the VSR device system parameters. </summary>
///
/// <remarks>
/// These parameters are typically constant and may change only after hardware or firmware
/// changes.
/// The functions ConfigureDetectorWithTrigger() and ConfigureDetector() will get the configurations
/// defined in this structure.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecSystemConfig {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adjust the sample point. </summary>
	///
	/// <remarks>
	/// Adjust the sample point (time) when the signal is sampled by the ADC in relation to the sensor
	/// stimuli. Unsharpen edges and crosstalk between column can be minimized using this parameter.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 2
	/// range    | 0..31
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	u8					AdcDelay;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adjust the position of the sync signals. </summary>
	///
	/// <remarks>
	/// Adjust the timing relationship between the sync signals and the pixel data. Column wise pixel
	/// shift can be adjusted using this parameter.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 10
	/// range    | 0..?? todo
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	u8					SyncSignalDelay;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the sample edge of the ADC. </summary>
	///
	/// <remarks>
	/// Sets at which edge (rising / falling) of the ADC clock the ADC samples the signal.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_HEXITEC_ADC_SAMPLE_FALLING_EDGE
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	HexitecAdcSample	AdcSample;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adjust the ADC clock phase. </summary>
	///
	/// <remarks>
	/// Adjust the phase between the ADC clock and the data stream of the ADC. This parameter can be
	/// used to avoid flickering pixels or false pixel values.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 4
	/// range    | 0..11
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	u8					AdcClockPhase;
} HexitecSystemConfig, *HexitecSystemConfigPtr, **HexitecSystemConfigHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the Hexitec sensor setup register. </summary>
///
/// <remarks>
/// The Hexitec sensor has 6 80bit setup register to configure the behavior of the columns and
/// rows. 3 to configure the columns and 3 to configure the rows. This structure holds the values
/// within 3 times 10 bytes either to configure the columns or the rows.
/// </remarks>
///
/// <value>
/// byte	 | bit    | column / row
/// ---------|--------|-------------
/// 0        | 0      | 0
/// 0        | 1      | 1
/// ...      | ...    | ...
/// 0        | 7      | 7
/// 1        | 0      | 8    
/// ...      | ...    | ...  
/// 9        | 7      | 79
/// </value>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecSetupRegister {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets either the column / row is read out or skipped. </summary>
	///
	/// <remarks>
	/// Setting a bit to 1 enables reading out the corresponding column / row. Setting a bit to 0 the
	/// corresponding column / row is skipped. This means also that the resolution will change,
	/// changing this parameters. In column direction the sensor is divided into 4 parts which are read
	/// out simultaneously. The resolution change will not be 1 to 1 as it is for rows. It is 4 times
	/// the biggest part. E.g. Disabling 1 column only will result in a resolution of 80 columns.
	/// Disabling 1 column within each part will result in a resolution of 76 columns.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | [0..9] 0xFF
	/// range    | [0..9] 0x00..0xFF
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8				ReadEn[AS_HEXITEC_SETUP_REGISTER_SIZE];

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables the power for a column / row. </summary>
	///
	/// <remarks>
	/// Setting a bit to 1 enables the power for the corresponding column / row.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | [0..9] 0xFF
	/// range    | [0..9] 0x00..0xFF
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	u8				PowerEn[AS_HEXITEC_SETUP_REGISTER_SIZE];

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables the calibration circuit for a column / row. </summary>
	///
	/// <remarks>
	/// Setting a bit to 1 enables the calibration circuit for the corresponding column / row.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | [0..9] 0x00
	/// range    | [0..9] 0x00..0xFF
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	u8				CalEn[AS_HEXITEC_SETUP_REGISTER_SIZE];
} HexitecSetupRegister, *HexitecSetupRegisterPtr, **HexitecSetupRegisterHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing sensor setup register. </summary>
/// 
/// <remarks>
/// The function GetSensorSetupReg() sets the values within this structure to the values set on the
/// sensor setup control. SetSensorSetupReg() will set these setup control to the defined values.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecSensorSetupRegister {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the setup register for all columns. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecSetupRegister Col;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the setup register for all rows. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecSetupRegister Row;
} HexitecSensorSetupRegister, *HexitecSensorSetupRegisterPtr, **HexitecSensorSetupRegisterHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing the sensor setup registers on one VSR board. </summary>
/// 
/// <remarks>
/// The function GetModulSetupReg() sets the values within this structure to the values set on the
/// sensors setup control.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecVsrSetupRegister {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the setup register sensor1. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecSensorSetupRegister Sensor1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the setup register sensor2. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecSensorSetupRegister Sensor2;
} HexitecVsrSetupRegister, *HexitecVsrSetupRegisterPtr, **HexitecVsrSetupRegisterHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Defines an alias representing the Hexitec sensor / state machine configuration.
/// </summary>
///
/// <remarks>
/// The function SetSensorConfig() sets the values within this structure to the values set on the
/// sensors configuration.
/// Furthermore this structure is needed when the detector gets configured.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecSensorConfig {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the gain of the preamplifier. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default  | AS_HEXITEC_GAIN_HIGH
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	HexitecGain				Gain;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the row and the S1 pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1..10000
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	Reg2Byte				Row_S1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the S1 and the SPH pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1..40
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						S1_Sph;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the SPH and the S2 pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 6
	/// range    | 1..40
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						Sph_S2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the VCAL pulse duration. </summary>
	/// 
	/// <remarks>
	/// If set to 0 the VCAL pulse is deactivated.
	/// </remarks>
	/// 
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 0
	/// range    | 0..22000
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	Reg2Byte				Vcal2_Vcal1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Reserved. </summary>
	///
	/// <remarks>
	/// Internal state machine parameter. Keep at default value.
	/// </remarks>
	///  
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1 todo
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						WaitClockCol;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Reserved. </summary>
	///
	/// <remarks>
	/// Internal state machine parameter. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 8
	/// range    | 8 todo
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						WaitClockRow;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Setup register for the VSR1 board. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecVsrSetupRegister	SetupRegVsr1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Setup register for the VSR2 board. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecVsrSetupRegister	SetupRegVsr2;
} HexitecSensorConfig, *HexitecSensorConfigPtr, **HexitecSensorConfigHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing VSR envirnmental values. </summary>
/// 
/// <remarks>
/// The function ReadModuleEnvironmentValues() and ReadEnvironmentValues() are used to set these values.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecVsrEnvironmentValues {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the VSR humidity in %. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl						humidity;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the VSR ambient temperature in degC. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl						ambientTemperature;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the VSR asic temperature 1 in degC. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl						asicTemperature1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the VSR asic temperature 2 in degC. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl						asicTemperature2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the VSR ADC temperature in degC. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	dbl						adcTemperature;
} HexitecVsrEnvironmentValues, *HexitecVsrEnvironmentValuesPtr, **HexitecVsrEnvironmentValuesHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Defines an alias representing VSR sensor configuration. </summary>
/// 
/// <remarks>
/// The function SetModuleSensorConfig() is used to configure these values to a defined VSR board.
/// </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecVsrSensorConfig {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the gain of the preamplifier. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_HEXITEC_GAIN_HIGH
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	HexitecGain				Gain;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the row and the S1 pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1..10000
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	Reg2Byte				Row_S1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the S1 and the SPH pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1..40
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						S1_Sph;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the duration between the SPH and the S2 pulse. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 6
	/// range    | 1..40
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						Sph_S2;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the VCAL pulse duration. </summary>
	/// 
	/// <remarks>
	/// If set to 0 the VCAL pulse is deactivated.
	/// </remarks>
	/// 
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 0
	/// range    | 0..22000
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	Reg2Byte				Vcal2_Vcal1;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Reserved. </summary>
	///
	/// <remarks>
	/// Internal state machine parameter. Keep at default value.
	/// </remarks>
	///  
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 1
	/// range    | 1 todo
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						WaitClockCol;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Reserved. </summary>
	///
	/// <remarks>
	/// Internal state machine parameter. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 8
	/// range    | 8 todo
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////		
	u8						WaitClockRow;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Setup register for the VSR board. </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	HexitecVsrSetupRegister	SetupReg;
} HexitecVsrSensorConfig, *HexitecVsrSensorConfigPtr, **HexitecVsrSensorConfigHdl;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Defines an alias representing the operation mode / data path configuration.
/// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct HexitecOperationMode {

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Starts uploading the offset values. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcUploadDarkCorrectionValues;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Starts collecting the offset values. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcCollectDarkCorrectionValues;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables hardware offset correction. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcEnableDarkCorrectionCountingMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables hardware offset correction. </summary>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_ENABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcEnableDarkCorrectionSpectroscopicMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables sending the hardware offset correction values. </summary>
	/// 
	/// <remarks>
	/// If enabled the current active offset correction values will be send. Use the function
	/// AcquireFrames() to get the offset values as a frame.
	/// </remarks>
	/// 
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcSendDarkCorrectionValues;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Disables the VCAL pulse. </summary>
	/// 
	/// <remarks>
	/// If enabled the VCAL pulse is disabled.
	/// </remarks>
	/// 
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_ENABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcDisableVcalPulse;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables test pattern within offset correction function block. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcTestMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables trigger counting mode. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			DcEnableTriggeredCountingMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Starts uploading the threshold values. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			EdUploadThresholdValues;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Disables counting mode function block. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			EdDisableCountingMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Enables test pattern within counting mode function block. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | AS_CONTROL_DISABLED
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Control			EdTestMode;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the number of frames used for the counting mode. </summary>
	///
	/// <remarks>
	/// Internal use only. Keep at default value.
	/// </remarks>
	///
	/// <value>
	/// property | value
	/// ---------|------
	/// default	 | 0
	/// range	 | 0..65535
	/// </value>
	////////////////////////////////////////////////////////////////////////////////////////////////////	
	Reg2Byte		EdCycles;
} HexitecOperationMode, *HexitecOperationModePtr, **HexitecOperationModeHdl;

#ifndef _M_X64
#pragma pack(pop)
#endif

EXTERN_C	VSR_API	i32		AcquireFrame(										// not modified
										aS_Pleora_NTx* deviceHdl,
										p_u32 frameCount,
										p_u8 buffer,
										u32 frameTimeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Acquires frames from the detector. </summary>
///
/// <remarks>
///		Acquires a specific number or continuously frames from the detector. The acquired frames
///		will be copied into a transfer buffer. Once a transfer buffer is filles the registered
///		callback function (RegisterTransferBufferReadyCallBack()) is called to transmit the pointer
///		and the number of frames within this buffer to a third party application for data
///		processing. Once the processing of the data within this buffer is done the buffer has to
///		be returned by calling ReturnBuffer() function. If the specified frames are acuired,
///		StopAcquisition() is called or an error occurs the function will return. Requires an opened
///		acuisition stream and pipeline (OpenStream() and CreatePipeline()).
///	</remarks>
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="frameCount">
///		[in]	Number of frames to acquire.
///		value | behavior
///		------|---------
///		0     | continuous until StopAcquisition() is called
/// 	>0    | returns after specified number of frames are acquired
/// </param>
/// <param name="framesAcquired">
///		[out]	Returns the number of the acquired frames.
/// </param>
/// <param name="frameTimeOut">
///		[in]	Frame time out (ms) to acquire the first frame. Should be set to 2.5 times the frame
///		time if not triggered or to the expected time of incoming trigger time if triggered. But at
///		least to 25ms.
/// </param>
/// 
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		AcquireFrames(										// not modified
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										p_u64 framesAcquired,
										u32 frameTimeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Checks the installed FPGA firmware version. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="controller">
///		[in,out]	Returns Version of controller board.
/// </param>
/// <param name="vsr1">
///		[in,out]	Returns Version of VSR1 board.
/// </param>
/// <param name="vsr2">
///		[in,out]	Returns Version of VSR2 board.
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
/// 
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		CheckFirmware(										// modified
										aS_Pleora_NTx* deviceHdl,
										FpgaFwVersionPtr controller,
										FpgaFwVersionPtr vsr1,
										FpgaFwVersionPtr vsr2,
										u32 timeOut );

EXTERN_C	VSR_API	i32		CheckModuleFirmware(								// created
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										p_u8 customerId,
										p_u8 projectId,
										p_u8 version,
										u8 forceEqualVersion,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ClearRegisterBit(									// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										u8 bitMask,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Closes a frame acquisition pipeline. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ClosePipeline(										// not modified
										aS_Pleora_NTx* deviceHdl );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Closes the serial communication port. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		CloseSerialPort(									// not modified
										aS_Pleora_NTx* deviceHdl );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Closes a frame acquisition stream. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		CloseStream(										// not modified
										aS_Pleora_NTx* deviceHdl );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Collects the offset values. </summary>
///
/// <remarks>
///		Collects the offset values for the offset value correction within the detector. Make sure
///		any X-Ray source is disabled and the high voltage is enabled and settled at the desired
///		value.
///	</remarks>
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
/// <param name="collectDcTimeOut">
///		[in]	Offset value collection time out (ms). Use returned value from ConfigureDetector()
///		or InitFwDefaults().
/// </param>
/// 
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		CollectOffsetValues(								// bugfix + modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut,
										u32 collectDcTimeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Configures the detector. </summary>
///
/// <remarks>
/// Configures the detector by applying the Hexitec sensor / state machine configuration, the
/// operation mode / data path configuration and the system parameters. If the resolution is
/// changed the acquisition pipeline has to be recreated by calling CreatePipeline(). The frame time
/// out for the acquisition is set to 2.5 times of the frame time (at least 25ms).
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="sensorConfig">
/// 	[in]	Sets the Hexitec sensor / state machine configuration.
/// </param>
/// <param name="operationMode">
/// 	[in]	Sets the operation mode / data path configuration.
/// </param>
/// <param name="systemConfig">
/// 	[in]	Sets the system parameters.
/// </param>
/// <param name="width">
/// 	[out] 	Returns the width / X-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="height">
/// 	[out] 	Returns the height / Y-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="frameTime">
/// 	[out]	Returns the frame time (s) based on the applied detector configuration.
/// </param>
/// <param name="collectDcTime">
/// 	[out]	Returns the time (ms) which is needed to collect the offset values within the
/// 	detecor. Use this value when calling CollectOffsetValues().
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ConfigureDetector(									// not modified
										aS_Pleora_NTx* deviceHdl,
										const HexitecSensorConfigPtr sensorConfig,
										const HexitecOperationModePtr operationMode,
										const HexitecSystemConfigPtr systemConfig,
										p_u32 width,
										p_u32 height,
										p_dbl frameTime,
										p_u32 collectDcTime,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Configures the detector with trigger options. </summary>
///
/// <remarks>
/// Configures the detector by applying the Hexitec sensor / state machine configuration, the
/// operation mode / data path configuration and the system parameters. If the resolution is
/// changed the acquisition pipeline has to be recreated by calling CreatePipeline(). The frame time
/// out for the acquisition is set to 2.5 times of the frame time (at least 25ms). The optional
/// trigger functionality (available since FW version 2) is set up additionally in comparison to
/// ConfigureDetector().
/// 
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="sensorConfig">
/// 	[in]	Sets the Hexitec sensor / state machine configuration.
/// </param>
/// <param name="operationMode">
/// 	[in]	Sets the operation mode / data path configuration.
/// </param>
/// <param name="systemConfig">
/// 	[in]	Sets the system parameters.
/// </param>
/// <param name="width">
/// 	[out] 	Returns the width / X-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="height">
/// 	[out] 	Returns the height / Y-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="frameTime">
/// 	[out]	Returns the frame time (s) based on the applied detector configuration.
/// </param>
/// <param name="collectDcTime">
/// 	[out]	Returns the time (ms) which is needed to collect the offset values within the
/// 	detecor. Use this value when calling CollectOffsetValues().
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
/// <param name="enSyncMode">
///		[in]	Enables the triggered synchronous state machine start. The detector state machine is
///		started	and kept running with an applied high level on trigger input 1. With this
///		functionality a	synchronous start of multiple detectors can be achieved.
///		property | value
///		---------|------
///		default	 | AS_CONTROL_DISABLED
/// </param>
/// <param name="enTriggerMode">
///		[in]	Enables the triggered data acquisition mode. The detector delivers n frames
///		(SetTriggeredFrameCount()) after a trigger event on trigger input 2 or frames as long as an
///		high level is applied on trigger input 3.
///		property | value
///		---------|------
///		default	 | AS_CONTROL_DISABLED
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ConfigureDetectorWithTrigger(						// modified
										aS_Pleora_NTx* deviceHdl,
										const HexitecSensorConfigPtr sensorConfig,
										const HexitecOperationModePtr operationMode,
										const HexitecSystemConfigPtr systemConfig,
										p_u32 width,
										p_u32 height,
										p_dbl frameTime,
										p_u32 collectDcTime,
										u32 timeOut,
										Control enSyncMode,
										Control enTriggerMode );

EXTERN_C	VSR_API	void	CopyBuffer(											// not modified
										p_u8 sourceBuffer,
										p_u8 destBuffer,
										u32 byteCount );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Create a frame acquisition pipeline. </summary>
///
/// <remarks>
///		Create a frame acquisition pipeline and the necessary internal buffer and transfer buffer.
///		The number of buffers and frames settings are tradeoff between memory usage and the
///		availability to achieve a losless data acquisition and data processing. Requires an opened
///		acuisition stream (OpenStream()). Has to be recalled if the resolution change.
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="bufferCount">
///		[in]	Number of internal frame acquisition buffers.
///		property         | value
///		-----------------|------
///		default          | 512
///	</param>
/// <param name="transferBufferCount">
///		[in]	Number of transfer buffers.
///		property         | value
///		-----------------|------
///		default          | 100
///	</param>
/// <param name="transferBufferFrameCount">
///		[in]	Number of frames within on transfer buffer.
///		property         | value
///		-----------------|------
///		default          | 100
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		CreatePipeline(										// not modified
										aS_Pleora_NTx* deviceHdl,
										u32 bufferCount,
										u32 transferBufferCount,
										u32 transferBufferFrameCount );

EXTERN_C	VSR_API	i32		DisableDetector(									// added
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		DisableModule(										// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u32 timeOut );

EXTERN_C	VSR_API	i32		DisableSM(											// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		DisableSyncMode(									// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		DisableTriggerGate(									// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		DisableTriggerMode(									// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableDetector(										// added
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableControllerFunctionBlocks(						// added
										aS_Pleora_NTx* deviceHdl,
										Control	dacEnable,
										Control	peltierEnable,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableFunctionBlocks(								// modified
										aS_Pleora_NTx* deviceHdl,
										Control	adcEnable,
										Control	dacEnable,
										Control	peltierEnable,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableModuleFunctionBlocks(							// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										Control	adcEnable,
										Control	dacEnable,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableModule(										// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableSM(											// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableSyncMode(										// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableTriggerGate(									// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		EnableTriggerMode(									// modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Closes a aS_Pleora_NTx device instance. </summary>
///
/// <remarks>
///		Disconnect from a GigE device and closes the aS_Pleora_NTx device instance.
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ExitDevice(											// not modified
										aS_Pleora_NTx* deviceHdl );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Returns an aSpect error code description. </summary>
/// 
/// <param name="asError">
///		[in]	An aSpect error code.
///	</param>
/// <param name="asErrorMsg">
///		[out]		Pointer to allocated memory. This memory is filled with the aSpect error
///		description.
///	</param>
/// <param name="length">
///		[in]	Size of the allocated memory.
///	</param>
///
/// <returns>	Returns 0 on success or an system error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		GetAsErrorMsg(										// not modified
										i32 asError,
										str8 asErrorMsg,
										u32 length );

EXTERN_C	VSR_API	i32		GetBufferHandlingThreadPriority(					// not modified
										aS_Pleora_NTx* deviceHdl,
										p_i32 priority );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Get information strings from the connected GigE device. </summary>
/// 
/// <remarks>
///		The returned pointer to the strings are valid as long the device instance is valid.
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="deviceInfoStr">
///		[out]	Pointer to the structure which contains the pointer to the strings.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		GetDeviceInformation(								// not modified
										aS_Pleora_NTx* deviceHdl,
										NTxDeviceInfoStrPtr deviceInfoStr );

EXTERN_C	VSR_API	dbl		GetFrameTime(										// modified
										const HexitecSensorConfigPtr sensorConfig,
										u32 width,
										u32 height );

EXTERN_C	VSR_API	i32		GetIntegerValue(									// not modified
										aS_Pleora_NTx* deviceHdl,
										const str8 propertyName,
										i64 &value );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the last occurred Pleora API error. </summary>
///
/// <remarks>
/// Gets the last occurred Pleora API error code, string and description. The string pointer can be
/// NULL. The memory will be filled partially if the length is not long enough to hold the entire
/// string. The necessary length to hold the complete string will be returned, as long the length
/// pointer is not NULL.
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="pleoraErrorCode">
/// 	[out]	Returns a Pleora API result code.
/// </param>
/// <param name="pleoraErrorCodeString">
/// 	[out,opt]	Pointer to allocated memory. This memory is filled with the Pleora code string.
/// </param>
/// <param name="pleoraErrorCodeStringLen">
/// 	[in,out,opt] 	Size of the allocated memory [in] and necessary size to hold the complete
/// 	Pleora code string [out].
/// </param>
/// <param name="pleoraErrorDescription">
/// 	[out,opt]	Pointer to allocated memory. This memory is filled with the Pleora description
/// 	string.
/// </param>
/// <param name="pleoraErrorDescriptionLen">
/// 	[in,out,opt]	Size of the allocated memory [in] and necessary size to hold the complete
/// 	Pleora description string [out].
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		GetLastResult(										// not modified
										aS_Pleora_NTx* deviceHdl,
										p_u32 pleoraErrorCode,
										str8 pleoraErrorCodeString,
										p_u32 pleoraErrorCodeStringLen,
										str8 pleoraErrorDescription,
										p_u32 pleoraErrorDescriptionLen );

EXTERN_C	VSR_API	i32		GetOperationMode(									// modified + bugfix
										aS_Pleora_NTx* deviceHdl,
										HexitecOperationModePtr operationMode,
										u32 timeOut );

EXTERN_C	VSR_API	i32		GetSensorConfig(									// modified
										aS_Pleora_NTx* deviceHdl,
										HexitecSensorConfigPtr sensorConfig,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Returns a system error code description. </summary>
///
/// <param name="sysError">
///		[in]	A system error code.
///	</param>
/// <param name="sysErrorMsg">
///		[out]		Pointer to allocated memory. This memory is filled with the system error
///		description.
///	</param>
/// <param name="length">
///		[in]	Size of the allocated memory.
///	</param>
///
/// <returns>	Returns 0 on success or an system error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		GetSystemErrorMsg(									// not modified
										i32 sysError,
										str8 sysErrorMsg,
										u32 length );
									
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Return the states of the trigger lines. </summary>
///
/// <remarks>
///		Return the states of the trigger lines.
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="trigger1">
///		[out]	State of trigger1.
///	</param>
/// <param name="trigger2">
///		[out]	State of trigger2.
///	</param>
/// <param name="trigger3">
///		[out]	State of trigger3.
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		GetTriggerState(									// modified
										aS_Pleora_NTx* deviceHdl,
										p_u8 trigger1,
										p_u8 trigger2,
										p_u8 trigger3,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Initialize a GigE device instance. </summary>
///
/// <remarks>
/// Searches for an available aS_Pleora_NTx device. Creates a device instance and connect to the device. Also
/// returns the internal Peora API result and the related description. Refer to GetLastResult().
/// </remarks>
///
/// <param name="deviceHdl">
///		[out]	Returns a handle to a aS_Pleora_NTx device instance on success. Otherwise NULL.
///	</param>
/// <param name="deviceDescriptor">
/// 	[in,opt]	Pointer to a string which contains a device descriptor.
///		value            | behavior
///		-----------------|---------
///		NULL or empty    | connect to the first available device
/// 	IP- or MAC-Addr. | connect to the specified device
/// </param>
/// <param name="internalResult">
/// 	[out]	Returns the last PvResult Code.
/// </param>
/// <param name="pleoraErrorCodeString">
/// 	[out,opt]	Pointer to allocated memory. This memory is filled with the Pleora code string.
/// </param>
/// <param name="pleoraErrorCodeStringLen">
/// 	[in,out,opt] 	Size of the allocated memory [in] and necessary size to hold the complete
/// 	Pleora code string [out].
/// </param>
/// <param name="pleoraErrorDescription">
/// 	[out,opt]	Pointer to allocated memory. This memory is filled with the Pleora description
/// 	string.
/// </param>
/// <param name="pleoraErrorDescriptionLen">
/// 	[in,out,opt]	Size of the allocated memory [in] and necessary size to hold the complete
/// 	Pleora description string [out].
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		InitDevice(											// not modified
										aS_Pleora_NTx** deviceHdl,
										const str8 deviceDescriptor,
										p_u32 internalResult,
										str8 pleoraErrorCodeString,
										p_u32 pleoraErrorCodeStringLen,
										str8 pleoraErrorDescription,
										p_u32 pleoraErrorDescriptionLen );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reinitializes a connected aS_Pleora_NTx device to its power on default settings. </summary>
///
/// <remarks>
/// Reinitializes / reconfigures a connected aS_Pleora_NTx device to its power on / firmware default
/// settings. The high voltage is set either to the power on default or the specified set point
/// during the offset value collection. After the offset collection the high voltage is set to 0 V
/// and have to be reset by calling SetDAC(). If the resolution is changed the acquisition pipeline
/// has to be recreated by calling CreatePipeline().
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="setHv">
/// 	[in]	Selects the source for the high voltage set point.
///		value | behavior
///		------|---------
///		0     | use the power on default high voltage set point
/// 	>0    | use the supplied high voltage set point
/// </param>
/// <param name="hvSetPoint">
///		[in,out]	Sets the high voltage.
///		property         | value
///		-----------------|------
///		default          | 0 V todo
/// 	range            | -750..0 V
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
/// <param name="width">
/// 	[out] 	Returns the width / X-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="height">
/// 	[out] 	Returns the height / Y-Resolution of the frame based on the applied sensor
/// 	configuration. Use this value when calling SetFrameFormatControl().
/// </param>
/// <param name="sensorConfig">
/// 	[out]	Returns the applied Hexitec sensor / state machine configuration.
/// </param>
/// <param name="operationMode">
/// 	[out]	Returns the applied operation mode / data path configuration.
/// </param>
/// <param name="frameTime">
/// 	[out]	Returns the frame time (s) based on the applied detector configuration.
/// </param>
/// <param name="collectDcTime">
/// 	[out]	Returns the time (ms) which is needed to collect the offset values within the
/// 	detecor. Use this value when calling CollectOffsetValues().
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		InitFwDefaults(										// modified
										aS_Pleora_NTx* deviceHdl,
										u8 setHv,
										p_dbl hvSetPoint,
										u32 timeOut,
										p_u32 width,
										p_u32 height,
										HexitecSensorConfigPtr sensorConfig,
										HexitecOperationModePtr operationMode,
										p_dbl frameTime,
										p_u32 collectDcTime );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Opens the serial communication port. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="serialPort">
///		[in]	Selects the serial communication port to use. Keep at 2.
///	</param>
/// <param name="rxBufferSize">
///		[in]	Sets the internal Pleora API receive buffer size.
///		property         | value
///		-----------------|------
///		default          | 2048
///	</param>
/// <param name="useTermChar">
///		[in] 	Selects the usage of a termination character.
///		value | behavior
///		------|---------
///		0     | termination character is not used
/// 	>0    | the specified termination character is used
///	</param>
/// <param name="termChar">
///    	Sets the used termination character. Keep at 0x0d.
/// </param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		OpenSerialPort(										// not modified
										aS_Pleora_NTx* deviceHdl,
										PvDeviceSerial serialPort,
										u32 rxBufferSize,
										u8 useTermChar,
										u8 termChar );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Opens a frame acquisition stream. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		OpenStream(											// not modified
										aS_Pleora_NTx* deviceHdl );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads the environment values from the whole setup. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="peltierCurrent">
/// 	[out] The current through the Peltier element.
/// </param>
/// <param name="ntcTemperature">
/// 	[out] The cooling control temperature.
/// </param>
/// <param name="daqPwrTemperature">
/// 	[out] Temperaure of the DAQ Board.
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////	
EXTERN_C	VSR_API	i32		ReadControllerEnvironmentValues(					// added
										aS_Pleora_NTx* deviceHdl,
										p_dbl peltierCurrent,
										p_dbl ntcTemperature,
										p_dbl daqPwrTemperature,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads the controller operating values. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="opValuesController">
///		[in]	Operation values for the controller board.
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ReadControllerOperatingValues(						// added
										aS_Pleora_NTx* deviceHdl,
										HexitecControllerOperatingValuesPtr opValuesController,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadControllerPowerState(							// added
										aS_Pleora_NTx* deviceHdl,
										p_u8 powerState,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads the environment values from the whole detector. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="envValuesVsr1">
/// 	[out] Environmental values of the VSR1 board.
/// </param>
/// <param name="envValuesVsr2">
/// 	[out] Environmental values of the VSR1 board.
/// </param>
/// <param name="ntcTemperature">
///  	[out] The cooling control temperature.
/// </param>
/// <param name="daqPwrTemperature">
///		[out]	DAQ board power temperature.
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
///
/// <returns>	The environment values. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ReadEnvironmentValues(								// modified
										aS_Pleora_NTx* deviceHdl,
										HexitecVsrEnvironmentValuesPtr envValuesVsr1,
										HexitecVsrEnvironmentValuesPtr envValuesVsr2,
										p_dbl ntcTemperature,
										p_dbl daqPwrTemperature,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadModuleEnvironmentValues(						// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrEnvironmentValuesPtr envValues,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads the operating values from the whole detector. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="opValuesController">			 	
///		[out] Operation values for the controller board.
///	</param>
/// <param name="opValuesVsr1">			 	
///		[out] Operation values for the VSR1 board.
///	</param>
/// <param name="opValuesVsr2">
/// 	[out] Operation values for the VSR2 board.
/// </param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
/// </param>
///
/// <returns>	The operating values. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ReadModuleOperatingValues(							// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrOperatingValuesPtr opValues,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadOperatingValues(								// modified
										aS_Pleora_NTx* deviceHdl,
										HexitecControllerOperatingValuesPtr opValuesController,
										HexitecVsrOperatingValuesPtr opValuesVsr1,
										HexitecVsrOperatingValuesPtr opValuesVsr2,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadRegister(										// modified
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadResolution(										// modified
										aS_Pleora_NTx* deviceHdl,
										p_u32 width,
										p_u32 height,
										u32 timeOut );

EXTERN_C	VSR_API	i32		RegisterAcqArmedCallBack(							// not modified
										aS_Pleora_NTx* deviceHdl,
										p_readyCallBack acqArmedCallBack );

EXTERN_C	VSR_API	i32		RegisterAcqFinishCallBack(							// not modified
										aS_Pleora_NTx* deviceHdl,
										p_readyCallBack acqFinishCallBack );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Registers a callback function to signalize a filled transfer buffer. </summary>
///
/// <remarks>
///		Registers a callback function to signalize a third party application a filled transfer
///		buffer, which is ready for data processing. The function should transmit the pointer to the
///		application and should return in a short term to avoid skipping frames. Once the application
///		is ready working on the bufferthe pointer has to be returned to the pool by calling
///		ReturnBuffer().
/// </remarks>
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="transferBufferReadyCallBack">
///		[in]	Pointer to the callback function. which is called once a transfer buffer is filled and ready
///		for data processing.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		RegisterTransferBufferReadyCallBack(				// not modified
										aS_Pleora_NTx* deviceHdl,
										p_bufferCallBack transferBufferReadyCallBack );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Returns a transfer buffer back to the available transfer buffer pool. </summary>
///
/// <remarks>
/// Returns a transfer buffer / pointer back to the available transfer buffer pool. The order
/// returning the pointers doesn't matter. If all buffer are handed over to the third party
/// application and no buffer is returned the acquisition function will return with a
/// 'No transfer buffer available' error.
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="transferBuffer">
///		[in]	Pointer to the transfer buffer.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		ReturnBuffer(										// not modified
										aS_Pleora_NTx* deviceHdl,
										p_u8 transferBuffer );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sends and / or receives bytes from the serial interface. </summary>
///
/// <remarks>
/// If sending bytes the receive buffer will be flushed before sending. The function will receive
/// a maximum number of bytes (rxBufferSize) within the specified time (timeOut). The function will
/// return a time out error if not enough bytes are received within the specified time. If the
/// interface is configured to use a termination char (OpenSerialPort()) the function will not
/// return a time out error if less bytes than specified are received and a the termination char is
/// received.
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="txBuffer">
///		[in]	Buffer which holds the bytes to send.
///	</param>
/// <param name="txBufferSize">
///		[in]	Number of bytes to send.
///	</param>
/// <param name="bytesWritten">
///		[out]	Number of bytes sent.
///	</param>
/// <param name="rxBuffer">
///		[out]	Pointer to allocated memory. The received bytes will filled into this buffer.
///	</param>
/// <param name="rxBufferSize">
///		[in]	Size of the allocated memory.
///	</param>
/// <param name="bytesRead">
///		[out]	Number of bytes received.
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SerialPortWriteRead(								// not modified
										aS_Pleora_NTx* deviceHdl,
										const p_u8 txBuffer,
										u32 txBufferSize,
										p_u32 bytesWritten,
										p_u8 rxBuffer,
										u32 rxBufferSize,
										p_u32 bytesRead,
										u32 timeOut );

// TODO: Describe functionality, uspare?						
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary> Sets controller Vvalues for HV and DAC </summary>
///
/// <remarks>
///				
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="uSpare">
///		[in]	??
///	</param>
/// <param name="hvSetPoint">
///		[in,out]	Sets the high voltage.
///		property         | value
///		-----------------|------
///		default          | 0 V todo
/// 	range            | -750..0 V
///	</param>
/// <param name="targetTemperature">
///		[in,out]	Sets the target temperature of the cooling system.
///		property         | value
///		-----------------|------
///		default          | 20 degree C
/// 	range            | 10..40 degree C
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SetControllerDAC(									// added
										aS_Pleora_NTx* deviceHdl,
										p_dbl uSpare,
										p_dbl hvSetPoint,
										p_dbl targetTemperature,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets the DAC within the detector. </summary>
///
/// <remarks>
///		If a value is out of range it will coerced either to the minimal or to the maximal value.
///		The values will be overwritten by the function with the values set by the function
///		after coercing / digitization. Coercing will not return an error or warning.
///		
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="vCal">
///		[in,out]	Sets the calibration voltage.
///		property         | value
///		-----------------|------
///		default          | 0.5 V
/// 	range            | 0..3 V
///	</param>
/// <param name="uMid">
///		[in,out]	Sets the buffer reference (mid) voltage.
///		property         | value
///		-----------------|------
///		default          | 1 V
/// 	range            | 0..3 V
///	</param>
/// <param name="hvSetPoint">
///		[in,out]	Sets the high voltage.
///		property         | value
///		-----------------|------
///		default          | 0 V todo
/// 	range            | -750..0 V
/// </param>
/// <param name="detCtrl">
///		[in,out]	Sets the DET voltage. Not used. Keep at default value.
///		property         | value
///		-----------------|------
///		default          | 0 V
/// 	range            | 0..3 V
///	</param>
/// <param name="targetTemperature">
///		[in,out]	Sets the target temperature of the cooling system.
///		property         | value
///		-----------------|------
///		default          | 20 degree C
/// 	range            | 10..40 degree C
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SetDAC(												// modified
										aS_Pleora_NTx* deviceHdl,
										p_dbl vCal,
										p_dbl uMid,
										p_dbl hvSetPoint,
										p_dbl detCtrl,
										p_dbl targetTemperature,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets the application frame format. </summary>
///
/// <remarks>
/// Sets the application frame format to the Pleora GigE device (fixed for this application except
/// the resolution).
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="pixelFormat">
///		[in]	Sets the pixel format for the acquisition. Keep at "Mono16"
///	</param>
/// <param name="width">
///		[in]	Sets the acquisition width / X-Resolution.
///		property         | value
///		-----------------|------
///		default          | use value from ConfigureDetector() or InitFwDefaults()
///		increment        | 4
/// 	range            | 4..16376-offsetX
///	</param>
/// <param name="height">
///		[in]	Sets the acquisition height / Y-Resolution.
///		property         | value
///		-----------------|------
///		default          | use value from ConfigureDetector() or InitFwDefaults()
///		increment        | 1
/// 	range            | 1..16383-offsetY
///	</param>
/// <param name="offsetX">
///		[in]	Sets the acquisition offset in X-Direction. Keep at default value.
///		property         | value
///		-----------------|------
///		default          | 0
///		increment        | 2
/// 	range            | 0..16376-width
///	</param>
/// <param name="offsetY">
///		[in]	Sets the acquisition offset in Y-Direction. Keep at default value.
///		property         | value
///		-----------------|------
///		default          | 0
///		increment        | 1
/// 	range            | 0..16383-height
///	</param>
/// <param name="sensorTaps">
///		[in]	Sets the number of sensor taps. Keep at "Two"
///	</param>
/// <param name="testPattern">
///		[in]	Enables the built in test pattern. Keep at "Off"
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SetFrameFormatControl(								// not modified
										aS_Pleora_NTx* deviceHdl,
										const str8 pixelFormat,
										u64 width,
										u64 height,
										u64 offsetX,
										u64 offsetY,
										const str8 sensorTaps,
										const str8 testPattern );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Sets the frame time out for the acquisition. </summary>
///
/// <remarks>
///		Sets the frame time out for the second and the following frames within an acquisition. It
///		should be set to 2.5 times of the frame time. If this is less than 25ms this function set
///		time out to 25ms. In this case no error or warning will be returned. This function is
///		optional as the time out is set during ConfigureDetector() or ConfigureDetectorWithTrigger()
///		based on the frame time set up and only needed in special cases.
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="frameTimeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SetFrameTimeOut(									// not modified
										aS_Pleora_NTx* deviceHdl,
										u32 frameTimeOut );

EXTERN_C	VSR_API	i32		SetModuleDAC(										// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										p_dbl vCal,
										p_dbl uMid,
										p_dbl detCtrl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		SetRegisterBit(										// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										u8 bitMask,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Set the number of frames acquired after a trigger event. </summary>
///
/// <remarks>
///		Set the number of frames acquired after a trigger event on trigger input 2
///		(EnableTriggerMode()).
/// </remarks>
/// 
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="frameCount">
///		[in]	Sets the number of frames to acquire.
///		property         | value
///		-----------------|------
/// 	range            | 1..4294967295
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		SetTriggeredFrameCount(								// modified
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Stops a running acquisition. </summary>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		StopAcquisition(									// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		TrainTransmisionChannel(							// added
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut );

EXTERN_C	VSR_API	i32		UploadModuleOffsetValues(							// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										Reg2BytePtr offsetValues,
										u32 offsetValuesLength,
										u32 timeOut );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Uploads offset correction values. </summary>
///
/// <remarks>
/// Uploads the offset correction values to the hardware. The existing values will be replaced.
/// The values must not exceed 80x80 values and have to fit to the current operating conditions
/// (sensor configuration, DAC settings, high voltage, temperature, ...). The values have to be
/// arranged like [r0c0, r0c1, ... , r0c(width-1), r1c0, r1c1, ... , r(height-1)c(width-1)]
/// </remarks>
///
/// <param name="deviceHdl">
///		[in]	Handle to a valid aS_Pleora_NTx device instance.
///	</param>
/// <param name="offsetValues">
///		[in]	Array containing the offset values.
///	</param>
/// <param name="offsetvalueslength">
///		[in]	Length of the array.
///	</param>
/// <param name="timeOut">
///		[in]	Communication time out (ms).
///	</param>
///
/// <returns>	Returns 0 on success or an aSpect error code on failure. </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C	VSR_API	i32		UploadOffsetValues(									// modified
										aS_Pleora_NTx* deviceHdl,
										Reg2BytePtr offsetValues,
										u32 offsetValuesLength,
										u32 timeOut );

EXTERN_C	VSR_API	i32		WriteAdcRegister(									// modified
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut );

EXTERN_C	VSR_API	i32		WriteRegister(										// modified
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut );

EXTERN_C	VSR_API	i32		WriteRegisterStream(								// modified
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										FpgaRegister_vec &registerStream,
										u32 timeOut );

u16 DacValFromVoltage( dbl voltage );											// not modified

dbl DacValToVoltage( u16 number );												// not modified

i32 HexToString( u32 number, u8 digits, p_u8 target );							// not modified

i32 HexToStringLE( u32 number, u8 digits, p_u8 target );						// not modified

i32 StringToHex( p_u8 source, u8 digits, p_u32 number );						// not modified

i32 CheckTemperatureLimit(														// modified
										aS_Pleora_NTx* deviceHdl,
										p_dbl temperature,
										u32 timeOut );

void DisableTriggerGateCB( hdl deviceHdl);										// not modified

void EnableTriggerGateCB( hdl deviceHdl);										// not modified

dbl GetAdcTemperature( u16 number );											// not modified

dbl GetAmbientTemperature( u16 number );										// not modified

dbl GetAsicTemperature( u16 number );											// not modified

u32	GetCollectDcTime( dbl frameTime );											// not modified

dbl GetHumidity( u16 number );													// not modified

dbl GetHvOut( dbl hvMon );														// not modified

dbl GetInternalReference( u16 number );											// not modified

dbl GetPeltierCurrent( dbl voltage );											// not modified

dbl GetVoltage( u16 number, dbl internalReference );							// not modified

i32 GetModulSetupReg(															// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrSetupRegisterPtr setupReg,
										u32 timeOut);

i32 GetSensorSetupReg(															// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										HexitecSensorSetupRegisterPtr setupReg,
										u32 timeOut);

i32 GetSetupReg(																// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										p_u8 setupReg,
										u32 timeOut);

i32	GetSyncModeState(															// added
										aS_Pleora_NTx* deviceHdl,
										ControlPtr syncModeState,
										u32 timeOut );

u16 HvDacValFromHv( dbl hv );													// not modified

dbl	HvDacValToHv( u16 number );													// not modified

i32	SetModuleOperationMode(														// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecOperationMode operationMode,
										u32 timeOut);

i32 SetModuleSensorConfig(														// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrSensorConfig sensorConfig,
										u32 timeOut );

i32	SetOperationMode(															// modified
										aS_Pleora_NTx* deviceHdl,
										HexitecOperationMode operationMode,
										u32 timeOut);

i32	SetSensorConfig(															// modified
										aS_Pleora_NTx* deviceHdl,
										HexitecSensorConfig sensorConfig,
										u32 timeOut);

i32 SetSensorSetupReg(															// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										HexitecSensorSetupRegister setupReg,
										u32 timeOut);

i32 SetSetupReg(																// added
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										p_u8 setupReg,
										u32 timeOut);

u16 TemperatureDacValFromTemperature( dbl temperature );						// modified

dbl TemperatureDacValToTemperature(												// modified
										u16 number,
										dbl internalReference,
										AdcDacSteps steps );

dbl PwrTemperatureDacValToTemperature(											// added
										u16 number,
										dbl internalReference,
										AdcDacSteps steps );

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of VSR.h
////////////////////////////////////////////////////////////////////////////////////////////////////	
