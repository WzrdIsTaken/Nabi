Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PSInput
{
    float4 m_Position : SV_POSITION;
    float2 m_Texture  : TEXCOORD;
    float3 m_Normal   : NORMAL;
};

float4 main(PSInput psIn) : SV_TARGET
{
    return Texture.Sample(Sampler, psIn.m_Texture);
}
