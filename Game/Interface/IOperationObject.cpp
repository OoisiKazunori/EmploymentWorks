#include "IOperationObject.h"

void IOperationObject::Hit()
{
	--iOperationData.rockOnNum;
}

bool IOperationObject::LockedOrNot()
{
	return iOperationData.rockOnNum <= 0;
}

bool IOperationObject::IsAlive()
{
	return iOperationData.enableToHitFlag && !LockedOrNot();
}

int IOperationObject::ReduceHp(int COUNT)
{
	if (!startToReduceHpFlag)
	{
		reduceHpNum = COUNT;
		startToReduceHpFlag = true;
	}
	//一定時間の間隔で指定した回数分体力を減らす
	if (startToReduceHpFlag)
	{
		++timer;
		if (5 <= timer)
		{
			--iOperationData.hp;
			--reduceHpNum;
			timer = 0;
			return 1;
		}
	}
	//回数分終えたら終了する
	if (reduceHpNum <= 0)
	{
		startToReduceHpFlag = false;
		return 2;
	}
	return 0;
}