#pragma once
#include"../Math/KazMath.h"
#include"../Pipeline/GraphicsPipeLineMgr.h"

struct IData
{
protected:
	void *address;
public:
	string GetName()
	{
		return typeid(address).name();
	}

	int cameraIndex = 0;
};

struct AddTextureData
{
	array<RESOURCE_HANDLE, 10> handle;
	array<GraphicsRootParamType, 10> paramType;

	AddTextureData()
	{
		for (int i = 0; i < handle.size(); ++i)
		{
			handle[i] = -1;
			paramType[i] = GRAPHICS_PRAMTYPE_NONE;
		}
	}
};

struct FlipData
{
	bool x;
	bool y;

	FlipData()
	{
		x = false;
		y = false;
	}
};

struct Sprite2DData :public IData
{
	KazMath::Transform2D transform;
	RESOURCE_HANDLE handle;
	RESOURCE_HANDLE animationHandle;
	FlipData flip;
	int pipelineName;

	bool changeSizeTypeFlag;
	XMFLOAT4 size;
	float alpha;

	AddTextureData addHandle;

	Sprite2DData()
	{
		address = this;
		animationHandle = -1;
		handle = -1;
		changeSizeTypeFlag = false;
		alpha = 255.0f;
		pipelineName = static_cast<int>(PIPELINE_NAME_SPRITE);
	}
};


struct Sprite3DData :public IData
{
	KazMath::Transform3D transform;
	RESOURCE_HANDLE handle;
	RESOURCE_HANDLE animationHandle;
	FlipData flip;
	bool billBoardFlag;
	XMMATRIX motherMat;
	int pipelineName;

	bool changeSizeTypeFlag;
	XMFLOAT4 size;
	float alpha;


	Sprite3DData()
	{
		address = this;
		animationHandle = -1;
		handle = -1;
		pipelineName = static_cast<int>(PIPELINE_NAME_SPRITE);

		alpha = 255.0f;
		changeSizeTypeFlag = false;
		size = { 1.0f,1.0f,1.0f,1.0 };
		motherMat = XMMatrixIdentity();
	}
};

struct Pera3DData :public IData
{
	KazMath::Transform3D transform;
	bool billBoardFlag;
	XMMATRIX motherMat;
	int pipelineName;

	bool changeSizeTypeFlag;
	XMFLOAT4 size;
	float alpha;

	Pera3DData()
	{
		address = this;
		pipelineName = static_cast<int>(PIPELINE_NAME_SPRITE);

		alpha = 255.0f;
		changeSizeTypeFlag = false;
		size = { 1.0f,1.0f,1.0f,1.0 };
		motherMat = XMMatrixIdentity();
	}
};

struct Obj3DData :public IData
{
	KazMath::Transform3D transform;
	RESOURCE_HANDLE handle;
	KazMath::Vec3<float> upVector;
	KazMath::Vec3<float> frontVector;
	XMMATRIX motherMat;
	int pipelineName;
	XMFLOAT4 color;
	bool removeMaterialFlag;

	AddTextureData addHandle;

	Obj3DData()
	{
		address = this;
		handle = -1;
		upVector = { 0,1,0 };
		frontVector = { 0,0,1 };
		pipelineName = static_cast<int>(PIPELINE_NAME_OBJ);
		color = { 0.0f,0.0f,0.0f,255.0f };
		removeMaterialFlag = false;
		motherMat = XMMatrixIdentity();
	}
};

struct LineDrawData :public IData
{
	KazMath::Vec3<float> startPos;
	KazMath::Vec3<float> endPos;
	XMFLOAT4 color;
	XMMATRIX motherMat;
	int pipelineName;


	LineDrawData()
	{
		address = this;
		pipelineName = static_cast<int>(PIPELINE_NAME_LINE);
		color = { 255.0f,255.0f,255.0f,255.0f };
		motherMat = XMMatrixIdentity();
	}
};

struct PolygonDrawData :public IData
{
	KazMath::Transform3D transform;
	XMFLOAT4 color;
	XMMATRIX motherMat;
	int pipelineName;

	PolygonDrawData()
	{
		address = this;
		color = { 255.0f,255.0f,255.0f,255.0f };
		pipelineName = static_cast<int>(PIPELINE_NAME_COLOR);
		motherMat = XMMatrixIdentity();
	}
};

struct FbxModelData :public IData
{
	KazMath::Transform3D transform;
	RESOURCE_HANDLE handle;
	XMMATRIX motherMat;
	bool isPlay;
	int animationNumber;
	int pipelineName;


	FbxModelData()
	{
		address = this;
		handle = -1;
		pipelineName = static_cast<int>(PIPELINE_NAME_FBX);
		animationNumber = 0;
		motherMat = XMMatrixIdentity();
	}
};

struct Circle2DDrawData :public IData
{
	KazMath::Transform3D transform;
	XMFLOAT4 color;
	float radius;
	int pipelineName;

	Circle2DDrawData()
	{
		address = this;
		color = { 255.0f,255.0f,255.0f,255.0f };
		radius = 10.0f;
		pipelineName = static_cast<int>(PIPELINE_NAME_COLOR);
	}
};
