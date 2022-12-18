#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../KazLibrary/Math/KazMath.h"

struct BoundingBoxData
{
	KazMath::Vec3<float> minPos;
	KazMath::Vec3<float> maxPos;
};

/// <summary>
/// 読み込んだモデルからBoundingBoxを形成する情報を作る
/// </summary>
class BoundingBox
{
public:
	BoundingBox(std::vector<DirectX::XMFLOAT4> VERT_DATA);
	BoundingBoxData GetData();
	D3D12_GPU_DESCRIPTOR_HANDLE GetViewHandle();

	void Compute();

	void DebugDraw();
private:
	CreateGpuBuffer buffers;

	struct BoundingBoxBufferData
	{
		DirectX::XMFLOAT3 minPos;
		DirectX::XMFLOAT3 maxPos;
	};

	RESOURCE_HANDLE vertBufferHandle;
	RESOURCE_HANDLE bbBufferHandle, bbViewHandle;
	RESOURCE_HANDLE matBufferHandle;
};