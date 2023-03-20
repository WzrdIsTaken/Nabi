Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PSInput
{
    float4 m_Position : SV_POSITION;
    float2 m_UV : TEXCOORD;
};

float4 main(PSInput psIn) : SV_TARGET
{
    float4 pixelColour = Texture.Sample(Sampler, psIn.m_UV);
    clip(pixelColour.a < 0.1F ? -1 : 1); 
    
    return pixelColour;
}
