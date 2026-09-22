// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

/*
std::vector<std::function<void()>> OnStartAir;

// 유투버 (1), 구독자 (N)

// C++ 내에서 함수포인터 연결 & 호출이 필요하다면
// 한명만 바인딩
DECLARE_DELEGATE(FInputBindingDelegate);



// C++ 과 블루프린트가 통신하고 싶다.

// 한개만 바인딩 (블루프린트 통신)
DECLARE_DYNAMIC_DELEGATE_TwoParam(FInputBindingDynamicDelegate, class UEnhancedInputComponent*, inputComp, float, deltaTime);	

// 여러개 바인딩 (블루프린트 통신)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBindingDynamicDelegate, class UEnhancedInputComponent*, inputComp);	// 여러개 바인딩 (블루프린트 통신)
*/

// 여러명 바인딩
DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*);

UCLASS()
class MYGAME_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// 입력 바인딩 델리게이트
	FInputBindingDelegate onInputBindingDelegate;

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

public:

	// 카메라랑 캐릭터랑 충분히 가까우면, 메시 숨기는 거리
	UPROPERTY(EditAnywhere, Category = "IHGame|Value")
	float MeshVisibleDistance = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr<class UPlayerMove> playerMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class UPlayerFire> playerFire;

	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 hp;
	// 초기 hp 값
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	int32 initialHp = 10;
	// 피격 당했을 때 처리
	UFUNCTION(BlueprintCallable, Category = Health)
	void OnHitEvent();

	// 게임 오버될 때 호출될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Health)
	void OnGameOver();

	// 유탄총 사용 여부가 바뀔 때 호출되는 함수
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Health)
	void OnUsingGrenade(bool isGrenade);

	// 크로스 헤어 관련 변수
	UPROPERTY(EditAnywhere, Category = "IHGame|Value")
	float CrosshairSpreadMax = 6.f;

	UPROPERTY(EditAnywhere, Category = "IHGame|Value")
	float CrosshairSpreadMin = 2.f;

	void UpdateCrossHair();


	float AO_StartYaw = 0;
	float AO_Yaw = 0;
	float AO_Pitch = 0;
	void UpdateAimOffset(float DeltaTime);
};
