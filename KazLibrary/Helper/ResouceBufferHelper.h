#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

class ResouceBufferHelper
{
public:
	ResouceBufferHelper();

	RESOURCE_HANDLE CreateBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	RESOURCE_HANDLE CreateBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);

	ComputeBufferHelper::BufferData CreateAndGetBuffer(UINT STRUCTURE_BYTE_STRIDE, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);
	ComputeBufferHelper::BufferData CreateAndGetBuffer(const KazBufferHelper::BufferResourceData &BUFFER_OPTION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT STRUCTURE_BYTE_STRIDE, UINT ELEMENT_NUM, bool GENERATE_COUNTER_BUFFER_FLAG = false);

	RESOURCE_HANDLE SetBuffer(const ComputeBufferHelper::BufferData &DATA, GraphicsRootParamType ROOTPARAM);

	void TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE);

	void StackToCommandListAndCallDispatch(ComputePipeLineNames NAME, const DispatchCallData &DISPATCH_DATA);
	void StackToCommandList(PipeLineNames NAME);

	void DeleteAllData();


	void InitCounterBuffer();
	const ComputeBufferHelper::BufferData &GetBufferData(RESOURCE_HANDLE HANDLE);
private:
	std::vector<ComputeBufferHelper::BufferData>bufferArrayData;
	HandleMaker handle;



	KazBufferHelper::BufferResourceData counterBufferData;
};