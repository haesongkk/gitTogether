#pragma once
#include "Component.h"
class Transform;
class Renderer;
class Render : public Component
{
protected:
	struct CB_WorldTM
	{
		Matrix worldTM;
	} m_worldTM;

	ID3D11Buffer* m_pWorldTMBuffer;

	struct Mesh
	{
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;

		UINT vertextBufferStride = 0;
		UINT vertextBufferOffset = 0;
		int indexCount = 0;

		void Update();
	};
	wpVector<Mesh> m_wpMeshes;

	weak_ptr<Transform> m_wpOwnerTransform;
	static weak_ptr<Renderer> ms_wpRenderer;
	static ID3D11DeviceContext* dc;

public:
	static void SetRenderer(shared_ptr<Renderer> _spRenderer) { ms_wpRenderer = _spRenderer; }

public:
	virtual void Run();

};

