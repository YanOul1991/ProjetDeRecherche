// struct VSOutput
// {
//   float3 color  : Color;
//   float4 pos    : SV_Position;
// };

cbuffer CBuf
{
  matrix transform;
};

float4 main(float3 pos : Position) : SV_Position
{
  
  // VSOutput vso;
  // vso.color   = float3(color.r, color.g, color.b);
  //vso.pos     = float4(pos.x, pos.y, 0.0f, 1.0f);
  //vso.pos     = mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
  return mul(float4(pos.x, pos.y, pos.z, 1.0f), transform);
}
