// TransformGizmo Pixel Shader

struct VSOut
{
  float4 position : SV_Position;
};

float4 main(VSOut input) : SV_TARGET
{
  return float4(1.0f, 0.0f, 0.0f, 1.0f);
}