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

/// @brief 方向光の計算
/// @param mat マテリアル
/// @param L LightVector
/// @param normal 法線ベクトル
/// @param toEye -CameraPos
/// @param ambient 環境光
/// @param diffuse ライト色
/// @param spec 鏡面
void ComputeDirectionalLight (Material mat, DirectionLight L, float3 normal, float3 toEye,
                              out float4 ambient,
                              out float4 diffuse,
                              out float4 spec)
{

    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);



    //ライトチェック
    if (L.isEnable == 0.0f)
    {
        return;
    }

	//環境光計算
    ambient = mat.ambient * L.ambient;

    float3 lightVec = -L.direction;

	//反射角度計算
    float diffuseFactor = saturate(dot(lightVec, normal));

	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;

}

/// @brief 点ライトの計算
/// @param mat マテリアル
/// @param L LightVector
/// @param pos 位置
/// @param normal 法線ベクトル
/// @param toEye -CameraPos
/// @param ambient 環境光
/// @param diffuse ライト色
/// @param spec 鏡面
void ComputePointLight (Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
                        out float4 ambient, 
                        out float4 diffuse, 
                        out float4 spec)
{
   
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//ライトチェック
    if (L.isEnable == 0.0f)
    {
        return;
    }
    
	//環境光計算
    ambient = mat.ambient * L.ambient;


    float3 lightVec = L.position - pos;

    //ライト範囲計算・比較
    float d = length(lightVec);
    if (d > L.range)
        return;

    //正規化
    lightVec =normalize(lightVec);

    //反射角度計算
    float diffuseFactor = saturate(dot(lightVec, normal));


	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;


    //減衰計算
    float att = 1.0f / dot(L.att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}

/// @brief スポットライトの計算
/// @param mat マテリアル
/// @param L LightVector
/// @param pos ピクセル位置
/// @param normal 法線ベクトル
/// @param toEye -CameraPos
/// @param ambient 環境光
/// @param diffuse ライト色
/// @param spec 鏡面
void ComputeSpotLight (Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
                       out float4 ambient, 
                       out float4 diffuse, 
                       out float4 spec)
{

    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//ライトチェック
    if (L.isEnable == 0.0f)
    {
        return;
    }

	//環境光計算
    ambient = mat.ambient * L.ambient;

	//ライト範囲計算・比較
    float3 lightVec = L.position - pos;
    float d = length(lightVec);
    if (d > L.range)
        return;

	//正規化
    lightVec = normalize(lightVec);

    //反射角度
    float diffuseFactor = saturate(dot(lightVec, normal));

	float3 v = reflect(-lightVec, normal);
	float specFactor = pow(max(dot(v, toEye), 0.0f), mat.specular.w);

	diffuse = diffuseFactor * mat.diffuse * L.diffuse;
	spec = specFactor * mat.specular * L.specular;

    //減衰計算
    float spot = pow(max(dot(-lightVec, L.direction), 0.0f), L.Spot);
    float att = spot / dot(L.att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}