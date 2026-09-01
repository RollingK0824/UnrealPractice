#include "Weapon/Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 충돌체 등록하기
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	if (collisionComp)
	{
		// 2. 충돌 프로파일 설정
		collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
		// 3. 충돌체 크기 설정
		collisionComp->SetSphereRadius(13);
		// 4. 루트로 등록
		RootComponent = collisionComp;
	}

	// 5. 외관 컴포넌트 등록하기
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	// 6. 부모 컴포넌트 지정
	bodyMeshComp->SetupAttachment(collisionComp);
	// 7. 충돌 비활성화
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외관 크기 설정
	bodyMeshComp->SetRelativeScale3D(FVector(5));
	bodyMeshComp->SetRelativeRotation(FRotator(0, -90, 0));

	// 발사체 컴포넌트
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	// movement 컴포넌트가 갱신시킬 컴포넌트 지정
	movementComp->SetUpdatedComponent(collisionComp);
	// 초기 속도
	movementComp->InitialSpeed = 2000;
	// 최대 속도
	movementComp->MaxSpeed = 2000;
	// 반동 여부
	movementComp->bShouldBounce = true;
	// 반동 값
	movementComp->Bounciness = 0.3f;
	// 회전이 속도를 따름
	movementComp->bRotationFollowsVelocity = true;

	// 생명 시간 주기
	InitialLifeSpan = 2.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	// 타이머를 별도로 변경할일이 없다. 2초뒤에 무조건 실행되게 하면 된다
	//FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	// 람다 사용시, 포인터를 캡처할때는 주의해야한다.
	// 타이머가 울려서 람다가 실행될때, 포인터가 소멸할수도 있다.
	//TObjectPtr<> // 일반적인 포인터
	// 수정 방법 1)
	TWeakObjectPtr<ABullet> WeakBullet = this;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
		FTimerDelegate::CreateLambda([this]()->void
			{
				Destroy();
			}), 2.0f, false);

	// 수정 방법 2)
	// this 캡처 그대로 사용하고 싶다면, Bullet 삭제될떄 TimerManager 에 타이머도 함께 제거
}

void ABullet::EndPlay(const EEndPlayReason::Type EndPlay)
{
	Super::EndPlay(EndPlay);

	// TimerManager 에 타이머도 함께 제거
	GetWorld()->GetTimerManager().ClearTimer(deathTimer);
}


void ABullet::Die()
{
	Destroy();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
#if WITH_EDITOR
// 액터의 특정 속성을 수정하면 호출되는 이벤트 함수
void ABullet::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// speed 값이 수정되었는지 체크
	if (PropertyChangedEvent.GetPropertyName() == TEXT("speed"))
	{
		// 프로젝타일 무브먼트 컴포넌트에 speed 값 적용
		movementComp->InitialSpeed = speed;
		movementComp->MaxSpeed = speed;
	}
}
#endif
