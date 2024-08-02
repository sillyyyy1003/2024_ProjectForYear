//Direction Light
struct DirectionLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float isEnable;
  
};

//Point Light
struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range;

    float3 att;
    float isEnable;
};

//Spotlight
struct SpotLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;

    float3 position;
    float range;

    float3 direction;
    float Spot;

    float3 att;
    float isEnable;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular; // w = SpecPower
    float4 emission;
};

/// @brief �������̌v�Z
/// @param mat �}�e���A��
/// @param L LightVector
/// @param normal �@���x�N�g��
/// @param toEye -CameraPos
/// @param ambient ����
/// @param diffuse ���C�g�F
/// @param spec ����
void ComputeDirectionalLight (Material mat, DirectionLight L, float3 normal, float3 toEye,
                              out float4 ambient,
                              out float4 diffuse,
                              out float4 spec)
{

    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);



    //���C�g�`�F�b�N
    if (L.isEnable == 0.0f)
    {
        return;
    }

	//�����v�Z
    ambient = mat.ambient * L.ambient;

    float3 lightVec = -L.direction;

	//���ˊp�x�v�Z
    float diffuseFactor = saturate(dot(lightVec, normal));

	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;

}

/// @brief �_���C�g�̌v�Z
/// @param mat �}�e���A��
/// @param L LightVector
/// @param pos �ʒu
/// @param normal �@���x�N�g��
/// @param toEye -CameraPos
/// @param ambient ����
/// @param diffuse ���C�g�F
/// @param spec ����
void ComputePointLight (Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
                        out float4 ambient, 
                        out float4 diffuse, 
                        out float4 spec)
{
   
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//���C�g�`�F�b�N
    if (L.isEnable == 0.0f)
    {
        return;
    }
    
	//�����v�Z
    ambient = mat.ambient * L.ambient;


    float3 lightVec = L.position - pos;

    //���C�g�͈͌v�Z�E��r
    float d = length(lightVec);
    if (d > L.range)
        return;

    //���K��
    lightVec =normalize(lightVec);

    //���ˊp�x�v�Z
    float diffuseFactor = saturate(dot(lightVec, normal));


	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;


    //�����v�Z
    float att = 1.0f / dot(L.att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}

/// @brief �X�|�b�g���C�g�̌v�Z
/// @param mat �}�e���A��
/// @param L LightVector
/// @param pos �s�N�Z���ʒu
/// @param normal �@���x�N�g��
/// @param toEye -CameraPos
/// @param ambient ����
/// @param diffuse ���C�g�F
/// @param spec ����
void ComputeSpotLight (Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
                       out float4 ambient, 
                       out float4 diffuse, 
                       out float4 spec)
{

    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//���C�g�`�F�b�N
    if (L.isEnable == 0.0f)
    {
        return;
    }

	//�����v�Z
    ambient = mat.ambient * L.ambient;

	//���C�g�͈͌v�Z�E��r
    float3 lightVec = L.position - pos;
    float d = length(lightVec);
    if (d > L.range)
        return;

	//���K��
    lightVec = normalize(lightVec);

    //���ˊp�x
    float diffuseFactor = saturate(dot(lightVec, normal));

	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;

    //�����v�Z
    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.Spot);
    float att = spot / dot(L.att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}