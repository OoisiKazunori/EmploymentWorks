#include"../ShaderHeader/GPUParticleHeader.hlsli"

TwoRender PSmain(PosUvOutput input) : SV_TARGET
{
    TwoRender output;
    output.target0 = matrixData[input.id].color;
    output.target1 = float4(0.0f,0.0f,0.0f,1.0f);

    return output;
}