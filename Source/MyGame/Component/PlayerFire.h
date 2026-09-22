#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"


// 총기 발사와 관련된 로직 -> Weapon 이사
// PlayerFire 역할 -> Weapon 관리 역할

UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class MYGAME_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInput) override;

	UPROPERTY( )
	class UCameraComponent* tpsCamComp;

public:


	// 총알 발사 처리함수
	void Input_Fire(const struct FInputActionValue& inputValue);
	// 스나이퍼 조준처리함수
	void Input_SniperAim(const struct FInputActionValue& inputValue);
	// 총 변경
	void Input_ChangeWeapon(const struct FInputActionValue& inputValue);

	// 유탄총으로 변경
	void ChangeToMainWeapon(const struct FInputActionValue& inputValue);
	// 스나이퍼건으로 변경
	void ChangeToSubWeapon(const struct FInputActionValue& inputValue);
	void ChangeWeapon(class AMyWeapon* NewWeapon, class AMyWeapon* OldWeapon);

	// 스나이퍼 조준처리함수
	void SniperAim(const struct FInputActionValue& inputValue);


	// 현재 가지고 있는 무기 : 레벨에 스폰된 인스턴스화된 객체
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AMyWeapon> CurrentWeapon = nullptr;

	// 주무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyWeapon> MainWeapon = nullptr;

	// 보조무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AMyWeapon> SubWeapon = nullptr;

	// 스타팅 무기
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TSubclassOf<class AMyWeapon> StartingMainWeapon;		// BP_Rifle, BP_Sniper

	// 스타팅 무기
	UPROPERTY(EditDefaultsOnly, Category = "IHGame|Weapon")
	TSubclassOf<class AMyWeapon> StartingSubWeapon;		// BP_Rifle, BP_Sniper
};
