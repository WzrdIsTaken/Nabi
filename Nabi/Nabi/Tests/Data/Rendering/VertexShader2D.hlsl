cbuffer PerFrame : register(b0)
{
    float4x4 m_ViewMatrix;
    float4x4 m_ProjectionMatrix;
    float3 m_CameraPosition;
};

cbuffer PerMesh : register(b1)
{
    float4x4 m_ModelMatrix;
}

struct VSInput
{
    float3 m_Position : POSITION;
    float2 m_UV       : TEXCOORD;
};

struct VSOutput
{
    float4 m_Position : SV_POSITION;
    float2 m_UV       : TEXCOORD;
};

VSOutput main(VSInput vsIn)
{
    VSOutput vsOut = (VSOutput) 0;
    float4 vertexPosition = float4(vsIn.m_Position.x, vsIn.m_Position.y, 0.0f, 1.0f);
    
    // Transform the vertex position into projection space
    vertexPosition = mul(vertexPosition, m_ModelMatrix);
    vertexPosition = mul(vertexPosition, m_ViewMatrix);
    vertexPosition = mul(vertexPosition, m_ProjectionMatrix);
    vsOut.m_Position = vertexPosition;
    
    // Set the texture
    vsOut.m_UV = vsIn.m_UV;
    
    return vsOut;
}
