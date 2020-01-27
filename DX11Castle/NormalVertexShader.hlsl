
cbuffer cbChangeOnResize : register(b0)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
	matrix MatWorld;
	matrix MatWorldViewProjection;
};

struct VS_INPUT
{
	float4 pos		 : POSITION;
	float3 normal : NORMAL;
	float2 tex		 : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct VS_OUTPUT
{
	float4 pos		 : SV_POSITION;
	float4 posw		 : POSITION;
	float3 normal : NORMAL;
	float2 tex		 : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(input.pos, MatWorldViewProjection);
	output.posw = input.pos;
	output.tex = input.tex;
	output.tangent = mul( input.tangent, (float3x3)MatWorld );
	output.binormal = mul( input.binormal, (float3x3)MatWorld );
	output.normal = mul(input.normal, (float3x3)MatWorld);
	return output;
}
