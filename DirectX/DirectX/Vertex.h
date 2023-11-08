#pragma once
class Vertex
{
public:
	Vector3 position;
	Vector2 tex;
	Vector3 norm;
	Vector3 tangent;

	int boneIndices[4] = {};
	float boneWeights[4] = {};

	bool AddBoneData(int _index, float _weight);

	Vertex() : position(Vector3()), norm(Vector3()), tex(Vector2()) { }

	Vertex(Vector3 position, Vector3 normal, Vector2 tex)
		: position(position), norm(normal), tex(tex) { }
	Vertex(Vector3 position, Vector3 normal, Vector2 tex, Vector3 tangent)
		: position(position), norm(normal), tex(tex), tangent(tangent) { }
};

