
struct VS_INPUT
{
	float3 PosL  : POSITION;
};

struct VS_OPTPUT
{
	float4 PosH  : SV_POSITION;
	float2 UV  : TEXTURE;
};

VS_OPTPUT main( VS_INPUT input )
{

	VS_OPTPUT vout = (VS_OPTPUT)0;

	// Transform to homogeneous clip space.
	vout.PosH = float4(input.PosL, 1.0f);
	vout.UV = (input.PosL.xy + 1) / 2;
	vout.UV.y = 1 - vout.UV.y;
	return vout;

}
