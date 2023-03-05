struct VSInput
{
    float3 m_Position : POSITION;
    float2 m_Texture  : TEXCOORD;
};

struct VSOutput
{
    float4 m_Position : SV_POSITION;
    float2 m_Texture  : TEXCOORD;
};

VSOutput main(VSInput vsIn)
{
    VSOutput vsOut = (VSOutput) 0;
    vsOut.m_Position = float4(vsIn.m_Position.x, vsIn.m_Position.y, 0.0F, 1.0F);
    vsOut.m_Texture = vsIn.m_Texture;
    
    return vsOut;
}
