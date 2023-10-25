#pragma once
class GameObject;
class Node;
class Animation
{
public:
	Animation(GameObject* _pOwner)
		:m_pOwner(_pOwner) { }
	struct Key
	{
		float time = 0.f;
		Vector3 position = { 0,0,0 };
		Quaternion rotation;
		Vector3 scale = { 1,1,1 };
	};
	vector<Key> m_keys;

	Node* m_pConnectNode;

	void Update();
	GameObject* m_pOwner;
};

