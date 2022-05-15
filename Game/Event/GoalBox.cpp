#include "GoalBox.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"

GoalBox::GoalBox()
{
	model = std::make_unique<ObjModelRender>();
	model->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iOperationData.Init(8);
	prevHpNum = iOperationData.rockOnNum;

	model->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::GoalPath + "goal.obj");

	lerpPos = { 0.0f,0.0f,0.0f };
	lerpRota = { 0.0f,0.0f,0.0f };
	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	//model->data.transform.scale = { 10.0f,10.0f,10.0f };

	lerpMoveRotaVel = 0.1f;
	lerpMoveVel = 0.1f;
	moveRotaVel = { 200.0f,200.0f,200.0f };
	moveVel = { 0.0f,0.0f,15.0f };

	goCenterPos = { 0.0f, 3.0f, 50.0f };
	lightEffect.Init(&model->data.motherMat);

	//std::shared_ptr<XMMATRIX>data = std::make_shared<XMMATRIX>(model->data.motherMat);
}

void GoalBox::Init(const XMVECTOR &POS)
{
	iOperationData.Init(8);
	prevHpNum = iOperationData.rockOnNum;


	lerpPos = POS;
	lerpRota = { 0.0f,0.0f,0.0f };
	lerpScale = { 1.0f,1.0f,1.0f };

	model->data.transform.pos = lerpPos;
	model->data.transform.rotation = lerpRota;

	lightEffect.Init(&model->data.motherMat);
	XMVECTOR adj = { 0.0f,0.0f,-5.0f };
	portalEffect.Init(goCenterPos + adj);
	addRota = 0;


}

void GoalBox::Update()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_M))
	{
		Hit();
	}
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_N))
	{
		Init({ 20.0f,10.0f,10.0f });
	}
	bool hitFlag = iOperationData.rockOnNum != prevHpNum;
	//当たった際の挙動
	if (hitFlag)
	{
		//後ろに飛ぶ
		lerpPos.m128_f32[2] = model->data.transform.pos.m128_f32[2] + moveVel.m128_f32[2];
		//回転させる
		lerpRota = model->data.transform.rotation + moveRotaVel;
		lightEffect.Appear();
	}

	//全弾ヒットしたら演出消して画面中央に向かう
	if (iOperationData.rockOnNum <= 0)
	{
		lightEffect.Disappear();
		lerpPos = goCenterPos;

		if (static_cast<int>(model->data.transform.pos.m128_f32[0]) == static_cast<int>(goCenterPos.m128_f32[0]) &&
			static_cast<int>(model->data.transform.pos.m128_f32[1]) == static_cast<int>(goCenterPos.m128_f32[1]) &&
			static_cast<int>(model->data.transform.pos.m128_f32[2]) == static_cast<int>(goCenterPos.m128_f32[2]))
		{
			addRota += addVel;
			addVel += 0.1f;
		}
		lerpRota.m128_f32[0] = 0.0f;
		lerpRota.m128_f32[1] = addRota;
		lerpRota.m128_f32[2] = 0.0f;


		if (60.0f * 5.0f <= addRota)
		{
			lerpScale.m128_f32[0] = 5.0f;
			lerpScale.m128_f32[1] = 5.0f;
			lerpScale.m128_f32[2] = 5.0f;
		}
		if (60.0f * 10.0f <= addRota)
		{
			portalEffect.Start();
		}
	}



	//座標のラープ
	{
		XMVECTOR distance = lerpPos - model->data.transform.pos;
		model->data.transform.pos += distance * lerpMoveVel;
	}
	//回転のラープ
	{
		XMVECTOR distance = lerpRota - model->data.transform.rotation;
		model->data.transform.rotation += distance * lerpMoveRotaVel;
	}

	{
		XMVECTOR distance = lerpScale - model->data.transform.scale;
		model->data.transform.scale += distance * 0.1f;
	}
	prevHpNum = iOperationData.rockOnNum;


	lightEffect.Update();
	portalEffect.Update();


	ImGui::Begin("Goal");
	ImGui::InputFloat("RotaX", &moveRotaVel.m128_f32[0]);
	ImGui::InputFloat("RotaY", &moveRotaVel.m128_f32[1]);
	ImGui::InputFloat("RotaZ", &moveRotaVel.m128_f32[2]);
	ImGui::InputFloat("Z", &moveVel.m128_f32[2]);
	ImGui::InputFloat("MoveLerp", &lerpMoveVel);
	ImGui::InputFloat("MoveRotaLerp", &lerpMoveRotaVel);
	ImGui::End();
}

void GoalBox::Draw()
{
	model->Draw();
	portalEffect.Draw();
}
