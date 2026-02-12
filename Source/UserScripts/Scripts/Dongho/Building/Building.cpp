#include "Building.h"
#include "rttr/registration"
#include "rttr/detail/policies/ctor_policies.h"
#include "Buildingball.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "MMMTime.h"
#include "../Battlestats.h"
#include "../../test/SnowBullet.h"
#include "../../Mingi/UI/MiniMap.h"
#include "../../Mingi/Manager/SoundManager.h"

RTTR_PLUGIN_REGISTRATION
{
	using namespace rttr;
	using namespace MMMEngine;

	registration::class_<Building>("Building")
		(rttr::metadata("wrapper_type_name", "ObjPtr<Building>"))
		.property("mSpawnScaleCurve", &Building::mSpawnScaleCurve)
		.property("mLevelScaleCurve", &Building::mLevelScaleCurve)
		.property("mMeshTrans", &Building::mMeshTrans)
		.property("level", &Building::level)
		.property("maxHP", &Building::maxHP)
		.property("exp", &Building::exp)
		.property("atk", &Building::atk)
		.property("point", &Building::point)
		.property("pre_bullet", &Building::pre_bullet)
		.property("bulletSpeed", &Building::bulletSpeed);

	registration::class_<ObjPtr<Building>>("ObjPtr<Building>")
		.constructor(
			[]() {
				return Object::NewObject<Building>();
			}).method("Inject", &ObjPtr<Building>::Inject);
}

void MMMEngine::Building::OnEnable()
{
	if (MiniMap::Instance.IsValid())
	{
		MiniMap::Instance->RegisterTracker(GetMUID(), GetTransform(), TrackerType::Building);
	}
}
void MMMEngine::Building::OnDisable()
{
	if (MiniMap::Instance.IsValid())
	{
		MiniMap::Instance->UnregisterTracker(GetMUID());
	}
}
void MMMEngine::Building::Start()
{
	SoundManager::Instance->PlaySFX2D("Build", SelfPtr(this));

	pos = GetTransform()->GetWorldPosition();
	for (int i = 0; i < 10;++i)
	{
		auto obj = Instantiate(pre_bullet);
		obj->GetTransform()->SetWorldPosition(pos);
		obj->GetComponent<Buildingball>()->SetOwner(GetGameObject());
		obj->SetActive(false);
		Buildingballs.push(obj);
	}

	if (!mMeshTrans) {
		std::cout << "Building::MeshTrans Not Found!!!" << std::endl;
		Destroy(GetGameObject());
	}
	else {
		mInitScale = mMeshTrans->GetLocalScale();
	}

	isAnimating = true;
	mAnimType = ANIMTYPE::SPAWN;
	prevLevel = level;
}

void MMMEngine::Building::Update()
{
	// 레벨업 감지
	if (prevLevel != level) {
		isAnimating = true;
		prevLevel = level;
	}

	// 애니메이션 재생
	if (isAnimating) {
		switch (mAnimType)
		{
		case MMMEngine::Building::SPAWN:
			UpdateSpawnAnim();
			break;
		case MMMEngine::Building::LEVEL:
			UpdateLevelAnim();
			break;
		case MMMEngine::Building::END:
			break;
		default:
			break;
		}
	}
	
	CheckEnemy();
	AutoAttack();
}

void MMMEngine::Building::Dead()
{
	isDead = true;
	GetGameObject()->SetActive(false);
}

void MMMEngine::Building::CheckEnemy()
{
	if (enemyTarget)
		return;
	auto enemys = GameObject::FindGameObjectsWithTag("Enemy");
	if (enemys.empty()) return;

	float bestD2 = attackdist * attackdist;
	ObjPtr<GameObject> best = nullptr;

	for (auto& e : enemys)
	{
		if (!e) continue;
		auto epos = e->GetTransform()->GetWorldPosition();
		float dx = epos.x - pos.x;
		float dz = epos.z - pos.z;
		float d2 = dx * dx + dz * dz;

		if (d2 < bestD2)
		{
			bestD2 = d2;
			best = e;
		}
	}
	if (best)
		enemyTarget = best;
}

void MMMEngine::Building::AutoAttack()
{
	if (point <= 0 || enemyTarget == nullptr)
		return;
	float bestD2 = attackdist * attackdist;
	auto epos = enemyTarget->GetTransform()->GetWorldPosition();
	float dx = epos.x - pos.x;
	float dz = epos.z - pos.z;
	float d2 = dx * dx + dz * dz;
	if (d2 > bestD2)
	{
		enemyTarget = nullptr;
		attackTimer = 0.0f;
		return;
	}
	attackTimer += Time::GetDeltaTime();
	if (attackTimer >= attackDelay)
	{
		if (Buildingballs.empty()){
			attackTimer = 0.0f;
			return;
		}
		auto obj = Buildingballs.front();
		Buildingballs.pop();
		if (!obj)
			return;
		obj->SetActive(true);
		obj->GetComponent<Buildingball>()->SetTarget(enemyTarget);
		obj->GetComponent<Buildingball>()->Setatk(atk);
		auto bulletpos = pos;
		bulletpos.y = 1.f;
		obj->GetComponent<SnowBullet>()->StartBullet(bulletpos, bulletsize, bulletSpeed, enemyTarget);
		attackTimer = 0.0f;
		point--;
	}
}

void MMMEngine::Building::ReturnBall(ObjPtr<GameObject> obj)
{
	Buildingballs.push(obj);
}

void MMMEngine::Building::PointUp(int t)
{
	point += t;
	exp += 10 * t;
}

void MMMEngine::Building::UpdateSpawnAnim()
{
	if (mSpawnScaleCurve.GetKeyframes().empty()) {
		std::cout << "Building::No SpawnScaleCurve!!!" << std::endl;
		isAnimating = false;
		return;
	}

	mElipsedTime += Time::GetDeltaTime();
	mCurveScale = mSpawnScaleCurve.Evaluate(mElipsedTime);

	mMeshTrans->SetLocalScale(mInitScale * mCurveScale);

	if (mElipsedTime >= mSpawnScaleCurve.GetKeyframes().back().time) {
		mElipsedTime = 0.0f;
		isAnimating = false;
		mAnimType = ANIMTYPE::LEVEL;

		/*if (auto col = GetComponent<ColliderComponent>(); col) {
			col->SetEn
		}*/
	}
}

void MMMEngine::Building::UpdateLevelAnim()
{
	if (mLevelScaleCurve.GetKeyframes().empty()) {
		std::cout << "Building::No LevelScaleCurve!!!" << std::endl;
		isAnimating = false;
		return;
	}

	mElipsedTime += Time::GetDeltaTime();
	mCurveScale = mLevelScaleCurve.Evaluate(mElipsedTime);

	mMeshTrans->SetLocalScale(mInitScale * mCurveScale);

	if (mElipsedTime >= mLevelScaleCurve.GetKeyframes().back().time) {
		mElipsedTime = 0.0f;
		isAnimating = false;
	}
}