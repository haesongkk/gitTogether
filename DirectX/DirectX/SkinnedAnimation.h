#pragma once
#include "Render.h"
class Mesh;
class Material;
class Animation;
class Node;
class Renderer;
class Transform;
class SkinnedAnimation : public Render
{
	wpVector<Material> m_wpMaterials;
	wpVector<Node> m_wpNodes;

	map<string, wpVector<Animation>> m_wpAnimations;
	string m_curAnim;

	weak_ptr<Node> m_wpRootNode;

public:
	virtual void Run();



	shared_ptr<Transform> GetTransform() { return m_wpOwnerTransform.lock(); }

	friend class FbxLoader;
};