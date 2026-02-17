// PHONG PIXEL SHADER

static const float3 lightPosition = { 0.0f, 2.0f, 10.0f }; // Position of the light souce in WorldSpace
static const float3 materialColor = { 0.0f, 1.0f, 1.0f }; // Color of the fragment (For now non textures)

static const float3 ambient = { 0.15f, 0.15f, 0.15f };

// static const float3 diffuseColor = { 1.0f, 0.75f, 0.52f }; // Light Color
static const float3 diffuseColor      = { 1.0f, 1.0f, 1.0f }; // Light Color
static const float  diffuseIntensity  = 5.0f;              // Light intensity ,k

static const float attenuation_constant   = 1.00f;
static const float attenuation_linear     = 0.00f;
static const float attenuation_quadtraic  = 0.02f;

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
  // Vector from fragment to light source
  float3 L = normalize(lightPosition - input.worldPosition);
  
  // Normalize the vector normal (as it was interpolated across primitive)
  float3 N = normalize(input.norm);
  
  // Distance to light
  float distanceToLight = length(lightPosition - input.worldPosition);
  
  // Light attenuation
  float attenuation = 1.0f / (attenuation_constant + (attenuation_linear * distanceToLight) + (attenuation_quadtraic * (distanceToLight * distanceToLight)));
  
  float diffuse = attenuation * diffuseIntensity * max(0, dot(N, L));
  
  // Final pixel value
  float3 finalValue = diffuse * diffuseColor;
  
  float3 litMaterialColor = tex.Sample(smplr, input.tex).xyz * finalValue;
  
  return float4(litMaterialColor, 1.0f);
  // return tex.Sample(smplr, input.tex) * float4(finalValue, 1.0f);
  // return tex.Sample(smplr, input.tex);
}
