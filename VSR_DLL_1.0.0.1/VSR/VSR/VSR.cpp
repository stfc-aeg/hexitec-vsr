// VSR.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "VSR.h"

InstanceManager			hTable;

EXTERN_C	VSR_API	i32		AcquireFrame(
										aS_Pleora_NTx* deviceHdl,
										p_u32 frameCount,
										p_u8 buffer,
										u32 frameTimeOut )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->AcquireImage( frameCount, buffer, frameTimeOut );
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		AcquireFrames(
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										p_u64 framesAcquired,
										u32 frameTimeOut )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->AcquireImageThread( frameCount, frameTimeOut );
		*framesAcquired = deviceHdl->GetAcquiredImageCount();
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		CheckFirmware(
										aS_Pleora_NTx* deviceHdl,
										FpgaFwVersionPtr controller,
										FpgaFwVersionPtr vsr1,
										FpgaFwVersionPtr vsr2,
										u32 timeOut )
{
	i32 lResult = AS_NO_ERROR;

	lResult = CheckModuleFirmware(
										deviceHdl,
										AS_MODULE_ADDRESS_CONTROLLER,
										&controller->customerId,
										&controller->projectId,
										&controller->version,
										controller->forceEqualVersion,
										timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = CheckModuleFirmware(
											deviceHdl,
											AS_MODULE_ADDRESS_VSR1,
											&vsr1->customerId,
											&vsr1->projectId,
											&vsr1->version,
											vsr1->forceEqualVersion,
											timeOut );
	}
	else
	{
		(*vsr1).customerId = 0;
		(*vsr1).projectId = 0;
		(*vsr1).version = 0;
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = CheckModuleFirmware(
											deviceHdl,
											AS_MODULE_ADDRESS_VSR2,
											&vsr2->customerId,
											&vsr2->projectId,
											&vsr2->version,
											vsr2->forceEqualVersion,
											timeOut );
	}
	else
	{
		(*vsr2).customerId = 0;
		(*vsr2).projectId = 0;
		(*vsr2).version = 0;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		CheckModuleFirmware(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										p_u8 customerId,
										p_u8 projectId,
										p_u8 version,
										u8 forceEqualVersion,
										u32 timeOut )
{
	i32 lResult = AS_NO_ERROR;
	u8 lRequiredCustomerId = *customerId;
	u8 lRequiredProjectId = *projectId;
	u8 lRequiredVersion = *version;

	lResult = ReadRegister( deviceHdl, moduleAddress, AS_FPGA_FW_CHECK_CUSTOMER_REG, customerId, timeOut);

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, moduleAddress, AS_FPGA_FW_CHECK_PROJECT_REG, projectId, timeOut);
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, moduleAddress, AS_FPGA_FW_CHECK_VERSION_REG, version, timeOut);
	}

	if( (lResult == AS_NO_ERROR) && (lRequiredCustomerId != *customerId) )
	{
		lResult = AS_FPGA_FW_CHECK_CUSTOMER_ERROR;
	}

	if( (lResult == AS_NO_ERROR) && (lRequiredProjectId != *projectId) )
	{
		lResult = AS_FPGA_FW_CHECK_PROJECT_ERROR;
	}

	if( (lResult == AS_NO_ERROR) && forceEqualVersion && (lRequiredVersion != *version) )
	{
		lResult = AS_FPGA_FW_CHECK_VERSION_ERROR;
	}
	else if( (lResult == AS_NO_ERROR) && (lRequiredVersion > *version) )
	{
		lResult = AS_FPGA_FW_CHECK_VERSION_ERROR;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		ClearRegisterBit(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										u8 bitMask,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	
	lRxBuffer.resize(7, 0);
	lTxBuffer.resize(8, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x43;
	HexToString( registerAddress, 2, (p_u8)&lTxBuffer[3] );
	HexToString( bitMask, 2, (p_u8)&lTxBuffer[5] );
	lTxBuffer[7] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		ClosePipeline(
										aS_Pleora_NTx* deviceHdl )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->ClosePipeline();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		CloseSerialPort(
										aS_Pleora_NTx* deviceHdl )
{				
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->CloseSerialPort();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		CloseStream(
										aS_Pleora_NTx* deviceHdl )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->CloseStream();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		CollectOffsetValues(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut,
										u32 collectDcTimeOut )
{
	i32 dResult = AS_NO_ERROR;
	u8	lValue = 0;
	HexitecOperationMode lCurrentMode;
	HexitecOperationMode lCollectMode;
	Control lSyncModeState = AS_CONTROL_DISABLED;

	dResult = GetOperationMode( deviceHdl, &lCurrentMode, timeOut );

	if( dResult == AS_NO_ERROR )
	{
		lCollectMode = lCurrentMode;
		lCollectMode.DcCollectDarkCorrectionValues = Control::AS_CONTROL_ENABLED;
		lCollectMode.DcUploadDarkCorrectionValues = Control::AS_CONTROL_DISABLED;
		lCollectMode.DcDisableVcalPulse = Control::AS_CONTROL_ENABLED;
		
		dResult = GetSyncModeState( deviceHdl, &lSyncModeState, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = DisableSM( deviceHdl, timeOut );
	}

	if( (dResult == AS_NO_ERROR) && (lSyncModeState == AS_CONTROL_ENABLED) )
	{
		dResult = DisableSyncMode( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = SetOperationMode( deviceHdl, lCollectMode, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = EnableSM( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		Sleep( collectDcTimeOut );
		dResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x89, &lValue, timeOut );
	}

	if( dResult == AS_NO_ERROR && !( lValue & 0x01 ) )
	{
		dResult = AS_COLLECT_DC_NOT_READY;
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x89, &lValue, timeOut );
	}

	if( dResult == AS_NO_ERROR && !( lValue & 0x01 ) )
	{
		dResult = AS_COLLECT_DC_NOT_READY;
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = DisableSM( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = SetOperationMode( deviceHdl, lCurrentMode, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = TrainTransmisionChannel( deviceHdl, timeOut );
	}

	if( (dResult == AS_NO_ERROR) && (lSyncModeState == AS_CONTROL_ENABLED) )
	{
		dResult = EnableSyncMode( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = EnableSM( deviceHdl, timeOut );
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		ConfigureDetector(
										aS_Pleora_NTx* deviceHdl,
										const HexitecSensorConfigPtr sensorConfig,
										const HexitecOperationModePtr operationMode,
										const HexitecSystemConfigPtr systemConfig,
										p_u32 width,
										p_u32 height,
										p_dbl frameTime,
										p_u32 collectDcTime,
										u32 timeOut )
{
	return ConfigureDetectorWithTrigger(
										deviceHdl,
										sensorConfig,
										operationMode,
										systemConfig,
										width,
										height,
										frameTime,
										collectDcTime,
										timeOut,
										AS_CONTROL_DISABLED,
										AS_CONTROL_DISABLED );
}

EXTERN_C	VSR_API	i32		ConfigureDetectorWithTrigger(
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
										Control enTriggerMode )
{
	i32 lResult = AS_NO_ERROR;
	u8 lValue = 0;

	lResult = DisableSM( deviceHdl, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = DisableSyncMode( deviceHdl, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		if( enTriggerMode )
		{
			lResult = EnableTriggerMode( deviceHdl, timeOut );
		}
		else
		{
			lResult = DisableTriggerMode( deviceHdl, timeOut );
		}
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x03;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x07, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x03;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x07, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcDelay;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x09, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcDelay;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x09, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).SyncSignalDelay;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x0e, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).SyncSignalDelay;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x0e, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcSample;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x14, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcSample;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x14, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetSensorConfig( deviceHdl, *sensorConfig, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetOperationMode( deviceHdl, *operationMode, timeOut );
	}

	//if( lResult == AS_NO_ERROR )
	//{
	//	// disable local VSR ADC PLL
	//	lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x07, 0x02, timeOut );
	//}

	//if( lResult == AS_NO_ERROR )
	//{
	//	// disable local VSR ADC PLL
	//	lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x07, 0x02, timeOut );
	//}

	//if( lResult == AS_NO_ERROR )
	//{
	//	lResult = EnableFunctionBlocks( deviceHdl, Control::AS_CONTROL_DISABLED, Control::AS_CONTROL_ENABLED, Control::AS_CONTROL_ENABLED, timeOut );
	//}

	if( lResult == AS_NO_ERROR )
	{
		lResult = EnableFunctionBlocks( deviceHdl, Control::AS_CONTROL_ENABLED, Control::AS_CONTROL_ENABLED, Control::AS_CONTROL_ENABLED, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = EnableSM( deviceHdl, timeOut );
	}

	//if( lResult == AS_NO_ERROR )
	//{
	//	lResult = EnableFunctionBlocks( deviceHdl, Control::AS_CONTROL_ENABLED, Control::AS_CONTROL_ENABLED, Control::AS_CONTROL_ENABLED, timeOut );
	//}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcClockPhase;
		lResult = WriteAdcRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x16, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = (*systemConfig).AdcClockPhase;
		lResult = WriteAdcRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x16, &lValue, timeOut );
	}

	//if( lResult == AS_NO_ERROR )
	//{
	//	 enable local VSR ADC PLL
	//	lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x07, 0x02, timeOut );
	//}

	//if( lResult == AS_NO_ERROR )
	//{
	//	 enable local VSR ADC PLL
	//	lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x07, 0x02, timeOut );
	//}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadResolution( deviceHdl, width, height, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		*frameTime = GetFrameTime( sensorConfig, *width, *height );
		*collectDcTime = GetCollectDcTime( *frameTime );
		deviceHdl->SetFrameTime(*frameTime);
		deviceHdl->SetFrameTimeOut( (u32)((*frameTime*1000*AS_HEXITEC_FRAME_TIMEOUT_MULTIPLIER)+1) );
	}

	if( (lResult == AS_NO_ERROR) && (enSyncMode) )
	{
		lResult = EnableSyncMode( deviceHdl, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	void	CopyBuffer(
										p_u8 sourceBuffer,
										p_u8 destBuffer,
										u32 byteCount )
{
	memcpy( destBuffer, sourceBuffer, byteCount );
}

EXTERN_C	VSR_API	i32		CreatePipeline(
										aS_Pleora_NTx* deviceHdl,
										u32 bufferCount,
										u32 transferBufferCount,
										u32 transferBufferFrameCount )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->SetTransferBuffer( transferBufferCount, transferBufferFrameCount );
		dResult = deviceHdl->CreatePipeline( bufferCount );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		DisableDetector(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;
	u8		lValue = 0x00;

	// set soft reset
	lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x00, &lValue, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = DisableModule( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, timeOut );
	}
	else
	{
		DisableModule( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		DisableModule(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";

	u32				lBytesWritten = 0;

	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = (i8)0xE2;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, NULL, 0, NULL, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		DisableSM(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x01, timeOut);
}

EXTERN_C	VSR_API	i32		DisableSyncMode(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x02, timeOut);
}

EXTERN_C	VSR_API	i32		DisableTriggerGate(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x04, timeOut);
}

EXTERN_C	VSR_API	i32		DisableTriggerMode(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x0a, 0x06, timeOut);

	if ( lResult == AS_NO_ERROR )
	{
		lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x0a, 0x06, timeOut);
	}

	if ( lResult == AS_NO_ERROR )
	{
		lResult = RegisterAcqArmedCallBack( deviceHdl, NULL );
	}
	
	if ( lResult == AS_NO_ERROR )
	{
		lResult = RegisterAcqFinishCallBack( deviceHdl, NULL );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableDetector(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	i32							lResult = AS_NO_ERROR;
	u8							lValue = 0;
	HexitecSensorSetupRegister	lSetupReg;

	lResult = EnableModule( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		Sleep( AS_VSR_POWER_ON_DELAY );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		// disable local VSR State Machine
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x01, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		// disable local VSR State Machine
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x01, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		// initialize controller trigger mode and disable State Machine
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x01;
		//  enable local VSR SyncMode
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x0A, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x01;
		//  enable local VSR SyncMode
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x0A, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = TrainTransmisionChannel( deviceHdl, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = GetSensorSetupReg(
			deviceHdl,
			AS_MODULE_ADDRESS_VSR1,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR1,
			&lSetupReg,
			timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetSensorSetupReg(
			deviceHdl,
			AS_MODULE_ADDRESS_VSR1,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR2,
			lSetupReg,
			timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetSensorSetupReg(
			deviceHdl,
			AS_MODULE_ADDRESS_VSR2,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR2,
			lSetupReg,
			timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x01;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x8F, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x01;
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x8F, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = EnableSM( deviceHdl, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableControllerFunctionBlocks(
										aS_Pleora_NTx* deviceHdl,
										Control	dacEnable,
										Control	peltierEnable,
										u32 timeOut )
{
	u8		lValue = 0;
	i32		lResult = AS_NO_ERROR;
	string8			lTxBuffer = "";
	string8			lRxBuffer = "";
	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;

	lValue = (dacEnable * 0x02);
	lValue = lValue + (peltierEnable * 0x04);

	lRxBuffer.resize(5, 0);
	lTxBuffer.resize(6, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = AS_MODULE_ADDRESS_CONTROLLER;
	lTxBuffer[2] = 0x55;
	HexToString( lValue, 2, (p_u8)&lTxBuffer[3] );
	lTxBuffer[5] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableFunctionBlocks(
										aS_Pleora_NTx* deviceHdl,
										Control	adcEnable,
										Control	dacEnable,
										Control	peltierEnable,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;

	lResult = EnableControllerFunctionBlocks( deviceHdl, dacEnable, peltierEnable, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = EnableModuleFunctionBlocks( deviceHdl, AS_MODULE_ADDRESS_VSR1, adcEnable, dacEnable, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = EnableModuleFunctionBlocks( deviceHdl, AS_MODULE_ADDRESS_VSR2, adcEnable, dacEnable, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableModuleFunctionBlocks(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										Control	adcEnable,
										Control	dacEnable,
										u32 timeOut )
{
	u8		lValue = 0;
	i32		lResult = AS_NO_ERROR;
	string8			lTxBuffer = "";
	string8			lRxBuffer = "";
	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;

	lValue = adcEnable;
	lValue = lValue + (dacEnable * 0x02);

	lRxBuffer.resize(5, 0);
	lTxBuffer.resize(6, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x55;
	HexToString( lValue, 2, (p_u8)&lTxBuffer[3] );
	lTxBuffer[5] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableModule(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";

	u32				lBytesWritten = 0;

	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = (i8)0xE3;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, NULL, 0, NULL, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		EnableSM(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x01, timeOut);
}

EXTERN_C	VSR_API	i32		EnableSyncMode(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x02, timeOut);
}

EXTERN_C	VSR_API	i32		EnableTriggerGate(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	return SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, 0x04, timeOut);
}

EXTERN_C	VSR_API	i32		EnableTriggerMode(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x0a, 0x06, timeOut);

	if ( lResult == AS_NO_ERROR )
	{
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x0a, 0x06, timeOut);
	}

	if ( lResult == AS_NO_ERROR )
	{
		lResult = RegisterAcqArmedCallBack( deviceHdl, EnableTriggerGateCB );
	}
	
	if ( lResult == AS_NO_ERROR )
	{
		lResult = RegisterAcqFinishCallBack( deviceHdl, DisableTriggerGateCB );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		ExitDevice(
										aS_Pleora_NTx* deviceHdl )
{				
	i32 dResult = AS_NO_ERROR;
	
	dResult = hTable.RemInstance(deviceHdl);
	
	if ( dResult == AS_NO_ERROR )
	{
		delete deviceHdl;
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		GetAsErrorMsg(
										i32 asError,
										str8 asErrorMsg,
										u32 length )
{
	hdl		ghResDll	= NULL;
	i32		charCnt		= 0;
	i32		lAsError	= asError;
	p_i32	pArgs[]		= { &lAsError };
	
	if (asErrorMsg != NULL)
	{
		ghResDll = LoadLibrary(AS_MESSAGE_DLL);
			
		if (ghResDll != NULL)
		{
			charCnt = FormatMessageA(FORMAT_MESSAGE_FROM_HMODULE, ghResDll, lAsError, 0, asErrorMsg, length, (va_list*)pArgs);
			FreeLibrary((HMODULE)ghResDll);
		}

		if ((ghResDll == NULL) || (charCnt == 0))
		{
			return GetLastError();
		}
	}

	return AS_NO_ERROR;
}

EXTERN_C	VSR_API	i32		GetBufferHandlingThreadPriority(
										aS_Pleora_NTx* deviceHdl,
										p_i32 priority )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		*priority = deviceHdl->GetBufferHandlingThreadPriority();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		GetDeviceInformation(
										aS_Pleora_NTx* deviceHdl,
										NTxDeviceInfoStrPtr deviceInfoStr )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		*deviceInfoStr = deviceHdl->GetDeviceInfoStr();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	dbl		GetFrameTime(
										const HexitecSensorConfigPtr sensorConfig,
										u32 width,
										u32 height )
{
	dbl clkPeriod = 1 / AS_HEXITEC_CLOCK_SPEED;
	u32 rowClks = ( ( (*sensorConfig).Row_S1.size2 + (*sensorConfig).S1_Sph + (*sensorConfig).Sph_S2 + (width / AS_HEXITEC_WIDTH_FACTOR) + (*sensorConfig).WaitClockCol ) * 2 ) + 10;
	u32 frameClks = ( (height / AS_HEXITEC_HEIGHT_FACTOR) * rowClks ) + 4 + ( (*sensorConfig).WaitClockRow * 2 );
	u32 frameClks3 = ( frameClks * 3 ) + 2;
	dbl FrameTime = ((dbl)frameClks3) * clkPeriod / 3;
	
	return FrameTime;
}

EXTERN_C	VSR_API	i32		GetIntegerValue(
										aS_Pleora_NTx* deviceHdl,
										const str8 propertyName,
										i64 &value )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->GetIntegerValue( propertyName, value );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		GetLastResult(
										aS_Pleora_NTx* deviceHdl,
										p_u32 pleoraErrorCode,
										str8 pleoraErrorCodeString,
										p_u32 pleoraErrorCodeStringLen,
										str8 pleoraErrorDescription,
										p_u32 pleoraErrorDescriptionLen )
{
	i32 dResult = AS_NO_ERROR;
	PvResult lResult = PvResult::Code::OK;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		lResult = deviceHdl->GetLastResult( );
		*pleoraErrorCode = lResult.GetCode();
		dResult = aS_Pleora_NTx::GetErrorDescription( lResult, pleoraErrorCodeString, pleoraErrorCodeStringLen, pleoraErrorDescription, pleoraErrorDescriptionLen );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		GetOperationMode(
										aS_Pleora_NTx* deviceHdl,
										HexitecOperationModePtr operationMode,
										u32 timeOut )
{
	u8		lValue = 0;
	i32		lResult = AS_NO_ERROR;

	lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x24, &lValue, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		(*operationMode).DcUploadDarkCorrectionValues = (Control)(lValue & 0x01);
		(*operationMode).DcCollectDarkCorrectionValues = (Control)((lValue & 0x02) >> 1);
		(*operationMode).DcEnableDarkCorrectionCountingMode = (Control)((lValue & 0x04) >> 2);
		(*operationMode).DcEnableDarkCorrectionSpectroscopicMode = (Control)((lValue & 0x08) >> 3);
		(*operationMode).DcSendDarkCorrectionValues = (Control)((lValue & 0x10) >> 4);
		(*operationMode).DcDisableVcalPulse = (Control)((lValue & 0x20) >> 5);
		(*operationMode).DcTestMode = (Control)((lValue & 0x40) >> 6);
		(*operationMode).DcEnableTriggeredCountingMode = (Control)((lValue & 0x80) >> 7);

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x27, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*operationMode).EdUploadThresholdValues = (Control)(lValue & 0x01);
		(*operationMode).EdDisableCountingMode = (Control)((lValue & 0x02) >> 1);
		(*operationMode).EdTestMode = (Control)((lValue & 0x04) >> 2);

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x28, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*operationMode).EdCycles.size1[0] = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x29, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*operationMode).EdCycles.size1[1] = lValue;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		GetSensorConfig(
										aS_Pleora_NTx* deviceHdl,
										HexitecSensorConfigPtr sensorConfig,
										u32 timeOut )
{
	u8		lValue = 0;
	i32		lResult = AS_NO_ERROR;
	u8		i = 0;

	lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x06, &lValue, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Gain = (HexitecGain)lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x02, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Row_S1.size1[0] = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x03, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Row_S1.size1[1] = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x04, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).S1_Sph = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x05, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Sph_S2 = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x18, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Vcal2_Vcal1.size1[0] = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x19, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).Vcal2_Vcal1.size1[1] = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x1a, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).WaitClockCol = lValue;

		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x1b, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		(*sensorConfig).WaitClockRow = lValue;
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = GetModulSetupReg( deviceHdl, AS_MODULE_ADDRESS_VSR1, &sensorConfig->SetupRegVsr1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = GetModulSetupReg( deviceHdl, AS_MODULE_ADDRESS_VSR2, &sensorConfig->SetupRegVsr2, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		GetSystemErrorMsg(
										i32 sysError,
										str8 sysErrorMsg,
										u32 length )
{
	i32		charCnt		= 0;
		
	if (sysErrorMsg != NULL)
	{
		charCnt = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, sysError, 0, sysErrorMsg, length, NULL);

		if( charCnt == 0 )
		{
			return GetLastError();
		}
	}

	return AS_NO_ERROR;
}

EXTERN_C	VSR_API	i32		GetTriggerState(
										aS_Pleora_NTx* deviceHdl,
										p_u8 trigger1,
										p_u8 trigger2,
										p_u8 trigger3,
										u32 timeOut )
{
	i32 lResult = AS_NO_ERROR;
	u8 lValue = 0;
	
	lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x85, &lValue, timeOut );
		
	if( lResult == AS_NO_ERROR )
	{
		*trigger1 = lValue & 0x01;
		*trigger2 = (lValue & 0x02) >> 1;
		*trigger3 = (lValue & 0x04) >> 2;
	}
	else
	{
		trigger1 = 0;
		trigger2 = 0;
		trigger3 = 0;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		InitDevice(
										aS_Pleora_NTx** deviceHdl,
										const str8 deviceDescriptor,
										p_u32 internalResult,
										str8 pleoraErrorCodeString,
										p_u32 pleoraErrorCodeStringLen,
										str8 pleoraErrorDescription,
										p_u32 pleoraErrorDescriptionLen )
{
	i32				iResult = AS_NO_ERROR;
	PvResult		lResult = PvResult::Code::OK;

	*deviceHdl = new aS_Pleora_NTx(deviceDescriptor, &iResult);

	lResult = (*deviceHdl)->GetLastResult();
	*internalResult = lResult.GetCode();
	aS_Pleora_NTx::GetErrorDescription( lResult, pleoraErrorCodeString, pleoraErrorCodeStringLen, pleoraErrorDescription, pleoraErrorDescriptionLen );

	if ( lResult.IsOK() && (iResult == AS_NO_ERROR) )
	{
		iResult = hTable.AddInstance(*deviceHdl);
		(*deviceHdl)->SetMinFrameTimeOut( AS_HEXITEC_MIN_FRAME_TIMEOUT );
	}
	else if ( iResult == AS_NO_ERROR )
	{
		iResult = AS_GIGE_INIT_DEVICE_ERROR;
	}

	if ( iResult != AS_NO_ERROR )
	{
		delete (*deviceHdl);
		*deviceHdl = NULL;
	}

	return iResult;
}

EXTERN_C	VSR_API	i32		InitFwDefaults(
										aS_Pleora_NTx* deviceHdl,
										u8 setHv,
										p_dbl hvSetPoint,
										u32 timeOut,
										p_u32 width,
										p_u32 height,
										HexitecSensorConfigPtr sensorConfig,
										HexitecOperationModePtr operationMode,
										p_dbl frameTime,
										p_u32 collectDcTime )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lHv = (u32)HvDacValFromHv( *hvSetPoint );
	
	lResult = DisableSyncMode( deviceHdl, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = DisableTriggerMode( deviceHdl, timeOut );
	}

	//HV ? Set Reset

	lRxBuffer.resize(7, 0);
	lTxBuffer.resize(4, 0);

	lTxBuffer[0] = 0x23;
	lTxBuffer[2] = 0x56;
	lTxBuffer[3] = 0x0d;

	if( lResult == AS_NO_ERROR )
	{
		lTxBuffer[1] = AS_MODULE_ADDRESS_VSR1;
		lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);
	}

	if( lResult == AS_NO_ERROR )
	{
		lTxBuffer[1] = AS_MODULE_ADDRESS_VSR2;
		lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);
	}

	//StringToHex( (p_u8)&lRxBuffer[2], 4, &lHv );

	//*hvSetPoint = HvDacValToHv( (u16)lHv );

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadResolution( deviceHdl, width, height, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = GetSensorConfig( deviceHdl, sensorConfig, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = GetOperationMode( deviceHdl, operationMode, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		*frameTime = GetFrameTime( sensorConfig, *width, *height );
		*collectDcTime = GetCollectDcTime( *frameTime );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		OpenSerialPort(
										aS_Pleora_NTx* deviceHdl,
										PvDeviceSerial serialPort,
										u32 rxBufferSize,
										u8 useTermChar,
										u8 termChar )
{				
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->OpenSerialPort( serialPort, rxBufferSize, useTermChar, termChar );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		OpenStream(
										aS_Pleora_NTx* deviceHdl )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->OpenStream( TRUE, TRUE );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		ReadControllerEnvironmentValues(
										aS_Pleora_NTx* deviceHdl,
										p_dbl peltierCurrent,
										p_dbl ntcTemperature,
										p_dbl daqPwrTemperature,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lNtcTemperature = 0;
	u32				lPeltierCurrent = 0;
	u32				lDaqPwrTemperature = 0;
	
	lRxBuffer.resize(15, 0);
	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = AS_MODULE_ADDRESS_CONTROLLER;
	lTxBuffer[2] = (i8)0xB7;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[2], 4, &lDaqPwrTemperature );
	StringToHex( (p_u8)&lRxBuffer[6], 4, &lPeltierCurrent );
	StringToHex( (p_u8)&lRxBuffer[10], 4, &lNtcTemperature );

	*ntcTemperature		= TemperatureDacValToTemperature( (u16)lNtcTemperature, AS_DAC_REF_VOLTAGE, AS_ADC_DAC_STEPS_10BIT );
	*peltierCurrent		= GetPeltierCurrent( GetVoltage( lPeltierCurrent, AS_DAC_REF_VOLTAGE ) );
	*daqPwrTemperature	= PwrTemperatureDacValToTemperature( (u16)lDaqPwrTemperature, AS_DAC_REF_VOLTAGE, AS_ADC_DAC_STEPS_10BIT ); 

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadControllerOperatingValues(
										aS_Pleora_NTx* deviceHdl,
										HexitecControllerOperatingValuesPtr opValuesController,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;
	dbl		lDaqPwrTemperature = 0;
	u8		lPowerState = 0;

	lResult = ReadControllerEnvironmentValues(
		deviceHdl,
		&(*opValuesController).peltierCurrent,
		&(*opValuesController).ntcTemperature,
		&lDaqPwrTemperature,
		timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadControllerPowerState( deviceHdl, &lPowerState, timeOut );
	}

	(*opValuesController).v1_8_pwrGood		= lPowerState & 0x01 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).v2_5_pwrGood		= lPowerState & 0x02 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).v3_3_pwrGood		= lPowerState & 0x04 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).v5_0_pwrGood		= lPowerState & 0x08 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).v5_5_pwrGood		= lPowerState & 0x10 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).v_spare_pwrGood	= lPowerState & 0x20 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).vPL_pwrGood		= lPowerState & 0x40 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;
	(*opValuesController).vPS_pwrGood		= lPowerState & 0x80 ? AS_CONTROL_ENABLED : AS_CONTROL_DISABLED;

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadControllerPowerState(
										aS_Pleora_NTx* deviceHdl,
										p_u8 powerState,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lPowerState = 0;
	
	lRxBuffer.resize(5, 0);
	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = AS_MODULE_ADDRESS_CONTROLLER;
	lTxBuffer[2] = 0x49;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	if( lResult == AS_NO_ERROR )
	{
		StringToHex( (p_u8)&lRxBuffer[2], 2, &lPowerState );
		*powerState = lPowerState;
	}
	else
	{
		*powerState = 0x00;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadEnvironmentValues(
										aS_Pleora_NTx* deviceHdl,
										HexitecVsrEnvironmentValuesPtr envValuesVsr1,
										HexitecVsrEnvironmentValuesPtr envValuesVsr2,
										p_dbl ntcTemperature,
										p_dbl daqPwrTemperature,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;
	dbl		lPeltierCurrent = 0;

	lResult = ReadControllerEnvironmentValues( deviceHdl, &lPeltierCurrent, ntcTemperature, daqPwrTemperature, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadModuleEnvironmentValues( deviceHdl, AS_MODULE_ADDRESS_VSR1, envValuesVsr1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadModuleEnvironmentValues( deviceHdl, AS_MODULE_ADDRESS_VSR2, envValuesVsr2, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadModuleEnvironmentValues(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrEnvironmentValuesPtr envValues,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lHumidity = 0;
	u32				lAmbientTemperature = 0;
	u32				lAsicTemperature1 = 0;
	u32				lAsicTemperature2 = 0;
	u32				lAdcTemperature = 0;
	
	lRxBuffer.resize(25, 0);
	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x52;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[2], 4, &lAmbientTemperature );
	StringToHex( (p_u8)&lRxBuffer[6], 4, &lHumidity );
	StringToHex( (p_u8)&lRxBuffer[10], 4, &lAsicTemperature1 );
	StringToHex( (p_u8)&lRxBuffer[14], 4, &lAsicTemperature2 );
	StringToHex( (p_u8)&lRxBuffer[18], 4, &lAdcTemperature );

	(*envValues).ambientTemperature	= GetAmbientTemperature( (u16)lAmbientTemperature );
	(*envValues).humidity			= GetHumidity( (u16)lHumidity );
	(*envValues).asicTemperature1	= GetAsicTemperature( (u16)lAsicTemperature1 );
	(*envValues).asicTemperature2	= GetAsicTemperature( (u16)lAsicTemperature2 );
	(*envValues).adcTemperature		= GetAdcTemperature( (u16)lAdcTemperature );

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadModuleOperatingValues(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrOperatingValuesPtr opValues,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				l3v3 = 0;
	u32				lHvMon = 0;
	u32				lHvOut = 0;
	u32				l1v2 = 0;
	u32				l1v8 = 0;
	u32				l2v5 = 0;
	u32				l3v3ln = 0;
	u32				l1v65ln = 0;
	u32				l1v8ana = 0;
	u32				l3v8ana = 0;
	
	lRxBuffer.resize(51, 0);
	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x50;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[38], 4, &l3v3 );
	StringToHex( (p_u8)&lRxBuffer[2], 4, &lHvMon );
	StringToHex( (p_u8)&lRxBuffer[6], 4, &l1v2 );
	StringToHex( (p_u8)&lRxBuffer[10], 4, &l1v8 );
	StringToHex( (p_u8)&lRxBuffer[18], 4, &l2v5 );
	StringToHex( (p_u8)&lRxBuffer[22], 4, &l3v3ln );
	StringToHex( (p_u8)&lRxBuffer[26], 4, &l1v65ln );
	StringToHex( (p_u8)&lRxBuffer[30], 4, &l1v8ana );
	StringToHex( (p_u8)&lRxBuffer[34], 4, &l3v8ana );

	(*opValues).v3_3 = GetInternalReference( (u16)l3v3 );
	(*opValues).hvMon = GetVoltage( lHvMon, (*opValues).v3_3 );
	(*opValues).hvOut = GetHvOut( (*opValues).hvMon );
	(*opValues).v1_2 = GetVoltage( l1v2, (*opValues).v3_3 );
	(*opValues).v1_8 = GetVoltage( l1v8, (*opValues).v3_3 );
	(*opValues).v2_5 = GetVoltage( l2v5, (*opValues).v3_3 );
	(*opValues).v3_3ln = GetVoltage( l3v3ln, (*opValues).v3_3 );
	(*opValues).v1_65ln = GetVoltage( l1v65ln, (*opValues).v3_3 );
	(*opValues).v1_8ana = GetVoltage( l1v8ana, (*opValues).v3_3 );
	(*opValues).v3_8ana = GetVoltage( l3v8ana, (*opValues).v3_3 );

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadOperatingValues(
										aS_Pleora_NTx* deviceHdl,
										HexitecControllerOperatingValuesPtr opValuesController,
										HexitecVsrOperatingValuesPtr opValuesVsr1,
										HexitecVsrOperatingValuesPtr opValuesVsr2,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;

	lResult = ReadControllerOperatingValues( deviceHdl, opValuesController, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadModuleOperatingValues( deviceHdl, AS_MODULE_ADDRESS_VSR1, opValuesVsr1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadModuleOperatingValues( deviceHdl, AS_MODULE_ADDRESS_VSR2, opValuesVsr2, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadRegister(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lValue = 0;

	lRxBuffer.resize(5, 0);
	lTxBuffer.resize(6, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x41;
	HexToString( registerAddress, 2, (p_u8)&lTxBuffer[3] );
	lTxBuffer[5] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[2], 2, &lValue );

	*value = (u8)lValue;

	return lResult;
}

EXTERN_C	VSR_API	i32		ReadResolution(
										aS_Pleora_NTx* deviceHdl,
										p_u32 width,
										p_u32 height,
										u32 timeOut )
{
	i32 lResult = AS_NO_ERROR;
	u8	lWidthVsr1 = 0;
	u8	lHeightVsr1 = 0;
	u8	lEqualRes1 = 0;
	u8	lWidthVsr2 = 0;
	u8	lHeightVsr2 = 0;
	u8	lEqualRes2 = 0;
	
	lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x83, &lWidthVsr1, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x83, &lWidthVsr2, timeOut );
	}
	
	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x84, &lHeightVsr1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x84, &lHeightVsr2, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x88, &lEqualRes1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x88, &lEqualRes2, timeOut );
	}

	if( (lResult == AS_NO_ERROR) && (lWidthVsr1 == lWidthVsr2) && (lHeightVsr1 == lHeightVsr2) && lEqualRes1 && lEqualRes2 )
	{
		*width = lWidthVsr1 * AS_HEXITEC_WIDTH_FACTOR;
		*height = lHeightVsr1 * AS_HEXITEC_HEIGHT_FACTOR;
	}
	else
	{
		*width = 0;
		*height = 0;
		lResult = AS_VSR_RESOLUTION_NOT_MATCH;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		RegisterAcqArmedCallBack(
										aS_Pleora_NTx* deviceHdl,
										p_readyCallBack acqArmedCallBack )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->RegisterAcqArmedCallBack( acqArmedCallBack );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		RegisterAcqFinishCallBack(
										aS_Pleora_NTx* deviceHdl,
										p_readyCallBack acqFinishCallBack )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->RegisterAcqFinishCallBack( acqFinishCallBack );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		RegisterTransferBufferReadyCallBack(
										aS_Pleora_NTx* deviceHdl,
										p_bufferCallBack transferBufferReadyCallBack )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->RegisterTransferBufferReadyCallBack( transferBufferReadyCallBack );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		ReturnBuffer(
										aS_Pleora_NTx* deviceHdl,
										p_u8 transferBuffer )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->ReturnBuffer( transferBuffer );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		SerialPortWriteRead(
										aS_Pleora_NTx* deviceHdl,
										const p_u8 txBuffer,
										u32 txBufferSize,
										p_u32 bytesWritten,
										p_u8 rxBuffer,
										u32 rxBufferSize,
										p_u32 bytesRead,
										u32 timeOut )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( (dResult == AS_NO_ERROR) && txBufferSize )
	{
		dResult = deviceHdl->FlushRxBuffer();

		if( dResult == AS_NO_ERROR )
		{
			dResult = deviceHdl->WriteSerialPort( txBuffer, txBufferSize, bytesWritten );
		}
	}

	if( (dResult == AS_NO_ERROR) && rxBufferSize )
	{
		dResult = deviceHdl->ReadSerialPort( rxBuffer, rxBufferSize, bytesRead, timeOut );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		SetControllerDAC(
										aS_Pleora_NTx* deviceHdl,
										p_dbl uSpare,
										p_dbl hvSetPoint,
										p_dbl targetTemperature,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lUspare = (u32)DacValFromVoltage( *uSpare );
	u32				lHvSetPoint = (u32)HvDacValFromHv( *hvSetPoint );
	
	lResult = CheckTemperatureLimit( deviceHdl, targetTemperature, timeOut );
	
	u32				lTargetTemperature = (u32)TemperatureDacValFromTemperature( *targetTemperature );
	
	if( lResult == AS_NO_ERROR )
	{
		lRxBuffer.resize(23, 0);
		lTxBuffer.resize(24, 0);
		lTxBuffer[0] = 0x23;
		lTxBuffer[1] = AS_MODULE_ADDRESS_CONTROLLER;
		lTxBuffer[2] = 0x54;
		HexToString( lUspare, 4, (p_u8)&lTxBuffer[3] );

		HexToString( lHvSetPoint, 4, (p_u8)&lTxBuffer[11] );

		HexToString( lTargetTemperature, 4, (p_u8)&lTxBuffer[19] );
		lTxBuffer[23] = 0x0d;

		lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

		StringToHex( (p_u8)&lRxBuffer[2], 4, &lUspare );
		StringToHex( (p_u8)&lRxBuffer[10], 4, &lHvSetPoint );
		StringToHex( (p_u8)&lRxBuffer[18], 4, &lTargetTemperature );

		*uSpare = DacValToVoltage( (u16)lUspare );
		*hvSetPoint = HvDacValToHv( (u16)lHvSetPoint );
		*targetTemperature = TemperatureDacValToTemperature( (u16)lTargetTemperature, AS_DAC_REF_VOLTAGE, AS_ADC_DAC_STEPS_12BIT );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		SetDAC(
										aS_Pleora_NTx* deviceHdl,
										p_dbl vCal,
										p_dbl uMid,
										p_dbl hvSetPoint,
										p_dbl detCtrl,
										p_dbl targetTemperature,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;
	dbl		lUspare = 0;

	lResult = SetControllerDAC( deviceHdl, &lUspare, hvSetPoint, targetTemperature, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetModuleDAC( deviceHdl, AS_MODULE_ADDRESS_VSR1, vCal, uMid, detCtrl, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetModuleDAC( deviceHdl, AS_MODULE_ADDRESS_VSR2, vCal, uMid, detCtrl, timeOut );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		SetFrameFormatControl(
										aS_Pleora_NTx* deviceHdl,
										const str8 pixelFormat,
										u64 width,
										u64 height,
										u64 offsetX,
										u64 offsetY,
										const str8 sensorTaps,
										const str8 testPattern )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		dResult = deviceHdl->SetImageFormatControl( pixelFormat, width, height, offsetX, offsetY, sensorTaps, testPattern );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		SetFrameTimeOut(
										aS_Pleora_NTx* deviceHdl,
										u32 frameTimeOut )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->SetFrameTimeOut( frameTimeOut );
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		SetModuleDAC(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										p_dbl vCal,
										p_dbl uMid,
										p_dbl detCtrl,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lVcal = (u32)DacValFromVoltage( *vCal );
	u32				lUmid = (u32)DacValFromVoltage( *uMid );
	u32				lDetCtrl = (u32)DacValFromVoltage( *detCtrl );
	
	if( lResult == AS_NO_ERROR )
	{
		lRxBuffer.resize(23, 0);
		lTxBuffer.resize(24, 0);
		lTxBuffer[0] = 0x23;
		lTxBuffer[1] = moduleAddress;
		lTxBuffer[2] = 0x54;
		HexToString( lVcal, 4, (p_u8)&lTxBuffer[3] );
		HexToString( lUmid, 4, (p_u8)&lTxBuffer[7] );
		lTxBuffer[11] = 0x30;
		lTxBuffer[12] = 0x30;
		lTxBuffer[13] = 0x30;
		lTxBuffer[14] = 0x30;
		HexToString( lDetCtrl, 4, (p_u8)&lTxBuffer[15] );
		lTxBuffer[19] = 0x30;
		lTxBuffer[20] = 0x30;
		lTxBuffer[21] = 0x30;
		lTxBuffer[22] = 0x30;
		lTxBuffer[23] = 0x0d;

		lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

		StringToHex( (p_u8)&lRxBuffer[2], 4, &lVcal );
		StringToHex( (p_u8)&lRxBuffer[6], 4, &lUmid );
		StringToHex( (p_u8)&lRxBuffer[14], 4, &lDetCtrl );

		*vCal = DacValToVoltage( (u16)lVcal );
		*uMid = DacValToVoltage( (u16)lUmid );
		*detCtrl = DacValToVoltage( (u16)lDetCtrl );
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		SetRegisterBit(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										u8 bitMask,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	
	lRxBuffer.resize(7, 0);
	lTxBuffer.resize(8, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x42;
	HexToString( registerAddress, 2, (p_u8)&lTxBuffer[3] );
	HexToString( bitMask, 2, (p_u8)&lTxBuffer[5] );
	lTxBuffer[7] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	return lResult;
}

EXTERN_C	VSR_API	i32		SetTriggeredFrameCount(
										aS_Pleora_NTx* deviceHdl,
										u32 frameCount,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	
	lRxBuffer.resize(19, 0);
	lTxBuffer.resize(14, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = AS_MODULE_ADDRESS_VSR1;
	lTxBuffer[2] = 0x44;
	lTxBuffer[3] = 0x32;
	lTxBuffer[4] = 0x41;
	HexToStringLE( frameCount, 8, (p_u8)&lTxBuffer[5] );
	lTxBuffer[13] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	if ( lResult == AS_NO_ERROR )
	{
		lTxBuffer[1] = AS_MODULE_ADDRESS_VSR2;
		lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		StopAcquisition(
										aS_Pleora_NTx* deviceHdl )
{
	i32 dResult = AS_NO_ERROR;

	dResult = hTable.SetInstanceBusy(deviceHdl);
	
	if( dResult == AS_NO_ERROR )
	{
		deviceHdl->StopAcquisition();
	}

	if ( dResult == AS_NO_ERROR )
	{
		dResult = hTable.SetInstanceReady(deviceHdl);
	}
	else
	{
		hTable.SetInstanceReady(deviceHdl);
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		TrainTransmisionChannel(
										aS_Pleora_NTx* deviceHdl,
										u32 timeOut )
{
	i32							lResult = AS_NO_ERROR;
	u8							lValue = 0;
	u8							lVsrTrainingState1 = 0;
	u8							lVsrTrainingState2 = 0;

	lValue = 0x00;
	// soft reset controller
	lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x00, &lValue, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		// release soft reset
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x00, 0x01, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// set local VSR clock mux and select clock output
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x01, 0x18, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// set local VSR clock mux and select clock output
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x01, 0x10, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		//  reset local VSR PLL
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x07, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		//  reset local VSR PLL
		lResult = WriteRegister( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x07, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR PLL
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x07, 0x03, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR PLL
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x07, 0x03, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR LVDS Interface
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x01, 0x20, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR LVDS Interface
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x01, 0x20, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR Ch. training
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x01, 0x80, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable local VSR Ch. training
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x01, 0x80, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// enable controller Ch. training
		lResult = SetRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x00, 0x80, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// read back training state VSR1
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0xFC, &lVsrTrainingState1, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// read back training state VSR2
		lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0xFD, &lVsrTrainingState2, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// disable local VSR Ch. training
		lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR1, 0x01, 0x80, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// disable local VSR Ch. training
		lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_VSR2, 0x01, 0x80, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		// disable controller Ch. training
		lResult = ClearRegisterBit( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x00, 0x80, timeOut );
	}

	if( (lResult == AS_NO_ERROR) && ((lVsrTrainingState1 != 0xff) || (lVsrTrainingState2 != 0xff)) )
	{
		lResult = AS_VSR_CH_TRAINING_NOT_COMPLETED;
	}

	return lResult;
}

EXTERN_C	VSR_API	i32		UploadModuleOffsetValues(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										Reg2BytePtr offsetValues,
										u32 offsetValuesLength,
										u32 timeOut )
{
	i32 dResult = AS_NO_ERROR;
	u32 lValuesToTransmit = offsetValuesLength * 2;
	FpgaRegister_vec lFpgaRegisterStream;
	u32 i = 0;
	u32 lRegsWritten = 0;

	while( (dResult == AS_NO_ERROR) && lValuesToTransmit )
	{
		if( lValuesToTransmit < AS_HEXITEC_MAX_STREAM_REGISTER_COUNT )
		{
			lFpgaRegisterStream.resize( lValuesToTransmit );
		}
		else
		{
			lFpgaRegisterStream.resize( AS_HEXITEC_MAX_STREAM_REGISTER_COUNT );
		}

		for( i=0 ; i<lFpgaRegisterStream.size() ; i++ )
		{
			lFpgaRegisterStream[i].address = (u8)(lRegsWritten%2) + 0x25;
			lFpgaRegisterStream[i].value = offsetValues[lRegsWritten/2].size1[lRegsWritten%2];
			lRegsWritten++;
		}
			
		dResult = WriteRegisterStream( deviceHdl, moduleAddress, lFpgaRegisterStream, timeOut );
		lValuesToTransmit = lValuesToTransmit - (u32)lFpgaRegisterStream.size();
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		UploadOffsetValues(
										aS_Pleora_NTx* deviceHdl,
										Reg2BytePtr offsetValues,
										u32 offsetValuesLength,
										u32 timeOut )
{
	i32 dResult = AS_NO_ERROR;
	u32 lLength = offsetValuesLength / 2;
	HexitecOperationMode lCurrentMode;
	HexitecOperationMode lUploadMode;
	Control lSyncModeState = AS_CONTROL_DISABLED;

	if( !offsetValues )
	{
		dResult = AS_NULL_POINTER;
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = GetOperationMode( deviceHdl, &lCurrentMode, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		lUploadMode = lCurrentMode;
		lUploadMode.DcCollectDarkCorrectionValues = Control::AS_CONTROL_DISABLED;
		lUploadMode.DcUploadDarkCorrectionValues = Control::AS_CONTROL_ENABLED;
		
		dResult = GetSyncModeState( deviceHdl, &lSyncModeState, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = DisableSM( deviceHdl, timeOut );
	}

	if( (dResult == AS_NO_ERROR) && (lSyncModeState == AS_CONTROL_ENABLED) )
	{
		dResult = DisableSyncMode( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = SetOperationMode( deviceHdl, lUploadMode, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = EnableSM( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = UploadModuleOffsetValues( deviceHdl, AS_MODULE_ADDRESS_VSR1, offsetValues, lLength, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = UploadModuleOffsetValues( deviceHdl, AS_MODULE_ADDRESS_VSR2, &offsetValues[lLength], lLength, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = DisableSM( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{		
		dResult = SetOperationMode( deviceHdl, lCurrentMode, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = TrainTransmisionChannel( deviceHdl, timeOut );
	}

	if( (dResult == AS_NO_ERROR) && (lSyncModeState == AS_CONTROL_ENABLED) )
	{
		dResult = EnableSyncMode( deviceHdl, timeOut );
	}

	if( dResult == AS_NO_ERROR )
	{
		dResult = EnableSM( deviceHdl, timeOut );
	}

	return dResult;
}

EXTERN_C	VSR_API	i32		WriteAdcRegister(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lValue = 0;

	lRxBuffer.resize(7, 0);
	lTxBuffer.resize(8, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x53;
	HexToString( registerAddress, 2, (p_u8)&lTxBuffer[3] );
	HexToString( *value, 2, (p_u8)&lTxBuffer[5] );
	lTxBuffer[7] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[4], 2, &lValue );

	*value = (u8)lValue;

	return lResult;
}

EXTERN_C	VSR_API	i32		WriteRegister(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 registerAddress,
										p_u8 value,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				lValue = 0;

	lRxBuffer.resize(7, 0);
	lTxBuffer.resize(8, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x40;
	HexToString( registerAddress, 2, (p_u8)&lTxBuffer[3] );
	HexToString( *value, 2, (p_u8)&lTxBuffer[5] );
	lTxBuffer[7] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[4], 2, &lValue );

	*value = (u8)lValue;

	return lResult;
}

EXTERN_C	VSR_API	i32		WriteRegisterStream(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										FpgaRegister_vec &registerStream,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32				lBytesWritten = 0;
	u32				lBytesRead = 0;
	u32				i = 0;
	u32				lBufferIndex = 5;
	u32				lValue = 0;
	
	lRxBuffer.resize( ( registerStream.size() * 4 ) + 3, 0 );
	lTxBuffer.resize( ( registerStream.size() * 4 ) + 6, 0 );
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = moduleAddress;
	lTxBuffer[2] = 0x46;
	HexToString( (u8)registerStream.size(), 2, (p_u8)&lTxBuffer[3] );

	for( i=0 ; i<registerStream.size() ; i++ )
	{
		HexToString( registerStream[i].address, 2, (p_u8)&lTxBuffer[ lBufferIndex ] );
		lBufferIndex = lBufferIndex + 2;
		HexToString( registerStream[i].value, 2, (p_u8)&lTxBuffer[ lBufferIndex ] );
		lBufferIndex = lBufferIndex + 2;
	}
	
	lTxBuffer[ lTxBuffer.size() - 1 ] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	if( lBytesRead >= lRxBuffer.size() )
	{
		lBufferIndex = 4;
		
		for( i=0 ; i<registerStream.size() ; i++ )
		{
			StringToHex( (p_u8)&lRxBuffer[lBufferIndex], 2, &lValue );
			registerStream[i].value = (u8)lValue;
			lBufferIndex = lBufferIndex + 4;
		}		
	}

	return lResult;
}

u16 DacValFromVoltage( dbl voltage )
{
	dbl lVoltage = voltage;
	dbl dacStepVoltage = AS_DAC_REF_VOLTAGE / 4095;

	if( lVoltage < 0 )
	{
		lVoltage = 0;
	}
	else if( lVoltage > AS_DAC_REF_VOLTAGE )
	{
		lVoltage = AS_DAC_REF_VOLTAGE;
	}
	
	return (u16)((lVoltage/dacStepVoltage)+0.5);
}

dbl DacValToVoltage( u16 number )
{
	dbl dacStepVoltage = AS_DAC_REF_VOLTAGE / 4095;
	return dacStepVoltage * (dbl)number;
}

i32 HexToString( u32 number, u8 digits, p_u8 target )
{
	i32	lResult = 0;
	string8	lBuffer;
	lBuffer.resize( digits + 1, 0 );
	lResult = sprintf_s( &lBuffer[0], digits + 1, "%0*X", digits, number );
	memcpy( target, &lBuffer[0], digits );
	return lResult;
}

i32 HexToStringLE( u32 number, u8 digits, p_u8 target )
{
	u8 lIdx = 0;
	i32	lResult = 0;
	string8	lBuffer;
	lBuffer.resize( digits + 1, 0 );
	lResult = sprintf_s( &lBuffer[0], digits + 1, "%0*X", digits, number );
	for ( lIdx = 0 ; lIdx < digits ; lIdx = lIdx + 2 )
	{
		target[lIdx] = lBuffer[digits - lIdx - 2];
		target[lIdx + 1] = lBuffer[digits - lIdx - 1];
	}
	return lResult;
}

i32 StringToHex( p_u8 source, u8 digits, p_u32 number )
{
	u32 lMaxDigits = 3;
	i32 lResult = 0;
	string8 lFormat;
	lFormat.resize( lMaxDigits + 2 );
	lFormat[0] = '%';
	lResult = sprintf_s( &lFormat[1], lMaxDigits, "%u", digits );
	if( lResult > 0 )
	{
		lFormat.resize( lResult + 2 );
		lFormat[ lResult + 1 ] = 'x';
	}
	else
	{
		return lResult;
	}
	return sscanf_s( (const char*)source, lFormat.c_str() , number );
}

i32 CheckTemperatureLimit(
										aS_Pleora_NTx* deviceHdl,
										p_dbl temperature,
										u32 timeOut )

{
	i32	lResult = AS_NO_ERROR;

	string8			lTxBuffer = "";
	string8			lRxBuffer = "";

	u32			lBytesWritten = 0;
	u32			lBytesRead = 0;
	u32			lDetectorType = 0xff;

	lRxBuffer.resize(83, 0);
	lTxBuffer.resize(4, 0);
	lTxBuffer[0] = 0x23;
	lTxBuffer[1] = AS_MODULE_ADDRESS_CONTROLLER;
	lTxBuffer[2] = 0x70;
	lTxBuffer[3] = 0x0d;

	lResult = SerialPortWriteRead( deviceHdl, (p_u8)&lTxBuffer[0], (u32)lTxBuffer.size(), &lBytesWritten, (p_u8)&lRxBuffer[0], (u32)lRxBuffer.size(), &lBytesRead, timeOut);	

	StringToHex( (p_u8)&lRxBuffer[2], 2, &lDetectorType );

	switch( lDetectorType )
	{
		case 0x00:
			if( *temperature < AS_HEXITEC_TARGET_TEMPERATURE_LL_0x00 )
			{
				*temperature = AS_HEXITEC_TARGET_TEMPERATURE_LL_0x00;
			}
			break;
		default:
			if( *temperature < AS_HEXITEC_TARGET_TEMPERATURE_LL )
			{
				*temperature = AS_HEXITEC_TARGET_TEMPERATURE_LL;
			}
			break;
	}

	if( *temperature > AS_HEXITEC_TARGET_TEMPERATURE_UL )
	{
		*temperature = AS_HEXITEC_TARGET_TEMPERATURE_UL;
	}

	return lResult;
}

void DisableTriggerGateCB( hdl deviceHdl)				// App Specific
{
	aS_Pleora_NTx* deviceHdlInt = (aS_Pleora_NTx*) deviceHdl;

	DisableTriggerGate( deviceHdlInt , AS_HEXITEC_SERIAL_TIMEOUT );
}

void EnableTriggerGateCB( hdl deviceHdl)				// App Specific
{
	aS_Pleora_NTx* deviceHdlInt = (aS_Pleora_NTx*) deviceHdl;

	EnableTriggerGate( deviceHdlInt , AS_HEXITEC_SERIAL_TIMEOUT );
}

dbl GetAdcTemperature( u16 number )
{
	return (dbl)number * 0.0625;
}

dbl GetAmbientTemperature( u16 number )
{
	return (((dbl)number * 175.72) / 65535) - 46.85;
}

dbl GetAsicTemperature( u16 number )
{
	return (dbl)number * 0.0625;
}

u32	GetCollectDcTime( dbl aFrameTime )
{
	return (u32)((aFrameTime * 1000 * AS_HEXITEC_DARK_CORRECTION_FRAME_COUNT) + 1000);
}

dbl GetHumidity( u16 number )
{
	return (((dbl)number * 125) / 65535) - 6;
}

dbl GetHvOut( dbl hvMon )
{
	return (hvMon * 1879.21) - 1145;//1119.02;
	//(hvMon * 1621.65) - 1043.22;
}

dbl GetInternalReference( u16 number )
{
	return (AS_INTERNAL_REF_VOLTAGE / AS_ADC_DAC_STEPS_12BIT) * (dbl)number;
}

dbl GetPeltierCurrent( dbl voltage )
{
	return (voltage - 1.5) / 0.2;
}

i32 GetModulSetupReg(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrSetupRegisterPtr setupReg,
										u32 timeOut)
{
	i32		lResult = AS_NO_ERROR;

	lResult = GetSensorSetupReg( 
		deviceHdl,
		moduleAddress,
		AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR1,
		&setupReg->Sensor1,
		timeOut);

	if ( lResult == AS_NO_ERROR )
	{
		lResult = GetSensorSetupReg(
			deviceHdl,
			moduleAddress,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR2,
			&setupReg->Sensor2,
			timeOut);
	}

	return lResult;
}

i32 GetSensorSetupReg(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										HexitecSensorSetupRegisterPtr setupReg,
										u32 timeOut)
{
	i32		lResult = AS_NO_ERROR;
	u8		lRegOffset = regOffset;

	lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Row.PowerEn, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Row.CalEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Row.ReadEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Col.PowerEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Col.CalEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = GetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg->Col.ReadEn, timeOut );
	}

	return lResult;
}

i32 GetSetupReg(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										p_u8 setupReg,
										u32 timeOut)
{
	u8		i = 0;
	i32		lResult = AS_NO_ERROR;
	
	for( i=0 ; i<AS_HEXITEC_SETUP_REGISTER_SIZE ; i++)
	{
		if( lResult == AS_NO_ERROR )
		{
			lResult = ReadRegister( deviceHdl, moduleAddress, regOffset + i, &setupReg[i], timeOut );
		}
	}

	return lResult;
}

i32	GetSyncModeState(
										aS_Pleora_NTx* deviceHdl,
										ControlPtr syncModeState,
										u32 timeOut )
{
	i32	lResult = AS_NO_ERROR;
	u8	lValue = 0;

	lResult = ReadRegister( deviceHdl, AS_MODULE_ADDRESS_CONTROLLER, 0x01, &lValue, timeOut);

	if ( (lResult == AS_NO_ERROR) && (lValue & 0x02) )
	{
		*syncModeState = AS_CONTROL_ENABLED;
	}
	else
	{
		*syncModeState = AS_CONTROL_DISABLED;
	}

	return lResult;
}

dbl GetVoltage( u16 number, dbl internalReference )
{
	return (internalReference / AS_ADC_DAC_STEPS_12BIT) * (dbl)number;
}

u16 HvDacValFromHv( dbl hv )
{
	dbl hvConverted = hv/-250;
	dbl dacStepVoltage = AS_DAC_REF_VOLTAGE / 4095;

	if( hvConverted < 0 )
	{
		hvConverted = 0;
	}
	else if( hvConverted > AS_DAC_REF_VOLTAGE )
	{
		hvConverted = AS_DAC_REF_VOLTAGE;
	}
	
	return (u16)((hvConverted/dacStepVoltage)+0.5);
}

dbl HvDacValToHv( u16 number )
{
	dbl dacStepVoltage = AS_DAC_REF_VOLTAGE / 4095;
	return dacStepVoltage * (dbl)number * -250;
}

i32	SetModuleOperationMode(
										aS_Pleora_NTx* aDeviceHdl,
										u8 moduleAddress,
										HexitecOperationMode aOperationMode,
										u32 aTimeOut)
{
	u8		lValue = 0;
	i32		lResult = AS_NO_ERROR;

	lValue = aOperationMode.DcUploadDarkCorrectionValues;
	lValue = lValue + (aOperationMode.DcCollectDarkCorrectionValues * 0x02);
	lValue = lValue + (aOperationMode.DcEnableDarkCorrectionCountingMode * 0x04);
	lValue = lValue + (aOperationMode.DcEnableDarkCorrectionSpectroscopicMode * 0x08);
	lValue = lValue + (aOperationMode.DcSendDarkCorrectionValues * 0x10);
	lValue = lValue + (aOperationMode.DcDisableVcalPulse * 0x20);
	lValue = lValue + (aOperationMode.DcTestMode * 0x40);
	lValue = lValue + (aOperationMode.DcEnableTriggeredCountingMode * 0x80);

	lResult = WriteRegister( aDeviceHdl, moduleAddress, 0x24, &lValue, aTimeOut );

	if( lResult == AS_NO_ERROR )
	{
		lValue = aOperationMode.EdUploadThresholdValues;
		lValue = lValue + (aOperationMode.EdDisableCountingMode * 0x02);
		lValue = lValue + (aOperationMode.EdTestMode * 0x04);

		lResult = WriteRegister( aDeviceHdl, moduleAddress, 0x27, &lValue, aTimeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = aOperationMode.EdCycles.size1[0];

		lResult = WriteRegister( aDeviceHdl, moduleAddress, 0x28, &lValue, aTimeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = aOperationMode.EdCycles.size1[1];

		lResult = WriteRegister( aDeviceHdl, moduleAddress, 0x29, &lValue, aTimeOut );
	}

	return lResult;
}

i32 SetModuleSensorConfig(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										HexitecVsrSensorConfig sensorConfig,
										u32 timeOut )
{
	i32		lResult = AS_NO_ERROR;
	u8		lValue = 0;
	
	lValue = sensorConfig.Gain;
	lResult = WriteRegister( deviceHdl, moduleAddress, 0x06, &lValue, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.Row_S1.size1[0];
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x02, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.Row_S1.size1[1];
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x03, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.S1_Sph;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x04, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.Sph_S2;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x05, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.Vcal2_Vcal1.size1[0];
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x18, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.Vcal2_Vcal1.size1[1];
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x19, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x20, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x21, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x22, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = 0x00;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x23, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.WaitClockCol;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x1a, &lValue, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lValue = sensorConfig.WaitClockRow;
		lResult = WriteRegister( deviceHdl, moduleAddress, 0x1b, &lValue, timeOut );
	}

	if ( lResult == AS_NO_ERROR )
	{
		lResult = SetSensorSetupReg( 
			deviceHdl,
			moduleAddress,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR1,
			sensorConfig.SetupReg.Sensor1,
			timeOut);
	}

	if ( lResult == AS_NO_ERROR )
	{
		lResult = SetSensorSetupReg(
			deviceHdl,
			moduleAddress,
			AS_HEXITEC_SETUP_REGISTER_START_ADDRESS_SENSOR2,
			sensorConfig.SetupReg.Sensor2,
			timeOut);
	}

	return lResult;
}

i32	SetOperationMode(
										aS_Pleora_NTx* deviceHdl,
										HexitecOperationMode operationMode,
										u32 timeOut)
{
	i32		lResult = AS_NO_ERROR;

	lResult = SetModuleOperationMode( deviceHdl, AS_MODULE_ADDRESS_VSR1, operationMode, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lResult = SetModuleOperationMode( deviceHdl, AS_MODULE_ADDRESS_VSR2, operationMode, timeOut );
	}

	return lResult;
}

i32	SetSensorConfig(
										aS_Pleora_NTx* deviceHdl,
										HexitecSensorConfig sensorConfig,
										u32 timeOut)
{
	i32						lResult = AS_NO_ERROR;
	HexitecVsrSensorConfig	lVsrConfig;

	lVsrConfig.Gain			= sensorConfig.Gain;
	lVsrConfig.Row_S1		= sensorConfig.Row_S1;
	lVsrConfig.S1_Sph		= sensorConfig.S1_Sph;
	lVsrConfig.Sph_S2		= sensorConfig.Sph_S2;
	lVsrConfig.Vcal2_Vcal1	= sensorConfig.Vcal2_Vcal1;
	lVsrConfig.WaitClockCol	= sensorConfig.WaitClockCol;
	lVsrConfig.WaitClockRow	= sensorConfig.WaitClockRow;
	lVsrConfig.SetupReg		= sensorConfig.SetupRegVsr1;

	lResult = SetModuleSensorConfig( deviceHdl, AS_MODULE_ADDRESS_VSR1, lVsrConfig, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lVsrConfig.SetupReg		= sensorConfig.SetupRegVsr2;
		lResult = SetModuleSensorConfig( deviceHdl, AS_MODULE_ADDRESS_VSR2, lVsrConfig, timeOut );
	}

	return lResult;
}

i32 SetSensorSetupReg(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										HexitecSensorSetupRegister setupReg,
										u32 timeOut)
{
	i32		lResult = AS_NO_ERROR;
	u8		lRegOffset = regOffset;

	lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Row.PowerEn, timeOut );

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Row.CalEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Row.ReadEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Col.PowerEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Col.CalEn, timeOut );
	}

	if( lResult == AS_NO_ERROR )
	{
		lRegOffset = lRegOffset + AS_HEXITEC_SETUP_REGISTER_SIZE;
		lResult = SetSetupReg( deviceHdl, moduleAddress, lRegOffset, setupReg.Col.ReadEn, timeOut );
	}

	return lResult;
}

i32 SetSetupReg(
										aS_Pleora_NTx* deviceHdl,
										u8 moduleAddress,
										u8 regOffset,
										p_u8 setupReg,
										u32 timeOut)
{
	u8		i = 0;
	i32		lResult = AS_NO_ERROR;
	
	for( i=0 ; i<AS_HEXITEC_SETUP_REGISTER_SIZE ; i++)
	{
		if( lResult == AS_NO_ERROR )
		{
			lResult = WriteRegister( deviceHdl, moduleAddress, regOffset + i, &setupReg[i], timeOut );
		}
	}

	return lResult;
}

u16 TemperatureDacValFromTemperature( dbl temperature )
{
	dbl temperatureConvertedExp = exp(((1 / ((dbl)temperature + 273.15)) - (1 / 298.15)) * AS_NTC_MATERIAL_CONST_B);
	dbl temperatureConverted = (temperatureConvertedExp * AS_DAC_REF_VOLTAGE) / (temperatureConvertedExp + 1);
	dbl dacStepVoltage = AS_DAC_REF_VOLTAGE / 4095;

	if( temperatureConverted < 0 )
	{
		temperatureConverted = 0;
	}
	else if( temperatureConverted > AS_DAC_REF_VOLTAGE )
	{
		temperatureConverted = AS_DAC_REF_VOLTAGE;
	}
	
	return (u16)((temperatureConverted/dacStepVoltage)+0.5);
}

dbl TemperatureDacValToTemperature( u16 number, dbl internalReference, AdcDacSteps steps )
{
	dbl dacVoltage = (internalReference / steps) * (dbl)number;
	dbl term_ln = log(dacVoltage / (AS_DAC_REF_VOLTAGE-dacVoltage));
	return (1/((term_ln / AS_NTC_MATERIAL_CONST_B) + (1 / 298.15))) - 273.15;
}

dbl PwrTemperatureDacValToTemperature( u16 number, dbl internalReference, AdcDacSteps steps )
{
	dbl dacVoltage = (internalReference / steps) * (dbl)number;
	return ( dacVoltage - 0,548976898 ) / -0,0023;
}
