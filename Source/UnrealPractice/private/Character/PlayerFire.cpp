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

	TPSCamComp = Me->TPSCamComp;
	GunMeshComp = Me->GunMeshComp;
	SniperGunComp = Me->SniperGunComp;

	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	ChangeToSniperRifle(FInputActionValue());
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

	if (bUsingAssaultRifle)
	{
		FTransform firePosition = GunMeshComp->GetSocketTransform(TEXT("FirePosition"));
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
		}
	}
}

void UPlayerFire::ChangeToAssaultRifle(const struct FInputActionValue& inputValue)
{
	bUsingAssaultRifle = true;
	SniperGunComp->SetVisibility(false);
	GunMeshComp->SetVisibility(true);
	Me->OnUsingGun(bUsingAssaultRifle);
}

void UPlayerFire::ChangeToSniperRifle(const struct FInputActionValue& inputValue)
{
	bUsingAssaultRifle = false;
	SniperGunComp->SetVisibility(true);
	GunMeshComp->SetVisibility(false);
	Me->OnUsingGun(bUsingAssaultRifle);
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
	playerInput->BindAction(IA_AssaultRifle, ETriggerEvent::Started, this, &UPlayerFire::ChangeToAssaultRifle);
	playerInput->BindAction(IA_SniperRifle, ETriggerEvent::Started, this, &UPlayerFire::ChangeToSniperRifle);
}
