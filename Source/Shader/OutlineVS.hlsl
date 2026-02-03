// Outline Vertex Shader

cbuffer CBuf
{
  float4x4 transform;           // The transform of the vertex
  float4x4 lookAtMatrix;        // The viewprojection matrix
  float4x4 perspectiveMatrix;
};

struct VSOut
{
  float3 worldPosition : POSITION;
  float3 norm : NORMAL;
  float2 tex : TEXCOORD;
  float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 tex : TEXCOORD, float3 normal : NORMAL)
{
  VSOut vso;
  
  // World position of the vertices
  vso.worldPosition = mul(float4(pos, 1.0f), transform).xyz;
  
  // W = 0 -> no translation
  vso.norm = mul(float4(normal, 0), transform).xyz;
  
  vso.pos = mul(float4(pos + (normalize(normal) * 0.01f), 1.0f), mul(transform, mul(lookAtMatrix, perspectiveMatrix)));
  
  vso.tex = tex;
  
  return vso;
}