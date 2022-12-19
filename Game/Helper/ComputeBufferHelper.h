#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"

struct DispatchCallData
{
	UINT x, y, z;
};

class IComputeParticle
{
public:
	IComputeParticle();

	void CreateBuffer(UINT TRANSMISSION_DATA, GraphicsRangeType RANGE, GraphicsRootParamType ROOTPARAM, UINT ELEMENT_NUM);
	void TransData(RESOURCE_HANDLE HANDLE, void *TRANS_DATA, UINT TRANSMISSION_DATA_SIZE);

	void Compute(ComputePipeLineNames NAME, const DispatchCallData &DATA);

private:
	struct BufferData
	{
		GraphicsRangeType rangeType;
		GraphicsRootParamType rootParamType;
		UINT bufferSize;
		RESOURCE_HANDLE bufferHandle;
		RESOURCE_HANDLE viewHandle;
	};

	std::vector<BufferData>bufferArrayData;
	CreateGpuBuffer buffers;

};
