#include "SplineMisile.h"
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"

SplineMisile::SplineMisile()
{
	maxTime = 60 * 5;
	splineBox.data.color = { 255,0,0,255 };

	iEnemy_ObjModelRender->data.handle = ObjResourceMgr::Instance()->LoadModel(KazFilePathName::EnemyPath + "Misile/" + "missile_Model.obj");
}

void SplineMisile::Init(const EnemyGenerateData &GENERATE_DATA, bool DEMO_FLAG)
{
	iEnemy_ObjModelRender->data.transform.pos = GENERATE_DATA.initPos;
	iEnemy_ObjModelRender->data.transform.scale = { 1.3f,1.3f,1.3f };
	iEnemy_ObjModelRender->data.pipelineName = PIPELINE_NAME_OBJ_MULTITEX;
	iEnemy_ObjModelRender->data.removeMaterialFlag = false;
	iEnemy_ObjModelRender->data.colorData.color.x = 255;
	iEnemy_ObjModelRender->data.colorData.color.y = 255;
	iEnemy_ObjModelRender->data.colorData.color.z = 255;
	iEnemy_ObjModelRender->data.colorData.color.a = 255;

	iEnemy_EnemyStatusData->timer = maxTime;
	iEnemy_EnemyStatusData->hitBox.radius = 5.0f;
	iEnemy_EnemyStatusData->hitBox.center = &iEnemy_ObjModelRender->data.transform.pos;

	iOperationData.Init(1, "gwJ-M");

	startIndex = 1;
	timeRate = 0.0f;


	points.clear();
	points.shrink_to_fit();

	KazMath::Vec3<float> startPos = GENERATE_DATA.initPos;
	KazMath::Vec3<float> endPos = { 0.0f,0.0f,0.0f };

	KazMath::Vec3<float> distance = startPos - endPos;
	float addDistance = distance.z / 4.0f;

	distance.z /= 2.0f;	//中間はZ距離の半分
	distance.z -= addDistance;


	//スタート地点
	points.push_back(startPos);
	points.push_back(startPos);

	//中間
	points.push_back(distance);

	//ゴール地点
	points.push_back(endPos);
	points.push_back(endPos);

	pointTime = maxTime / (static_cast<int>(points.size() )- 3);
	nowTime = 0;
	initDeadSoundFlag = false;
	hitFlag = false;

	demoFlag = DEMO_FLAG;
}

void SplineMisile::Finalize()
{
}

void SplineMisile::Update()
{
	//当たったら描画しなくなる
	if (iEnemy_EnemyStatusData->timer <= 0)
	{
		iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
		hitFlag = true;
	}


	//死亡演出処理
	//デバックキーor当たり判定内&&死亡時
	if (!ProcessingOfDeath(DEATH_ROLL) && !hitFlag)
	{
		if (iOperationData.enableToHitFlag)
		{
			--iEnemy_EnemyStatusData->timer;
		}

		//移動時間----------------------------------------------
		timeRate = static_cast<float>(nowTime) / static_cast<float>(pointTime);
		if (1.0f <= timeRate)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex++;
				timeRate = 0.0f;
				nowTime = 0;
			}
			else
			{
				timeRate = 1.0f;
			}
		}
		++nowTime;
		//移動時間----------------------------------------------


		//座標設定----------------------------------------------
		position = KazMath::SplinePosition(points, static_cast<size_t>(startIndex), timeRate, true);

		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].data.transform.pos = points[i];
		}

		iEnemy_ObjModelRender->data.transform.pos = position;
		//座標設定----------------------------------------------

		//前ベクトルの設定----------------------------------------------
		float nextTimeRate = static_cast<float>(nowTime + 1) / static_cast<float>(pointTime);
		KazMath::Vec3<float> nextpos = KazMath::SplinePosition(points, startIndex, nextTimeRate, true);

		KazMath::Vec3<float> upVector = nextpos - position;
		upVector.Normalize();
		iEnemy_ObjModelRender->data.upVector = upVector;
		//前ベクトルの設定----------------------------------------------
	}
}

void SplineMisile::Draw()
{
	if (demoFlag)
	{
		for (int i = 0; i < pointsRender.size(); ++i)
		{
			pointsRender[i].Draw();
		}
	}

	if (iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag)
	{
		iEnemy_ObjModelRender->Draw();
		LockOnWindow(iEnemy_ObjModelRender->data.transform.pos);
	}
}
