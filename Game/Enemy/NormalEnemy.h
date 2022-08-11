#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"
#include"../Game/Effect/RocketEffect.h"

/// <summary>
/// サンプル用の敵
/// </summary>
class NormalEnemy:public IEnemy
{
public:
	NormalEnemy();

	void Init(const KazMath::Vec3<float> &POS, bool DEMO_FLAG);
	void Finalize();
	void Update();
	void Draw();

private:
	RocketEffect rocketEffect;
};

