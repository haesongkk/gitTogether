#include "framework.h"
#include "Bone.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"

Bone::Bone(int boneIndex, shared_ptr<Mesh> wpOwnerMesh, string connectedNodeName, Matrix offsetMatrix)
	:m_boneIndex{boneIndex}
	, m_wpOwnerMesh{wpOwnerMesh}
	, m_connectedNodeName{connectedNodeName}
	, m_offsetMatrix{offsetMatrix}
{
}

void Bone::Run()
{
	auto connectedNode = m_wpOwnerMesh.lock()->GetOwnerModel()->GetNode(m_connectedNodeName);

	assert(connectedNode);
	Matrix matrix = m_offsetMatrix * connectedNode->GetMatrix();
	m_wpOwnerMesh.lock()->BonePallete(m_boneIndex, matrix.Transpose());
}
