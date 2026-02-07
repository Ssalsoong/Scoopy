#include "Export.h"
#include "ScriptBehaviour.h"
#include "PlayerController.h"
#include "PlayerMove.h"
#include "MMMInput.h"
#include "Transform.h"
#include "SnowCollider.h"
#include "TileMap.h"
#include "SnowballManager2.h"


void MMMEngine::PlayerController::Start()
{
	MoveComponent = GetComponent<PlayerMove>();
	if (!MoveComponent.IsValid()) { std::cout << "MoveComponent not found" << std::endl; }
}

void MMMEngine::PlayerController::Update()
{
	InPutMove();
	InPutHoldSnow();
}

void MMMEngine::PlayerController::AddSnowList(ObjPtr<GameObject> obj)
{
	m_Snows.insert(obj);
}

void MMMEngine::PlayerController::RemoveSnowList(ObjPtr<GameObject> obj)
{
	m_Snows.erase(obj);
}

void MMMEngine::PlayerController::InPutMove()
{
	float x = 0.f, z = 0.f;
	if (Input::GetKey(KeyCode::W)) z += 1.f;
	if (Input::GetKey(KeyCode::S)) z -= 1.f;
	if (Input::GetKey(KeyCode::D)) x += 1.f;
	if (Input::GetKey(KeyCode::A)) x -= 1.f;

	m_InputDir = DirectX::SimpleMath::Vector3(x, 0.f, z);

	auto t = GetComponent<PlayerMove>();
	t->SetInputDir(m_InputDir);
}

void MMMEngine::PlayerController::InPutHoldSnow()
{
	if (Input::GetKeyDown(KeyCode::Space))
	{
		m_holdSpace = true;

		if (!m_Snows.empty())
		{
			AttachNearestSnow();
		}
		else
		{
			if (m_TileMap.IsValid())
				m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

			if (auto mv = GetComponent<PlayerMove>(); mv.IsValid())
				mv->SetScoopMode(true, nullptr);
		}
	}

	if (Input::GetKeyUp(KeyCode::Space))
	{
		m_holdSpace = false;
		m_pendingAttach = false;
		m_attachDelayFrames = 0;
		SnowScoopCount = 0;
		DetachSnow();
		return;
	}

	if (m_holdSpace && !curSnow.IsValid() && !m_Snows.empty())
	{
		AttachNearestSnow();
	}

	if (m_pendingAttach)
	{
		if (m_attachDelayFrames > 0)
		{
			--m_attachDelayFrames;
		}
		else
		{
			m_pendingAttach = false;
			if (m_holdSpace && !curSnow.IsValid() && !m_Snows.empty())
				AttachNearestSnow();
		}
	}
}

void MMMEngine::PlayerController::HasSnow(bool value)
{
	is_Snow = value;
}

void MMMEngine::PlayerController::AddScoop(int SnowCount)
{
	if (!m_holdSpace) return;

	// 눈 들고 있으면 거기에만 증가
	if (curSnow.IsValid())
	{
		auto sc = curSnow->GetComponent<SnowCollider>();
		if (sc)
		{
			int newCount = std::min(MaxPlayerScoop, sc->GetScoopCount() + SnowCount);
			sc->SetScoopCount(newCount);
			SnowScoopCount = newCount; // 캐시
		}
		return;
	}

	// 눈이 없으면 pending만 증가
	int prev = SnowScoopCount;
	SnowScoopCount = std::min(MaxPlayerScoop, SnowScoopCount + SnowCount);

	if (prev == 0 && SnowScoopCount == 1)
	{
		auto mgr = m_SnowManager->GetComponent<SnowballManager2>();
		mgr->Make_snow(GetGameObject());
		m_pendingAttach = true;
		m_attachDelayFrames = 1;
	}
}

void MMMEngine::PlayerController::SetMaxScoop(int MaxCount)
{
	MaxPlayerScoop = MaxCount;
}

void MMMEngine::PlayerController::AttachNearestSnow()
{
	curSnow = nullptr;
	float bestDist = FLT_MAX;

	for (auto& s : m_Snows)
	{
		if (!s.IsValid()) continue;

		float d = (s->GetTransform()->GetWorldPosition() - GetTransform()->GetWorldPosition()).LengthSquared();
		if (d < bestDist)
		{
			bestDist = d;
			curSnow = s;
		}
	}

	if (curSnow.IsValid())
	{
		if (auto snow = curSnow->GetComponent<SnowCollider>(); snow)
		{
			snow->SetOnPlayer(true, GetGameObject());
			SnowScoopCount = std::min(snow->GetScoopCount(), MaxPlayerScoop);
		}
	}

	is_Snow = true;
	m_TileMap->GetComponent<TileMap>()->NoticePlayer(true);

	auto t = GetComponent<PlayerMove>();
	t->SetScoopMode(true, curSnow);
}

void MMMEngine::PlayerController::DetachSnow()
{
	if (curSnow.IsValid())
	{
		if (auto sc = curSnow->GetComponent<SnowCollider>(); sc)
			sc->SetOnPlayer(false, nullptr);
	}

	is_Snow = false;
	curSnow = nullptr;

	if (m_TileMap.IsValid())
		m_TileMap->GetComponent<TileMap>()->NoticePlayer(false);

	if (auto mv = GetComponent<PlayerMove>(); mv.IsValid())
		mv->SetScoopMode(false, nullptr);
}
