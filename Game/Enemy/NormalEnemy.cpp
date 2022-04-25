#include "NormalEnemy.h"
#include"../KazLibrary/Loader/ObjResourceMgr.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

NormalEnemy::NormalEnemy()
{
}

void NormalEnemy::Init(const XMVECTOR &POS)
{
	iEnemy_ModelRender->data.transform.pos = POS;	//座標の初期化
	iEnemy_ModelRender->data.transform.scale = { 10.0f,10.0f,10.0f };
	iEnemy_ModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::TestPath + "hamster.obj");	//モデル読み込み
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;	//当たり判定の大きさ変更
	iOperationData.Init(1);							//残りロックオン数等の初期化

	//iEnemy_EnemyStatusData->genarateData.enemyType = 1;
	//iEnemy_EnemyStatusData->genarateData.initPos = { 30.0f, 30.0f,30.0f };

	generateTimer = 0;
}

void NormalEnemy::Finalize()
{
}

void NormalEnemy::Update()
{
	if (120 <= generateTimer && generateTimer < 121)
	{
		//iEnemy_EnemyStatusData->genarateData.enemyType = 1;
		//iEnemy_EnemyStatusData->genarateData.initPos= { -30.0f, 30.0f,30.0f };
	}


	if (iEnemy_EnemyStatusData->oprationObjData->rockOnNum <= 0)
	{
		iEnemy_ModelRender->data.color = { 255.0f,0.0f,0.0f,255.0f };
	}


	if (!iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ModelRender->data.pipelineName = PIPELINE_NAME_COLOR_WIREFLAME;
		DradEffect(&iEnemy_ModelRender->data.transform.pos, &iEnemy_ModelRender->data.transform.rotation, &iEnemy_ModelRender->data.color.w);
	}


	++generateTimer;
}

void NormalEnemy::Draw()
{
	iEnemy_ModelRender->Draw();
}
