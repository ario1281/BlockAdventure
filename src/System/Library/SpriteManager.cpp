#pragma once
#include"define.h"
#include"SpriteManager.h"

void SpriteManager::Init() {
	D3DXCreateSprite(DEVICE, &m_lpSprite);
}

void SpriteManager::Release()
{
	SafeRelease(m_lpSprite);
}

void SpriteManager::DrawTex(const Matrix& mat, const TextureManager* tex, const RECT* rc, D3DCOLOR color)
{
	RECT rect;
	rc != nullptr ? rect = *rc : rect = tex->GetRect();

	m_lpSprite->SetTransform(&mat);
	m_lpSprite->Draw(
		tex->GetTex(),
		&rect,
		nullptr, nullptr, color);
}
