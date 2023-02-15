#include"../ShaderHeader/KazMathHeader.hlsli"
#include"../ShaderHeader/GPUParticle.hlsli"

//����
RWStructuredBuffer<ParticleData> updateParticleData : register(u0);
RWStructuredBuffer<matrix> motherMatData : register(u1);
RWStructuredBuffer<float4> larpColorData : register(u2);
//�o��
AppendStructuredBuffer<GPUParticleInput> inputGPUParticleData : register(u3);

cbuffer RootConstants : register(b0)
{    
    matrix scaleRotaBillBoardMat;
    //X...�ꊇ���点�AY...�����Ō��点��
    uint2 lightData;
    float alpha;
};

[numthreads(1024, 1, 1)]
void CSmain(uint3 groupId : SV_GroupID, uint groupIndex : SV_GroupIndex,uint3 groupThreadID : SV_GroupThreadID)
{
    uint index = groupThreadID.x;
    index += 1024 * groupId.x;

    if(1500000 < index)
    {
        return;
    }


    ParticleData particleData = updateParticleData[index];
    float4 color = particleData.color;

    //�F�o��------------------------------------------------------------------
    bool TopFlag = particleData.pos.x <= lightData.y + 2.0f;
    bool ButtonFlag = lightData.y - 2.0f <= particleData.pos.x;
    //�����ɂ����点
    if(TopFlag && ButtonFlag)
    {
        color = float4(1,1,1,1);
    }
    //�ꊇ�Ō��点��
    else if(lightData.x)
    {
        color = float4(1,1,1,1);
    }
    else
    {
        color = float4(0.5,0.5,0.5,1.0);
    }
    color.a = alpha;
    //�F�o��------------------------------------------------------------------


    matrix worldMat = scaleRotaBillBoardMat;
    worldMat[0][3] = particleData.pos.x;
    worldMat[1][3] = particleData.pos.y;
    worldMat[2][3] = particleData.pos.z;
    worldMat = mul(motherMatData[particleData.id],worldMat);


    GPUParticleInput inputData;
    inputData.worldMat = worldMat;

    larpColorData[index] = Larp(color,larpColorData[index],0.1f);
    inputData.color = larpColorData[index];
    inputGPUParticleData.Append(inputData);
}