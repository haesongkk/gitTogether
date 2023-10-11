#include <Shared.hlsli>

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 vNormal = normalize(input.Norm);
    float NdotL = max(dot(vNormal, -LightDirection), 0);
    float3 vView = normalize(EyePosition - input.WorldPos.xyz);
    
    float4 ambient = LightAmbient * MaterialAmbient;
    
    float4 texDiffuse = txDiffuse.Sample(samLinear, input.Tex);
    float4 diffuse = LightDiffuse * MaterialDiffuse * texDiffuse * NdotL;

    float3 vHalf = normalize(-LightDirection + vView);
    float SDot = max(dot(vNormal, vHalf), 0);

    float power = pow(SDot, MaterialSpecularPower);
    float4 specular = power * LightSpecular * MaterialSpecular;


    return diffuse + specular + ambient;

    
}