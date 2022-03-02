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

#include "../../aS_lib/aS_Pleora_NTx/aS_Pleora_NTx.h"
#include "../../aS_lib/aS_InstanceManager/aS_InstanceManager.h"

using namespace aS;

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

typedef enum HexitecGain : u8 {
	AS_HEXITEC_GAIN_HIGH		= 0,
	AS_HEXITEC_GAIN_LOW			= 1
	} HexitecGain;

typedef enum HexitecAdcSample : u8 {
	AS_HEXITEC_ADC_SAMPLE_RISING_EDGE	= 0,
	AS_HEXITEC_ADC_SAMPLE_FALLING_EDGE	= 1
	} HexitecAdcSample;

#ifndef _M_X64
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct HexitecVsrOperatingValues {
	dbl		v3_3;
	dbl		hvMon;
	dbl		hvOut;
	dbl		v1_2;
	dbl		v1_8;
	dbl		v2_5;
	dbl		v3_3ln;
	dbl		v1_65ln;
	dbl		v1_8ana;
	dbl		v3_8ana;
	} HexitecVsrOperatingValues, *HexitecVsrOperatingValuesPtr, **HexitecVsrOperatingValuesHdl;

typedef struct HexitecControllerOperatingValues {
	dbl		peltierCurrent;
	dbl		ntcTemperature;
	dbl		hvCurrent;
	dbl		hvVoltage;
	Control	v1_8_pwrGood;
	Control	v2_5_pwrGood;
	Control	v3_3_pwrGood;
	Control	v5_0_pwrGood;
	Control	v5_5_pwrGood;
	Control	v_spare_pwrGood;
	Control	vPL_pwrGood;
	Control	vPS_pwrGood;
	} HexitecControllerOperatingValues, *HexitecControllerOperatingValuesPtr, **HexitecControllerOperatingValuesHdl;

typedef struct HexitecSystemConfig {
	u8					AdcDelay;
	u8					SyncSignalDelay;
	HexitecAdcSample	AdcSample;
	u8					AdcClockPhase;
	} HexitecSystemConfig, *HexitecSystemConfigPtr, **HexitecSystemConfigHdl;

typedef struct HexitecSetupRegister {
	u8				ReadEn[AS_HEXITEC_SETUP_REGISTER_SIZE];
	u8				PowerEn[AS_HEXITEC_SETUP_REGISTER_SIZE];
	u8				CalEn[AS_HEXITEC_SETUP_REGISTER_SIZE];
	} HexitecSetupRegister, *HexitecSetupRegisterPtr, **HexitecSetupRegisterHdl;

typedef struct HexitecSensorSetupRegister {
	HexitecSetupRegister Col;
	HexitecSetupRegister Row;
	} HexitecSensorSetupRegister, *HexitecSensorSetupRegisterPtr, **HexitecSensorSetupRegisterHdl;

typedef struct HexitecVsrSetupRegister {
	HexitecSensorSetupRegister Sensor1;
	HexitecSensorSetupRegister Sensor2;
	} HexitecVsrSetupRegister, *HexitecVsrSetupRegisterPtr, **HexitecVsrSetupRegisterHdl;

typedef struct HexitecSensorConfig {
	HexitecGain				Gain;
	Reg2Byte				Row_S1;
	u8						S1_Sph;
	u8						Sph_S2;
	Reg2Byte				Vcal2_Vcal1;
	u8						WaitClockCol;
	u8						WaitClockRow;
	HexitecVsrSetupRegister	SetupRegVsr1;
	HexitecVsrSetupRegister	SetupRegVsr2;
	} HexitecSensorConfig, *HexitecSensorConfigPtr, **HexitecSensorConfigHdl;

typedef struct HexitecVsrEnvironmentValues {
	dbl						humidity;
	dbl						ambientTemperature;
	dbl						asicTemperature1;
	dbl						asicTemperature2;
	dbl						adcTemperature;
	} HexitecVsrEnvironmentValues, *HexitecVsrEnvironmentValuesPtr, **HexitecVsrEnvironmentValuesHdl;

typedef struct HexitecVsrSensorConfig {
	HexitecGain				Gain;
	Reg2Byte				Row_S1;
	u8						S1_Sph;
	u8						Sph_S2;
	Reg2Byte				Vcal2_Vcal1;
	u8						WaitClockCol;
	u8						WaitClockRow;
	HexitecVsrSetupRegister	SetupReg;
	} HexitecVsrSensorConfig, *HexitecVsrSensorConfigPtr, **HexitecVsrSensorConfigHdl;

typedef struct HexitecOperationMode {
	Control			DcUploadDarkCorrectionValues;
	Control			DcCollectDarkCorrectionValues;
	Control			DcEnableDarkCorrectionCountingMode;
	Control			DcEnableDarkCorrectionSpectroscopicMode;
	Control			DcSendDarkCorrectionValues;
	Control			DcDisableVcalPulse;
	Control			DcTestMode;
	Control			DcEnableTriggeredCountingMode;
	Control			EdUploadThresholdValues;
	Control			EdDisableCountingMode;
	Control			EdTestMode;
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

EXTERN_C	VSR_API	i32		AcquireFrames(										// not modified
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										p_u64 framesAcquired,
										u32 frameTimeOut );

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

EXTERN_C	VSR_API	i32		ClosePipeline(										// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		CloseSerialPort(									// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		CloseStream(										// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		CollectOffsetValues(								// bugfix + modified
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut,
										u32 collectDcTimeOut );

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

EXTERN_C	VSR_API	i32		ExitDevice(											// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		GetAsErrorMsg(										// not modified
										i32 asError,
										str8 asErrorMsg,
										u32 length );

EXTERN_C	VSR_API	i32		GetBufferHandlingThreadPriority(					// not modified
										aS_Pleora_NTx* deviceHdl,
										p_i32 priority );

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

EXTERN_C	VSR_API	i32		GetSystemErrorMsg(									// not modified
										i32 sysError,
										str8 sysErrorMsg,
										u32 length );

EXTERN_C	VSR_API	i32		GetTriggerState(									// modified
										aS_Pleora_NTx* deviceHdl,
										p_u8 trigger1,
										p_u8 trigger2,
										p_u8 trigger3,
										u32 timeOut );

EXTERN_C	VSR_API	i32		InitDevice(											// not modified
										aS_Pleora_NTx** deviceHdl,
										const str8 deviceDescriptor,
										p_u32 internalResult,
										str8 pleoraErrorCodeString,
										p_u32 pleoraErrorCodeStringLen,
										str8 pleoraErrorDescription,
										p_u32 pleoraErrorDescriptionLen );

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

EXTERN_C	VSR_API	i32		OpenSerialPort(										// not modified
										aS_Pleora_NTx* deviceHdl,
										PvDeviceSerial serialPort,
										u32 rxBufferSize,
										u8 useTermChar,
										u8 termChar );

EXTERN_C	VSR_API	i32		OpenStream(											// not modified
										aS_Pleora_NTx* deviceHdl );

EXTERN_C	VSR_API	i32		ReadControllerEnvironmentValues(					// added
										aS_Pleora_NTx* deviceHdl,
										p_dbl peltierCurrent,
										p_dbl ntcTemperature,
										p_dbl daqPwrTemperature,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadControllerOperatingValues(						// added
										aS_Pleora_NTx* deviceHdl,
										HexitecControllerOperatingValuesPtr opValuesController,
										u32 timeOut );

EXTERN_C	VSR_API	i32		ReadControllerPowerState(							// added
										aS_Pleora_NTx* deviceHdl,
										p_u8 powerState,
										u32 timeOut );

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

EXTERN_C	VSR_API	i32		RegisterTransferBufferReadyCallBack(				// not modified
										aS_Pleora_NTx* deviceHdl,
										p_bufferCallBack transferBufferReadyCallBack );

EXTERN_C	VSR_API	i32		ReturnBuffer(										// not modified
										aS_Pleora_NTx* deviceHdl,
										p_u8 transferBuffer );

EXTERN_C	VSR_API	i32		SerialPortWriteRead(								// not modified
										aS_Pleora_NTx* deviceHdl,
										const p_u8 txBuffer,
										u32 txBufferSize,
										p_u32 bytesWritten,
										p_u8 rxBuffer,
										u32 rxBufferSize,
										p_u32 bytesRead,
										u32 timeOut );

EXTERN_C	VSR_API	i32		SetControllerDAC(									// added
										aS_Pleora_NTx* deviceHdl,
										p_dbl uSpare,
										p_dbl hvSetPoint,
										p_dbl targetTemperature,
										u32 timeOut );

EXTERN_C	VSR_API	i32		SetDAC(												// modified
										aS_Pleora_NTx* deviceHdl,
										p_dbl vCal,
										p_dbl uMid,
										p_dbl hvSetPoint,
										p_dbl detCtrl,
										p_dbl targetTemperature,
										u32 timeOut );

EXTERN_C	VSR_API	i32		SetFrameFormatControl(								// not modified
										aS_Pleora_NTx* deviceHdl,
										const str8 pixelFormat,
										u64 width,
										u64 height,
										u64 offsetX,
										u64 offsetY,
										const str8 sensorTaps,
										const str8 testPattern );

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

EXTERN_C	VSR_API	i32		SetTriggeredFrameCount(								// modified
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										u32 timeOut );

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