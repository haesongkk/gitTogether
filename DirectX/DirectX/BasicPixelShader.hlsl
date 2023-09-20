#include <Shared.fxh>

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = saturate(dot((float3) LightDir, input.Norm) * LightColor);
    finalColor.a = 1;

    return txDiffuse.Sample(samLinear, input.Tex) * finalColor;
}