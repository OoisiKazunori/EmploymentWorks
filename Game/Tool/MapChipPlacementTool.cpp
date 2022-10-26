#include "MapChipPlacementTool.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Helper/KazHelper.h"
#include"../KazLibrary/DirectXCommon/Base.h"

MapChipPlacementTool::MapChipPlacementTool() :BLOCK_SIZE(10.0f), objectName("BlockIndex3"), filePass(KazFilePathName::StageParamPath + "blockPosData.json")
{
	instanceFlameRender = std::make_unique<BoxPolygonRender>(true, matSize);
	instanceFlameRender->data.pipelineName = PIPELINE_NAME_INSTANCE_COLOR_WIREFLAME;
	instanceHandle = instanceFlameRender->CreateConstBuffer(sizeof(MatData) * matSize, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA);

	matIndex = 0;
	for (int y = 0; y < defaultBlockPosArray.size(); ++y)
	{
		for (int x = 0; x < defaultBlockPosArray[y].size(); ++x)
		{
			defaultBlockPosArray[y][x] = KazMath::Transform3D(KazMath::Vec3<float>(static_cast<float>(y) * 2.0f, 0.0f, static_cast<float>(x) * 2.0f));

			lMatData[matIndex].mat = KazMath::CaluMat(
				defaultBlockPosArray[y][x],
				CameraMgr::Instance()->GetViewMatrix(),
				CameraMgr::Instance()->GetPerspectiveMatProjection(),
				{ 0,1,0 },
				{ 0,0,1 }
			);
			++matIndex;
		}
	}

	cursorR.data.handleData = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::TestPath + "tex.png");


	//配置したブロックがどれか分かる用の描画準備--------------------------------------------
	instanceObjRender = std::make_unique<ObjModelRender>(true, BLOCK_MAX_NUM, true);
	instanceObjRender->data.pipelineName = PIPELINE_NAME_INSTANCE_OBJ;
	instanceObjRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "testPlacementBlock.obj");
	{
		RESOURCE_HANDLE lHandle = instanceObjRender->CreateConstBuffer(sizeof(ConstBufferDataB1), typeid(ConstBufferDataB1).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA);
		ConstBufferDataB1 lData;
		lData.alpha = 1.0f;
		lData.ambient = { 0.0f,0.0f,0.0f };
		lData.diffuse = { 0.0f,0.0f,0.0f };
		instanceObjRender->TransData(&lData, lHandle, typeid(ConstBufferDataB1).name());
	}
	instanceBoxHandle = instanceObjRender->CreateConstBuffer(sizeof(MatData) * BLOCK_MAX_NUM, typeid(MatData).name(), GRAPHICS_RANGE_TYPE_UAV, GRAPHICS_PRAMTYPE_DATA2);
	//配置したブロックがどれか分かる用の描画準備--------------------------------------------


	for (int x = 0; x < blockPosArray.size(); ++x)
	{
		for (int y = 0; y < blockPosArray[x].size(); ++y)
		{
			for (int z = 0; z < blockPosArray[x][y].size(); ++z)
			{
				blockPosArray[x][y][z].x = REV_VALUE;
			}
		}
	}

	blockPosArray[0][0][0] = { 0.0f,0.0f,0.0f };

	blockFileMgr.LoadFile(filePass);

	//rapidjson::Document *lDoc = &blockFileMgr.doc;
	//std::vector<std::vector<char>> lNameArray;
	////パラメータから判断
	//for (int blockNum = 0; blockNum < BLOCK_MAX_NUM; ++blockNum)
	//{
	//	rapidjson::Value lChild(rapidjson::kObjectType);
	//	lChild.AddMember("X", 0, lDoc->GetAllocator());
	//	lChild.AddMember("Y", 0, lDoc->GetAllocator());
	//	lChild.AddMember("Z", 0, lDoc->GetAllocator());

	//	lNameArray.push_back(KazHelper::CovertStringToChar(objectName + "_" + std::to_string(blockNum)));
	//	blockFileMgr.doc.AddMember(rapidjson::GenericStringRef<char>(lNameArray[lNameArray.size() - 1].data()), lChild, lDoc->GetAllocator());
	//}
	//blockFileMgr.ExportFile(filePass);

}

void MapChipPlacementTool::Input(bool PUT_FLAG, bool RELEASE_FLAG, const DirectX::XMFLOAT2 &MOUSE_POS)
{
	//ブロックの設置
	inputFlag = PUT_FLAG;
	//ブロックの削除
	releaseFlag = RELEASE_FLAG;

	KazMath::Vec3<float> nearPos = { MOUSE_POS.x,MOUSE_POS.y,0.0f };
	KazMath::Vec3<float> farPos = { MOUSE_POS.x,MOUSE_POS.y,1.0f };
	KazMath::Vec3<float> endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	ray.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	KazMath::Vec3<float> dir(endPos - ray.start);
	dir.Normalize();
	ray.dir = dir;

	cursorR.data.transform.pos = { MOUSE_POS.x,MOUSE_POS.y };
}

void MapChipPlacementTool::Init()
{
	isLoadFlag = false;
}

void MapChipPlacementTool::Update()
{
	if (inputFlag)
	{
		int u = 0;
		u = 0;
	}
	bool maxBlockFlag = false;

	int lMaxCountNum = 0;
	{
		int lBlockNum = 0;
		putIndex = { 0,0,0 };
		selectingIndex = { 0,0,0 };
		//ブロックとレイとの判定
		for (int x = 0; x < blockPosArray.size(); ++x)
		{
			for (int y = 0; y < blockPosArray[x].size(); ++y)
			{
				for (int z = 0; z < blockPosArray[x][y].size(); ++z)
				{
					if (BLOCK_MAX_NUM <= lMaxCountNum)
					{
						maxBlockFlag = true;
						break;
					}
					if (blockPosArray[x][y][z].x == REV_VALUE)
					{
						continue;
					}

					KazMath::Vec3<float>lYVec(0, 1, 0);
					KazMath::Vec3<float>lZVec(0, 0, 1);
					blockMatData[lBlockNum].mat = KazMath::CaluWorld(KazMath::Transform3D(blockPosArray[x][y][z], KazMath::Vec3<float>(5.0f, 5.0f, 5.0f), {}), lYVec, lZVec);
					++lBlockNum;
					if (blockPosArray[x][y][z].x != CONTINE_VALUE)
					{
						++lMaxCountNum;
					}


					MESH_DIR lDir;
					if ((lDir = CheckBlock(blockPosArray[x][y][z])) != NONE)
					{
						selectR.data.transform.pos = blockPosArray[x][y][z];
						selectingIndex = KazMath::Vec3<int>(x, y, z);
						putIndex = selectingIndex + AdjIndex(lDir);
					}
				}
			}
		}

		bool debug = false;
		debug = true;
	}

	ImGui::Begin("MapChipPlacementTool");
	//現在指定しているブロックの座標
	ImGui::Text("selectingIndex,X:%d,Y:%d,Z:%d", selectingIndex.x, selectingIndex.y, selectingIndex.z);
	ImGui::Text("PutPos,X:%d,Y:%d,Z:%d", putIndex.x, putIndex.y, putIndex.z);
	if (maxBlockFlag)
	{
		ImGui::Text("Max!!");
	}
	else
	{
		ImGui::Text("Block:%d/%d", lMaxCountNum, BLOCK_MAX_NUM);
	}
	//一括配置
	KazImGuiHelper::InputVec3("StartPutBlockInOnce", &startBlockPutInOnceSize);
	KazImGuiHelper::InputVec3("EndPutBlockInOnce", &endBlockPutInOnceSize);
	bool lPutFlag = ImGui::Button("Put");
	ImGui::SameLine();
	bool lDeleteFlag = ImGui::Button("Delete");
	bool lExportFlag = ImGui::Button("Export");
	ImGui::SameLine();
	bool lImportFlag = ImGui::Button("Import");
	ImGui::End();

	if (lExportFlag)
	{
		int lBlockNum = 0;
		for (int x = 0; x < blockPosArray.size(); ++x)
		{
			for (int y = 0; y < blockPosArray[x].size(); ++y)
			{
				for (int z = 0; z < blockPosArray[x][y].size(); ++z)
				{
					if (blockPosArray[x][y][z].x == REV_VALUE)
					{
						continue;
					}
					std::string name = objectName + "_" + std::to_string(lBlockNum);

					KazMath::Vec3<int> lNum(GetBlockIndex(KazMath::Vec3<int>(x, y, z)));

					blockFileMgr.doc[name.c_str()]["X"].SetInt(lNum.x);
					blockFileMgr.doc[name.c_str()]["Y"].SetInt(lNum.y);
					blockFileMgr.doc[name.c_str()]["Z"].SetInt(lNum.z);
					++lBlockNum;
				}
			}
		}

		isLoadFlag = true;
		blockFileMgr.ExportFile(filePass);
	}
	if (lImportFlag)
	{
		for (int i = 0; i < BLOCK_MAX_NUM; ++i)
		{
			std::string name = objectName + "_" + std::to_string(i);
			KazMath::Vec3<int> lNum(
				blockFileMgr.doc[name.c_str()]["X"].GetInt(),
				blockFileMgr.doc[name.c_str()]["Y"].GetInt(),
				blockFileMgr.doc[name.c_str()]["Z"].GetInt()
			);
			blockPosArray[lNum.x][lNum.y][lNum.z] = { lNum.x * (BLOCK_SIZE * 2.0f),lNum.y * (BLOCK_SIZE * 2.0f),lNum.z * (BLOCK_SIZE * 2.0f) };
		}
	}


	//一括配置の処理
	if (lPutFlag)
	{
		for (int x = startBlockPutInOnceSize.x; x < endBlockPutInOnceSize.x; ++x)
		{
			for (int y = startBlockPutInOnceSize.y; y < endBlockPutInOnceSize.y; ++y)
			{
				for (int z = startBlockPutInOnceSize.z; z < endBlockPutInOnceSize.z; ++z)
				{
					blockPosArray[x][y][z] = { x * (BLOCK_SIZE * 2.0f),y * (BLOCK_SIZE * 2.0f),z * (BLOCK_SIZE * 2.0f) };
				}
			}
		}
	}
	if (lDeleteFlag)
	{
		for (int x = startBlockPutInOnceSize.x; x < endBlockPutInOnceSize.x; ++x)
		{
			for (int y = startBlockPutInOnceSize.y; y < endBlockPutInOnceSize.y; ++y)
			{
				for (int z = startBlockPutInOnceSize.z; z < endBlockPutInOnceSize.z; ++z)
				{
					blockPosArray[x][y][z] = { CONTINE_VALUE,0.0f,0.0f };
				}
			}
		}
	}

	if (putIndex.x <= -1)
	{
		putIndex.x = 0;
	}
	if (putIndex.y <= -1)
	{
		putIndex.y = 0;
	}
	if (putIndex.z <= -1)
	{
		putIndex.z = 0;
	}
	if (selectingIndex.x <= -1)
	{
		selectingIndex.x = 0;
	}
	if (selectingIndex.y <= -1)
	{
		selectingIndex.y = 0;
	}
	if (selectingIndex.z <= -1)
	{
		selectingIndex.z = 0;
	}



	//配置
	if (inputFlag)
	{
		blockPosArray[putIndex.x][putIndex.y][putIndex.z] = putIndex.Float() * (BLOCK_SIZE * 2.0f);
	}

	//削除,別の所に置く
	if (releaseFlag)
	{
		blockPosArray[selectingIndex.x][selectingIndex.y][selectingIndex.z] = { CONTINE_VALUE,0.0f,0.0f };
	}

	matIndex = 0;
	//for (int y = 0; y < defaultBlockPosArray.size(); ++y)
	//{
	//	for (int x = 0; x < defaultBlockPosArray[y].size(); ++x)
	//	{
	//		lMatData[matIndex].mat = KazMath::CaluMat(
	//			defaultBlockPosArray[y][x],
	//			CameraMgr::Instance()->GetViewMatrix(),
	//			CameraMgr::Instance()->GetPerspectiveMatProjection(),
	//			{ 0,1,0 },
	//			{ 0,0,1 }
	//		);
	//		++matIndex;
	//	}
	//}
	//instanceFlameRender->TransData(lMatData.data(), instanceHandle, typeid(MatData).name());

	for (int i = 0; i < blockMatData.size(); ++i)
	{
		blockMatData[i].mat *= CameraMgr::Instance()->GetViewMatrix();
		blockMatData[i].mat *= CameraMgr::Instance()->GetPerspectiveMatProjection();
	}

	instanceObjRender->TransData(blockMatData.data(), instanceBoxHandle, typeid(MatData).name());

}

void MapChipPlacementTool::Draw()
{
	//instanceFlameRender->Draw();
	instanceObjRender->Draw();
	selectR.Draw();

	//cursorR.Draw();
	//b.Draw();
}

MapChipPlacementTool::MESH_DIR MapChipPlacementTool::CheckBlock(const KazMath::Vec3<float> &POS)
{
	KazMath::Vec3<float>lCentralPos = POS;

	struct HitData
	{
		HitData(MESH_DIR DIR, float DISTANCE) :dir(DIR), distance(DISTANCE)
		{};
		MESH_DIR dir;
		float distance;
	};

	std::vector<float>lDistanceArray;
	std::vector<HitData>lHitDataArray;

	std::array<ModiRectangle, 6>lPlane;
	for (int planeIndex = 0; planeIndex < lPlane.size(); ++planeIndex)
	{
		MESH_DIR lDir = static_cast<MESH_DIR>(planeIndex);

		const float HALF_SCALE = BLOCK_SIZE;
		//どの方向に向くかで板の位置と向きを決める
		lCentralPos += AdjIndex(lDir).Float() * HALF_SCALE;

		//左上、左下、右上、右下の順で並べる
		switch (lDir)
		{
		case MapChipPlacementTool::NONE:
			break;
		case MapChipPlacementTool::TOP:
		case MapChipPlacementTool::BUTTOM:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, 0.0f, HALF_SCALE);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, 0.0f, -HALF_SCALE);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, 0.0f, HALF_SCALE);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, 0.0f, -HALF_SCALE);
			break;
		case MapChipPlacementTool::FRONT:
		case MapChipPlacementTool::BACK:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, HALF_SCALE, 0.0f);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(-HALF_SCALE, -HALF_SCALE, 0.0f);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, HALF_SCALE, 0.0f);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(HALF_SCALE, -HALF_SCALE, 0.0f);
			break;
		case MapChipPlacementTool::LEFT:
		case MapChipPlacementTool::RIGHT:
			lPlane[planeIndex].p0 = lCentralPos + KazMath::Vec3<float>(0.0f, HALF_SCALE, HALF_SCALE);
			lPlane[planeIndex].p1 = lCentralPos + KazMath::Vec3<float>(0.0f, -HALF_SCALE, HALF_SCALE);
			lPlane[planeIndex].p2 = lCentralPos + KazMath::Vec3<float>(0.0f, HALF_SCALE, -HALF_SCALE);
			lPlane[planeIndex].p3 = lCentralPos + KazMath::Vec3<float>(0.0f, -HALF_SCALE, -HALF_SCALE);
			break;
		default:
			break;
		}

		float lDistance;
		//判定が取れたら座標を入手できたことを示す
		if (CollisionManager::Instance()->CheckRayAndPlane3D(ray, lPlane[planeIndex], &lDistance))
		{
			lDistanceArray.push_back(lDistance);
			lHitDataArray.push_back(HitData(lDir, lDistance));
		}
	}
	//sort(lDistanceArray.begin(), lDistanceArray.end());

	std::vector<HitData>lResultArray;

	//ソートをかける
	for (int i = 0; i < lDistanceArray.size(); ++i)
	{
		for (int dataIndex = 0; dataIndex < lHitDataArray.size(); ++dataIndex)
		{
			if (lDistanceArray[i] == lHitDataArray[dataIndex].distance)
			{
				lResultArray.push_back(lHitDataArray[dataIndex]);
			}
		}
	}

	if (lResultArray.size() != 0)
	{
		return static_cast<MESH_DIR>(lResultArray[0].dir);
	}
	else
	{
		return static_cast<MESH_DIR>(-1);
	}
}