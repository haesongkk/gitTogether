#include "framework.h"
#include "Material.h"

#include "Renderer.h"
#include "Helper.h"
#include "Model.h"

Material::Material(shared_ptr<Model> _spOwnerModel)
    :m_wpOwnerModel(_spOwnerModel)
{
}

Material::~Material()
{
    for (int i = 0; i < TextureIndex::End; i++)
        Helper::SafeRelease(m_pTextures[i]);
}

void Material::Run()
{
    ID3D11DeviceContext* dc = m_wpOwnerModel.lock()->m_wpRenderer.lock()->m_pDeviceContext;

    m_material.Diffuse = m_baseColor;

    for (int i = 0; i < TextureIndex::End; i++)
    {
        dc->PSSetShaderResources(i, 1, &m_pTextures[i]);
        m_using.UseMap[i] = static_cast<bool>(m_pTextures[i]);
    }

    if (m_using.UseMap[Opacity])
        dc->OMSetBlendState(m_pBlendState, nullptr, 0xffffffff);
    else
        dc->OMSetBlendState(nullptr, nullptr, 0xffffffff);

    dc->UpdateSubresource(m_pUsingBuffer, 0, nullptr, &m_using, 0, 0);
    dc->UpdateSubresource(m_pMaterialBuffer, 0, nullptr, &m_material, 0, 0);

    dc->PSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
    dc->PSSetConstantBuffers(3, 1, &m_pUsingBuffer);
    dc->VSSetConstantBuffers(2, 1, &m_pMaterialBuffer);
    dc->VSSetConstantBuffers(3, 1, &m_pUsingBuffer);

    dc->PSSetSamplers(0, 1, &m_pSamplerLinear);
}

void Material::CreateTextureFromFile(const wstring& _filePath, TextureIndex _txId)
{
    ID3D11Device* device = m_wpOwnerModel.lock()->m_wpRenderer.lock()->m_pDevice;

    CreateDDSTextureFromFile(device, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    if (!m_pTextures[_txId])
        CreateWICTextureFromFile(device, _filePath.c_str(), nullptr, &m_pTextures[_txId]);

    assert(m_pTextures[_txId]);
}