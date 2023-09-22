#include <Shared.fxh>

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 light = saturate(dot((float3) -LightDir, input.Norm) * LightColor);
    
    return txDiffuse.Sample(samLinear, input.Tex) * light;
}