
cbuffer cbNeverChanges : register(b0)
{
	float4 materialColour;
	float4 lightPos;
	float lightPower;
};

Texture2D    txDiffuse : register(t0);
Texture2D    txNormal : register(t1);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 pos		 : SV_POSITION;
	float4 posw	: POSITION;
	float3 normal : NORMAL;
	float2 tex		 : TEXCOORD0;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 main( PS_INPUT input ) : SV_Target
{
	float4 textureColor;
	float4 bumpMap;
	float3 bumpNormal;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	float3 lightDiff = lightPos - input.posw.xyz;
	float3 lightVector = normalize( lightDiff );
	float lightDist = length( lightDiff );
	float isq = lightPower / (lightDist * lightDist);

	// Sample the texture pixel at this location.
	textureColor = txDiffuse.Sample( samLinear, input.tex );

	// Sample the pixel in the bump map.
	bumpMap = txNormal.Sample( samLinear, input.tex );

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);

	// Normalize the resulting bump normal.
	bumpNormal = normalize( bumpNormal );

	// Invert the light direction for calculations.
	//lightDir = -lightVector;
	lightDir = lightVector;

	// Calculate the amount of light on this pixel based on the bump map normal value.
	lightIntensity = saturate( dot( bumpNormal, lightDir ) ) * isq;

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.

	//color = saturate( diffuseColor * 1.0f );
	float4 diffuseColor = float4(1, 1, 1, 1);
	color = saturate( diffuseColor * lightIntensity );

	// Combine the final bump light color with the texture color.
	color = color * textureColor;

	return color;
}