#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"

struct ConstLineData
{
	DirectX::XMFLOAT4 distance;
	DirectX::XMFLOAT4 color;
};

class LineEffect
{
public:
	LineEffect();
	~LineEffect();

	void RockOn(const KazMath::Vec3<float> &START_POS, const KazMath::Vec3<float> &END_POS, const KazMath::Vec3<float> &START_PLAYER_DISTANCE, const KazMath::Vec3<float> &END_PLAYER_DISTANCE);
	void StartEffect();
	void ReleaseEffect();
	void Finalize();
	void Update();
	void Draw();

	void MoveLine(const KazMath::Vec3<float> &VALUE);
	bool FinishFlag();

	bool finishRockOnFlag;
	bool finishReleaseFlag;
	KazMath::Vec3<float> playerPos;
private:
	const float NORMAL_LOCKON_SPEED = 6.0f;
	const float RAPID_LOCKON_SPEED = 12.0f;

	LineRenderPtr line;
	Sprite3DRenderPtr circle;
	Sprite3DRenderPtr releaseCircle;
	RESOURCE_HANDLE constBufferHandle;
	ConstLineData lineData;

	bool rockOnFlag;
	KazMath::Vec3<float> startPos, endPos;
	KazMath::Vec3<float> startPlayerDistance, endPlayerDistance;
	KazMath::Vec3<float> distance;
	KazMath::Vec3<float> value;

	float rate,lockOnTime,releaseTime;
	float lockOnSpeed,releaseSpeed;
	int axis;

	bool minusFlag;
	RESOURCE_HANDLE circleTexHandle;

	bool releaseFlag;
	bool releaseCircleEffectFlag;

	bool finishFlag;
	bool allFinishFlag;
};

