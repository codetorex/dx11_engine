
cbuffer cbNeverChanges : register(b0)
{
	float4 materialColour;
	float4 vecLight;
};


Texture2D    gNormMap : register(t0);
TextureCube gCubeMap: register(t1);

SamplerState gSampler : register(s0);


struct PS_INPUT
{
	float4 HPosition  : SV_POSITION;  // in clip space
	float2 UV  : TEXCOORD0;
	float3 T2WXf1 : TEXCOORD1; // first row of the 3x3 transform from tangent to cube space
	float3 T2WXf2 : TEXCOORD2; // second row of the 3x3 transform from tangent to cube space
	float3 T2WXf3 : TEXCOORD3; // third row of the 3x3 transform from tangent to cube space
	float2 bumpUV0 : TEXCOORD4;
	float2 bumpUV1 : TEXCOORD5;
	float2 bumpUV2 : TEXCOORD6;
	float3 WorldView  : TEXCOORD7;
};



float4 main( PS_INPUT IN ) : SV_Target
{
	float FresnelBias = 0.1;
	float FresnelExp = 2.0;
	float HDRMultiplier = 2.0;
	float3 DeepColor = { 0.0f, 0.0f, 0.1f };
	float3 ShallowColor = { 0.0f, 0.5f, 0.5f };
	float3 ReflTint = { 1.0f, 1.0f, 1.0f };

	float Kr = 1.0f;
	float KWater = 1.0f;


	float4 t0 = gNormMap.Sample( gSampler, IN.bumpUV0 )*2.0 - 1.0;
	float4 t1 = gNormMap.Sample( gSampler, IN.bumpUV1 )*2.0 - 1.0;
	float4 t2 = gNormMap.Sample( gSampler, IN.bumpUV2 )*2.0 - 1.0;
	float3 Nt = t0.xyz + t1.xyz + t2.xyz;

	float3x3 m; // tangent to world matrix
	m[0] = IN.T2WXf1;
	m[1] = IN.T2WXf2;
	m[2] = IN.T2WXf3;
	float3 Nw = mul( m, Nt );
	float3 Nn = normalize( Nw );

	// reflection
	float3 Vn = normalize( IN.WorldView );
	float3 R = reflect( -Vn, Nn );

	float4 reflection = gCubeMap.Sample( gSampler, R );


	//Fresnel
	float facing = max( dot( Vn, Nn ), 0 );

	float fres = Kr*(FresnelBias + (1.0 - FresnelBias)*pow( abs( facing ), abs( FresnelExp ) ));

	float3 waterColor = KWater * lerp( DeepColor, ShallowColor, facing );

	float3 result = (waterColor*(1 - fres)) + (fres * reflection.rgb);//* ReflTint

	float alpha = min( max( (length( IN.WorldView ) / 200.0f), 0.7 ), 0.98 );
	//float alpha = 1.0f;

	return float4(result.rgb, alpha);
}