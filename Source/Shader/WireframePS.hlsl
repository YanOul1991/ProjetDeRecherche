// Wirframe Pixel Shader

struct VSOut
{
  float3 worldPosition : POSITION;
  float3 norm : NORMAL;
  float2 tex : TEXCOORD;
  float4 pos : SV_POSITION;
};

Texture2D tex : register(t0);
SamplerState smplr : register(s0);

float4 main(VSOut input) : SV_Target
{
  return float4(0.15f, 0.15f, 0.15f, 1.0f);
}
