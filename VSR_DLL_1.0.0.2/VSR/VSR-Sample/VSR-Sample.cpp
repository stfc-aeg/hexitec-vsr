// VSR-Sample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VSR.h"

#define STR_LENGTH 1024

aS_Pleora_NTx*			deviceHdl = NULL;
u32						pleoraErrorCode = 0;
i8						pleoraErrorCodeStr[STR_LENGTH] = { 0 };
i8						pleoraErrorDescription[STR_LENGTH] = { 0 };
i8						asErrorMessage[STR_LENGTH] = { 0 };
i8						sysErrorMessage[STR_LENGTH] = { 0 };
i32						asError = 0;
u32						width = 0;
u32						height = 0;
dbl						frameTime = 0;
u32						collectDcTime = 0;

HexitecSystemConfig		systemConfig = { 2, 10, AS_HEXITEC_ADC_SAMPLE_FALLING_EDGE, 4 };
HexitecSetupRegister RC = { { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
HexitecSensorSetupRegister Sensor = { RC, RC };
HexitecVsrSetupRegister Vsr = { Sensor, Sensor };
HexitecSensorConfig sensorConfig = { AS_HEXITEC_GAIN_HIGH,{ { 1 } }, 1, 14,{ { 0 } }, 1, 8, Vsr, Vsr };
HexitecSetupRegister	setupConfig;
HexitecOperationMode	operationMode = { AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, AS_CONTROL_ENABLED, AS_CONTROL_DISABLED, AS_CONTROL_ENABLED, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, AS_CONTROL_DISABLED, 0 };

u8	customerID = 0x01;
u8	daqProjectID = 0x19;
u8	daqVersion = 0x02;
u8	vsrProjectID = 0x17;
u8	vsrVersion1 = 0x03, vsrVersion2 = 0x03;

VOID showError(const str8 context)
{
	i32 result = 1;
	i32 sysError = 0;
	u32 pleoraErrorCodeStrLen = STR_LENGTH;
	u32 pleoraErrorDescriptionLen = STR_LENGTH;

	sysError = GetAsErrorMsg(asError, asErrorMessage, STR_LENGTH);

	if (sysError)
	{
		sysError = GetSystemErrorMsg(sysError, sysErrorMessage, STR_LENGTH);
		printf("\n%s\n\tSystem Error (%d, %s) occured trying to get aSpect error message for aSpect error: %d\n", context, sysError, sysErrorMessage, asError);
		return;
	}
	else if (asError)
	{
		if (context != "InitDevice")
		{
			result = GetLastResult(deviceHdl, &pleoraErrorCode, pleoraErrorCodeStr, &pleoraErrorCodeStrLen, pleoraErrorDescription, &pleoraErrorDescriptionLen);
		}
		else
		{
			result = 0;
		}
	}

	printf("\n%s\n\taSpect Result: %s", context, asErrorMessage);

	if (!result)
	{
		printf("\n\tPleora Result Code: %d", pleoraErrorCode);
		printf("\n\tPleora Result Code String: %s", pleoraErrorCodeStr);
		printf("\n\tPleora Result Description: %s\n", pleoraErrorDescription);
	}
}

/// <summary>
/// Example function, which could be registered as a callback to handle filled / available transfer buffers.
/// </summary>
/// <remarks>
/// This an example and not useful for a final implementation. The 'transferBuffer' and the 'frameCount' should be placed in a queue for a data processing thread. The function 'ReturnBuffer'
/// have to be called from the data processing thread once the data within the buffer is processed. The implementation of this function should have a short runtime without any blocking point.
/// Otherwise this function will block the frame acquisition process.
/// </remarks>
/// <param name="transferBuffer">Pointer to the transfer buffer.</param>
/// <param name="frameCount">Specify how many valid frames the transfer buffer contains.</param>
VOID TransferBufferReady(p_u8 transferBuffer, u32 frameCount)
{
	printf("\n\tTransfer Buffer Pointer: 0x%016I64x, Frame Count: %u\n", (u64)transferBuffer, frameCount);
	ReturnBuffer(deviceHdl, transferBuffer);
}

VOID initRoutine()
{
	u32					pleoraErrorCodeStrLen = STR_LENGTH;
	u32					pleoraErrorDescriptionLen = STR_LENGTH;
	NTxDeviceInfoStr	deviceInformation = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	FpgaFwVersion 		controller, vsr1, vsr2;

	controller = { customerID,	daqProjectID, daqVersion, AS_CONTROL_DISABLED };
	vsr1 = { customerID,	vsrProjectID, vsrVersion1, AS_CONTROL_DISABLED };
	vsr2 = { customerID,	vsrProjectID, vsrVersion2, AS_CONTROL_DISABLED };

	asError = InitDevice(&deviceHdl, "", &pleoraErrorCode, pleoraErrorCodeStr, &pleoraErrorCodeStrLen, pleoraErrorDescription, &pleoraErrorDescriptionLen);
	showError("InitDevice");
	if ( (asError != AS_NO_ERROR) || (deviceHdl == NULL) ) 
	{
		return;
	}

	asError = GetDeviceInformation(deviceHdl, &deviceInformation);
	showError("GetDeviceInformation");

	if (!asError)
	{
		printf("\n\tVendor: %s", deviceInformation.Vendor);
		printf("\n\tModel: %s", deviceInformation.Model);
		printf("\n\tManufacturerInfo: %s", deviceInformation.ManufacturerInfo);
		printf("\n\tSerialNumber: %s", deviceInformation.SerialNumber);
		printf("\n\tUserId: %s", deviceInformation.UserId);
		printf("\n\tMacAddress: %s", deviceInformation.MacAddress);
		printf("\n\tIpAddress: %s", deviceInformation.IpAddress);
		printf("\n\tNetMask: %s", deviceInformation.NetMask);
		printf("\n\tGateWay: %s\n", deviceInformation.GateWay);
	}

	asError = OpenSerialPort(deviceHdl, PvDeviceSerialBulk0, 2048, 1, 0x0d);
	showError("OpenSerialPort");

	asError = EnableDetector(deviceHdl, 1000);
	showError("EnableDetector");

	asError = CheckFirmware(deviceHdl, &controller, &vsr1, &vsr2, 1000);
	if (asError)
		showError("CheckFirmware");

	if (!asError)
	{
		printf("DAQ Board");
		printf("\nFPGA Firmware - installed customerID: \t0x%02x", controller.customerId);
		printf("\nFPGA Firmware - installed projectID: \t0x%02x", controller.projectId);
		printf("\nFPGA Firmware - installed version: \t0x%02x\n\n", controller.version);

		printf("VSR Board                              \tVSR1\tVSR2");
		printf("\nFPGA Firmware - installed customerID: \t0x%02x\t0x%02x", vsr1.customerId, vsr2.customerId);
		printf("\nFPGA Firmware - installed projectID: \t0x%02x\t0x%02x", vsr1.projectId, vsr2.projectId);
		printf("\nFPGA Firmware - installed version: \t0x%02x\t0x%02x\n", vsr1.version, vsr2.version);

		vsrVersion1 = vsr1.version;
		vsrVersion2 = vsr2.version;
	}

#define		lTxBufferSize	4
#define		lRxBufferSize	256
	u8		lTxBuffer[lTxBufferSize] = { 0x23, 0x90, 0x72, 0x0d };
	u8		lRxBuffer[lRxBufferSize] = { 0 };

	u32		lBytesWritten = 0;
	u32		lBytesRead = 0;
	u32		lDetectorType = 0xff;

	asError = SerialPortWriteRead(deviceHdl, (p_u8)&lTxBuffer[0], lTxBufferSize, &lBytesWritten, (p_u8)&lRxBuffer[0], lRxBufferSize, &lBytesRead, 1000);

	printf("\nuC Firmware - installed version: %s\n", &lRxBuffer[2]);


	asError = OpenStream(deviceHdl);
	showError("OpenStream");

	asError = RegisterTransferBufferReadyCallBack(deviceHdl, TransferBufferReady);
	showError("RegisterTransferBufferReadyCallBack");
}

VOID exitRoutine()
{
	dbl		vCal = 0.5;
	dbl		uMid = 1;
	dbl		hvSetPoint = 0;
	dbl		detCtrl = 0;
	dbl		targetTemperature = 25;

	asError = SetDAC(deviceHdl, &vCal, &uMid, &hvSetPoint, &detCtrl, &targetTemperature, 1000);
	showError("SetDAC");

	asError = ClosePipeline(deviceHdl);
	showError("ClosePipeline");

	asError = CloseStream(deviceHdl);
	showError("CloseStream");

	asError = DisableDetector(deviceHdl, 1000);
	showError("DisableDetecor");

	asError = CloseSerialPort(deviceHdl);
	showError("CloseSerialPort");

	asError = ExitDevice(deviceHdl);
	showError("ExitDevice");
}

VOID systemMonitor()
{
	dbl									ntcTemperature = 0;
	dbl									daqPwrTemperature = 0;
	HexitecVsrEnvironmentValues			envValuesVsr1, envValuesVsr2;
	HexitecControllerOperatingValues 	opValuesController;
	HexitecVsrOperatingValues 			opValuesVsr1, opValuesVsr2;

	asError = ReadEnvironmentValues(deviceHdl, &envValuesVsr1, &envValuesVsr2, &ntcTemperature, &daqPwrTemperature, 1000);
	showError("ReadEnvironmentValues");

	if (!asError)
	{
		printf("\n\t                              \t\tVSR1\t\tVSR2");
		printf("\n\tEnvironment Value - humidity: \t\t%.2f %%\t\t%.2f %%", envValuesVsr1.humidity, envValuesVsr2.humidity);
		printf("\n\tEnvironment Value - ambientTemperature: %.2f degC\t%.2f degC", envValuesVsr1.ambientTemperature, envValuesVsr1.ambientTemperature);
		printf("\n\tEnvironment Value - asicTemperature1: \t%.2f degC\t%.2f degC", envValuesVsr1.asicTemperature1, envValuesVsr2.asicTemperature1);
		printf("\n\tEnvironment Value - asicTemperature2: \t%.2f degC\t%.2f degC", envValuesVsr1.asicTemperature2, envValuesVsr2.asicTemperature2);
		printf("\n\tEnvironment Value - adcTemperature: \t%.2f degC\t%.2f degC", envValuesVsr1.adcTemperature, envValuesVsr2.adcTemperature);

		printf("\n\n\tEnvironment Value - ntcTemperature: %.2f degC", ntcTemperature);
		printf("\n\tEnvironment Value - daqPwrTemperature: %.2f degC\n", daqPwrTemperature);
	}


	opValuesVsr1.v3_3 = 0;
	opValuesVsr1.hvMon = 0;
	opValuesVsr1.hvOut = 0;
	opValuesVsr1.v1_2 = 0;
	opValuesVsr1.v1_8 = 0;
	opValuesVsr1.v2_5 = 0;
	opValuesVsr1.v3_3ln = 0;
	opValuesVsr1.v1_65ln = 0;
	opValuesVsr1.v1_8ana = 0;
	opValuesVsr1.v3_8ana = 0;

	opValuesVsr2.v3_3 = 0;
	opValuesVsr2.hvMon = 0;
	opValuesVsr2.hvOut = 0;
	opValuesVsr2.v1_2 = 0;
	opValuesVsr2.v1_8 = 0;
	opValuesVsr2.v2_5 = 0;
	opValuesVsr2.v3_3ln = 0;
	opValuesVsr2.v1_65ln = 0;
	opValuesVsr2.v1_8ana = 0;
	opValuesVsr2.v3_8ana = 0;

	opValuesController.peltierCurrent = 0;
	opValuesController.hvCurrent = 0;
	opValuesController.hvVoltage = 0;

	asError = ReadOperatingValues(deviceHdl, &opValuesController, &opValuesVsr1, &opValuesVsr2, 1000);
	showError("ReadOperatingValues");

	if (!asError)
	{
		printf("\n\t                        \tVSR1\tVSR2");
		printf("\n\tOperating Value - v3_3: \t%.2f V\t%.2f V", opValuesVsr1.v3_3, opValuesVsr2.v3_3);
		printf("\n\tOperating Value - hvMon: \t%.2f V\t%.2f V", opValuesVsr1.hvMon, opValuesVsr2.hvMon);
		printf("\n\tOperating Value - hvOut: \t%.2f V\t%.2f V", opValuesVsr1.hvOut, opValuesVsr2.hvOut);
		printf("\n\tOperating Value - v1_2: \t%.2f V\t%.2f V", opValuesVsr1.v1_2, opValuesVsr2.v1_2);
		printf("\n\tOperating Value - v1_8: \t%.2f V\t%.2f V", opValuesVsr1.v1_8, opValuesVsr2.v1_8);
		printf("\n\tOperating Value - v2_5: \t%.2f V\t%.2f V", opValuesVsr1.v2_5, opValuesVsr2.v2_5);
		printf("\n\tOperating Value - v3_3ln: \t%.2f V\t%.2f V", opValuesVsr1.v3_3ln, opValuesVsr2.v3_3ln);
		printf("\n\tOperating Value - v1_65ln: \t%.2f V\t%.2f V", opValuesVsr1.v1_65ln, opValuesVsr2.v1_65ln);
		printf("\n\tOperating Value - v1_8ana: \t%.2f V\t%.2f V", opValuesVsr1.v1_8ana, opValuesVsr2.v1_8ana);
		printf("\n\tOperating Value - v3_8ana: \t%.2f V\t%.2f V", opValuesVsr1.v3_8ana, opValuesVsr2.v3_8ana);

		printf("\n\n\tOperating Value - peltierCurrent: %f A", opValuesController.peltierCurrent);
		printf("\n\tOperating Value - ntcTemperature: %.2f degC\n", opValuesController.ntcTemperature);
	}
}

VOID configureDetectorRoutine(Control EnSyncMode, Control EnTriggerMode)
{
	asError = ClosePipeline(deviceHdl);
	showError("ClosePipeline");

	asError = ConfigureDetectorWithTrigger(deviceHdl, &sensorConfig, &operationMode, &systemConfig, &width, &height, &frameTime, &collectDcTime, 1000, EnSyncMode, EnTriggerMode);
	showError("ConfigureDetector");

	if (!asError)
	{
		printf("\n\tConfigure Detector - width: %u Columns", width);
		printf("\n\tConfigure Detector - height: %u Rows", height);
		printf("\n\tConfigure Detector - frameTime: %f s", frameTime);
		printf("\n\tConfigure Detector - collectDcTime: %u ms\n", collectDcTime);
	}

	dbl		vCal = 0.5;
	dbl		uMid = 1;
	dbl		hvSetPoint = -100;
	dbl		detCtrl = 0;
	dbl		targetTemperature = 20;

	asError = SetDAC(deviceHdl, &vCal, &uMid, &hvSetPoint, &detCtrl, &targetTemperature, 1000);
	showError("SetDAC");

	asError = SetFrameFormatControl(deviceHdl, "Mono16", width, height, 0, 0, "Two", "Off");
	showError("SetFrameFormatControl");

	asError = CreatePipeline(deviceHdl, 512, 100, 100);
	showError("CreatePipeline");
}

VOID acquireFramesRoutine(u32 frameCount, u32 triggerTimeOut)
{
	u64			acquiredFrames = 0;
	u32			frameTimeOut = triggerTimeOut + ((u32)(frameTime * 2500.0));

	if (frameTimeOut < 100)
	{
		frameTimeOut = 100;
	}

	asError = AcquireFrames(deviceHdl, frameCount, &acquiredFrames, frameTimeOut);				// frameTimeOut should be set to (2.5 * FrameTime) and 100ms at least
	showError("AcquireFrames");

	printf("\n\tRequested Frames: %u, Acquired Frames %u\n", frameCount, (u32)acquiredFrames);
}

VOID checkTriggerStateRoutine()
{
	u8	 triggerState1 = 0;
	u8	 triggerState2 = 0;
	u8	 triggerState3 = 0;

	asError = GetTriggerState(deviceHdl, &triggerState1, &triggerState2, &triggerState3, 1000);
	showError("CheckTrigger");

	printf("\n\tT1: %u, T2: %u, T3: %u\n", triggerState1, triggerState2, triggerState3);
}

VOID syncModeRoutine(u32 frameCount, u32 triggerTimeOut)
{
	configureDetectorRoutine(AS_CONTROL_ENABLED, AS_CONTROL_DISABLED);

	printf("\n\tApply a rising edge on trigger input 1 within %.3fs and keep high for at least %.3fs\n", ((dbl)triggerTimeOut) / 1000, frameTime*frameCount);

	acquireFramesRoutine(frameCount, triggerTimeOut);
}

VOID triggerModeRoutine(u32 frameCountAcquire, u32 frameCountTrigger, u32 triggerTimeOut)
{
	configureDetectorRoutine(AS_CONTROL_DISABLED, AS_CONTROL_ENABLED);

	asError = SetTriggeredFrameCount(deviceHdl, frameCountTrigger, 1000);
	showError("SetTriggeredFrameCount");

	printf("\n\tApply %u rising edges on trigger input 2 with max %.3fs between the edges\n", (frameCountAcquire / frameCountTrigger) + 1, ((dbl)triggerTimeOut) / 1000);
	printf("\nor\n");
	printf("\n\tApply a high level for at least %.3fs on trigger input 3 within %.3fs\n", frameTime*frameCountAcquire, ((dbl)triggerTimeOut) / 1000);

	acquireFramesRoutine(frameCountAcquire, triggerTimeOut);
}

VOID offsetValueFunction()
{
	asError = CollectOffsetValues(deviceHdl, 1000, collectDcTime);								// make sure to have stable operating conditions (high voltage, temperature, x-ray turned off)
	showError("CollectOffsetValues");
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Initialization routine
	initRoutine();

	if ( asError == AS_NO_ERROR )
	{
		// Monitoring routines
		systemMonitor();

		configureDetectorRoutine(AS_CONTROL_DISABLED, AS_CONTROL_DISABLED);

		acquireFramesRoutine(248, 0);

		acquireFramesRoutine(316, 0);

		// Change Resolution
		sensorConfig.SetupRegVsr1.Sensor1.Col.ReadEn[2] = 0x00;
		sensorConfig.SetupRegVsr1.Sensor1.Col.ReadEn[7] = 0x00;
		sensorConfig.SetupRegVsr1.Sensor2.Col.ReadEn[2] = 0x00;
		sensorConfig.SetupRegVsr1.Sensor2.Col.ReadEn[7] = 0x00;

		sensorConfig.SetupRegVsr2.Sensor1.Col.ReadEn[2] = 0x00;
		sensorConfig.SetupRegVsr2.Sensor1.Col.ReadEn[7] = 0x00;
		sensorConfig.SetupRegVsr2.Sensor2.Col.ReadEn[2] = 0x00;
		sensorConfig.SetupRegVsr2.Sensor2.Col.ReadEn[7] = 0x00;

		configureDetectorRoutine(AS_CONTROL_DISABLED, AS_CONTROL_DISABLED);

		offsetValueFunction();

		acquireFramesRoutine(167, 0);

		acquireFramesRoutine(497, 0);

		if ((vsrVersion1 >= 0x02) && (vsrVersion2 >= 0x02))
		{

			int i = 0;

			checkTriggerStateRoutine();

			while (scanf_s("%d", &i))
			{
				checkTriggerStateRoutine();
			}

			syncModeRoutine(153, 20000);

			triggerModeRoutine(871, 206, 20000);

			configureDetectorRoutine(AS_CONTROL_DISABLED, AS_CONTROL_DISABLED);
		}
		else
		{
			printf("\nTrigger functionality is not supported from the device.\n");
		}

		systemMonitor();
		exitRoutine();
	}

	printf("\n\nPress Enter Key to continue");

	getchar();

	return 0;
}