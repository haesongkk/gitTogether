#include "framework.h"
#include "Bone.h"
#include "Node.h"
#include "Mesh.h"
#include "Model.h"

void Bone::SetConnectNode(string name)
{
	for (auto node : m_pOwner->m_pOwner->m_pNodes)
		if (node->m_name == name)
			m_pConnectNode = node;
}

void Bone::Update()
{
	SetConnectNode(m_name);
	//m_pConnectNode = m_pOwner->m_pParentNode;
	if(m_pConnectNode)
		m_matrix = m_pConnectNode->m_worldMatrix;
}
