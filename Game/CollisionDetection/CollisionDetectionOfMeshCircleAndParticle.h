#pragma once
#include"../Game/Helper/ComputeBufferHelper.h"

/// <summary>
/// BB内に配置された当たり判定座標とメッシュパーティクル座標との当たり判定を行います
/// </summary>
class CollisionDetectionOfMeshCircleAndParticle
{
public:
	CollisionDetectionOfMeshCircleAndParticle(
		const ComputeBufferHelper::BufferData &MESH_HITBOX_POS,
		const ComputeBufferHelper::BufferData &MESH_HITBOX_ID,
		const ComputeBufferHelper::BufferData &PARTICLE_POS
	);
	void Compute();

private:
	ComputeBufferHelper computeHelper;
	RESOURCE_HANDLE particleHitBoxIDHandle;
	RESOURCE_HANDLE commonDataHandle;

	struct CommonData
	{
		UINT meshHitBoxNum;
		float hitBoxRadius;
	};
};

