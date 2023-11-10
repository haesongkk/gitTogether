#include "framework.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"
#include "Node.h"
#include "Animation.h"
#include "Bone.h"
#include "GameObject.h"

Model::Model()
    :Component(make_shared<GameObject>(nullptr))
    , m_wpOwnerTransform{}
    , m_wpMaterials{}
    , m_wpMeshes{}
    , m_wpNodes{}
    , m_wpAnimations{}
    , m_curAnim{}
    , m_wpRootNode{}
{

}

Model::~Model()
{
}

void Model::Run()
{
    for (auto anim : m_wpAnimations[m_curAnim])
        if (auto a = anim.lock()) a->Update();

    if(auto root = m_wpRootNode.lock()) root->Run();
}

shared_ptr<Node> Model::GetNode(string name)
{
    for (auto node : m_wpNodes)
        if (node.lock()->GetName() == name)
            return node.lock();
    return nullptr;
}


