struct VS_INPUT
{
	float4 Pos       : POSITION0;
	float Size : SIZE0;
	float Alpha : ALPHA0;
};

struct VS_OPTPUT
{
	float4 Pos       : SV_POSITION;
	float Size : SIZE0;
	float Alpha : ALPHA0;
};

VS_OPTPUT main( VS_INPUT input )
{
	VS_OPTPUT output;

	output.Pos = input.Pos;
	output.Size = input.Size;
	output.Alpha = input.Alpha;

	return output;
}

