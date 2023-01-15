#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/MeshCollision.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//BBの値
RWStructuredBuffer<BoundingBoxData> bbPosData : register(u0);
//当たり判定
RWStructuredBuffer<MeshHitBox> hitBoxData : register(u1);
//デバック用、当たり判定確認
AppendStructuredBuffer<GPUParticleInput> outputData : register(u2);

cbuffer RootConstants : register(b0)
{
    float diameter;
    uint id;
    uint xMax;
    uint xyMax;
};

//BoundingBox内に球の当たり判定の座標を均等に並べる
[numthreads(1, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    //端から順に球を並べる
    float3 pos = (bbPosData[0].minPos + diameter / 2.0f) + groupId * diameter;
    hitBoxData[index].pos = pos;
    hitBoxData[index].id = groupId;
    hitBoxData[index].meshID = id;
}


//BoundingBox内に球の当たり判定の座標を均等に並べる
[numthreads(1, 1, 1)]
void DebugCSmain(uint3 groupId : SV_GroupID)
{
    uint index = ThreadGroupIndex(groupId,xMax,xyMax);

    //端から順に球を並べる
    float3 pos = (bbPosData[0].minPos + diameter / 2.0f) + groupId * diameter;
    hitBoxData[index].pos = pos;
    hitBoxData[index].id = groupId;
    hitBoxData[index].meshID = id;
    
    //BB内にきちんと配置出来ているか計算する。
    GPUParticleInput debugOutput;
    debugOutput.worldMat = CalucurateWorldMat(pos,float3(0.1f,0.1f,0.1f),float3(0.0f,0.0f,0.0f));
    debugOutput.color = float4(1,1,1,1);
    outputData.Append(debugOutput);
}
