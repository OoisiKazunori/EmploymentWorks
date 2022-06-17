#pragma once
#include"../KazLibrary/DirectXCommon/Base.h"
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/LineEffect/LineEffect.h"

struct EnemyMoveData
{
	bool straightFlag;
	bool leftFlag;
	bool backFlag;
	bool rightFlag;
};

enum EnumLineMove
{
	LINE_MOVE_RIGHT,
	LINE_MOVE_LEFT,
	LINE_MOVE_UP,
	LINE_MOVE_DOWN,
	LINE_MOVE_STRAIGHT,
	LINE_MOVE_BACK
};

enum eLineMove
{
	LINE_MOVE_NONE = -1 ,
	LINE_MOVE_X,
	LINE_MOVE_Y,
	LINE_MOVE_Z
};

enum eSurface
{
	SURFACE_NONE = -1,
	SURFACE_LEFT,
	SURFACE_RIGHT,
	SURFACE_FRONT,
	SURFACE_BACK,
	SURFACE_TOP,
	SURFACE_BUTTOM,
};


class LineLevel1
{
public:
	LineLevel1();

	void CalucurateDistance(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS);

	void Attack(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS, const EnemyMoveData &FLAG_DATA);
	void Attack2(const KazMath::Vec3<float> &PLAYER_POS, const KazMath::Vec3<float> &ENEMY_POS, const EnemyMoveData &FLAG_DATA);

	void ReleaseShot();

	void Release();

	void Update();
	void Draw();

	bool allFinishFlag;
	bool lineReachObjFlag;
	bool initFlag;
	KazMath::Vec3<float> playerPos;
private:
	std::vector<KazMath::Vec3<float>> limitPos;
	vector<BoxPolygonRenderPtr> limitPolygon;
	vector<unique_ptr<LineEffect>> line;

	array<int, 3> countVec;

	KazMath::Vec3<float> rockOnDistance;//ロックオンした際の敵の座標
	KazMath::Vec3<float> distanceValue;//ロックオン時の距離と現在のプレイヤーと敵の距離の割合

	KazMath::Vec3<float> CalucurateMoveVector(const int &RANDM_NUM, const float &LENGTH);
	int CalucurateDirection(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM);
	int CalucurateDirection2(const int &MOVE_VEC, const int &MOVE_VEC_MAX_NUM);

	eLineMove CaluRandom(int DONT_USE, int OLD_NUM);

	KazMath::Vec3<float> firstDir(eSurface SURFACE);

	bool IsMinus(float VALUE)
	{
		return VALUE < 0;
	}
};

