#include "GalacticParticle.h"
#include"../KazLibrary/DirectXCommon/DirectX12Device.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Pipeline/GraphicsRootSignature.h"
#include"../KazLibrary/Pipeline/GraphicsPipeLineMgr.h"
#include"../KazLibrary/Helper/KazRenderHelper.h"
#include"../KazLibrary/RenderTarget/RenderTargetStatus.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Buffer/UavViewHandleMgr.h"

GalacticParticle::GalacticParticle()
{
	buffers = std::make_unique<CreateGpuBuffer>();


	//�R�}���h�V�O�l�`��---------------------------
	std::array<D3D12_INDIRECT_ARGUMENT_DESC, 2> args{};
	args[0].Type = D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	args[0].UnorderedAccessView.RootParameterIndex = 0;
	args[1].Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

	D3D12_COMMAND_SIGNATURE_DESC desc{};
	desc.pArgumentDescs = args.data();
	desc.NumArgumentDescs = static_cast<UINT>(args.size());
	desc.ByteStride = sizeof(IndirectCommand);

	HRESULT lR =
		DirectX12Device::Instance()->dev->CreateCommandSignature(&desc, GraphicsRootSignature::Instance()->GetRootSignature(ROOTSIGNATURE_DATA_DRAW_UAV).Get(), IID_PPV_ARGS(&commandSig));
	//�R�}���h�V�O�l�`��---------------------------
	if (lR != S_OK)
	{
		assert(0);
	}

	std::array<Vertex, 4>vertices;
	std::array<USHORT, 6> indices;
	indices = KazRenderHelper::InitIndciesForPlanePolygon();
	KazRenderHelper::InitVerticesPos(&vertices[0].pos, &vertices[1].pos, &vertices[2].pos, &vertices[3].pos, { 0.5f,0.5f });
	KazRenderHelper::InitUvPos(&vertices[0].uv, &vertices[1].uv, &vertices[2].uv, &vertices[3].uv);

	BUFFER_SIZE vertBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(vertices.size(), sizeof(Vertex));
	BUFFER_SIZE indexBuffSize = KazBufferHelper::GetBufferSize<BUFFER_SIZE>(indices.size(), sizeof(unsigned int));

	//�o�b�t�@����-------------------------
	vertexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetVertexBufferData(vertBuffSize));
	indexBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetIndexBufferData(indexBuffSize));
	commonBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetConstBufferData(sizeof(CommonMoveData)));
	outputInitBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(OutputInitData) * PARTICLE_MAX_NUM));
	outputBufferHandle = buffers->CreateBuffer(KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(OutputData) * PARTICLE_MAX_NUM));
	drawCommandHandle = buffers->CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(IndirectCommand) * DRAW_CALL));
	//�o�b�t�@����-------------------------

	//�]��-------------------------
	buffers->TransData(vertexBufferHandle, vertices.data(), vertBuffSize);
	buffers->TransData(indexBufferHandle, indices.data(), indexBuffSize);

	IndirectCommand command;
	command.drawArguments.IndexCountPerInstance = static_cast<UINT>(indices.size());
	command.drawArguments.InstanceCount = PARTICLE_MAX_NUM;
	command.drawArguments.StartIndexLocation = 0;
	command.drawArguments.StartInstanceLocation = 0;
	command.drawArguments.BaseVertexLocation = 0;

	command.uav = buffers->GetGpuAddress(outputBufferHandle);
	buffers->TransData(drawCommandHandle, &command, sizeof(IndirectCommand) * DRAW_CALL);
	//�]��-------------------------

	computeMemSize = DescriptorHeapMgr::Instance()->GetSize(DESCRIPTORHEAP_MEMORY_TEXTURE_COMPUTEBUFFER);
	outputInitViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputInitViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputInitData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputInitBufferHandle).Get(),
		nullptr
	);


	outputViewHandle = UavViewHandleMgr::Instance()->GetHandle();
	DescriptorHeapMgr::Instance()->CreateBufferView(
		outputViewHandle,
		KazBufferHelper::SetUnorderedAccessView(sizeof(OutputData), PARTICLE_MAX_NUM),
		buffers->GetBufferData(outputBufferHandle).Get(),
		nullptr
	);


	vertexBufferView = KazBufferHelper::SetVertexBufferView(buffers->GetGpuAddress(vertexBufferHandle), vertBuffSize, sizeof(vertices[0]));
	indexBufferView = KazBufferHelper::SetIndexBufferView(buffers->GetGpuAddress(indexBufferHandle), indexBuffSize);


	//����������--------------------------------------------
	DescriptorHeapMgr::Instance()->SetDescriptorHeap();
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_FLOORPARTICLE);
	DirectX12CmdList::Instance()->cmdList->SetComputeRootUnorderedAccessView(0, buffers->GetBufferData(outputInitBufferHandle)->GetGPUVirtualAddress());
	DirectX12CmdList::Instance()->cmdList->Dispatch(PARTICLE_MAX_NUM / 1000, 1, 1);
	//����������--------------------------------------------



	box = std::make_unique<BoxPolygonRender>(true, BOX_MAX_NUM);
	box->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_MULTITEX;

	RESOURCE_HANDLE lHandle = box->CreateConstBuffer(sizeof(DirectX::XMFLOAT4), typeid(DirectX::XMFLOAT4).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA2);
	DirectX::XMFLOAT4 lColor = { 1.0f,0.0f,0.0f,1.0f };
	box->TransData(&lColor, lHandle, typeid(DirectX::XMFLOAT4).name());

	instanceBufferHandle = box->CreateConstBuffer(sizeof(MatData) * BOX_MAX_NUM, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);


	//�󒆂ɎU��΂��`�̏���������
	for (int i = 0; i < boxDataArray.size(); ++i)
	{
		boxDataArray[i].transform.pos =
		{
			0.0f,
			KazMath::Rand(5000.0f,-5000.0f),
			KazMath::Rand(10000.0f,-100.0f)
		};

		if (1000.0f <= abs(boxDataArray[i].transform.pos.y))
		{
			if (3 <= KazMath::Rand(6, 0))
			{
				boxDataArray[i].transform.pos.x = -KazMath::Rand(700.0f, 10000.0f);
			}
			else
			{
				boxDataArray[i].transform.pos.x = KazMath::Rand(700.0f, 10000.0f);
			}
		}
		else
		{
			boxDataArray[i].transform.pos.x = KazMath::Rand(5000.0f, -5000.0f);
		}

		boxDataArray[i].transform.scale = { KazMath::Rand(50.0f,10.0f),KazMath::Rand(50.0f,10.0f),KazMath::Rand(50.0f,10.0f) };
		boxDataArray[i].rotaVel = KazMath::Vec3<float>(KazMath::Rand(5.0f, 1.0f), KazMath::Rand(5.0f, 1.0f), KazMath::Rand(5.0f, 1.0f));
		boxDataArray[i].color = { KazMath::Rand(255,155),KazMath::Rand(255,155),KazMath::Rand(255,155),255 };
	}

}

GalacticParticle::~GalacticParticle()
{
}

void GalacticParticle::Update()
{
	GraphicsPipeLineMgr::Instance()->SetComputePipeLineAndRootSignature(PIPELINE_COMPUTE_NAME_FLOORPARTICLE_MOVE);

	//���ʗp�o�b�t�@�̃f�[�^���M
	{
		constBufferData.cameraMat = CameraMgr::Instance()->GetViewMatrix();
		constBufferData.projectionMat = CameraMgr::Instance()->GetPerspectiveMatProjection();
		constBufferData.billboardMat = CameraMgr::Instance()->GetMatBillBoard();

		buffers->TransData(commonBufferHandle, &constBufferData, sizeof(CommonMoveData));
		DirectX12CmdList::Instance()->cmdList->SetComputeRootConstantBufferView(2, buffers->GetGpuAddress(commonBufferHandle));
	}

	{
		//�������l
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(0, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputInitViewHandle));
		//���ʒl
		DirectX12CmdList::Instance()->cmdList->SetComputeRootDescriptorTable(1, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(outputViewHandle));
	}

	DirectX12CmdList::Instance()->cmdList->Dispatch((PARTICLE_MAX_NUM) / 1000, 1, 1);


	std::array<MatData, BOX_MAX_NUM> lData;
	for (int i = 0; i < boxDataArray.size(); ++i)
	{
		boxDataArray[i].transform.pos += -KazMath::Vec3<float>(0.0f, 0.0f, 5.0f);
		if (boxDataArray[i].transform.pos.z <= -100.0f)
		{
			boxDataArray[i].transform.pos.z = 10000.0f;
		}
		boxDataArray[i].transform.rotation += boxDataArray[i].rotaVel;

		lData[i].mat = KazMath::CaluMat(boxDataArray[i].transform, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection(), { 0,1,0 }, { 0,0,1 });
		lData[i].color = boxDataArray[i].color.ConvertColorRateToXMFLOAT4();
	}


	box->TransData
	(
		lData.data(),
		instanceBufferHandle,
		typeid(MatData).name()
	);
}

void GalacticParticle::Draw()
{
	GraphicsPipeLineMgr::Instance()->SetPipeLineAndRootSignature(PIPELINE_NAME_GPUPARTICLE);
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&indexBufferView);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
	);

	DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	(
		commandSig.Get(),
		1,
		buffers->GetBufferData(drawCommandHandle).Get(),
		0,
		nullptr,
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		buffers->GetBufferData(drawCommandHandle).Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);


	box->Draw();
}