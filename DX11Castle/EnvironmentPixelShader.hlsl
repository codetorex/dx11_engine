
cbuffer cbNeverChanges : register(b0)
{
	float4 materialColour;
	float4 vecLight;
};

TextureCube    environmentMap : register(t0);
SamplerState samLinear : register(s0);


struct PS_INPUT
{
	float4 Pos		 : SV_POSITION;
	float3 vecNormal : NORMAL;
	float2 Tex		 : TEXCOORD0;
	float3 R		 : TEXCOORD1;
};


float4 main( PS_INPUT input ) : SV_Target
{
	float4 reflectedColor = environmentMap.Sample( samLinear, normalize( input.R ) );
	float4 light = saturate( dot( vecLight, input.vecNormal ) + 0.5f ) + 0.2f;
	return reflectedColor * light;
}