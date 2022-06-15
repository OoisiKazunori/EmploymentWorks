#include "DebugScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Math/KazMath.h"
#include"../Helper/KazHelper.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Loader/TextureResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"

DebugScene::DebugScene()
{
	//short texHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "");

	buffer = std::make_unique<CreateGpuBuffer>();

	//コマンドシグネチャの生成---------------------------
	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;//DrawIndexedInstanced命令を複数呼び出す
	args[0].ConstantBufferView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;

	//コマンドシグネチャの設定-------------------------
	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = args.data();
	desc.NumArgumentDescs = args.size();
	desc.ByteStride = sizeof(IndirectCommand); //サンプルだと一つだが、ここ本来描画分サイズを用意しないといけない気がするのだが...
	//コマンドシグネチャの設定-------------------------

	auto result = DirectX12Device::Instance()->dev->CreateCommandSignature(&desc, GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE_DATA_DRAW).Get(), IID_PPV_ARGS(&commandSig));
	//コマンドシグネチャの生成---------------------------



	//定数バッファの生成---------------------------
	const UINT constantBufferDataSize = TRIANGLE_RESOURCE_COUNT * sizeof(SceneConstantBuffer);
	paramCBHandle = buffer->CreateBuffer(KazBufferHelper::SetConstBufferData(constantBufferDataSize));
	{

		//移動量の初期化
		for (UINT n = 0; n < TRIANGLE_ARRAY_NUM; n++)
		{
			constantBufferData[n].velocity = XMFLOAT4(KazMath::FloatRand(0.01f, 0.02f), 0.0f, 0.0f, 0.0f);
			constantBufferData[n].offset = XMFLOAT4(KazMath::FloatRand(-5.0f, -1.5f), KazMath::FloatRand(-1.0f, 1.0f), KazMath::FloatRand(0.0f, 2.0f), 0.0f);
			constantBufferData[n].color = XMFLOAT4(KazMath::FloatRand(0.5f, 1.0f), KazMath::FloatRand(0.5f, 1.0f), KazMath::FloatRand(0.5f, 1.0f), 1.0f);
			XMStoreFloat4x4(&constantBufferData[n].projection, CameraMgr::Instance()->orthographicMatProjection);
		}

		buffer->TransData(paramCBHandle, &constantBufferData[0], TRIANGLE_ARRAY_NUM * sizeof(SceneConstantBuffer));

		//定数バッファのビューは作る
		cbvSize = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_CBV);

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.NumElements = TRIANGLE_ARRAY_NUM;
		srvDesc.Buffer.StructureByteStride = sizeof(SceneConstantBuffer);
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		DescriptorHeapMgr::Instance()->CreateBufferView(cbvSize.startSize, srvDesc, buffer->GetBufferData(paramCBHandle).Get());
	}
	//定数バッファの生成---------------------------


	//頂点バッファ関連-------------------------
	{
		const float TriangleHalfWidth = 0.05f;
		const float TriangleDepth = 0.0f;
		const float CullingCutoff = 0.5f;
		struct Vert
		{
			XMFLOAT3 pos;
		};
		std::array<Vert, 3>vert;
		vert[0].pos = { 0.0f, TriangleHalfWidth, TriangleDepth };
		vert[1].pos = { TriangleHalfWidth, -TriangleHalfWidth, TriangleDepth };
		vert[2].pos = { -TriangleHalfWidth, -TriangleHalfWidth, TriangleDepth };

		int size = vert.size() * sizeof(Vert);
		short handle = buffer->CreateBuffer(KazBufferHelper::SetVertexBufferData(size));
		buffer->TransData(handle, vert.data(), size);

		//ComPtr<ID3D12Resource> vertexBufferUpload;
		//D3D12_SUBRESOURCE_DATA vertexData = {};
		//vertexData.pData = reinterpret_cast<UINT8 *>(vert.data());
		//vertexData.RowPitch = vert.size();
		//vertexData.SlicePitch = vertexData.RowPitch;
		//UpdateSubresources<1>(DirectX12CmdList::Instance()->cmdList.Get(), buffer->GetBufferData(handle).Get(), vertexBufferUpload.Get(), 0, 0, 1, &vertexData);

		vertexBufferView = KazBufferHelper::SetVertexBufferView(buffer->GetGpuAddress(handle), size, sizeof(vert[0]));
	}
	//頂点バッファ関連-------------------------




	//コマンドバッファ生成-------------------------
	{
		std::array<IndirectCommand, TRIANGLE_RESOURCE_COUNT> commands;
		const UINT commandBufferSize = CommandSizePerFrame * FRAME_COUNT;
		commandBufferHandle = buffer->CreateBuffer(KazBufferHelper::SetCommandBufferData(commandBufferSize));
		short uplod = buffer->CreateBuffer(KazBufferHelper::SetStructureBuffer(commandBufferSize));

		D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = buffer->GetGpuAddress(paramCBHandle);//定数バッファの生成がいる
		UINT commandIndex = 0;
		for (UINT frame = 0; frame < FRAME_COUNT; frame++)
		{
			for (UINT n = 0; n < TRIANGLE_ARRAY_NUM; n++)
			{
				commands[commandIndex].cbv = gpuAddress;
				commands[commandIndex].drawArguments.VertexCountPerInstance = 3;
				commands[commandIndex].drawArguments.InstanceCount = 1;
				commands[commandIndex].drawArguments.StartVertexLocation = 0;
				commands[commandIndex].drawArguments.StartInstanceLocation = 0;

				commandIndex++;
				gpuAddress += sizeof(SceneConstantBuffer);
			}
		}
		//コマンドバッファ生成-------------------------

		D3D12_SUBRESOURCE_DATA commandData = {};
		commandData.pData = reinterpret_cast<UINT8 *>(&commands[0]);
		commandData.RowPitch = commandBufferSize;
		commandData.SlicePitch = commandData.RowPitch;
		UpdateSubresources<1>(DirectX12CmdList::Instance()->cmdList.Get(), buffer->GetBufferData(commandBufferHandle).Get(), buffer->GetBufferData(uplod).Get(), 0, 0, 1, &commandData);


		size = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);


		//コマンドバッファのSRV用意-------------------------
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.NumElements = TRIANGLE_ARRAY_NUM;
		srvDesc.Buffer.StructureByteStride = sizeof(IndirectCommand);
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

		for (int i = 0; i < FRAME_COUNT; ++i)
		{
			srvDesc.Buffer.FirstElement = i * TRIANGLE_ARRAY_NUM;

			//定数バッファのビューは作る
			BufferMemorySize s = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_SRV);
			DescriptorHeapMgr::Instance()->CreateBufferView(s.startSize + i, srvDesc, buffer->GetBufferData(commandBufferHandle).Get());
		}
		//コマンドバッファのSRV用意-------------------------


		//入力用バッファの生成-------------------------
		KazBufferHelper::BufferResourceData bufferSet
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(commandBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			"InputBuffer"
		);
		//入力用のバッファ作成
		inputHandle = buffer->CreateBuffer(bufferSet);
		//入力用バッファの生成-------------------------


		//データのコピー-------------------------
		buffer->TransData(inputHandle, constantBufferData.data(), commandBufferSize);
		//データのコピー-------------------------




		//出力用のバッファの生成---------------------------
		outPutHandle = buffer->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(commandBufferSize));

		const UINT alignment = D3D12_UAV_COUNTER_PLACEMENT_ALIGNMENT;
		const UINT64 bufferSize = (CommandSizePerFrame + (alignment - 1)) & ~(alignment - 1);

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = TRIANGLE_ARRAY_NUM;
		uavDesc.Buffer.StructureByteStride = sizeof(IndirectCommand);
		uavDesc.Buffer.CounterOffsetInBytes = bufferSize;
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

		DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize + 1, uavDesc, buffer->GetBufferData(outPutHandle).Get(), buffer->GetBufferData(outPutHandle).Get());
		//出力用のバッファの生成---------------------------

		//リセット用のバッファ-------------------------

		//リセット用のバッファ-------------------------
	}






	////出力用のバッファ生成---------------------------
	//int num = 1000000;
	////共通用のバッファ
	//commonHandle = buffer->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonData)));

	////データを入力してみる
	////inputData = 1;

	////二つのバッファをデスクリプタヒープに登録
	//size = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);

	////仮設定
	//D3D12_SHADER_RESOURCE_VIEW_DESC inputDesc = {};
	//inputDesc.Format = DXGI_FORMAT_UNKNOWN;
	//inputDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	//inputDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//inputDesc.Buffer.FirstElement = 0;
	//inputDesc.Buffer.NumElements = static_cast<UINT>(1);
	//inputDesc.Buffer.StructureByteStride = sizeof(InputData) * num;
	//inputDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;


	////仮設定
	//D3D12_UNORDERED_ACCESS_VIEW_DESC outPutDesc = {};
	//outPutDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	//outPutDesc.Format = DXGI_FORMAT_UNKNOWN;
	//outPutDesc.Buffer.NumElements = static_cast<UINT>(1);
	//outPutDesc.Buffer.StructureByteStride = sizeof(OutPutData) * num;
	//outPutDesc.Buffer.CounterOffsetInBytes = 0;



	//D3D12_CONSTANT_BUFFER_VIEW_DESC commonDesc;
	//commonDesc.BufferLocation = buffer->GetBufferData(commonHandle)->GetGPUVirtualAddress();
	//commonDesc.SizeInBytes = (UINT)buffer->GetBufferData(commonHandle)->GetDesc().Width;


	//DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize, inputDesc, buffer->GetBufferData(inputHandle).Get());
	//DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize + 1, outPutDesc, buffer->GetBufferData(outPutHandle).Get());
	////後で第三引数を消そう
	//DescriptorHeapMgr::Instance()->CreateBufferView(size.startSize + 2, commonDesc, buffer->GetBufferData(commonHandle).Get());
	////入力用と出力用のバッファ生成---------------------------


	rootConst.xOffset = 0.05f;
	rootConst.zOffset = 1.0f;
	rootConst.cullOffset = 0.5f;
	rootConst.commandCount = TRIANGLE_ARRAY_NUM;
}

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });


	for (UINT n = 0; n < TRIANGLE_ARRAY_NUM; n++)
	{
		const float offsetBounds = 2.5f;

		// Animate the triangles.
		constantBufferData[n].offset.x += constantBufferData[n].velocity.x;
		if (constantBufferData[n].offset.x > offsetBounds)
		{
			constantBufferData[n].velocity.x = KazMath::FloatRand(0.01f, 0.02f);
			constantBufferData[n].offset.x = -offsetBounds;
		}
		XMStoreFloat4x4(&constantBufferData[n].projection, CameraMgr::Instance()->orthographicMatProjection);
	}
	buffer->TransData(inputHandle, &constantBufferData, TRIANGLE_ARRAY_NUM * sizeof(SceneConstantBuffer));





	//コンピュートシェーダーの計算-------------------------
	//コンピュート用のパイプライン設定
	//GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_TEST);

	//入力用のデータ転送
	//buffer->TransData(inputHandle, &inputData, sizeof(InputData));

	//入力用のバッファ設定
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(cbvSize.startSize));
	//出力用のバッファ設定
	//DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(size.startSize + 1));

	//ルート定数
	//DirectX12CmdList::Instance()->cmdList->SetComputeRoot32BitConstants(1, 4, reinterpret_cast<void *>(&rootConst), 0);

	//ディスパッチ
	//DirectX12CmdList::Instance()->cmdList->Dispatch(static_cast<UINT>(ceil(TRIANGLE_ARRAY_NUM / static_cast<float>(ComputeThreadBlockSize))), 1, 1);
	//コンピュートシェーダーの計算-------------------------

}

void DebugScene::Draw()
{

	//描画命令発行-------------------------
	int num = RenderTargetStatus::Instance()->copySwapchain->GetCurrentBackBufferIndex();
	RenderTargetStatus::Instance()->bbIndex = num;

	std::array<D3D12_RESOURCE_BARRIER, 2> barriers = {
	CD3DX12_RESOURCE_BARRIER::Transition(
		buffer->GetBufferData(commandBufferHandle).Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT),
	CD3DX12_RESOURCE_BARRIER::Transition(
		RenderTargetStatus::Instance()->backBuffers[num].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET)
	};
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(barriers.size(), barriers.data());



	//セット-------------------------
	//レンダータゲットの設定
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	rtvH = RenderTargetStatus::Instance()->rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += num * DirectX12Device::Instance()->dev->GetDescriptorHandleIncrementSize(RenderTargetStatus::Instance()->heapDesc.Type);
	DirectX12CmdList::Instance()->cmdList->OMSetRenderTargets(1, &rtvH, false, &RenderTargetStatus::Instance()->gDepth.dsvH[RenderTargetStatus::Instance()->handle]);


	RenderTargetStatus::Instance()->gDepth.Clear(RenderTargetStatus::Instance()->handle);
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	//セット-------------------------


	GraphicsPipeLineMgr::Instance()->SetPipeLineAndRootSignature(PIPELINE_NAME_COLOR);

	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);

	//PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, L"Cull invisible triangles");
	DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	(
		commandSig.Get(),
		TRIANGLE_ARRAY_NUM,
		buffer->GetBufferData(commandBufferHandle).Get(),
		CommandSizePerFrame * num,//リソースバリアの切り替えで値を変える必要があるかも
		nullptr,
		0
	);
	//PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());

	barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
	barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(barriers.size(), barriers.data());
	//描画命令発行-------------------------
}

void DebugScene::Input()
{
	KeyBoradInputManager *input = KeyBoradInputManager::Instance();

#pragma region カメラ操作
	debugCameraMove = { 0,0,0 };
	float debugSpeed = 1;
	//�J�����ړ�
	if (input->InputState(DIK_D))
	{
		debugCameraMove.x = -debugSpeed;
	}
	if (input->InputState(DIK_A))
	{
		debugCameraMove.x = debugSpeed;
	}
	if (input->InputState(DIK_W))
	{
		//y�Ȃ̂ɑO�ɐi��....
		debugCameraMove.y = debugSpeed;
	}
	if (input->InputState(DIK_S))
	{
		debugCameraMove.y = -debugSpeed;
	}


	//�J�����p�x
	if (input->InputState(DIK_RIGHTARROW))
	{
		angle.x += debugSpeed;
	}
	if (input->InputState(DIK_LEFTARROW))
	{
		angle.x += -debugSpeed;
	}

	if (input->InputState(DIK_UPARROW))
	{
		angle.y += debugSpeed;
	}
	if (input->InputState(DIK_DOWNARROW))
	{
		angle.y += -debugSpeed;
	}
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);

#pragma endregion

}

//�R���g���[���[�Ή�
//void DebugScene::Input(ControllerInputManager *INPUT)
//{
//#pragma region �f�o�b�N
//	//�f�o�b�N���[�h
//	if (INPUT->InputTrigger(XINPUT_GAMEPAD_B))
//	{
//		debugFlag = !debugFlag;
//	}
//
//	if (debugFlag)
//	{
//		debugCameraMove = { 0,0,0 };
//		float debugSpeed = 1;
//		//�J�����ړ�
//		if (INPUT->InputStickState(LEFT_STICK, RIGHT_SIDE))
//		{
//			debugCameraMove.x = -debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, LEFT_SIDE))
//		{
//			debugCameraMove.x = debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, UP_SIDE))
//		{
//			debugCameraMove.y = debugSpeed;
//		}
//		if (INPUT->InputStickState(LEFT_STICK, DOWN_SIDE))
//		{
//			debugCameraMove.y = -debugSpeed;
//		}
//
//
//
//		//�J�����p�x
//		if (INPUT->InputStickState(RIGHT_STICK, RIGHT_SIDE))
//		{
//			angle.x += debugSpeed;
//		}
//		if (INPUT->InputStickState(RIGHT_STICK, LEFT_SIDE))
//		{
//			angle.x += -debugSpeed;
//		}
//
//		if (INPUT->InputStickState(RIGHT_STICK, UP_SIDE))
//		{
//			angle.y += debugSpeed;
//		}
//		if (INPUT->InputStickState(RIGHT_STICK, DOWN_SIDE))
//		{
//			angle.y += -debugSpeed;
//		}
//
//
//	}
//#pragma endregion
//
//	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
//	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
//
//}

int DebugScene::SceneChange()
{
	return -1;
}
