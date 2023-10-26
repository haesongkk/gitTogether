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
	int m_curKey = 0;
	float m_curTime = 0.f;

	float m_timer = 0.f;
	static int fps;
	Node* m_pConnectNode = nullptr;

	void Update();
	GameObject* m_pOwner;
};

