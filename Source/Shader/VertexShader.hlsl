cbuffer CBuf
{
  float4x4 transform;
  float4x4 viewProj;
};

struct VSOut
{
  float2 tex : TEXCOORD;
  float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 tex : TEXCOORD)
{
  VSOut vso;
  vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), mul(transform, viewProj));
  vso.tex = tex;
  return vso;
}