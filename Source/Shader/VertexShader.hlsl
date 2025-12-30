struct VSOutput
{
  float3 color : Color;
  float4 pos : SV_Position;
};

cbuffer CBuf
{
  row_major matrix transform;
};

VSOutput main(float2 pos : Position, float3 color : Color)
{
  VSOutput vso;
  vso.color   = float3(color.r, color.g, color.b);
  // vso.pos     = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
  vso.pos     = float4(pos.x, pos.y, 0.0f, 1.0f);
  return vso;
}
