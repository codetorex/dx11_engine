
Texture2D    txParticle : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos       : SV_POSITION;
	float Alpha : ALPHA0;
	float2 TexCoord  : TEXCOORD0;
};


float4 main( PS_INPUT input ) : SV_Target
{
	const float4 color = txParticle.Sample( samLinear, input.TexCoord );
	return float4(color.xyz, input.Alpha * ((color.x + color.y + color.z) / 3.0));
}
