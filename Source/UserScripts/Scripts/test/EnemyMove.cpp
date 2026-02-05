#include "Export.h"
#include "ScriptBehaviour.h"
#include "EnemyMove.h"
#include "Transform.h"
#include "MMMTime.h"


static float WrapPi(float a)
{
	while (a > DirectX::XM_PI)  a -= DirectX::XM_2PI;
	while (a < -DirectX::XM_PI)  a += DirectX::XM_2PI;
	return a;
}

static float LerpAngleRad(float cur, float target, float a)
{
	float d = WrapPi(target - cur);
	return cur + d * a;
}

static float YawFromQuat_YUp(const Quaternion& q)
{
	// yaw(Y) = atan2(2*(w*y + x*z), 1 - 2*(y*y + x*x))
	float siny_cosp = 2.f * (q.w * q.y + q.x * q.z);
	float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.x * q.x);
	return atan2f(siny_cosp, cosy_cosp);
}


void MMMEngine::EnemyMove::Start()
{
	myPos = GetGameObject()->GetTransform()->GetWorldPosition();
	m_GO = GetGameObject();
	if (!m_GO.IsValid()) { std::cout << "GameObject not found" << std::endl; }
}

void MMMEngine::EnemyMove::FixedUpdate()
{
    auto rb = GetComponent<RigidBodyComponent>();
    auto col = GetComponent<ColliderComponent>();
    float dt = TimeManager::Get().GetDeltaTime();

    if(is_move)
    {
        //목표 속도
        Vector3 desiredVel = ComputeChaseVelocity();
        Vector3 chaseDir = desiredVel;
        chaseDir.y = 0.f;

        if (chaseDir.LengthSquared() > 1e-6f)
            chaseDir.Normalize();
        else
            chaseDir = Vector3(0, 0, 1);

        //SweepSphere로 "앞에 벽" 미리 감지
        SweepHit sh{};
        Vector3 my = m_GO->GetTransform()->GetWorldPosition();
        my.y = 0.f;

        // layer는 자기자신의 layer를 넣음
        uint32_t wallLayer = col->GetLayer();

        bool hit = PhysxManager::Get().SweepSphere(
            my, sweepRadius,
            chaseDir, sweepAhead,
            sh, wallLayer,
            col,
            rb,
            true
        );

        if (hit && sh.gameObject.IsValid())
        {
            //hit.normal로 슬라이드 방향 계산 (후진/왕복 방지)
            Vector3 n = sh.normal;
            n.y = 0.f;

            if (n.LengthSquared() > 1e-6f)
            {
                n.Normalize();

                // (a) chaseDir에서 벽 안으로 파고드는 성분 제거
                Vector3 slide = chaseDir;
                float dn = slide.Dot(n);
                if (dn < 0.f) slide -= n * dn;

                // (b) 정면 막힘이면 tangent 강제 + side 고정
                if (slide.LengthSquared() < 1e-4f)
                {
                    Vector3 t = Vector3(0, 1, 0).Cross(n);
                    t.y = 0.f;
                    if (t.LengthSquared() > 1e-6f)
                    {
                        t.Normalize();

                        // side 고정 로직(프레임마다 뒤집히면 지터)
                        if (!hasAvoidSide)
                        {
                            // 목표 방향과 더 가까운 쪽 선택
                            float s = t.Dot(chaseDir);
                            avoidSide = (s >= 0.f) ? +1 : -1;
                            hasAvoidSide = true;
                        }
                        slide = t * (float)avoidSide;
                    }
                }
                else
                {
                    // 슬라이드가 잘 나오면 side 고정 해제
                    hasAvoidSide = false;
                }

                //최종 방향 섞기 (슬라이드만 써도 되고, 섞으면 더 부드러움)
                Vector3 dir = chaseDir + slide * avoidGain;
                if (dir.LengthSquared() > 1e-6f) dir.Normalize();

                desiredVel = dir * movespeed;
            }
        }
        else
        {
            hasAvoidSide = false;
        }

        float m_sharpness = 8.0f;

        //관성(가속 제한)
        curVel = DirectX::SimpleMath::Vector3::Lerp(curVel, desiredVel, m_sharpness * Time::GetDeltaTime());
        curVel.y = 0.f;

        rb->SetLinearVelocity(curVel);

        FaceVelocityYaw(curVel);
    }
}


DirectX::SimpleMath::Vector3 MMMEngine::EnemyMove::ComputeChaseVelocity()
{
	myPos = m_GO->GetTransform()->GetWorldPosition();
	
	if (!Obj_target) 
	{
		std::cout << "Target Nullptr" << std::endl;
		return Vector3{ 0,0,0 };
	}
	Target = Obj_target->GetTransform()->GetWorldPosition();

	DirectX::SimpleMath::Vector3 dir = (Target - myPos);
	dir.y = 0.f;

	DirectX::SimpleMath::Vector3 EnemyVec = DirectX::SimpleMath::Vector3{ 0, 0, 0 };

	if (dir.LengthSquared() > 0.f)
	{
		dir.Normalize();
		EnemyVec = dir * movespeed;
	}

	return EnemyVec;
}


void MMMEngine::EnemyMove::FaceVelocityYaw(const DirectX::SimpleMath::Vector3& vel)
{
    Vector3 v = vel;
    v.y = 0.f;

    if (v.LengthSquared() < 0.01f)
        return;

    v.Normalize();

    float targetYaw = atan2f(v.x, v.z);

    auto tr = m_GO->GetTransform();

    Quaternion curQ = tr->GetWorldRotation();
    float curYaw = YawFromQuat_YUp(curQ);

    float newYaw = LerpAngleRad(curYaw, targetYaw, yawLerpA);

    Quaternion newQ =
        Quaternion::CreateFromYawPitchRoll(newYaw, 0.f, 0.f);

    tr->GetComponent<RigidBodyComponent>()->SnapRotation(newQ);
}


//trigger 함수들
void MMMEngine::EnemyMove::AddObjPtr(ObjPtr<GameObject> snow)
{
	if (!snow.IsValid()) return;
	for (auto& o : SnowObjPtr)
		if (o == snow) return;
	SnowObjPtr.push_back(snow);
}

void MMMEngine::EnemyMove::Remove(ObjPtr<GameObject> snow)
{
	SnowObjPtr.erase(
		std::remove_if(SnowObjPtr.begin(), SnowObjPtr.end(),
			[&](const ObjPtr<GameObject>& o) { return !o.IsValid() || o == snow; }),
		SnowObjPtr.end());
}

void MMMEngine::EnemyMove::ChangeTarget(ObjPtr<GameObject> target)
{
    Obj_target = target;
}

void MMMEngine::EnemyMove::MoveTriggerSet(bool value)
{
    is_move = value;
}
