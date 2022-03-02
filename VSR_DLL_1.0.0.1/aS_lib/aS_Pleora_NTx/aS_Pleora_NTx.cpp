#include "stdafx.h"
#include "aS_Pleora_NTx.h"

aS_Pleora_NTx::aS_Pleora_NTx(const str8 aDeviceDescriptor, p_i32 aResult)
{
	cDeviceInfo					= NULL;
	cDeviceInfoGEV				= NULL;
	cDeviceInfoU3V				= NULL;
	cDevice						= NULL;
	cDeviceParams				= NULL;
	cStreamParams				= NULL;
	cDeviceAdapter				= NULL;
	cStream						= NULL;
	cPipeline					= NULL;
	cStartCmd					= NULL;
	cStopCmd					= NULL;
	cResetCmd					= NULL;
	cTransferBufferFrameCount	= 0;
	cTransferBuffer.clear();
	cUseTermChar				= 0;
	cTermChar					= 0;
	cReadyCallBack				= NULL;
	cFinishCallBack				= NULL;
	cBufferCallBack				= NULL;
	cAcquiredImages				= 0;
	cContinuous					= 0;
	cStopAcquisition			= 0;
	cBlocksDroppedVal			= 0;
	cBlocksDropped				= NULL;
	cBlockIDsMissingVal			= 0;
	cBlockIDsMissing			= NULL;
	cFrameTime					= 0;
	cFrameTimeOut				= 0;
	cMinFrameTimeOut			= 0;

	cResult						= PvResult::Code::OK;
	cAcqResult					= PvResult::Code::OK;

	*aResult	= AS_NO_ERROR;

	InitializeCriticalSectionAndSpinCount( &cAvailableTransferBufferLock, 0x0400 );
	
	ClearQueue();

	cResult = Connect( aDeviceDescriptor );

	if( cResult.IsOK() )
	{
		cDeviceAdapter	= new PvDeviceAdapter( cDevice );
		cDeviceParams	= cDevice->GetParameters();
		cStartCmd		= dynamic_cast<PvGenCommand *>( cDeviceParams->Get( "AcquisitionStart" ) );
		cStopCmd		= dynamic_cast<PvGenCommand *>( cDeviceParams->Get( "AcquisitionStop" ) );
	}

	if( cResult.IsOK() )
	{
		cDeviceInformation.Vendor				= cDeviceInfo->GetVendorName();
		cDeviceInformationStr.Vendor			= (str8)cDeviceInformation.Vendor.GetAscii();

		cDeviceInformation.Model				= cDeviceInfo->GetModelName();
		cDeviceInformationStr.Model				= (str8)cDeviceInformation.Model.GetAscii();

		cDeviceInformation.ManufacturerInfo		= cDeviceInfo->GetManufacturerInfo();
		cDeviceInformationStr.ManufacturerInfo	= (str8)cDeviceInformation.ManufacturerInfo.GetAscii();

		cDeviceInformation.SerialNumber			= cDeviceInfo->GetSerialNumber();
		cDeviceInformationStr.SerialNumber		= (str8)cDeviceInformation.SerialNumber.GetAscii();

		cDeviceInformation.UserId				= cDeviceInfo->GetUserDefinedName();
		cDeviceInformationStr.UserId			= (str8)cDeviceInformation.UserId.GetAscii();

		cDeviceInfoType							= cDeviceInfo->GetType();
		
		switch( cDeviceInfoType )
		{
			case PvDeviceInfoType::PvDeviceInfoTypeGEV:
				cDeviceInfoGEV = (PvDeviceInfoGEV *)(cDeviceInfo);

				cDeviceInformation.MacAddress		= cDeviceInfoGEV->GetMACAddress();
				cDeviceInformation.IpAddress		= cDeviceInfoGEV->GetIPAddress();
				cDeviceInformation.NetMask			= cDeviceInfoGEV->GetSubnetMask();
				cDeviceInformation.GateWay			= cDeviceInfoGEV->GetDefaultGateway();

				break;

			case PvDeviceInfoType::PvDeviceInfoTypeU3V:
				cDeviceInfoU3V = (PvDeviceInfoU3V *)(cDeviceInfo);

				cDeviceInformation.MacAddress		= PvString("");
				cDeviceInformation.IpAddress		= PvString("");
				cDeviceInformation.NetMask			= PvString("");
				cDeviceInformation.GateWay			= PvString("");

				if( !(cDeviceInfoU3V->IsSuperSpeedSupported()) )
				{
					 *aResult	= AS_GIGE_USB3_NOT_ACTIVE;
				}

				if( !(cDeviceInfoU3V->IsPleoraDriverInstalled()) )
				{
					 *aResult	= AS_GIGE_PLEORA_USB_DRIVER_NOT_INSTALLED;
				}

				break;

			default:
				break;
		}

		cDeviceInformationStr.MacAddress	= (str8)cDeviceInformation.MacAddress.GetAscii();
		cDeviceInformationStr.IpAddress		= (str8)cDeviceInformation.IpAddress.GetAscii();
		cDeviceInformationStr.NetMask		= (str8)cDeviceInformation.NetMask.GetAscii();
		cDeviceInformationStr.GateWay		= (str8)cDeviceInformation.GateWay.GetAscii();
	}
}

aS_Pleora_NTx::~aS_Pleora_NTx(void)
{
	DeleteCriticalSection( &cAvailableTransferBufferLock );

	CloseStream();

	if( cPort.IsOpened() )
	{
		cPort.Close();
	}

	if( cDeviceAdapter != NULL )
	{
		delete cDeviceAdapter;
	}

	if( cDevice != NULL )
	{
		if( cDevice->IsConnected() )
		{
			cDevice->Disconnect();
		}

		PvDevice::Free( cDevice );
	}
}

i32	aS_Pleora_NTx::AcquireImage( p_u32 ImageCount, p_u8 Buffer, u32 FrameTimeOut )
{
	PvBuffer		*lBuffer = NULL;
	p_u8			lRawBuffer = NULL;
	u32				lSize = 0;
	PvResult		lResult;
	PvPayloadType	lType;
	u32				i=0;
	u32				lImageCount = *ImageCount;
	p_u8			lPointer = Buffer;
	u32				lTimeOut = FrameTimeOut;

	cAcqResult = PvResult::Code::OK;

	if( !cStream )
	{
		return AS_GIGE_STREAM_NOT_AVAILABLE;
	}

	if( !cPipeline )
	{
		return AS_GIGE_PIPELINE_NOT_AVAILABLE;
	}
	
	cResult = cResetCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_RESET_COMMAND_ERROR;
	}	
	
	cResult = cPipeline->Start();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_PIPELINE_START_ERROR;
	}

	cResult = cDevice->StreamEnable();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STREAM_ENABLE_ERROR;
	}

	cResult = cStartCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_START_COMMAND_ERROR;
	}

	if( cReadyCallBack )
	{
		cReadyCallBack( this );
	}

	while ( cAcqResult.IsOK() && (i<lImageCount) )
	{
		cAcqResult = cPipeline->RetrieveNextBuffer( &lBuffer, lTimeOut, &lResult );
	
		if( cAcqResult.IsOK() )
		{
			if( lResult.IsOK() )
			{
				lType = lBuffer->GetPayloadType();

				if( lType == PvPayloadTypeImage )
				{
					lRawBuffer = lBuffer->GetDataPointer();
					lSize = lBuffer->GetSize();

					memcpy( lPointer, lRawBuffer, lSize );
					
					if (!i)
					{
						lTimeOut = cFrameTimeOut;
					}

					i++;

					lPointer = lPointer + lSize;
				}
				else
				{
					cAcqResult = PvResult::Code::INVALID_DATA_FORMAT;
				}
			}
			else
			{
				cAcqResult = lResult;
			}

			if( cAcqResult.IsOK() )
			{
				cResult = cPipeline->ReleaseBuffer( lBuffer );
			}
			else
			{
				cPipeline->ReleaseBuffer( lBuffer );
			}

			if( cAcqResult.IsOK() )
			{
				cAcqResult = cBlocksDropped->GetValue( cBlocksDroppedVal );

				if( cAcqResult.IsOK() && cBlocksDroppedVal )
				{
					cAcqResult = PvResult::Code::ABORTED;
				}
			}

			if( cAcqResult.IsOK() )
			{
				cAcqResult = cBlockIDsMissing->GetValue( cBlockIDsMissingVal );

				if( cAcqResult.IsOK() && cBlockIDsMissingVal )
				{
					cAcqResult = PvResult::Code::ERR_OVERFLOW;
				}
			}
		}
	}

	*ImageCount = i;
	
	cResult = cStopCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STOP_COMMAND_ERROR;
	}

	if( cFinishCallBack )
	{
		cFinishCallBack( this );
	}

	cResult = cDevice->StreamDisable();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STREAM_DISABLE_ERROR;
	}

	cResult = cPipeline->Stop();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_PIPELINE_STOP_ERROR;
	}

	if( !cAcqResult.IsOK() )
	{
		cResult = cAcqResult;
		return AS_GIGE_ACQUISION_ABORTED_ERROR;
	}

	return AS_NO_ERROR;
}

i32	aS_Pleora_NTx::AcquireImageThread( u32 ImageCount, u32 FrameTimeOut )
{
	PvBuffer		*lBuffer = NULL;
	p_u8			lRawBuffer = NULL;
	u32				lSize = 0;
	PvResult		lAcqResult = PvResult::Code::OK;
	PvPayloadType	lType;
	u32				lCurrentFrameWithinBuffer=0;
	p_u8			lTransferBuffer = NULL;
	p_u8			lPointer = NULL;
	i32				lResult = AS_NO_ERROR;
	u32				lTimeOut = FrameTimeOut;

	cAcqResult = PvResult::Code::OK;
	
	ClearQueue();
	InitializeQueue();

	cAcquiredImages = 0;

	if( ImageCount )
	{
		cContinuous = 0;
	}
	else
	{
		cContinuous = 1;
	}

	if( !cStream )
	{
		return AS_GIGE_STREAM_NOT_AVAILABLE;
	}

	if( !cPipeline )
	{
		return AS_GIGE_PIPELINE_NOT_AVAILABLE;
	}
	
	cResult = cResetCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_RESET_COMMAND_ERROR;
	}

	cResult = cPipeline->Start();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_PIPELINE_START_ERROR;
	}

	cResult = cDevice->StreamEnable();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STREAM_ENABLE_ERROR;
	}

	cResult = cStartCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_START_COMMAND_ERROR;
	}

	cStopAcquisition = 0;

	if( cReadyCallBack )
	{
		cReadyCallBack( this );
	}

	while ( cAcqResult.IsOK() && ((cAcquiredImages<ImageCount) || cContinuous) )
	{
		if( cStopAcquisition )
		{
			break;
		}
		
		if( lCurrentFrameWithinBuffer == 0 )	// new Transferbuffer
		{
			EnterCriticalSection( &cAvailableTransferBufferLock );
			if( cAvailableTransferBuffer.size() )
			{			
				lTransferBuffer = cAvailableTransferBuffer.front();
				lPointer = lTransferBuffer;
				cAvailableTransferBuffer.pop();
				LeaveCriticalSection( &cAvailableTransferBufferLock );
			}
			else
			{
				LeaveCriticalSection( &cAvailableTransferBufferLock );
				lResult = AS_GIGE_NO_TRANSFER_BUFFER_AVAILABLE;
				break;
			}
		}
		
		cAcqResult = cPipeline->RetrieveNextBuffer( &lBuffer, lTimeOut, &lAcqResult );
	
		if( cAcqResult.IsOK() )
		{
			if( lAcqResult.IsOK() )
			{
				lType = lBuffer->GetPayloadType();

				if( lType == PvPayloadTypeImage )
				{
					lRawBuffer = lBuffer->GetDataPointer();
					lSize = lBuffer->GetSize();

					std::copy( lRawBuffer, lRawBuffer + lSize, lPointer );	// try first to skip this
					
					lCurrentFrameWithinBuffer++;

					if (!cAcquiredImages)
					{
						lTimeOut = cFrameTimeOut;
					}

					cAcquiredImages++;
					lPointer = lPointer + lSize;

					if( lCurrentFrameWithinBuffer >= cTransferBufferFrameCount )
					{
						BufferReadyCallBack( lTransferBuffer, lCurrentFrameWithinBuffer );

						lCurrentFrameWithinBuffer = 0;
					}
				}
				else
				{
					cAcqResult = PvResult::Code::INVALID_DATA_FORMAT;
				}
			}
			else
			{
				cAcqResult = lAcqResult;
			}

			if( cAcqResult.IsOK() )
			{
				cAcqResult = cPipeline->ReleaseBuffer( lBuffer );
			}
			else
			{
				cPipeline->ReleaseBuffer( lBuffer );
			}
			
			if( cAcqResult.IsOK() && !( cAcquiredImages % 10000 ) )
			{
				cAcqResult = cBlocksDropped->GetValue( cBlocksDroppedVal );

				if( cAcqResult.IsOK() && cBlocksDroppedVal )
				{
					lResult = AS_GIGE_BLOCKS_DROPPED;
					break;
				}
			}

			if( cAcqResult.IsOK() && !( cAcquiredImages % 10000 ) )
			{
				cAcqResult = cBlockIDsMissing->GetValue( cBlockIDsMissingVal );

				if( cAcqResult.IsOK() && cBlockIDsMissingVal )
				{
					lResult = AS_GIGE_BLOCKS_IDS_MISSING;
					break;
				}
			}
		}
	}

	cStopAcquisition = 0;

	if( lCurrentFrameWithinBuffer )
	{
		BufferReadyCallBack( lTransferBuffer, lCurrentFrameWithinBuffer );
	}

	cResult = cStopCmd->Execute();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STOP_COMMAND_ERROR;
	}

	if( cFinishCallBack )
	{
		cFinishCallBack( this );
	}

	cResult = cDevice->StreamDisable();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_STREAM_DISABLE_ERROR;
	}

	cResult = cPipeline->Stop();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_PIPELINE_STOP_ERROR;
	}

	if( !cAcqResult.IsOK() )
	{
		cResult = cAcqResult;
		return AS_GIGE_ACQUISION_ABORTED_ERROR;
	}

	return lResult;
}

void aS_Pleora_NTx::BufferReadyCallBack( p_u8 aTransferBuffer, u32 aCurrentFrameWithinBuffer )
{
	if ( cBufferCallBack )
	{
		cBufferCallBack( aTransferBuffer, aCurrentFrameWithinBuffer );
	}
	else
	{
		EnterCriticalSection( &cAvailableTransferBufferLock );
		cAvailableTransferBuffer.push( aTransferBuffer );
		LeaveCriticalSection( &cAvailableTransferBufferLock );
	}
}

void aS_Pleora_NTx::ClearQueue()
{
	while( cAvailableTransferBuffer.size() )
	{
		cAvailableTransferBuffer.pop();
	}
}

i32	aS_Pleora_NTx::ClosePipeline()
{
	cResult = PvResult::Code::OK;
	
	if( !cPipeline )
	{
		return AS_NO_ERROR;
	}

	if( cPipeline->IsStarted() )
	{
		cResult = cPipeline->Stop();
	}

	delete cPipeline;
	cPipeline = NULL;

	if( !cResult.IsOK() )
	{
		return AS_GIGE_PIPELINE_STOP_ERROR;
	}

	return AS_NO_ERROR;
}

i32 aS_Pleora_NTx::CloseSerialPort()
{
	cResult = cPort.Close();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_CLOSE_ERROR;
	}

	return AS_NO_ERROR;
}

i32 aS_Pleora_NTx::CloseStream()
{
	i32	lResult = AS_NO_ERROR;
	
	if( cPipeline )
	{
		lResult = ClosePipeline();
	}

	if( ( lResult == AS_NO_ERROR ) && ( cStream != NULL ) )
	{
		cResult = cStream->Close();

		if( !cResult.IsOK() )
		{
			return AS_GIGE_STREAM_CLOSE_ERROR;
		}

		PvStream::Free( cStream );
		cStream = NULL;
	}

	return AS_NO_ERROR;
}

PvResult aS_Pleora_NTx::ConfigureSerialBulk( PvDeviceSerial SerialPort )
{
	PvResult	lResult;
	u64			lBulkSelektor = SerialPort - 2;

	lResult = cDeviceParams->SetEnumValue( "BulkSelector", lBulkSelektor );
	lResult = cDeviceParams->SetEnumValue( "BulkMode", "UART" );
	lResult = cDeviceParams->SetEnumValue( "BulkBaudRate", "Baud38400" );
	lResult = cDeviceParams->SetEnumValue( "BulkNumOfStopBits", "One" );
	lResult = cDeviceParams->SetEnumValue( "BulkParity", "None" );

	return lResult;
}

PvResult aS_Pleora_NTx::ConfigureSerialUart( PvDeviceSerial SerialPort )
{
	PvResult lResult;

	lResult = cDeviceParams->SetEnumValue( "Uart0BaudRate", "Baud38400" );
	lResult = cDeviceParams->SetEnumValue( "Uart0NumOfStopBits", "One" );
	lResult = cDeviceParams->SetEnumValue( "Uart0Parity", "None" );

	return lResult;
}

PvResult aS_Pleora_NTx::ConfigureStream( bool TimeoutCountedAsError, bool AbortCountedAsError )
{
	PvResult lResult = PvResult::Code::OK;
	PvDeviceGEV* lDeviceGEV = dynamic_cast<PvDeviceGEV *>( cDevice );

	switch( cDeviceInfoType )
	{
		case PvDeviceInfoType::PvDeviceInfoTypeGEV:
			if ( lDeviceGEV != NULL )
			{
				PvStreamGEV *lStreamGEV = static_cast<PvStreamGEV *>( cStream );

				// Negotiate packet size
				lResult = lDeviceGEV->NegotiatePacketSize();

				if( lResult.IsOK() )
				{
					// Configure device streaming destination
					lResult = lDeviceGEV->SetStreamDestination( lStreamGEV->GetLocalIPAddress(), lStreamGEV->GetLocalPort() );
				}
			}

			if( lResult.IsOK() )
			{
				lResult = cStreamParams->SetBooleanValue( "TimeoutCountedAsError", TimeoutCountedAsError );
			}

			break;

		case PvDeviceInfoType::PvDeviceInfoTypeU3V:
			if( lResult.IsOK() )
			{
				lResult = cStreamParams->SetBooleanValue( "ResyncCountedAsError", TRUE /*ResyncCountedAsError*/ );
			}

			break;

		default:
			break;
	}

	if( lResult.IsOK() )
	{
		lResult = cStreamParams->SetBooleanValue( "AbortCountedAsError", AbortCountedAsError );
	}

	return lResult;
}

PvResult aS_Pleora_NTx::Connect(const str8 aDeviceDescriptor)
{
	PvResult lResult;
	lResult= cSystem.FindDevice(aDeviceDescriptor, &cDeviceInfo);

	if( lResult.IsOK() )
	{
		cDevice = PvDevice::CreateAndConnect( cDeviceInfo, &lResult);
	}

	return lResult;
}

i32	aS_Pleora_NTx::CreatePipeline( u32 BufferCount )
{
	u32 NeededBufferSize = 0;
	
	if( !cStream )
	{
		return AS_GIGE_STREAM_NOT_AVAILABLE;
	}

	if( cPipeline )
	{
		return AS_GIGE_PIPELINE_CREATED_ALREADY;
	}

	cPipeline = new PvPipeline( cStream );

	NeededBufferSize = cDevice->GetPayloadSize();

	for ( u32 i=0 ; i<cTransferBuffer.size() ; i++ )
	{
		try
		{
			cTransferBuffer[i].resize( cTransferBufferFrameCount * NeededBufferSize , 0 );
		}
		catch( std::bad_alloc )
		{
			cResult = PvResult::Code::NOT_ENOUGH_MEMORY;
		}
	}

	if( cResult.IsOK() )
	{
		cPipeline->SetBufferSize( NeededBufferSize );
		cResult = cPipeline->SetBufferCount( BufferCount );
	}

	if( !cResult.IsOK() )
	{
		cTransferBuffer.clear();
		delete cPipeline;
		cPipeline = NULL;
		return AS_GIGE_PIPELINE_CREATION_ERROR;
	}

	return AS_NO_ERROR;
}

i32	aS_Pleora_NTx::FlushRxBuffer()
{
	cResult = cPort.FlushRxBuffer();

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_FLUSH_ERROR;
	}

	return AS_NO_ERROR;
}

u64 aS_Pleora_NTx::GetAcquiredImageCount()
{
	return cAcquiredImages;
}

NTxDeviceInfoStr aS_Pleora_NTx::GetDeviceInfoStr()
{
	return cDeviceInformationStr;
}

i32 aS_Pleora_NTx::GetErrorDescription( PvResult aPleoraErrorCode,
									 str8 aPleoraErrorCodeString,
									 p_u32 aPleoraErrorCodeStringLen,
									 str8 aPleoraErrorDescription,
									 p_u32 aPleoraErrorDescriptionLen)
{
	u32			lSize = 0;
	i32			dResult = AS_NO_ERROR;

	if( aPleoraErrorCodeString && aPleoraErrorCodeStringLen && *aPleoraErrorCodeStringLen )
	{
		if( aPleoraErrorCode.GetCodeString().GetLength() > ((*aPleoraErrorCodeStringLen)-1) )
		{
			lSize = (*aPleoraErrorCodeStringLen)-1;
			dResult = AS_BUFFER_TO_SMALL;
		}
		else
		{
			lSize = aPleoraErrorCode.GetCodeString().GetLength();
		}		
		memcpy( aPleoraErrorCodeString, aPleoraErrorCode.GetCodeString().GetAscii(), lSize );
		aPleoraErrorCodeString[lSize] = 0x00;
	}

	if( aPleoraErrorCodeStringLen )
	{
		*aPleoraErrorCodeStringLen = (aPleoraErrorCode.GetCodeString().GetLength())+1;
	}

	if( aPleoraErrorDescription && aPleoraErrorDescriptionLen && *aPleoraErrorDescriptionLen )
	{
		if( aPleoraErrorCode.GetDescription().GetLength() > ((*aPleoraErrorDescriptionLen)-1) )
		{
			lSize = (*aPleoraErrorDescriptionLen)-1;
			dResult = AS_BUFFER_TO_SMALL;
		}
		else
		{
			lSize = aPleoraErrorCode.GetDescription().GetLength();
		}
		memcpy( aPleoraErrorDescription, aPleoraErrorCode.GetDescription().GetAscii(), lSize );
		aPleoraErrorDescription[lSize] = 0x00;
	}

	if( aPleoraErrorDescriptionLen )
	{
		*aPleoraErrorDescriptionLen = (aPleoraErrorCode.GetDescription().GetLength())+1;
	}

	return dResult;
}

i32	aS_Pleora_NTx::GetBufferHandlingThreadPriority()
{
	if( cPipeline )
	{
		return cPipeline->GetBufferHandlingThreadPriority();
	}
	else
	{
		return -1;
	}
}

i32 aS_Pleora_NTx::GetIntegerValue( const str8 Property, i64 &Value )
{
	i32 lResult = AS_NO_ERROR;
	
	cResult = cStreamParams->GetIntegerValue( Property, Value );

	if( !cResult.IsOK() )
	{
		lResult = AS_GIGE_GET_INTEGER_VALUE_ERROR;
	}

	return lResult;
}

PvResult aS_Pleora_NTx::GetLastResult()
{
	return cResult;
}

void aS_Pleora_NTx::InitializeQueue()
{
	for( u32 i=0; i<cTransferBuffer.size() ; i++ )
	{
		cAvailableTransferBuffer.push( &cTransferBuffer[i][0] );
	}
}

i32 aS_Pleora_NTx::OpenSerialPort( PvDeviceSerial SerialPort, u32 RxBufferSize, u8 UseTermChar, u8 TermChar )
{
	if( cPort.IsOpened() )
	{
		cResult = cPort.Close();
	}
	
	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_CLOSE_ERROR;
	}

	if( !cPort.IsSupported( cDeviceAdapter, SerialPort ) )
	{
		return AS_GIGE_SERIAL_PORT_NOT_SUPPORTED;
	}

	if( (SerialPort == PvDeviceSerial0) || (SerialPort == PvDeviceSerial1) )
	{
		cResult = ConfigureSerialUart( SerialPort );
	}
	else if( (SerialPort >= PvDeviceSerialBulk0) && (SerialPort <= PvDeviceSerialBulk7) )
	{
		cResult = ConfigureSerialBulk( SerialPort );
	}

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_CONFIG_FAILED;
	}

	cResult = cPort.SetRxBufferSize( RxBufferSize );

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_SET_RX_BUFFER_FAILED;
	}

	cResult = cPort.Open( cDeviceAdapter, SerialPort );

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_OPEN_ERROR;
	}

	cUseTermChar = UseTermChar;
	cTermChar = TermChar;
		
	return AS_NO_ERROR;
}

i32 aS_Pleora_NTx::OpenStream( bool TimeoutCountedAsError, bool AbortCountedAsError )
{
	if( !cStream )
	{
		cStream = PvStream::CreateAndOpen( cDeviceInfo->GetConnectionID(), &cResult );

		if( !cResult.IsOK() )
		{
			return AS_GIGE_STREAM_OPEN_ERROR;
		}

		cStreamParams		= cStream->GetParameters();
		cResetCmd			= dynamic_cast<PvGenCommand *>( cStreamParams->Get( "Reset" ) );
		cBlocksDropped		= dynamic_cast<PvGenInteger *>( cStreamParams->Get( "BlocksDropped" ) );
		cBlockIDsMissing	= dynamic_cast<PvGenInteger *>( cStreamParams->Get( "BlockIDsMissing" ) );

		cResult = ConfigureStream( TimeoutCountedAsError, AbortCountedAsError );

		if( !cResult.IsOK() )
		{
			return AS_GIGE_STREAM_CONFIG_ERROR;
		}
	}
	else
	{
		return AS_GIGE_STREAM_ALREADY_OPENED;
	}

	return AS_NO_ERROR;
}

i32	aS_Pleora_NTx::ReadSerialPort( p_u8 RxBuffer, u32 RxBufferSize, u32 *BytesRead, u32 TimeOut )
{
	u32 lBytesRead = 0;
	
	if( cUseTermChar )
	{
		*BytesRead = 0;

		while ( *BytesRead < RxBufferSize )
		{
			cResult = cPort.Read( RxBuffer + *BytesRead, RxBufferSize - *BytesRead, lBytesRead, TimeOut );

			if( !cResult.IsOK() )
			{
				return AS_GIGE_SERIAL_PORT_READ_ERROR;
			}

			*BytesRead = *BytesRead + lBytesRead;

			if ( RxBuffer[ *BytesRead-1 ] == cTermChar )
			{
				return AS_NO_ERROR;
			}
		}

		return  AS_GIGE_SERIAL_PORT_READ_BUFFER_FULL;
	}
	else
	{
		cResult = cPort.Read( RxBuffer, RxBufferSize, lBytesRead, TimeOut );

		*BytesRead = lBytesRead;

		if( !cResult.IsOK() )
		{
			return AS_GIGE_SERIAL_PORT_READ_ERROR;
		}
	}

	return AS_NO_ERROR;
}

void aS_Pleora_NTx::RegisterAcqArmedCallBack( p_readyCallBack aAcqArmedCallBack )
{
	cReadyCallBack = aAcqArmedCallBack;
}

void aS_Pleora_NTx::RegisterAcqFinishCallBack( p_readyCallBack aAcqFinishCallBack )
{
	cFinishCallBack = aAcqFinishCallBack;
}

void aS_Pleora_NTx::RegisterTransferBufferReadyCallBack( p_bufferCallBack aTransferBufferReadyCallBack )
{
	cBufferCallBack = aTransferBufferReadyCallBack;
}

void aS_Pleora_NTx::ReturnBuffer( p_u8 Buffer )
{
	EnterCriticalSection( &cAvailableTransferBufferLock );
	cAvailableTransferBuffer.push( Buffer );
	LeaveCriticalSection( &cAvailableTransferBufferLock );
}

void aS_Pleora_NTx::SetFrameTime( dbl FrameTime )
{
	cFrameTime = FrameTime;
}

void aS_Pleora_NTx::SetFrameTimeOut( u32 FrameTimeOut )
{
	if( FrameTimeOut > cMinFrameTimeOut )
	{
		cFrameTimeOut = FrameTimeOut;
	}
	else
	{
		cFrameTimeOut = cMinFrameTimeOut;
	}
}

i32	aS_Pleora_NTx::SetImageFormatControl( const str8 PixelFormat, u64 Width, u64 Height, u64 OffsetX, u64 OffsetY, const str8 SensorTaps, const str8 TestPattern )
{
	cResult = cDeviceParams->SetEnumValue( "PixelFormat", "Mono8" );	// Done to avoid selection of unsupport mode

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetEnumValue( "SensorDigitizationTaps", SensorTaps );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetEnumValue( "PixelFormat", PixelFormat );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetIntegerValue( "Width", Width );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetIntegerValue( "Height", Height );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetIntegerValue( "OffsetX", OffsetX );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetIntegerValue( "OffsetY", OffsetY );
	}

	if( cResult.IsOK() )
	{
		cResult = cDeviceParams->SetEnumValue( "TestPattern", TestPattern );
	}

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SET_IMAGE_FORMAT_ERROR;
	}

	return AS_NO_ERROR;
}

void aS_Pleora_NTx::SetMinFrameTimeOut( u32 MinFrameTimeOut )
{
	cMinFrameTimeOut = MinFrameTimeOut;
}

void aS_Pleora_NTx::SetTransferBuffer( u32 TransferBufferCount, u32 TransferBufferFrameCount )
{
	cTransferBufferFrameCount = TransferBufferFrameCount;
	cTransferBuffer.resize( TransferBufferCount );
}

void aS_Pleora_NTx::StopAcquisition()
{
	cStopAcquisition = 1;
}

i32	aS_Pleora_NTx::WriteSerialPort( const p_u8 TxBuffer, u32 TxBufferSize, u32 *BytesWritten )
{
	u32 lBytesWritten = 0;

	cResult = cPort.Write( TxBuffer, TxBufferSize, lBytesWritten );

	*BytesWritten = lBytesWritten;

	if( !cResult.IsOK() )
	{
		return AS_GIGE_SERIAL_PORT_WRITE_ERROR;
	}

	return AS_NO_ERROR;
}