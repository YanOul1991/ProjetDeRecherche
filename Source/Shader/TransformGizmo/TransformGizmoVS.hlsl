// TransformGizmo Vertex Shader

cbuffer CBuf : register(b0)
{
  float4x4 worldTransform;
  float4x4 viewMatrix;
  float4x4 projectionMatrix;
};

struct VSOut
{
  float4 color      : COLOR;
  float4 position : SV_Position;
};

VSOut main(float3 pos : POSITION, float4 color : COLOR)
{
  VSOut output;
  
  output.position = mul(float4(pos, 1.0f),mul(worldTransform, mul(viewMatrix, projectionMatrix)));
  output.color = color;
  
  return output;
}