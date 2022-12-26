#include "InstanceMeshParticle.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"
#include"../KazLibrary/Render/GPUParticleRender.h"

InstanceMeshParticle::InstanceMeshParticle(std::vector<InitMeshParticleData> &INIT_DATA) :setCountNum(0)
{
	initData = INIT_DATA;

#pragma region 初期化用のバッファ生成

	commonAndColorBufferData = computeInitMeshParticle.CreateAndGetBuffer(
		COMMON_BUFFER_SIZE,
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA,
		1,
		false
	);

	for (int i = 0; i < commonBufferData.size(); ++i)
	{
		commonBufferData[i] = computeInitMeshParticle.CreateAndGetBuffer(
			sizeof(CommonData),
			GRAPHICS_RANGE_TYPE_CBV_VIEW,
			GRAPHICS_PRAMTYPE_DATA,
			1,
			false
		);
	}

	//メッシュパーティクルの初期化処理の出力情報
	meshParticleBufferData = computeInitMeshParticle.CreateAndGetBuffer(
		KazBufferHelper::SetOnlyReadStructuredBuffer(sizeof(InitOutputData) * PARTICLE_MAX_NUM),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA,
		sizeof(InitOutputData),
		PARTICLE_MAX_NUM,
		true);

	for (int i = 0; i < initData.size(); ++i)
	{
		motherMatArray.push_back(initData[i].motherMat);
	}

	UINT lNum = 0;
	KazRenderHelper::ID3D12ResourceWrapper lBuffer;

	lBuffer.CreateBuffer(KazBufferHelper::SetRWStructuredBuffer(sizeof(UINT)));
	lBuffer.TransData(&lNum, sizeof(UINT));

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(meshParticleBufferData.counterWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(lBuffer.buffer.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
			D3D12_RESOURCE_STATE_COPY_SOURCE
		)
	);

	DirectX12CmdList::Instance()->cmdList->CopyResource(meshParticleBufferData.counterWrapper.buffer.Get(), lBuffer.buffer.Get());

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(meshParticleBufferData.counterWrapper.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(lBuffer.buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_SOURCE,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS
		)
	);


	//何の情報を読み込むかでパイプラインの種類を変える
	for (int i = 0; i < initData.size(); ++i)
	{
		computeInitMeshParticle.DeleteAllData();
		setCountNum = 0;

		IsSetBuffer(initData[i].vertData);
		IsSetBuffer(initData[i].uvData);


		ComputePipeLineNames lPipelineName = PIPELINE_COMPUTE_NAME_NONE;
		RESOURCE_HANDLE lCommonHandle;
		CommonWithColorData lCommonAndColorData;
		CommonData lCommonData;
		switch (setCountNum)
		{
		case 0:
			//メッシュパーティクルに必要な情報が何も入ってない
			assert(0);
			break;
		case 1:
			lCommonHandle = computeInitMeshParticle.SetBuffer(commonAndColorBufferData, GRAPHICS_PRAMTYPE_DATA3);

			lCommonAndColorData.meshData = initData[i].triagnleData;
			lCommonAndColorData.color = initData[i].color.ConvertXMFLOAT4();
			lCommonAndColorData.id = static_cast<UINT>(i);
			computeInitMeshParticle.TransData(lCommonHandle, &lCommonAndColorData, sizeof(CommonWithColorData));

			lPipelineName = PIPELINE_COMPUTE_NAME_INIT_POS_MESHPARTICLE;
			break;
		case 2:
			lCommonHandle = computeInitMeshParticle.SetBuffer(commonBufferData[i], GRAPHICS_PRAMTYPE_DATA4);

			lCommonData.meshData = initData[i].triagnleData;
			lCommonData.id = static_cast<UINT>(i);
			computeInitMeshParticle.TransData(lCommonHandle, &lCommonData, sizeof(CommonData));

			lPipelineName = PIPELINE_COMPUTE_NAME_INIT_POSUV_MESHPARTICLE;
			break;
		default:
			break;
		}

		meshParticleOutputHandle = computeInitMeshParticle.SetBuffer(meshParticleBufferData, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + setCountNum));

		//テクスチャのセット
		if (initData[i].textureHandle != -1)
		{
			ComputeBufferHelper::BufferData lData;
			lData.viewHandle = initData[i].textureHandle;
			lData.rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
			computeInitMeshParticle.SetBuffer(lData, GRAPHICS_PRAMTYPE_TEX);
		}


		computeInitMeshParticle.StackToCommandListAndCallDispatch(lPipelineName, { 1000,1,1 });
	}
#pragma endregion


	//computeConvert.SetBuffer(initData[0].vertData, GRAPHICS_PRAMTYPE_DATA);
	//computeConvert.SetBuffer(meshParticleBufferData, GRAPHICS_PRAMTYPE_DATA2);
	//computeConvert.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_COVERT_INITMESH_TO_UPDATEMESH, { 3,1,1 });


	//パーティクルデータ
	computeUpdateMeshParticle.SetBuffer(meshParticleBufferData, GRAPHICS_PRAMTYPE_DATA);

	//親行列
	particleMotherMatrixHandle = computeUpdateMeshParticle.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_UAV_DESC,
		GRAPHICS_PRAMTYPE_DATA2,
		static_cast<UINT>(motherMatArray.size()),
		false
	);

	//ワールド行列
	computeUpdateMeshParticle.SetBuffer(GPUParticleRender::Instance()->GetStackWorldMatBuffer(), GRAPHICS_PRAMTYPE_DATA3);
	//色
	//computeUpdateMeshParticle.SetBuffer(GPUParticleRender::Instance()->GetStackColorBuffer(), GRAPHICS_PRAMTYPE_DATA4);
	//Transformを除いたワールド行列
	scaleRotateBillboardMatHandle = computeUpdateMeshParticle.CreateBuffer(
		sizeof(DirectX::XMMATRIX),
		GRAPHICS_RANGE_TYPE_CBV_VIEW,
		GRAPHICS_PRAMTYPE_DATA4,
		1,
		false);

	float lScale = 0.2f;
	scaleRotMat = KazMath::CaluScaleMatrix({ lScale,lScale,lScale }) * KazMath::CaluRotaMatrix({ 0.0f,0.0f,0.0f });
}

void InstanceMeshParticle::Compute()
{
	scaleRotBillBoardMat = scaleRotMat * CameraMgr::Instance()->GetMatBillBoard();
	computeUpdateMeshParticle.TransData(scaleRotateBillboardMatHandle, &scaleRotBillBoardMat, sizeof(DirectX::XMMATRIX));

	std::vector<DirectX::XMMATRIX>lMatArray(motherMatArray.size());
	for (int i = 0; i < lMatArray.size(); ++i)
	{
		lMatArray[i] = *motherMatArray[i];
	}
	computeUpdateMeshParticle.TransData(particleMotherMatrixHandle, lMatArray.data(), sizeof(DirectX::XMMATRIX) * static_cast<int>(lMatArray.size()));
	computeUpdateMeshParticle.StackToCommandListAndCallDispatch(PIPELINE_COMPUTE_NAME_UPDATE_MESHPARTICLE, { 1000,1,1 });
}