#include "Snowball.h"
#include "MMMTime.h"
#include "../Player/Player.h"
#include "Transform.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "../../test/SnowCollider.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Snowball>("Snowball")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Snowball>"))
		.property("point", &Snowball::point)
		.property("m_player", &Snowball::m_player);

	registration::class_<ObjPtr<Snowball>>("ObjPtr<Snowball>")
		.constructor(
			[]() {
				return Object::NewObject<Snowball>();
			}).method("Inject", &ObjPtr<Snowball>::Inject);
}

void MMMEngine::Snowball::Start()
{
	m_player = GetGameObject()->Find("Player");
}

void MMMEngine::Snowball::Update()
{
	
}

void MMMEngine::Snowball::EatSnow(ObjPtr<GameObject> other)
{
	if (!other || other == GetGameObject()) return;
	auto snowcomp = other->GetComponent<Snowball>();
	if (snowcomp == nullptr)
		return;
	point += snowcomp->GetPoint();
	int maxpoint = m_player->GetComponent<Player>()->maxpoint;
	point = std::min(point, maxpoint);
	GetComponent<SnowCollider>()->SetScoopCount(point - 1);
}

void MMMEngine::Snowball::PointUp()
{
	point++;
	int maxpoint = m_player->GetComponent<Player>()->maxpoint;
	point = std::min(point, maxpoint);
}

void MMMEngine::Snowball::SetPoint(int t)
{
	point = t;
	int maxpoint = m_player->GetComponent<Player>()->maxpoint;
	point = std::min(point, maxpoint);
}