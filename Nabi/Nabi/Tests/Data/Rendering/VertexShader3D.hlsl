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
    float2 m_Texture  : TEXCOORD;
    float3 m_Normal   : NORMAL;
};

struct VSOutput
{
    float4 m_Position : SV_POSITION;
    float2 m_Texture  : TEXCOORD;
    float3 m_Normal   : NORMAL;
    
    float3 m_CameraWorldPosition : POSITION0;
    float3 m_WorldPosition : POSITION1;
};

VSOutput main(VSInput vsIn)
{
    VSOutput vsOut = (VSOutput) 0;
    float4 vertexPosition = float4(vsIn.m_Position, 1.0f);
    
    // Transform the vertex position into projection space
    vertexPosition = mul(vertexPosition, m_ModelMatrix);
    vertexPosition = mul(vertexPosition, m_ViewMatrix);
    vertexPosition = mul(vertexPosition, m_ProjectionMatrix);
    vsOut.m_Position = vertexPosition;
    
    // Set the texture
    vsOut.m_Texture = vsIn.m_Texture;
    
    // Work out stuff for lighting magic
    float4 vertexNormal = float4(vsIn.m_Normal, 0.0f);
    vsOut.m_Normal = mul(vertexNormal, m_ModelMatrix).xyz;
   
    float4 worldPosition = float4(vsIn.m_Position, 1.0f);
    vsOut.m_WorldPosition = mul(worldPosition, m_ModelMatrix).xyz;
    vsOut.m_CameraWorldPosition = m_CameraPosition;
    
    return vsOut;
}
