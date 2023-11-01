#pragma once
class Model;
class Renderer;
class Material
{
public:
	static Renderer* pRenderer;
	Model* m_pOwner;

	enum TextureIndex
	{
		Diffuse, Normal, Specular, Emissive, Opacity, End,
	};

	ID3D11ShaderResourceView* m_pTextures[TextureIndex::End];
	Vector4 m_baseColor = { };


public:
	Material(Model* _pOwner);
	~Material();
	void Render();

	void CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId);
};