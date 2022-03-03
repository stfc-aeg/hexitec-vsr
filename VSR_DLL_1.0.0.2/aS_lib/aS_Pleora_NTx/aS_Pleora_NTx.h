#pragma once

#include <PvSystem.h>
#include <PvDevice.h>
#include <PvDeviceGEV.h>
#include <PvDeviceU3V.h>
#include <PvStream.h>
#include <PvStreamGEV.h>
#include <PvStreamU3V.h>
#include <PvPipeline.h>
#include <PvDeviceSerialPort.h>
#include <PvDeviceAdapter.h>
#include <queue>

#include "..\aS_TypeDefinitions\aS_TypeDefinitions.h"

using namespace aS;

typedef void (__cdecl *p_readyCallBack)( hdl deviceHdl );
typedef void (__cdecl *p_bufferCallBack)( p_u8 transferBuffer, u32 frameCount );

typedef struct NTxDeviceInfo {
	PvString	Vendor;
	PvString	Model;
	PvString	ManufacturerInfo;
	PvString	SerialNumber;
	PvString	UserId;
	PvString	MacAddress;
	PvString	IpAddress;
	PvString	NetMask;
	PvString	GateWay;
	} NTxDeviceInfo, *NTxDeviceInfoPtr, **NTxDeviceInfoHdl;

typedef struct NTxSerialPort {
	PvDeviceSerialPort	*port;
	u8					useTermChar;
	u8					termChar;
	} NTxSerialPort, *NTxSerialPortPtr, **NTxSerialPortHdl;

typedef std::vector<NTxSerialPort>			NTxSerialPort_vec;
typedef NTxSerialPort_vec::size_type		NTxSerialPort_vec_i;

#ifndef _M_X64
#pragma pack(push)
#pragma pack(1)
#endif

typedef struct NTxSerialPortConfig {
	PvDeviceSerial	SerialPort;
	str8			StopBits;
	str8			Parity;
	bool			LoopBack;
	str8			Mode;
	str8			BaudRate;
	u64				BaudRateFactor;
	dbl				BaudRateValue;
	str8			SystemClockDivider;
	dbl				OutputClockFrequency;
	} NTxSerialPortConfig, *NTxSerialPortConfigPtr, **NTxSerialPortConfigHdl;

typedef struct NTxDeviceInfoStr {
	str8		Vendor;
	str8		Model;
	str8		ManufacturerInfo;
	str8		SerialNumber;
	str8		UserId;
	str8		MacAddress;
	str8		IpAddress;
	str8		NetMask;
	str8		GateWay;
	} NTxDeviceInfoStr, *NTxDeviceInfoStrPtr, **NTxDeviceInfoStrHdl;

#ifndef _M_X64
#pragma pack(pop)
#endif

class aS_Pleora_NTx
{
private:
	PvSystem				cSystem;
	PvResult				cResult;
	PvResult				cAcqResult;
	PvDevice				*cDevice;
	PvGenParameterArray		*cDeviceParams;
	PvGenParameterArray		*cStreamParams;
	PvDeviceInfoType		cDeviceInfoType;
	const PvDeviceInfo		*cDeviceInfo;
	PvDeviceInfoGEV			*cDeviceInfoGEV;
	PvDeviceInfoU3V			*cDeviceInfoU3V;

	NTxDeviceInfo			cDeviceInformation;
	NTxDeviceInfoStr		cDeviceInformationStr;
	
	PvStream				*cStream;
	PvGenCommand			*cStartCmd;
	PvGenCommand			*cStopCmd;
	PvGenCommand			*cResetCmd;
	PvPipeline				*cPipeline;
	i64						cBlocksDroppedVal;
	PvGenInteger			*cBlocksDropped;
	i64						cBlockIDsMissingVal;
	PvGenInteger			*cBlockIDsMissing;

	PvDeviceAdapter			*cDeviceAdapter;
	NTxSerialPort_vec		cPort;
	fb_vec					cTransferBuffer;
	u32						cTransferBufferFrameCount;
	std::queue <p_u8>		cAvailableTransferBuffer;
	CRITICAL_SECTION		cAvailableTransferBufferLock;
	p_readyCallBack			cReadyCallBack;
	p_readyCallBack			cFinishCallBack;
	p_bufferCallBack		cBufferCallBack;
	u64						cAcquiredImages;
	u8						cContinuous;
	u8						cStopAcquisition;
	dbl						cFrameTime;
	u32						cFrameTimeOut;
	u32						cMinFrameTimeOut;

public:
	aS_Pleora_NTx(const str8 aDeviceDescriptor, p_i32 aResult);
	~aS_Pleora_NTx();
	PvResult				GetLastResult();
	i32						OpenSerialPort( u8 AppPortIdx, NTxSerialPortConfigPtr PortConfig, u32 RxBufferSize, u8 UseTermChar, u8 TermChar );
	i32						CloseSerialPort( u8 AppPortIdx );
	i32						FlushRxBuffer( u8 AppPortIdx );
	i32						ReadSerialPort( u8 AppPortIdx, p_u8 RxBuffer, u32 RxBufferSize, u32 *BytesRead, u32 TimeOut );
	i32						WriteSerialPort( u8 AppPortIdx, const p_u8 TxBuffer, u32 TxBufferSize, u32 *BytesWritten );

	i32						OpenStream( bool TimeoutCountedAsError, bool AbortCountedAsError );
	i32						CloseStream();
	
	i32						SetImageFormatControl( const str8 PixelFormat,
												   u64 Width,
												   u64 Height,
												   u64 OffsetX,
												   u64 OffsetY,
												   const str8 SensorTaps,
												   const str8 TestPattern,
												   u8 UseDval );
	i32						CreatePipeline( u32 BufferCount );
	i32						ClosePipeline();
	i32						AcquireImage( p_u32 ImageCount, p_u8 Buffer, u32 FrameTimeOut );
	i32						AcquireImageThread( u32 ImageCount, u32 FrameTimeOut );

	i32						GetIntegerValue( const str8 Property, i64 &Value );
	i32						GetBufferHandlingThreadPriority();
	void					SetTransferBuffer( u32 TransferBufferCount, u32 TransferBufferFrameCount );
	void					ReturnBuffer( p_u8 Buffer );
	void					RegisterAcqArmedCallBack( p_readyCallBack aAcqArmedCallBack );
	void					RegisterAcqFinishCallBack( p_readyCallBack aAcqFinishCallBack );
	void					RegisterTransferBufferReadyCallBack( p_bufferCallBack aTransferBufferReadyCallBack );
	u64						GetAcquiredImageCount();
	void					StopAcquisition();
	void					SetFrameTime( dbl FrameTime );
	void					SetFrameTimeOut( u32 FrameTimeOut );
	void					SetMinFrameTimeOut( u32 MinFrameTimeOut );
	
	static i32				GetErrorDescription( PvResult aPleoraErrorCode,
												 str8 aPleoraErrorCodeString,
												 p_u32 aPleoraErrorCodeStringLen,
												 str8 aPleoraErrorDescription,
												 p_u32 aPleoraErrorDescriptionLen);

	NTxDeviceInfoStr		GetDeviceInfoStr();

private:
	i32						Connect(const str8 aDeviceDescriptor);
	PvResult				ConfigureSerialUart( NTxSerialPortConfigPtr PortConfig );
	PvResult				ConfigureSerialBulk( NTxSerialPortConfigPtr PortConfig );
	PvResult				ConfigureStream( bool TimeoutCountedAsError, bool AbortCountedAsError );
	void					ClearQueue();
	void					InitializeQueue();
	void					BufferReadyCallBack( p_u8 aTransferBuffer, u32 aCurrentFrameWithinBuffer );
};
