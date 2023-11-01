#include <Shared.fxh>
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = mul(input.Pos, World);

    float4x4 weightBlended;
    weightBlended = mul(input.BoneWeights.x, BoneMatrixPalletes[input.BoneIndicies.x]);
    weightBlended += mul(input.BoneWeights.y, BoneMatrixPalletes[input.BoneIndicies.y]);
    weightBlended += mul(input.BoneWeights.z, BoneMatrixPalletes[input.BoneIndicies.z]);
    weightBlended += mul(input.BoneWeights.w, BoneMatrixPalletes[input.BoneIndicies.w]);

    output.Pos = mul(output.Pos, weightBlended);
    
    output.WorldPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    output.Norm = mul(float4(input.Norm, 1), World).xyz;

    output.TexCoord = input.Tex;

    output.Norm = normalize(mul(input.Norm, (float3x3)World));

    output.TangentWorld = normalize(mul(input.TangentModel, (float3x3) World));

    return output;
}