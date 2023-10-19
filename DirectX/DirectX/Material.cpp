#include "framework.h"
#include "Material.h"

#include "Renderer.h"
#include "Helper.h"

Renderer* Material::pRenderer = nullptr;

Material::Material(GameObject* _pOwner)
    :m_pOwner(_pOwner)
{
}

void Material::Init()
{
}

void Material::Render()
{
    ID3D11DeviceContext* dc = pRenderer->m_pDeviceContext;
    if (!m_pTextures[TextureIndex::Opacity])
        m_pTextures[TextureIndex::Opacity] = m_pTextures[TextureIndex::Diffuse];
    for (int i = 0; i < TextureIndex::End; i++)
    {
        if (!m_pTextures[i])
        {
            if (i == TextureIndex::Emissive)
                pRenderer->m_using.UsingEmissiveMap = false;
        }
        dc->PSSetShaderResources(i, 1, &m_pTextures[i]);
    }
    dc->UpdateSubresource(pRenderer->m_pUsingBuffer, 0, nullptr, &pRenderer->m_using, 0, 0);

    dc->PSSetConstantBuffers(3, 1, &pRenderer->m_pUsingBuffer);
    dc->VSSetConstantBuffers(3, 1, &pRenderer->m_pUsingBuffer);
}

void Material::Final()
{
    for (int i = 0; i < TextureIndex::End; i++)
        Helper::SafeRelease(m_pTextures[i]);
}

void Material::CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId)
{
    CreateDDSTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    if (!m_pTextures[_txId])
        CreateWICTextureFromFile(pRenderer->m_pDevice, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    assert(m_pTextures[_txId]);
}