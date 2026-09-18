#include "Weapon/SniperRifle.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"
#include "Components/DecalComponent.h"

void ASniperRifle::BeginPlay()
{
	Super::BeginPlay();
	TPSCamComp = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (SniperUIFactory)
	{
		SniperUI = CreateWidget<UUserWidget>(GetWorld(), SniperUIFactory);
	}
}
void ASniperRifle::SecondaryAction()
{
	bSniperAim = !bSniperAim;

	if (SniperUI == nullptr || TPSCamComp == nullptr)return;

	if (bSniperAim)
	{
		SniperUI->AddToViewport();
		TPSCamComp->SetFieldOfView(45.0f);
	} 
	else
	{
		SniperUI->RemoveFromParent();
		TPSCamComp->SetFieldOfView(90.0f);
	}
}

void ASniperRifle::Fire()
{
	if (TPSCamComp == nullptr)return;

	FVector startPos = TPSCamComp->GetComponentLocation();
	FVector endPos = TPSCamComp->GetComponentLocation() + TPSCamComp->GetForwardVector() * 5000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
	if (bHit)
	{
		FTransform bulletTrans;
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			EffectFactory,
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
			DecalMaterial,	// 데칼 머티리얼 자체를 변수로
			DecalSize,	// 사이즈는 원하는 데칼 크기
			hitInfo.ImpactPoint,
			hitInfo.ImpactNormal.Rotation(),
			DecalLifetime);	// 탄흔이 몇초동안 유지되어야 하는지

		Decal->SetFadeScreenSize(0); // 화면 크기에 따른 페이드 설정
	}
	EffectPos = WeaponMesh->GetSocketLocation(TEXT("MuzzleFlash"));

	if (BeamParticles)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			BeamParticles,  // UNiagaraSystem* 타입
			EffectPos,
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
