#pragma once
class GameObject;
class Renderer;
class Material
{
public:
	static Renderer* pRenderer;
	GameObject* m_pOwner;

	enum TextureIndex
	{
		Diffuse, Normal, Specular, Emissive, Opacity, End,
	};

	ID3D11ShaderResourceView* m_pTextures[TextureIndex::End];

public:
	Material(GameObject* _pOwner);
	void Init();
	void Render();
	void Final();

	void CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId);
};