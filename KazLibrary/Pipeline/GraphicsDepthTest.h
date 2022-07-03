#pragma once
#include"../DirectXCommon/Base.h"
#include"../Helper/HandleMaker.h"

//一個しか生成できないので注意
class GraphicsDepthTest {
public:
	GraphicsDepthTest();
	~GraphicsDepthTest();

	RESOURCE_HANDLE CreateBuffer();
	void Clear(RESOURCE_HANDLE HANDLE);

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> dsvH;
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	vector<Microsoft::WRL::ComPtr<ID3D12Resource>> depthBuffer;

	HandleMaker handle;
};

