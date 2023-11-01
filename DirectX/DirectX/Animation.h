#pragma once
class Model;
class Node;
class Animation
{
public:
	struct Key
	{
		float time = 0.f;
		Vector3 position = { 0,0,0 };
		Quaternion rotation;
		Vector3 scale = { 1,1,1 };
	};
	vector<Key> m_keys;
	Node* m_pConnectNode = nullptr;

	Animation(Model* _pOwner)
		:m_pOwner(_pOwner) { }
	Model* m_pOwner;

	void Update();

	static int fps;

private:
	int m_curKey = 0;
	float m_timer = 0.f;

};

