﻿#include "ClassScene.h"
#include"../Camera/CameraMgr.h"
#include"../Input/KeyBoradInputManager.h"
#include"../Math/KazMath.h"

ClassScene::ClassScene()
{
	boxRender = std::make_unique<BoxPolygonRender>();
}

ClassScene::~ClassScene()
{
}

void ClassScene::Init()
{
}

void ClassScene::Finalize()
{
}

void ClassScene::Input()
{

	KeyBoradInputManager *input = KeyBoradInputManager::Instance();
	//	ControllerInputManager *inputController = ControllerInputManager::Instance();

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

#pragma endregion
}

void ClassScene::Update()
{
	eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle);
	targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	CameraMgr::Instance()->Camera(eyePos, targetPos, { 0.0f,1.0f,0.0f });


	//速度の公式①
	{
		caluSpeed = KazPhysics::CalucurateHorizontalSpeed(0.8, 5.0);
	}

	//速度の公式②
	{
		caluSpeed2 = KazPhysics::CalucurateHorizontalSpeed2(0.8, 10.0);
	}

	//距離の公式1
	{
		double a = KazPhysics::CaluAcceleration(5.0, 5.0, 3.0);
		caluDistance = KazPhysics::CalucurateHorizontalDistance(a, 5.0, 3.0);
	}

	//距離の公式2
	{
		caluDistance2 = KazPhysics::CalucurateHorizontalDistance2(5.0, 5.0, 3.0);
	}



	////例題1
	//{
	//	caluSpeed = KazPhysics::CalucurateVerticalDistance(9.8, 5.0);
	//}

	////例題2
	//{
	//	caluSpeed2 = KazPhysics::CalucurateVerticalDistance(9.8, -10.0);
	//}

	////例題3
	//{
	//	double a = KazPhysics::CalucurateVerticalSpeed2(9.8, -10.0, -10.0);
	//	//KazPhysics::CalucurateVerticalSpeed(a,)
	//}

	////例題4
	//{
	//	caluDistance2 = KazPhysics::CalucurateVerticalSpeed2(5.0, 5.0, 20.0);
	//}



	boxRender->data.transform.pos;
}

void ClassScene::Draw()
{
	bg.Draw();
	boxRender->Draw();
}

int ClassScene::SceneChange()
{
	return SCENE_NONE;
}
