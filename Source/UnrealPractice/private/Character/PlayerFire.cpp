#include "Character/PlayerFire.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Weapon/Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
#include <Camera/CameraComponent.h>
#include "Character/PlayerAnim.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/DecalComponent.h"
#include "Weapon/MyWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
UPlayerFire::UPlayerFire()
{
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundCue'/Game/Assets/MilitaryWeapSilver/Sound/Rifle/Cues/RifleA_Fire_Cue.RifleA_Fire_Cue'"));
	if (tempSound.Succeeded())
	{
		BulletSound = tempSound.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Me; 

	INT8 currentWeaponIdx = 0;
	for (auto element : StartingWeapons)
	{
		WeaponList[currentWeaponIdx] = GetWorld()->SpawnActor<AMyWeapon>(element, Me->GetActorTransform(), SpawnParams);
		++currentWeaponIdx;
	}

	CurrentWeapon = WeaponList[0];
	if (CurrentWeapon)
	{
		const USkeletalMeshSocket* HandSocket = Me->GetMesh()->GetSocketByName(FName("hand_rSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(CurrentWeapon, Me->GetMesh());
		}
	}

	TPSCamComp = Me->TPSCamComp;

	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);
}

void UPlayerFire::InputFire(const struct FInputActionValue& inputValue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);

	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
	}

	auto anim = Cast<UPlayerAnim>(Me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	if (bUsingSniperRifle)
	{
		FTransform firePosition = CurrentWeapon->WeaponMesh->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);
	}
	else
	{
		FVector startPos = TPSCamComp->GetComponentLocation();
		FVector endPos = TPSCamComp->GetComponentLocation() + TPSCamComp->GetForwardVector() * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(Me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				BulletEffectFactory,
				hitInfo.ImpactPoint);

			auto hitComp = hitInfo.GetComponent();
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				FVector dir = (endPos - startPos).GetSafeNormal();
				FVector force = dir * hitComp->GetMass() * 50000;
				hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}

			UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
				BulletDecalMaterial,	// 데칼 머티리얼 자체를 변수로
				DecalSize,	// 사이즈는 원하는 데칼 크기
				hitInfo.ImpactPoint,
				hitInfo.ImpactNormal.Rotation(),
				DecalLifetime);	// 탄흔이 몇초동안 유지되어야 하는지

			Decal->SetFadeScreenSize(0); // 화면 크기에 따른 페이드 설정
		}
		FVector test = CurrentWeapon->WeaponMesh->GetSocketLocation(TEXT("MuzzleFlash"));

		if (BeamParticles)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				BeamParticles,  // UNiagaraSystem* 타입
				test,
				FRotator::ZeroRotator,
				FVector(1.0f, 1.0f, 1.0f),  // Scale
				true,  // AutoDestroy
				true,  // AutoActivate
				ENCPoolMethod::AutoRelease  // Pooling 방식
			);

			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
				NiagaraComp,
				FName("ImpactPositions"),  // Niagara 변수 이름
				TArray<FVector>({ hitInfo.ImpactPoint })  // ImpactPoint를 포함하는 배열
			);

			NiagaraComp->SetVariableBool(FName(TEXT("Trigger")), true);
		}
	}
}

void UPlayerFire::Input_ChangeToNextWeapon(const FInputActionValue& inputValue)
{
	++CurrentWeaponIdx;
	if (CurrentWeaponIdx == WeaponList.Num())
	{
		CurrentWeaponIdx = 0;
	}
	EquipWeapon();
}

void UPlayerFire::Input_ChangeToPrevWeapon(const FInputActionValue& inputValue)
{
	--CurrentWeaponIdx;
	if (CurrentWeaponIdx < 0)
	{
		CurrentWeaponIdx = WeaponList.Num()-1;
	}
	EquipWeapon();
}

void UPlayerFire::EquipWeapon()
{
	CurrentWeapon = WeaponList[CurrentWeaponIdx];
}

void UPlayerFire::SniperAim(const FInputActionValue& inputValue)
{
	if (bSniperAim == false)
	{
		bSniperAim = true;
		SniperUI->AddToViewport();
		TPSCamComp->SetFieldOfView(45.0f);
	}
	else
	{
		bSniperAim = false;
		SniperUI->RemoveFromParent();
		TPSCamComp->SetFieldOfView(90.0f);
	}
}

void UPlayerFire::SetupInputBinding(UEnhancedInputComponent* playerInput)
{
	playerInput->BindAction(IA_SniperZoom, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	playerInput->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &UPlayerFire::InputFire);
	playerInput->BindAction(IA_NextWeapon, ETriggerEvent::Started, this, &UPlayerFire::Input_ChangeToNextWeapon);
	playerInput->BindAction(IA_PrevWeapon, ETriggerEvent::Started, this, &UPlayerFire::Input_ChangeToPrevWeapon);
}
