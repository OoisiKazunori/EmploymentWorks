#include "EnemyHelper.h"

void KazEnemyHelper::GenerateEnemy(std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES, array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA, std::array<int, 10> &ENEMISE_HANDLE)
{
	for (int enemyType = 0; enemyType < RESPONE_DATA.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < RESPONE_DATA[enemyType].size(); ++enemyCount)
		{
			if (RESPONE_DATA[enemyType][enemyCount].layerLevel != -1)
			{
				switch (enemyType)
				{
				case ENEMY_TYPE_NORMAL:
					ENEMIES[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					break;

				case ENEMY_TYPE_MOTHER:
					ENEMIES[enemyType][enemyCount] = std::make_unique<NormalEnemy>();
					//子敵の生成(テスト用)
					for (int i = 0; i < 8; ++i)
					{
						int index = ENEMISE_HANDLE[ENEMY_TYPE_KID];
						ENEMIES[ENEMY_TYPE_KID][index] = std::make_unique<KidEnemy>();
						++ENEMISE_HANDLE[ENEMY_TYPE_KID];
					}
					break;

				case ENEMY_TYPE_MISILE:
					ENEMIES[enemyType][enemyCount] = std::make_unique<NormalMisileEnemy>();
					ENEMIES[ENEMY_TYPE_MISILE_SPLINE][ENEMISE_HANDLE[ENEMY_TYPE_MISILE_SPLINE]] = std::make_unique<SplineMisile>();
					++ENEMISE_HANDLE[ENEMY_TYPE_MISILE_SPLINE];
					break;

				case ENEMY_TYPE_BATTLESHIP:
					ENEMIES[enemyType][enemyCount] = std::make_unique<BattleshipEnemy>();
					for (int i = 0; i < 8; ++i)
					{
						int index = ENEMISE_HANDLE[ENEMY_TYPE_BATTLESHIP_MISILE];
						ENEMIES[ENEMY_TYPE_BATTLESHIP_MISILE][index] = std::make_unique<SplineMisileForBattleShip>();
						++ENEMISE_HANDLE[ENEMY_TYPE_BATTLESHIP_MISILE];
					}
					break;

				default:
					break;
				}
				++ENEMISE_HANDLE[enemyType];
			}
		}
	}
}

void KazEnemyHelper::AddEnemy(const std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES, array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &RESPONE_DATA, std::array<int, 10> &ADD_ENEMISE_HANDLE, int GAME_FLAME, int GAME_STAGE_LEVEL)
{
	for (int enemyType = 0; enemyType < ENEMIES.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < ENEMIES[enemyType].size(); ++enemyCount)
		{
			//生成されている、初期化している敵のみ更新処理を通す
			bool enableToUseDataFlag = ENEMIES[enemyType][enemyCount] != nullptr && ENEMIES[enemyType][enemyCount]->GetData()->oprationObjData->initFlag;
			if (enableToUseDataFlag)
			{
				EnemyData *enemyData = ENEMIES[enemyType][enemyCount]->GetData().get();
				//追加で生成するデータを検知したら生成する
				if (enemyData->genarateData.enemyType != -1)
				{
					//生成する敵の種類
					int enemyTypeData = enemyData->genarateData.enemyType;
					//最後に生成して次のハンドル
					int nowHandle = ADD_ENEMISE_HANDLE[enemyTypeData];
					//現在のレイヤーレベルに合わせる
					RESPONE_DATA[enemyTypeData][nowHandle].layerLevel = GAME_STAGE_LEVEL;
					//現在のフレーム数+インターバルフレーム*個数で設定する
					RESPONE_DATA[enemyTypeData][nowHandle].flame = GAME_FLAME;
					RESPONE_DATA[enemyTypeData][nowHandle].generateData = enemyData->genarateData.enemyGenerateData;
					//ハンドルを増やす
					++ADD_ENEMISE_HANDLE[enemyTypeData];

					//追加したら終了処理を入れる
					ENEMIES[enemyType][enemyCount]->GetData()->genarateData.Finalize();
				}
			}
		}
	}
}

void KazEnemyHelper::InitEnemy(std::array<std::array<std::unique_ptr<IEnemy>, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> &ENEMIES, array<array<ResponeData, ENEMY_NUM_MAX>, LAYER_LEVEL_MAX> RESPONE_DATA, int GAME_FLAME, int GAME_STAGE_LEVEL, bool DEMO_FLAG)
{
	for (int enemyType = 0; enemyType < RESPONE_DATA.size(); ++enemyType)
	{
		for (int enemyCount = 0; enemyCount < RESPONE_DATA[enemyType].size(); ++enemyCount)
		{
			bool enableToUseThisDataFlag = RESPONE_DATA[enemyType][enemyCount].layerLevel != -1;
			bool readyToInitDataFlag = RESPONE_DATA[enemyType][enemyCount].flame == GAME_FLAME &&
				RESPONE_DATA[enemyType][enemyCount].layerLevel == GAME_STAGE_LEVEL;

			if (enableToUseThisDataFlag && readyToInitDataFlag && ENEMIES[enemyType][enemyCount] != nullptr)
			{
				ENEMIES[enemyType][enemyCount]->Init(RESPONE_DATA[enemyType][enemyCount].generateData, DEMO_FLAG);
			}
		}
	}
}
