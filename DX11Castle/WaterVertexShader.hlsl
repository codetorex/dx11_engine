
cbuffer cbChangeOnResize : register(b0)
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
	matrix gWorld;
	matrix gWorldViewProj;
	float3 gEyePos;
	float Timer;
};

struct VS_INPUT
{
	float4 Position  : POSITION;
	float3 VecNormal : NORMAL;
	float2 UV       : TEXCOORD0;
};

struct VS_OPTPUT
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

#define NWAVES 2

struct Wave
{
	float freq;  // 2*PI / wavelength
	float amp;   // amplitude
	float phase; // speed * 2*PI / wavelength
	float2 dir;
};

static float BumpScale = 0.6;
static float TexReptX = 4.0;
static float TexReptY = 4.0;
static float BumpSpeedX = -0.002;
static float BumpSpeedY = 0.002;

static float2 TextureScale = float2(TexReptX, TexReptY);
static float2 BumpSpeed = float2(BumpSpeedX, BumpSpeedY);



// these are redundant, but makes the ui easier:

float WaveAmp = 0.06;
float WaveFreq = 2.0;

float evaluateWave( Wave w, float2 pos, float t )
{
	return w.amp * sin( dot( w.dir, pos )*w.freq + t*w.phase );
}

// derivative of wave functionWaveFreq
float evaluateWaveDeriv( Wave w, float2 pos, float t )
{
	return w.freq*w.amp * cos( dot( w.dir, pos )*w.freq + t*w.phase );
}


VS_OPTPUT main( VS_INPUT IN )
{
	VS_OPTPUT OUT = (VS_OPTPUT)0;
	Wave wave[NWAVES] = {
		{ WaveFreq, WaveAmp, 0.5, float2(-1, 0) },
		{ WaveFreq * 2, WaveAmp*0.5, 1.3, float2(-0.7, 0.7) }
	};
	float4 Po = float4(IN.Position.xyz, 1.0);
		// sum waves	
		//Po.y = 14.0;
	float ddx = 0.0, ddy = 0.0;
	for ( int i = 0; i<NWAVES; i++ )
	{
		Po.y += evaluateWave( wave[i], Po.xz, Timer );
		float deriv = evaluateWaveDeriv( wave[i], Po.xz, Timer );
		ddx += deriv * wave[i].dir.x;
		ddy += deriv * wave[i].dir.y;
	}
	// compute tangent basis
	float3 B = float3(1, ddx, 0);
		float3 T = float3(0, ddy, 1);
		float3 N = float3(-ddx, 1, -ddy);

		OUT.HPosition = mul( Po, gWorldViewProj );

	// pass texture coordinates for fetching the normal map
	OUT.UV = IN.UV.xy*TextureScale;
	float cycle = fmod( Timer, 100.0 );
	OUT.bumpUV0.xy = IN.UV.xy*TextureScale + cycle*BumpSpeed;
	OUT.bumpUV1.xy = IN.UV.xy*TextureScale*2.0 + cycle*BumpSpeed*4.0;
	OUT.bumpUV2.xy = IN.UV.xy*TextureScale*4.0 + cycle*BumpSpeed*8.0;

	// compute the 3x3 tranform from tangent space to object space
	float3x3 objToTangentSpace;
	// first rows are the tangent and binormal scaled by the bump scale
	objToTangentSpace[0] = BumpScale * normalize( T );
	objToTangentSpace[1] = BumpScale * normalize( B );
	objToTangentSpace[2] = normalize( N );

	OUT.T2WXf1.xyz = mul( objToTangentSpace, gWorld[0].xyz );
	OUT.T2WXf2.xyz = mul( objToTangentSpace, gWorld[1].xyz );
	OUT.T2WXf3.xyz = mul( objToTangentSpace, gWorld[2].xyz );

	// compute the eye vector (going from shaded point to eye) in cube space
	float3 Pw = mul( Po, gWorld ).xyz;
		OUT.WorldView = gEyePos - Pw; //eye vector
	return OUT;
}
