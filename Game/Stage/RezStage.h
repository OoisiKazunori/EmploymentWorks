#pragma once
#include"../KazLibrary/Buffer/CreateGpuBuffer.h"
#include"../Game/Interface/IStage.h"
#include<array>
#include<vector>
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Event/LineFlashLight.h"
#include"../Game/Debug/ParameterMgr.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../Game/Stage/DrawGrid.h"

class RezStage :public IStage
{
public:
	RezStage();
	void Update()override;
	void Draw()override;

private:

	//グリッドの描画ーーー
	int timer;
	int maxTimer;
	//グリッドの描画ーーー


	//床にOBJ配置
	struct FloorObjData
	{
		std::array<BoxPolygonRender, 2> objRender;
		KazMath::Vec3<float>initScale;
	};
	std::array<FloorObjData, 50>floorObjectRender;

	float scaleRate;
	bool reversValueFlag;

	Sprite3DRender vaporWaveSunRender;


	FbxModelRender model;
	ObjModelRender objModel;

	std::array<std::array<char, 100>, 50>filePassChar;
	ParameterMgr stageParamLoader;

	int selectNum;
	BoxPolygonRender selectingR;


	Sphere sphere;
	KazMath::Vec3<float>pos;
	LineRenderPtr ray;

	BoxPolygonRender c1, c2;


	DrawGrid floorGrid, leftWallGrid, rightWallGrid;
};