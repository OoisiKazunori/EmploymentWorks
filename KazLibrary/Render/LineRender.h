#pragma once
#include"../DirectXCommon/Base.h"
#include"../Render/IRender.h"
#include"../Math/KazMath.h"
#include"../Helper/DirtyFlag.h"
#include"../Render/RenderData.h"

class LineRender :public IRender
{
public:
	LineRender();
	~LineRender();

	void Draw();

	LineDrawData data;
private:
	PipeLineNames pipeline;
	array<LineVertex, 2>vertices;
	UINT vertByte;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	RESOURCE_HANDLE vertexBufferHandle;
	RESOURCE_HANDLE indexBufferHandle;
	RESOURCE_HANDLE constBufferHandle;
};

