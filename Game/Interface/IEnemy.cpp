#include "IEnemy.h"

IEnemy::IEnemy()
{
	//描画の初期化----------------------------------------------------------------
	iEnemy_ModelRender = std::make_unique<BoxPolygonRender>();
	//描画の初期化----------------------------------------------------------------

	//敵情報の初期化----------------------------------------------------------------
	iEnemy_EnemyStatusData = std::make_unique<EnemyData>(
		&iEnemy_ModelRender->data.transform.pos,
		-1.0f,
		&iOperationData
	);
	//敵情報の初期化----------------------------------------------------------------
}

void IEnemy::Dead()
{
	iEnemy_EnemyStatusData->oprationObjData->enableToHitFlag = false;
}

const unique_ptr<EnemyData> &IEnemy::GetData()
{
	return iEnemy_EnemyStatusData;
}
