Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct Light
{
    float3 m_Position;
    float3 m_Direction;
    float3 m_Colour;
    
    float m_Intensity;
    float m_SpotAngle;
    float m_AttenuationRadius;
    
    float m_AmbientIntensity;
    float m_DiffuseIntensity;
    float m_SpecularAttenuation;
    float m_SpecularIntensity;
};

cbuffer PerLightChange : register(b0)
{
    Light m_DirectionalLight;
};

struct PSInput
{
    float4 m_Position : SV_POSITION;
    float2 m_Texture  : TEXCOORD;
    float3 m_Normal   : NORMAL;
    
    float3 m_CameraWorldPosition : POSITION0;
    float3 m_WorldPosition : POSITION1;
};

float4 main(PSInput psIn) : SV_TARGET
{
    float3 diffuseLight = float3(0, 0, 0);
    float3 specularLight = float3(0, 0, 0);
    
    // Ambient
    const float ambientStrength = 0.1f;
    float3 ambientLight = m_DirectionalLight.m_Colour * ambientStrength * m_DirectionalLight.m_AmbientIntensity;
    
     // Diffuse
    float diffuseFactor = dot(normalize(psIn.m_Normal), -m_DirectionalLight.m_Direction);
    if (diffuseFactor > 0)
    {
        diffuseLight = m_DirectionalLight.m_Colour * diffuseFactor * m_DirectionalLight.m_DiffuseIntensity;

		// Specular
        float3 vertexToCamera = normalize(psIn.m_CameraWorldPosition - psIn.m_WorldPosition);
        float3 lightReflect = normalize(reflect(m_DirectionalLight.m_Direction, psIn.m_Normal));
        float specularFactor = dot(vertexToCamera, lightReflect);

        if (specularFactor > 0)
        {
            specularFactor = pow(specularFactor, m_DirectionalLight.m_SpecularAttenuation);
            specularLight = m_DirectionalLight.m_Colour * specularFactor * m_DirectionalLight.m_SpecularIntensity;
        }
    }
    
    float4 lighting = float4(ambientLight + diffuseLight + specularLight, 1.0f) * m_DirectionalLight.m_Intensity;
    return Texture.Sample(Sampler, psIn.m_Texture) * lighting;
}
