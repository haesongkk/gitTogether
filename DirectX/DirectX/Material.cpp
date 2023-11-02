#include "framework.h"
#include "Material.h"

#include "Renderer.h"
#include "Helper.h"

Renderer* Material::pRenderer = nullptr;

Material::Material(Model* _pOwner)
    :m_pOwner(_pOwner), m_pTextures{}
{
}

Material::~Material()
{
    for (int i = 0; i < TextureIndex::End; i++)
        Helper::SafeRelease(m_pTextures[i]);
}

void Material::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;

    pRenderer->m_material.Diffuse = m_baseColor;

    for (int i = 0; i < TextureIndex::End; i++)
    {
        dc->PSSetShaderResources(i, 1, &m_pTextures[i]);
    }

    pRenderer->m_using.UsingDiffuseMap = m_pTextures[TextureIndex::Diffuse];
    pRenderer->m_using.UsingNormalMap = m_pTextures[TextureIndex::Normal];
    pRenderer->m_using.UsingSpecularMap = m_pTextures[TextureIndex::Specular];
    pRenderer->m_using.UsingEmissiveMap = m_pTextures[TextureIndex::Emissive];
    pRenderer->m_using.UsingOpacityMap = m_pTextures[TextureIndex::Opacity];

    if (pRenderer->m_using.UsingOpacityMap)
        dc->OMSetBlendState(pRenderer->m_pAlphaBlendState, nullptr, 0xffffffff);
    else
        dc->OMSetBlendState(nullptr, nullptr, 0xffffffff);

    dc->UpdateSubresource(pRenderer->m_pUsingBuffer, 0, nullptr, &(pRenderer->m_using), 0, 0);
    dc->UpdateSubresource(pRenderer->m_pMaterialBuffer, 0, nullptr, &(pRenderer->m_material), 0, 0);

    dc->PSSetConstantBuffers(2, 1, &(pRenderer->m_pMaterialBuffer));
    dc->PSSetConstantBuffers(3, 1, &(pRenderer->m_pUsingBuffer));
    dc->VSSetConstantBuffers(2, 1, &(pRenderer->m_pMaterialBuffer));
    dc->VSSetConstantBuffers(3, 1, &(pRenderer->m_pUsingBuffer));

    dc->PSSetSamplers(0, 1, &(pRenderer->m_pSamplerLinear));
}

void Material::CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId)
{
    CreateDDSTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    if (!m_pTextures[_txId])
        CreateWICTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    assert(m_pTextures[_txId]);
}