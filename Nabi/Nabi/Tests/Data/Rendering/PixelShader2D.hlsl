Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PSInput
{
    float4 m_Position : SV_POSITION;
    float2 m_Texture  : TEXCOORD;
};

float4 main(PSInput psIn) : SV_TARGET
{
    float4 colour = Texture.Sample(Sampler, psIn.m_Texture);
    return colour;
}
