#pragma once

#include "..\aS_TypeDefinitions\aS_TypeDefinitions.h"

using namespace aS;

#define AS_IM_TIMEOUT 5000

// Instance Manager

typedef struct IM_HANDLE_STRUCT{
void			*Handle;
hdl				Status;
} IM_HANDLE_STRUCT, *PIM_HANDLE_STRUCT, **HIM_HANDLE_STRUCT;

class InstanceManager
{
private:
	u32								TimeOut;
	std::vector<IM_HANDLE_STRUCT>	HandleVector;
	void							*VectorMutex;

public:
	InstanceManager();
	InstanceManager(u32 TimeOut);
	~InstanceManager();

	i32 AddInstance(void *InstanceHandle);
	i32 SetInstanceBusy(void *InstanceHandle);
	i32 SetInstanceReady(void *InstanceHandle);
	i32 RemInstance(void *InstanceHandle);

private:
	i32 FindInstance(void *InstanceHandle, u32 *Position);
};
