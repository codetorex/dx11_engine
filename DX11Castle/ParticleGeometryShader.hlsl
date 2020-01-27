struct GS_INPUT
{
	float4 Pos       : SV_POSITION;
	float Size : SIZE0;
	float Alpha : ALPHA0;
};

struct GS_OPTPUT
{
	float4 Pos       : SV_POSITION;
	float Alpha : ALPHA0;
	float2 TexCoord  : TEXCOORD0;
};

cbuffer cbChangesEveryFrame : register(b0)
{
	vector gEyePos;
	matrix MatWorldViewProjection;
};

[maxvertexcount( 6 )]
void main(
	point GS_INPUT input[1],
	inout TriangleStream< GS_OPTPUT > output
	)
{
	const float halvedSize = input[0].Size;
	float4 projectionPos = input[0].Pos;

	float3 look = normalize( projectionPos.xyz - gEyePos.xyz );

	float3 right = normalize( cross( float3(0, 1, 0), look ) ) * halvedSize;
	float3 up = normalize( cross( look, right ) ) * halvedSize;

	GS_OPTPUT topLeft;
	topLeft.Pos = mul( float4(projectionPos.xyz - right + up, 1), MatWorldViewProjection );
	topLeft.Alpha = input[0].Alpha;
	topLeft.TexCoord = float2(0.0f, 1.0f);

	GS_OPTPUT topRight;
	topRight.Pos = mul( float4(projectionPos.xyz + right + up, 1), MatWorldViewProjection );
	topRight.Alpha = input[0].Alpha;
	topRight.TexCoord = float2(1.0f, 1.0f);

	GS_OPTPUT bottomLeft;
	bottomLeft.Pos = mul( float4(projectionPos.xyz - right - up, 1), MatWorldViewProjection );
	bottomLeft.Alpha = input[0].Alpha;
	bottomLeft.TexCoord = float2(0.0f, 0.0f);

	GS_OPTPUT bottomRight;
	bottomRight.Pos = mul( float4(projectionPos.xyz + right - up, 1), MatWorldViewProjection );
	bottomRight.Alpha = input[0].Alpha;
	bottomRight.TexCoord = float2(1.0f, 0.0f);

	output.Append( bottomLeft );
	output.Append( topLeft );
	output.Append( topRight );

	output.Append( bottomLeft );
	output.Append( topRight );
	output.Append( bottomRight );
}