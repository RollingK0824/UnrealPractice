// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MyWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/TPSPlayer.h"
#include "Character/PlayerAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "MyGame.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Enemy/EnemyFSM.h"
#include "Bullet.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 설정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// 시작 무기는 가지고 태어날거기때문에 충돌체를 끄고 시작
	CollisionComp->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionComp->SetSphereRadius(64);

	// 충돌을 루트로 설정
	RootComponent = CollisionComp;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(CollisionComp);

}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();


	// 1. 스나이퍼 UI 위젯 인스턴스 생성
	if (sniperUIFactory)
	{
		sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	}
}

// Called every frame
void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::Fire()
{
	ATPSPlayer* me = Cast<ATPSPlayer>(GetOwner());
	if (me == nullptr)
	{
		return;
	}

	// 캐릭터 애니메이션 몽타주 재생
	UPlayerAnim* PlayerAnim = Cast<UPlayerAnim>(me->GetMesh()->GetAnimInstance());
	if (PlayerAnim)
	{
		PlayerAnim->PlayAttackAnim();
	}

	// 총기 애니메이션 재생
	if (FireWeaponAnimation)
	{
		// Loop : false
		WeaponMesh->PlayAnimation(FireWeaponAnimation, false);
	}

	// 카메라 셰이크 재생
	APlayerController* PlayerController = Cast<APlayerController>(me->GetController());
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(cameraShake);
	}

	// 2D 재생 — UI, BGM 등 공간감(3D 감쇠) 없는 사운드
	UGameplayStatics::PlaySound2D(
		this,           // WorldContextObject
		bulletSound,     // USoundBase* (SoundCue도 여기 대입 가능)
		1.0f,           // VolumeMultiplier
		1.0f,           // PitchMultiplier
		0.0f            // StartTime
	);

	// 총구 위치
	FVector firePosition = WeaponMesh->GetSocketTransform(TEXT("MuzzleFlash"/*"FirePosition"*/)).GetLocation();

	// 
	if (bUsingGrenadeGun)
	{
		// 총알 발사 처리
		FVector CameraForward = me->tpsCamComp->GetForwardVector();
		FTransform SpawnTransform(CameraForward.Rotation(), firePosition);

		// 메모리 할당 미리 받았지만, 엔진이 BeginPlay() 호출 안해준다.
		ABullet* SpawnedBullet = GetWorld()->SpawnActorDeferred<ABullet>(bulletFactory, SpawnTransform, this, me);

		// 내가 소환한 총알은 나와 충돌체크 안하도록 무시
		SpawnedBullet->collisionComp->IgnoreActorWhenMoving(me, true);
		// 양방향으로 충돌체크를 해서 그렇다. 캐릭터에도 총알과 충돌체크 무시 설정
		me->GetCapsuleComponent()->IgnoreActorWhenMoving(SpawnedBullet, true);

		// 작업 다했으니, 엔진에게 완료 요청을 보내면, 컴포넌트->BeginPlay() 나머지 작업들을 처리한다.
		SpawnedBullet->FinishSpawning(SpawnTransform);


		// 엔진이 Bullet 메모리를 할당
		// 월드에 스폰시킨다. BeginPlay() 호출. 
		// 메모리를 반환
		//ABullet* SpawnedBullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition, CameraForward.Rotation());

		// 내가 소환한 총알은 나와 충돌체크 안하도록 무시
		//SpawnedBullet->collisionComp->IgnoreActorWhenMoving(me, true);

	}
	// 스나이퍼건 사용 시
	else
	{
		// LineTrace 의 시작 위치
		FVector startPos = me->tpsCamComp->GetComponentLocation();
		// LineTrace 의 종료 위치
		FVector endPos = me->tpsCamComp->GetComponentLocation() + me->tpsCamComp->GetForwardVector() * 5000;
		// LineTrace 의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어)는 충돌에서 제외
		params.AddIgnoredActor(me);
		// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos,
			ECC_WEAPON_TRACE /*ECC_Visibility*/, params);
		// LineTrace가 부딪혔을 때
		if (bHit)
		{
			// 총알 파편 효과 트랜스폼
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, bulletEffectFactory, hitInfo.ImpactPoint);

			// 추가 연출 : 총알 흔적 데칼 붙이기
			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
				BulletDecalMaterial,	// 데 칼 머티리얼 자체를 변수로
				DecalSize,	// 사이즈는 원하는 데칼 크기
				hitInfo.ImpactPoint, // LineTrace가 부딪힌 위치
				hitInfo.ImpactNormal.Rotation(),
				DecalLifetime);	// 탄흔이 몇초동안 유지되어야 하는지

			if (Decal)
			{
				Decal->SetFadeScreenSize(0); // 화면 크기에 따른 페이드 설정
			}


			// 추가 연출 : 궤적 나이아가라
			if (BeamParticles)
			{
				UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					BeamParticles,  // UNiagaraSystem* 타입
					firePosition,
					FRotator::ZeroRotator,
					FVector(1.0f, 1.0f, 1.0f),  // Scale
					true,  // AutoDestroy
					true,  // AutoActivate
					ENCPoolMethod::AutoRelease  // Pooling 방식
				);

				// 리소스 제작자가 배열형태로 담아서 보내달라고 했다.
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
					NiagaraComp,
					FName("ImpactPositions"),  // Niagara 변수 이름
					TArray<FVector>({ hitInfo.ImpactPoint })  // ImpactPoint를 포함하는 배열
				);

				// 이것도, 리소스 제작자가 Trigger 호출해달라고 만들어서... 맞춰주자.
				NiagaraComp->SetVariableBool(FName(TEXT("Trigger")), true);
			}



			auto hitComp = hitInfo.GetComponent();
			// 1. 만약 컴포넌트에 물리가 적용되어 있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				// 2. 조준한 방향이 필요
				FVector dir = (endPos - startPos).GetSafeNormal();
				// 날려 버릴 힘(F = ma)
				FVector force = dir * hitComp->GetMass() * 100000;
				// 3. 그 방향으로 날려 버리고 싶다.
				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			// 부딪힌 대상이 적인지 판단하기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}
	}
}

void AMyWeapon::SniperAim()
{
	// 스나이퍼건 모드가 아니라면 처리하지 않는다.
	if (bUsingGrenadeGun)
	{
		return;
	}

	if (sniperUI == nullptr)
	{
		return;
	}

	ATPSPlayer* me = Cast<ATPSPlayer>(GetOwner());
	if (me == nullptr)
	{
		return;
	}

	// Pressed 입력 처리
	if (bSniperAim == false)
	{
		// 1. 스나이퍼 조준 모드 활성화
		bSniperAim = true;
		// 2. 스나이퍼조준 UI 등록
		sniperUI->AddToViewport();
		// 3. 카메라의 시야각 Field Of View 설정
		me->tpsCamComp->SetFieldOfView(45.0f);
		// 4. 일반 조준 UI 제거
		//_crosshairUI->RemoveFromParent();
	}
	// Released 입력 처리
	else
	{
		// 1. 스나이퍼 조준 모드 비활성화
		bSniperAim = false;
		// 2. 스나이퍼 조준 UI 화면에서 제거
		sniperUI->RemoveFromParent();
		// 3. 카메라 시야각 원래대로 복원
		me->tpsCamComp->SetFieldOfView(90.0f);
		// 4. 일반 조준 UI 등록
		//_crosshairUI->AddToViewport();
	}
}

