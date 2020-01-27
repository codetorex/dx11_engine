
cbuffer cbChangeOnResize : register(b0)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
	matrix MatWorld;
	matrix MatWorldViewProjection;
	matrix WorldInverseTranspose;
	vector cameraPos;
};

struct VS_INPUT
{
	float4 Pos       : POSITION;
	float3 VecNormal : NORMAL;
	float2 Tex       : TEXCOORD0;
};

struct VS_OPTPUT
{
	float4 Pos		 : SV_POSITION;
	float3 VecNormal : NORMAL;
	float2 Tex		 : TEXCOORD0;
	float3 R		 : TEXCOORD1;
};


VS_OPTPUT main( VS_INPUT input )
{
	VS_OPTPUT output;

	output.Pos = mul( input.Pos, MatWorldViewProjection );
	output.VecNormal = normalize( mul( input.VecNormal, (float3x3)MatWorld ) );
	output.Tex = input.Tex;

	float4 P = mul( input.Pos, MatWorld );
	float3 I = cameraPos - P;

	float3 N = normalize( mul( input.VecNormal, WorldInverseTranspose ) );
	output.R = reflect( -normalize( I ), N );

	return output;
}
