
Texture2D gSourceMap : register(t0);
SamplerState samLinear : register(s0);


struct PS_INPUT
{
	float4 PosH  : SV_POSITION;
	float2 UV  : TEXTURE;
};

// single pass solution
float4 main2( PS_INPUT input ) : SV_Target
{
	float BlurScale = 0.002;
	float3 col = 0;
	for ( int i = -15; i <= 15; i++ )
	{
		for ( int j = -15; j <= 15; j++ )
		{
			float2 offset = float2(i, j)*BlurScale;
				col += gSourceMap.Sample( samLinear, input.UV + offset ).xyz;
		}
	}
	col /= 600;

	return float4(col.x, col.y, col.z, 1);
}

float4 main( PS_INPUT input ) : SV_Target
{
	float BlurScale = 0.002;
	float3 col = 0;
	for ( int j = -15; j <= 15; j++ )
	{
		float2 offset = float2(j, 0)*BlurScale;
			col += gSourceMap.Sample( samLinear, input.UV + offset ).xyz;
	}
	col /= 30;
	return float4(col, 1);
}