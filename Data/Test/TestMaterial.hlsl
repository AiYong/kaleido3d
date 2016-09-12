cbuffer cbPerObjectVS : register(b0)
{
	float4x4 WorldViewProjection	: packoffset(c0);
	float4x4 World					: packoffset(c4);
};

struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal: NORMAL;
};

VS_OUTPUT RenderSceneVS(in VS_INPUT input)
{
	VS_OUTPUT output;
	float3 normalWorldSpace;
	output.Position = mul(input.Position, WorldViewProjection);

	return output;
}

