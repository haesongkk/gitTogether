#include "framework.h"
#include "Vertex.h"

bool Vertex::AddBoneData(int _index, float _weight)
{
    for (int i = 0; i < 4; i++)
    {
        if (boneWeights[i] == 0.f)
        {
            boneIndices[i] = _index;
            boneWeights[i] = _weight;
            return true;
        }
    }
    return false; // 정보 저장 실패
}