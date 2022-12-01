#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/ExcuteIndirectHeader.hlsli"

struct OutputData
{
    float3 pos;
    float4 color;
    float4 baseColor;
};

struct InputData
{
    float3 pos;
    float2 uv;
};

//更新
RWStructuredBuffer<InputData> vertciesData : register(u0);
//出力
RWStructuredBuffer<OutputData> worldPosData : register(u1);

cbuffer RootConstants : register(b0)
{
    float4 pos;
    //x vertNum, y bias,z perTriangleNum,w faceCountNum
    uint4 TriangleData;
};

Texture2D<float4> tex : register(t2);
SamplerState smp : register(s0);


float CalucurateTriangleArea(float3 P0,float3 P1,float3 P2)
{
    float3 p0p1Vec = P1 - P0;
    float3 p1p2Vec = P2 - P1;
    return length(cross(p0p1Vec,p1p2Vec)) / 2.0f;
}

float CalucurateUVW(float3 P0,float3 P1,float3 ATTACK_POINT,float TRIANGLE_AREA)
{
    float3 p0p1Vec = ATTACK_POINT - P0;
    float3 p1p2Vec = ATTACK_POINT - P1;
    float area = length(cross(p0p1Vec,p1p2Vec)) / 2.0f;   
    float rate = area / TRIANGLE_AREA;
    return rate;
}

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(TriangleData.w <= index)
    {
        return;
    }

    //インデックス数以内なら処理する
    //三角形を構成するインデックスの指定--------------------------------------------
    uint firstVertIndex = index * 3;
    uint secondVertIndex = index * 3 + 1;
    uint thirdVertIndex = index * 3 + 2;

    uint uvFirstVertIndex = firstVertIndex;
    uint uvSecondVertIndex = secondVertIndex;
    uint uvThirdVertIndex = thirdVertIndex;
    //三角形を構成するインデックスの指定--------------------------------------------

    //頂点座標からワールド座標に変換後----------------------------------------------
    InputData firstVertWorldPos;
    InputData secondVertWorldPos;
    InputData thirdVertWorldPos;

    firstVertWorldPos.pos = GetPos(vertciesData[firstVertIndex].pos.xyz,pos.xyz);
    secondVertWorldPos.pos = GetPos(vertciesData[secondVertIndex].pos.xyz,pos.xyz);
    thirdVertWorldPos.pos = GetPos(vertciesData[thirdVertIndex].pos.xyz,pos.xyz);
    firstVertWorldPos.uv = vertciesData[uvFirstVertIndex].uv;
    secondVertWorldPos.uv = vertciesData[uvSecondVertIndex].uv;
    thirdVertWorldPos.uv = vertciesData[uvThirdVertIndex].uv;
    //頂点座標からワールド座標に変換------------------------------------------------

    //三角形を構成するレイ--------------------------------------------
    const int RAY_MAX_NUM = 3;
    const int RAY_POS_MAX_NUM = 2;
    float3 triangleRay[RAY_MAX_NUM][RAY_POS_MAX_NUM];
    triangleRay[0][0] = firstVertWorldPos.pos.xyz;
    triangleRay[0][1] = secondVertWorldPos.pos.xyz;
    triangleRay[1][0] = secondVertWorldPos.pos.xyz;
    triangleRay[1][1] = thirdVertWorldPos.pos.xyz;
    triangleRay[2][0] = thirdVertWorldPos.pos.xyz;
    triangleRay[2][1] = firstVertWorldPos.pos.xyz;

    //三角形を構成するレイ--------------------------------------------

    //重心座標
    float3 triangleCentralPos = (firstVertWorldPos.pos.xyz + secondVertWorldPos.pos.xyz + thirdVertWorldPos.pos.xyz) / 3.0f;

    //三角形の面積を計算
    float triangleArea = CalucurateTriangleArea(firstVertWorldPos.pos.xyz,secondVertWorldPos.pos.xyz,thirdVertWorldPos.pos.xyz);

 
    //パーティクルの配置--------------------------------------------
    const int PARTICLE_MAX_NUM = TriangleData.z;
    const int PER_PARTICLE_MAX_NUM = PARTICLE_MAX_NUM / 3;
    for(int rayIndex = 0; rayIndex < RAY_MAX_NUM; ++rayIndex)
    {
        //一辺の長さ
        float3 distance = triangleRay[rayIndex][1] - triangleRay[rayIndex][0];

        for(int particleIndex = 0; particleIndex < PER_PARTICLE_MAX_NUM; ++particleIndex)
        {
            uint outputIndex = index * PARTICLE_MAX_NUM + rayIndex * PER_PARTICLE_MAX_NUM + particleIndex;
            float rate = RandVec3(outputIndex + 100,1,0).x;
            //始点算出
            float3 startPos = triangleRay[rayIndex][0] + distance * rate;
            //終点算出
            float3 endPos = triangleCentralPos;
            //一辺のとある位置から重心座標までの距離
            float3 resultDistance = endPos - startPos;

            //パーティクルの配置
            float3 resultPos;
            const int PARTICLE_MAX_BIAS = 100;
            const int RANDOM_NUMBER_BIAS = TriangleData.y;
            
            if(RandVec3(outputIndex,PARTICLE_MAX_BIAS,0).x <= RANDOM_NUMBER_BIAS)
            {
                //エッジ周辺に偏らせる
                rate = RandVec3(outputIndex + 1000,10,0).x / 100.0f;
                resultPos = startPos + resultDistance * rate;

                //座標からUVを取る
                float3 uvw;
                uvw.x = CalucurateUVW(firstVertWorldPos.pos.xyz,secondVertWorldPos.pos.xyz,resultPos,triangleArea);
                uvw.y = CalucurateUVW(secondVertWorldPos.pos.xyz,thirdVertWorldPos.pos.xyz,resultPos,triangleArea);
                uvw.z = CalucurateUVW(thirdVertWorldPos.pos.xyz,firstVertWorldPos.pos.xyz,resultPos,triangleArea);

                float2 firstVec;
                firstVec.x = firstVertWorldPos.uv.x * uvw.x;
                firstVec.y = firstVertWorldPos.uv.y * uvw.x;

                float2 secondVec;
                secondVec.x = secondVertWorldPos.uv.x * uvw.y;
                secondVec.y = secondVertWorldPos.uv.y * uvw.y;

                float2 thirdVec;
                thirdVec.x = thirdVertWorldPos.uv.x * uvw.z;
                thirdVec.y = thirdVertWorldPos.uv.y * uvw.z;

                float2 uv = firstVec + secondVec + thirdVec;

                worldPosData[outputIndex].pos.xyz = resultPos;
                worldPosData[outputIndex].color = tex.SampleLevel(smp,uv,0);
            }
            else
            {
                //面周辺に偏らせる
                rate = RandVec3(startPos.y * 10.0f + outputIndex + 10000,1,0).x;
                resultPos = startPos + resultDistance * rate;

                //座標からUVを取る
                float3 uvw;
                uvw.x = CalucurateUVW(firstVertWorldPos.pos.xyz,secondVertWorldPos.pos.xyz,resultPos,triangleArea);
                uvw.y = CalucurateUVW(secondVertWorldPos.pos.xyz,thirdVertWorldPos.pos.xyz,resultPos,triangleArea);
                uvw.z = CalucurateUVW(thirdVertWorldPos.pos,firstVertWorldPos.pos,resultPos,triangleArea);
                
                float2 firstVec;
                firstVec.x = firstVertWorldPos.uv.x * uvw.x;
                firstVec.y = firstVertWorldPos.uv.y * uvw.x;

                float2 secondVec;
                secondVec.x = secondVertWorldPos.uv.x * uvw.y;
                secondVec.y = secondVertWorldPos.uv.y * uvw.y;

                float2 thirdVec;
                thirdVec.x = thirdVertWorldPos.uv.x * uvw.z;
                thirdVec.y = thirdVertWorldPos.uv.y * uvw.z;

                float2 uv = firstVec + secondVec + thirdVec;

                worldPosData[outputIndex].pos.xyz = resultPos;
                worldPosData[outputIndex].color = tex.SampleLevel(smp,uv,0);
            }

           worldPosData[outputIndex].baseColor = worldPosData[outputIndex].color;
        }
    }
    //パーティクルの配置--------------------------------------------
}