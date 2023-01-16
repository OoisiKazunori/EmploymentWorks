#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

cbuffer RootConstants : register(b0)
{
    float3 pos;
    float radius;
};

//パーティクル当たり判定
RWStructuredBuffer<ParticleHitBoxData> hitBoxData : register(u0);

//当たったインデックス
RWStructuredBuffer<HitIDData> hitIndexData : register(u1);
//出力
AppendStructuredBuffer<GPUParticleInput> inputGPUParticleData : register(u2);

//周りの当たり判定ともとる
bool CheckLinkHitBox(uint3 BASE_HIT_INDEX,uint3 CHECK_HIT_INDEX)
{
    bool linkHitFlagArray[7];
    linkHitFlagArray[0] = CHECK_HIT_INDEX == BASE_HIT_INDEX;                    //真ん中
    linkHitFlagArray[1] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,1,0));  //上
    linkHitFlagArray[2] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,-1,0)); //下
    linkHitFlagArray[3] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(-1,0,0)); //左
    linkHitFlagArray[4] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(1,0,0));  //右
    linkHitFlagArray[5] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,1));  //前
    linkHitFlagArray[6] = CHECK_HIT_INDEX == (BASE_HIT_INDEX + float3(0,0,-1)); //後

    for(int i = 0;i < 7;++i)
    {
        if(linkHitFlagArray[i])
        {
            return true;
        }
    }
    return false;
}

//メッシュパーティクルの球判定を取ったインデックスを参考にパーティクルの挙動を決める
[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = ThreadGroupIndex(groupId,groupIndex,groupThreadID,1024);

    //同じインデックスの場合、パーティクルを動かす処理を追加する
    //for(int i = 0;i < hitIndexCounter[0]; ++i)
    //{
    //    //当たり判定+リンク付け込みの処理を取る
    //    if(CheckLinkHitBox(hitIndexData[i].id,hitBoxData[index].id))
    //    {
    //        float3 posHitBoxVec = pos;
    //        float3 posParticleVec = hitBoxData[index].pos - pos;
    //
    //        //なす角の判断
    //        float cos = dot(posParticleVec,posHitBoxVec) / (length(posParticleVec) * lenght(posHitBoxVec));
    //        float sin = acos(cos);
    //
    //        int angle = RadianToAngle(sin) - 90;
    //        float rate = angle / 90.0f;
    //
    //        float3 vel = normalize(posParticleVec) * 5.5f * rate;
    //        vel.x = 0.0f;
    //
    //        //パーティクルの基本座標とラープ用の座標が必要
    //        float3 larpPos = float3(0,0,0) + vel;
    //    }
    //}


    //行列計算ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
    ParticleData particleData;
    particleData.pos = hitBoxData[index].pos;
    particleData.color = hitBoxData[index].color;

    matrix worldMat = CalucurateWorldMat(float3(0,0,0),float3(0.01,0.01,0.01),float3(0,0,0));
    worldMat[0][3] = particleData.pos.x;
    worldMat[1][3] = particleData.pos.y;
    worldMat[2][3] = particleData.pos.z;
    //worldMat = mul(motherMatData[particleData.id],worldMat);    
    //行列計算ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー


    //パーティクル情報の描画,当たったかどうかも表示する
    GPUParticleInput inputData;    
    inputData.worldMat = worldMat;
    inputData.color = float4(1,1,1,1);

    for(int i = 0;i < 216; ++i)
    {
        bool isHitFlag = hitIndexData[i].meshID == hitBoxData[index].meshID && hitIndexData[i].id == hitBoxData[index].id;
        if(isHitFlag)
        {
            inputData.color = float4(1,0,0,1);
            break;
        }
    }

    inputGPUParticleData.Append(inputData);
    //パーティクル情報の描画,当たったかどうかも表示する
}
