// PHONG PIXEL SHADER

struct VSOut
{
  float2 tex : TEXCOORD;
  float4 pos : SV_POSITION;
};

Texture2D tex : register(t0);
SamplerState smplr : register(s0);

float4 main(VSOut input) : SV_Target
{
  return tex.Sample(smplr, input.tex);
}
