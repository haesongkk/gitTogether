#include <Shared.fxh>

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 vNormal = normalize(input.Norm);
    float3 vTangent = normalize(input.TangentWorld);
    float3 vBiTanget = cross(vNormal, vTangent);

    // ≥Î∏ª
    float3 vNormalTangentSpace = txNormal.Sample(samLinear, input.TexCoord).rgb * 2.0f - 1.0f;
    float3x3 WorldTransform = float3x3(vTangent, vBiTanget, vNormal);
    if (UseNormalMap)
    {
        vNormal = mul(vNormalTangentSpace, WorldTransform);
        vNormal = normalize(vNormal);
    }
    

    // Ω∫∆Â≈ß∑Ø
    float NdotL = max(dot(vNormal, -LightDirection), 0);
    float3 vView = normalize(EyePosition - input.WorldPos.xyz);
    
    float4 ambient = LightAmbient * MaterialAmbient;
    
    float4 texDiffuse = txDiffuse.Sample(samLinear, input.TexCoord);
    float4 diffuse = LightDiffuse * MaterialDiffuse * texDiffuse * NdotL;

    float3 vHalf = normalize(-LightDirection + vView);
    float SDot = max(dot(vNormal, vHalf), 0);

    float power = pow(SDot, MaterialSpecularPower);
    float4 specular = power * LightSpecular * MaterialSpecular;
    if (UseSpecularMap)
    {
        specular *= txSpecular.Sample(samLinear, input.TexCoord);
    }

    return diffuse + specular + ambient;

    
}