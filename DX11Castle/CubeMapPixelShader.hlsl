
TextureCube    txCube : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos		 : SV_POSITION;
	float3 vecNormal : NORMAL;
	float3 texCoord  : TEXCOORD;
};

float4 main( PS_INPUT input ) : SV_Target
{
	return txCube.Sample( samLinear, input.texCoord );
}
