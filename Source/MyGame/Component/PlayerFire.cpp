#include "PlayerFire.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/MyWeapon.h"
#include "Data/MyDataSubsystem.h"
#include "Data/MyInputDataAsset.h"
#include "MyGameplayTags.h"

UPlayerFire::UPlayerFire()
{
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;

	// 무기를 스폰시킨다.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = me; // 스폰시킨 캐릭터를 Owner로 설정하고 Spawn시킨다.
							// 네트워크 -> Owner 중요.
		
	MainWeapon = GetWorld()->SpawnActor<AMyWeapon>(StartingMainWeapon, me->GetActorTransform(), SpawnParams);
	SubWeapon = GetWorld()->SpawnActor<AMyWeapon>(StartingSubWeapon, me->GetActorTransform(), SpawnParams);

	// 오른손 소켓에 생성한 Weapon BP를 붙인다.
	const USkeletalMeshSocket* HandSocket = me->GetMesh()->GetSocketByName(FName("hand_rSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(MainWeapon, me->GetMesh());
		HandSocket->AttachActor(SubWeapon, me->GetMesh());
	}	

	// 기본으로 스나이퍼건을 사용하도록 설정
	ChangeToMainWeapon(FInputActionValue());
}

void UPlayerFire::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	UMyDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem< UMyDataSubsystem>();
	if (DataSubsystem == nullptr)
		return;

	// 총알 발사 이벤트 처리 함수 바인딩
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_Fire), ETriggerEvent::Started, this, &UPlayerFire::Input_Fire);
	// 총 교체 이벤트 처리함수 바인딩
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_ChangeWeapon), ETriggerEvent::Started, this, &UPlayerFire::Input_ChangeWeapon);
	// 스나이퍼 조준 모드 이벤트 처리 함수 바인딩
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_SniperAim), ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	PlayerInput->BindAction(DataSubsystem->FindInputActionByTag(MyGameplayTags::Input_Action_SniperAim), ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
}

// 유탄총/스나이퍼건 토글
void UPlayerFire::Input_ChangeWeapon(const FInputActionValue& inputValue)
{
	if (CurrentWeapon == MainWeapon)
	{
		ChangeToSubWeapon(inputValue);
	}
	else
	{
		ChangeToMainWeapon(inputValue);
	}
}

void UPlayerFire::Input_Fire(const struct FInputActionValue& inputValue)
{
	if (CurrentWeapon == nullptr)
	{
		return;
	}

	CurrentWeapon->Fire();
}

// 유탄총으로 변경
void UPlayerFire::ChangeToMainWeapon(const FInputActionValue& inputValue)
{
	// 유탄총 사용 중으로 체크
	//bUsingGrenadeGun = true;
	ChangeWeapon(MainWeapon, SubWeapon);

	// 유탄총 사용할지 여부 전달
	me->OnUsingGrenade(true);
}

// 스나이퍼건으로 변경
void UPlayerFire::ChangeToSubWeapon(const FInputActionValue& inputValue)
{
	//bUsingGrenadeGun = false;
	ChangeWeapon(SubWeapon, MainWeapon);

	// 유탄총 사용할지 여부 전달
	me->OnUsingGrenade(false);
}

void UPlayerFire::ChangeWeapon(AMyWeapon* NewWeapon, AMyWeapon* OldWeapon)
{
	CurrentWeapon = NewWeapon;

	// 새로운 무기
	if (NewWeapon)
	{
		NewWeapon->SetActorHiddenInGame(false);		// 게임에서 숨기기 X
		NewWeapon->SetActorEnableCollision(true);	// 충돌체 활성화
		NewWeapon->SetActorTickEnabled(true);		// 틱 활성화
	}
	
	// 예전 무기
	if (OldWeapon)
	{
		OldWeapon->SetActorHiddenInGame(true);
		OldWeapon->SetActorEnableCollision(false);
		OldWeapon->SetActorTickEnabled(false);
	}
}

// 스나이퍼 조준
void UPlayerFire::SniperAim(const FInputActionValue& inputValue)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SniperAim();
	}
}
