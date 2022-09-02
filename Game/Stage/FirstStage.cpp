#include "FirstStage.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary//Helper/KazHelper.h"

FirstStage::FirstStage()
{

	for (int i = 0; i < floorStage.size(); ++i)
	{
		floorStage[i].data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::StagePath + "road.obj");
		floorStage[i].data.transform.pos = { 0.0f,-23.0f,52.0f + static_cast<float>(i) * 200.0f };
		floorStage[i].data.transform.scale = { 1.2f,1.0f,1.0f };
		floorStage[i].data.pipelineName = PIPELINE_NAME_OBJ_FOG_GRADATION;
		RESOURCE_HANDLE lHandle = floorStage[i].CreateConstBuffer(sizeof(FogD), typeid(FogD).name(), GRAPHICS_RANGE_TYPE_CBV, GRAPHICS_PRAMTYPE_DATA2);


		FogD lD;
		lD.rateAndFogLine.z = 100.0f;
		lD.rateAndFogLine.w = 300.0f;

		lD.rateAndFogLine.x = -401.0f;
		lD.rateAndFogLine.y = 407.0f;

		lD.depthX.z = 0;
		if (i == 2 || i == 4)
		{
			lD.depthX.z = 1;
		}
		if (16 <= i && i <= 24)
		{
			lD.depthX.z = 1;
		}

		DirectX::XMFLOAT3 first(0.93f, 0.65f, 0.53f);
		DirectX::XMFLOAT3 end(0.24f, 0.09f, 0.62f);
		DirectX::XMFLOAT3 result;
		result.x = first.x - end.x;
		result.y = first.y - end.y;
		result.z = first.z - end.z;
		lD.endColor = end;
		lD.subValue = result;

		floorStage[i].TransData(&lD, lHandle, typeid(lD).name());
	}

	for (int i = 0; i < pillarObj.size(); ++i)
	{
		InitPillar(&pillarObj[i]);
		InitPillar(&farSidePillarObj[i]);
	}

	for (int y = 0; y < topPillarObj.size(); ++y)
	{
		for (int x = 0; x < topPillarObj[y].size(); ++x)
		{
			InitPillar(&topPillarObj[y][x]);
		}
	}

	InitPillar(&testPillarOj[0]);
	InitPillar(&testPillarOj[1]);


	for (int i = 0; i < pillarObj.size(); ++i)
	{
		const int L_HALF_SIZE = static_cast<int>(pillarObj.size() / 2);
		if (i < L_HALF_SIZE)
		{
			pillarObj[i].data.transform.pos = { 250.0f,-35.0f,20.0f + static_cast<float>(i) * 550.0f };
		}
		else
		{
			pillarObj[i].data.transform.pos = { -250.0f,-35.0f,20.0f + static_cast<float>(i - L_HALF_SIZE) * 550.0f };
		}
	}

	for (int i = 0; i < farSidePillarObj.size(); ++i)
	{
		const int L_HALF_SIZE = static_cast<int>(farSidePillarObj.size() / 2);
		if (i < L_HALF_SIZE)
		{
			farSidePillarObj[i].data.transform.pos = { -560.0f,-200.0f,20.0f + static_cast<float>(i) * 550.0f };
		}
		else
		{
			farSidePillarObj[i].data.transform.pos = { 560.0f,-200.0f,20.0f + static_cast<float>(i - L_HALF_SIZE) * 550.0f };
		}
	}

	for (int y = 0; y < topPillarObj.size(); ++y)
	{
		for (int x = 0; x < topPillarObj[y].size(); ++x)
		{
			if (y % 2 == 0)
			{
				topPillarObj[y][x].data.transform.pos = { -10000.0f + static_cast<float>(x) * 550.0f,250.0f,-100.0f + static_cast<float>(y) * 268.0f };
			}
			else
			{
				topPillarObj[y][x].data.transform.pos = { 4500.0f + static_cast<float>(x) * 550.0f,250.0f,-100.0f + static_cast<float>(y) * 268.0f };
			}
			topPillarObj[y][x].data.transform.rotation = { 0.0f,90.0f,90.0f };

			initTopPillarPosX[y][x] = topPillarObj[y][x].data.transform.pos.x;
		}
	}



	InitBackGround(KazMath::Color(237, 165, 135, 255), KazMath::Color(61, 23, 158, 255), 650.0f, 400.0f);
	t = 0.0f;

}

void FirstStage::Update()
{
	if (startFlag)
	{
		if (t <= 1.0f)
		{
			t += 1.0f / 60.0f;
		}
		if (1.0f <= t)
		{
			t = 1.0f;
		}
	}
	else
	{
		t = 0.0f;
	}

	easeY = EasingMaker(Out, Cubic, t);
	for (int i = 0; i < pillarObj.size(); ++i)
	{
		float lStartYPos = 800;
		float lEndYPos = -35;
		float lFarEndYPos = -200;
		pillarObj[i].data.transform.pos.y = (-lStartYPos + easeY * abs(lEndYPos + lStartYPos));
		farSidePillarObj[i].data.transform.pos.y = (-lStartYPos + easeY * abs(lFarEndYPos + lStartYPos));
	}


	float lVel = -1.0f;
	for (int i = 0; i < floorStage.size(); ++i)
	{
		floorStage[i].data.transform.pos.y = -30.0f;
		floorStage[i].data.transform.pos.z += lVel;

		if (floorStage[i].data.transform.pos.z <= -200.0f)
		{
			floorStage[i].data.transform.pos.z = 1000.0f;
		}
	}

	for (int i = 0; i < pillarObj.size(); ++i)
	{
		pillarObj[i].data.transform.pos.z += lVel;
		farSidePillarObj[i].data.transform.pos.z += lVel;

		if (pillarObj[i].data.transform.pos.z <= -1100.0f)
		{
			pillarObj[i].data.transform.pos.z = 2200.0f;
			farSidePillarObj[i].data.transform.pos.z = 2200.0f;
		}
	}

	if (startFlag && easeY < 1.0f)
	{
		for (int y = 0; y < topPillarObj.size(); ++y)
		{
			for (int x = 0; x < topPillarObj[y].size(); ++x)
			{
				if (y % 2 == 0)
				{
					topPillarObj[y][x].data.transform.pos.x = initTopPillarPosX[y][x] + easeY * 8500.0f;
				}
				else
				{
					topPillarObj[y][x].data.transform.pos.x = initTopPillarPosX[y][x] + -easeY * 6500.0f;
				}
			}
		}
	}
	else
	{
		for (int y = 0; y < topPillarObj.size(); ++y)
		{
			for (int x = 0; x < topPillarObj[y].size(); ++x)
			{
				if (y % 2 == 0)
				{
					topPillarObj[y][x].data.transform.pos.x += -lVel;
				}
				else
				{
					topPillarObj[y][x].data.transform.pos.x += lVel;
				}


				if (topPillarObj[y][x].data.transform.pos.x <= -1500.0f - 950.0f)
				{
					topPillarObj[y][x].data.transform.pos.x = 4550;
				}
				if (4550.0f + 550.0f <= topPillarObj[y][x].data.transform.pos.x)
				{
					topPillarObj[y][x].data.transform.pos.x = -1500.0f;
				}
			}
		}
	}


	ImGui::Begin("Obj");
	ImGui::DragFloat("POS_X", &testPillarOj[0].data.transform.pos.x);
	ImGui::DragFloat("POS_Y", &testPillarOj[0].data.transform.pos.y);
	ImGui::DragFloat("POS_Z", &testPillarOj[0].data.transform.pos.z);
	ImGui::DragFloat("SCALE_X", &testPillarOj[0].data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y", &testPillarOj[0].data.transform.scale.y);
	ImGui::DragFloat("SCALE_Z", &testPillarOj[0].data.transform.scale.z);
	ImGui::DragFloat("ROTA_X", &testPillarOj[0].data.transform.rotation.x);
	ImGui::DragFloat("ROTA_Y", &testPillarOj[0].data.transform.rotation.y);
	ImGui::DragFloat("ROTA_Z", &testPillarOj[0].data.transform.rotation.z);

	ImGui::DragFloat("POS_X1", &testPillarOj[1].data.transform.pos.x);
	ImGui::DragFloat("POS_Y1", &testPillarOj[1].data.transform.pos.y);
	ImGui::DragFloat("POS_Z1", &testPillarOj[1].data.transform.pos.z);
	ImGui::DragFloat("SCALE_X1", &testPillarOj[1].data.transform.scale.x);
	ImGui::DragFloat("SCALE_Y1", &testPillarOj[1].data.transform.scale.y);
	ImGui::DragFloat("SCALE_Z1", &testPillarOj[1].data.transform.scale.z);
	ImGui::DragFloat("ROTA_X1", &testPillarOj[1].data.transform.rotation.x);
	ImGui::DragFloat("ROTA_Y1", &testPillarOj[1].data.transform.rotation.y);
	ImGui::DragFloat("ROTA_Z1", &testPillarOj[1].data.transform.rotation.z);
	ImGui::End();
}

void FirstStage::Draw()
{
	for (int i = 0; i < pillarObj.size(); ++i)
	{
		pillarObj[i].Draw();
		farSidePillarObj[i].Draw();
	}
	for (int i = 0; i < testPillarOj.size(); ++i)
	{
		testPillarOj[i].data.colorData.color.a = 255;
		//testPillarOj[i].Draw();
	}

	for (int y = 0; y < topPillarObj.size(); ++y)
	{
		for (int x = 0; x < topPillarObj[y].size(); ++x)
		{
			topPillarObj[y][x].Draw();
		}
	}
	for (int i = 0; i < floorStage.size(); ++i)
	{
		floorStage[i].Draw();
	}
	DrawBackGround();
}
