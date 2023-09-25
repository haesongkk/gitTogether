//***************************************************************************************
// wire.fx by LeHide (C) 2021 All Rights Reserved.
//
// Axes of world, grid등을 그리기 위한 와이어프레임용 쉐이더
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : NORMAL;
};

struct VertexOut
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : NORMAL;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout = (VertexOut) 0;
    
    vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);

    vout.Norm = mul(float4(vin.Norm, 1), gWorldViewProj).xyz;

    vout.Tex = vin.Tex;

    return vout;
    
	//VertexOut vout;
	
	//// Transform to homogeneous clip space.
 //   // 바깥쪽에서 월드-뷰-프로젝션 행렬을 받아서 동차절단좌표계 (아직 NDC아님)으로 변환해준다.
	//vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	//// Just pass vertex color into the pixel shader.
 //   // 헬퍼 오브젝트는 버텍스컬러에 의존해서 그리게 한다.
 //   vout.Color = vin.Color;
    
 //   return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    //pin.Color.r = 1.0f;
    //pin.Color.g = 0.0f;
    //pin.Color.b = 0.0f;

    //return pin.Color;
    
    float4 color = pin.Pos;
    //{
    //    1, 0, 0, 1
    //};
    return color; //txDiffuse.Sample(samLinear, pin.Tex); //pin.Color;

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
