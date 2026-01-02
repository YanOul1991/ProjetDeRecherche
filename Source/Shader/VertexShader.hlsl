struct VSOutput
{
  float3 color  : Color;
  float4 pos    : SV_Position;
};

cbuffer CBuf
{
  matrix transform;
};

VSOutput main(float2 pos : Position, float3 color : Color)
{
  
  VSOutput vso;
  vso.color   = float3(color.r, color.g, color.b);
  //vso.pos     = float4(pos.x, pos.y, 0.0f, 1.0f);
  vso.pos     = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
  return vso;
}
