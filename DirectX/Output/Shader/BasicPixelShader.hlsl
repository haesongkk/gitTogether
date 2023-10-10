#include <Shared.fxh>

float4 main(PS_INPUT input) : SV_TARGET
{
    float normal = normalize(input.Norm);
    
    float3 viewDir = normalize(EyePosition - input.WorldPos.xyz);
    float3 specDir = normalize(-LightDirection + viewDir);

    float LDot = max(dot(normal, -LightDirection), 0);
    float SDot = max(dot(specDir, normal), 0);

    float4 texDiffuse = txDiffuse.Sample(samLinear, input.Tex);


    float4 ambient = LightAmbient * MaterialAmbient;
    float4 diffuse = LightDiffuse * MaterialDiffuse 
        * texDiffuse * LDot;
    float4 specular = pow(SDot, MaterialSpecularPower) 
        * LightSpecular * MaterialSpecular;

    
    float4 finalColor = diffuse + specular + ambient;
    return finalColor;
}