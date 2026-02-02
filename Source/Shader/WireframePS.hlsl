// Wirframe Pixel Shader

struct VSOut
{
  float3 worldPosition  : POSITION;
  float3 norm           : NORMAL;
  float2 tex            : TEXCOORD;
  float3 camPosition    : CAMPOSITION;
  float4 pos            : SV_POSITION;
};

Texture2D tex : register(t0);
SamplerState smplr : register(s0);

float4 main(VSOut input) : SV_Target
{
  float dist = distance(input.pos.xyz, input.camPosition);
  float fade = saturate(1.0 - dist * 0.0002);
  return float4(1.0f, 1.0f, 1.0f, 1.0f) * fade;
}
