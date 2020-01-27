
Texture2D gSourceMap : register(t0);
SamplerState samLinear : register(s0);


struct PS_INPUT
{
	float4 PosH  : SV_POSITION;
	float2 UV  : TEXTURE;
};

float4 main( PS_INPUT input ) : SV_Target
{

	float BlurScale = 0.002;
	float3 col = 0;
	for ( int i = -15; i <= 15; i++ )
	{
		float2 offset = float2(0, i)*BlurScale;
			col += gSourceMap.Sample( samLinear, input.UV + offset ).xyz;
	}
	col /= 30;
	return float4(col, 1);
}