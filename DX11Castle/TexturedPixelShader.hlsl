
cbuffer cbNeverChanges : register(b0)
{
	float4 materialColour;
	float4 lightPos;
	float lightPower;
	bool lightUseDist;
};


Texture2D    txDiffuse : register(t0);
SamplerState samLinear : register(s0);


struct PS_INPUT
{
	float4 Pos		 : SV_POSITION;
	float4 PosW		 : POSITION;
	float3 vecNormal : NORMAL;
	float2 Tex		 : TEXCOORD0;
};


float4 main2( PS_INPUT input ) : SV_Target
{
	return txDiffuse.Sample( samLinear, input.Tex ) * materialColour;
}

float4 main( PS_INPUT input ) : SV_Target
{
	float3 lightDiff = lightPos - input.PosW.xyz;


	float3 lightVector = normalize( lightDiff );
	float lightDist = length( lightDiff );

	float isq = lightPower / (lightDist * lightDist);
	float4 light = saturate( dot( lightVector, input.vecNormal ) );
	if ( lightUseDist )
	{
		light *= isq;
	}

	float4 diffuse = txDiffuse.Sample( samLinear, input.Tex );
	return  float4(diffuse.rgb * materialColour * light, diffuse.a);
}