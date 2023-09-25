//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
	//float3 PosL  : POSITION;
    //float4 Color : COLOR;
    
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : NORMAL;
};

struct VertexOut
{
	//float4 PosH  : SV_POSITION;
    //float4 Color : COLOR;
    
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : NORMAL;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut) 0;
    
    vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
    //vout.Pos.z *= -1;
    vout.Norm = mul(float4(vin.Norm, 1), gWorldViewProj).xyz;

    vout.Tex = vin.Tex;

    return vout;
    
	//VertexOut vout;
	
	//// Transform to homogeneous clip space.
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	//// Just pass vertex color into the pixel shader.
 //   vout.Color = vin.Color;

 //   //vout.Color.x = 1.0f;
 //   //vout.Color.y = 0;
 //   //vout.Color.z = 0;
 //   //vout.Color.w = 1.0f;

 //   return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    //float4 test;
    //test.r = 1.0f;
    //test.g = 0.0f;
    //test.b = 1.0f;
    //test.a = 1.0f;
    //return test;

    //pin.Color.x = pin.PosH.z;
    //pin.Color.y = pin.PosH.z;
    //pin.Color.z = pin.PosH.z;

    //pin.Color.x = 0.0f;
    //pin.Color.y = 0.7f;
    //pin.Color.z = 0.0f;
    float4 color = pin.Pos * 0.5;
    //{
    //    1, 1, 0, 1
    //};
    //return color; //txDiffuse.Sample(samLinear, pin.Tex); //pin.Color;
    return txDiffuse.Sample(samLinear, pin.Tex);

}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
