#pragma once
class Model;
class Renderer;
class Material
{
	weak_ptr<Model> m_wpOwnerModel;

	struct CB_Material
	{
		Vector4 Ambient = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 Diffuse = { 1.0f,1.0f,1.0f,1.0f };
		Vector4 Specular = { 1.0f,1.0f,1.0f,1.0f };
		float  SpecularPower = 200;
		Vector3 pad0;
	} m_material;

	struct CB_Using
	{
		BOOL UseMap[5] = { true, true, true, true, true };
		Vector3 padding5;
	} m_using;

	ID3D11Buffer* m_pMaterialBuffer = nullptr;
	ID3D11Buffer* m_pUsingBuffer = nullptr;
	ID3D11BlendState* m_pBlendState = nullptr;
	ID3D11SamplerState* m_pSamplerLinear = nullptr;

public:
	enum TextureIndex
	{
		Diffuse,
		Normal,
		Specular,
		Emissive,
		Opacity,
		End,
	};
	ID3D11ShaderResourceView* m_pTextures[TextureIndex::End];
	Vector4 m_baseColor = { };

public:
	Material(shared_ptr<Model> _spOwnerModel);

public:
	~Material();

public:
	void Run();

public:
	void CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId);
};