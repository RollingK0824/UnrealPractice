#include "Character/TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "MyGame.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/Bullet.h"
#include "Enemy/EnemyFSM.h"
#include "PlayerAnim.h"
#include "Component/PlayerMove.h"
#include "Component/PlayerFire.h"
#include "UI/MyHUD.h"
#include "Data/MyDataSubsystem.h"
#include "Data/MyInputDataAsset.h"
#include "Data/MyDataConfigAsset.h"
#include "System/MyGameInstance.h"

ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈메시 데이터를 불러오고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Game/Assets/ThirdPersonTemplate/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh 컴포넌트의 위치와 회전값을 설정하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. TPS 카메라를 붙이고 싶다.
	// 3-1. SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	//_springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->SocketOffset = FVector(0, 70, 90);  // 소켓 오프셋으로 변경
	springArmComp->ProbeSize = 20;						// 프로브 크기 수정(충돌체 사이즈 키우자)
	springArmComp->TargetArmLength = 400; 
	springArmComp->bUsePawnControlRotation = true;

	// 3-2. Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	// 컨트롤러가 회전하면 -> Pawn 회전도 적용해주세요. 엔진에 요구
	bUseControllerRotationYaw = true;

	// 2단 점프
	JumpMaxCount = 2;

	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));


	hp = initialHp;
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	auto PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			UDataAsset* InputData = Cast<UMyGameInstance>(GetGameInstance())->DataConfig->DA_Input;
			UMyInputDataAsset* DA_Input = Cast<UMyInputDataAsset>(InputData);

			if (DA_Input)
			{
				Subsystem->AddMappingContext(DA_Input->IMC_TPS, 0);
			}
		}
	}

	// 파라곤 캐릭터는 무기를 숨긴다.
	if (GetMesh())
	{
		GetMesh()->HideBoneByName(TEXT("weapon"), EPhysBodyOp::PBO_None);
	}

	// 시작 AimOffset Yaw 값 저장
	AO_StartYaw = GetController()->GetControlRotation().Yaw;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 거리, 캐릭터 거리 비교. 
	// 거리값이 내가 원하는 거리보다 작다면,, 메시를 숨김처리한다.
	FVector DistVector = tpsCamComp->GetComponentLocation() - GetActorLocation();
	double Dist = DistVector.Size();

	// unit 단위 : 1 = 1cm
	if (Dist < MeshVisibleDistance)
	{
		// 메시를 숨긴다.
		GetMesh()->SetVisibility(false);
	}
	else
	{
		GetMesh()->SetVisibility(true);
	}

	// 크로스헤어 업데이트
	UpdateCrossHair();

	// 에임 오프셋 업데이트
	UpdateAimOffset(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		onInputBindingDelegate.Broadcast(PlayerInput);
	}
}

void ATPSPlayer::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged !!!!!"));
	hp--;
	if (hp <= 0)
	{
		PRINT_LOG(TEXT("Player is dead!"));
		OnGameOver();
	}
}

void ATPSPlayer::UpdateCrossHair()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return;

	// 크로스헤어가 그려지고 있는 HUD 가져오기
	AMyHUD* HUD = Cast<AMyHUD>(PlayerController->GetHUD());
	if (HUD == nullptr)
		return;


	// 1. 이동이 있으면 크로스헤어 벌어진다.
	// Velocity 값 기준으로 얼만큼 벌어지게 할지 결정.
	// 최대 이속 대비, 현재 이속값을 계산해서 얼마나 크로스헤어가 벌어지는지 계산
	FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
	FVector2D VelocityMulRange(0.f, 1.f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0; // 점프속도는 무시

	// 최대속도 600, 현재 이속 : 300 -> 0.5f 라는 수치를 얻고싶다.
	// 최대속도 600, 현재 이속 : 600 -> 1.0f
	float CrosshairVelocityAlpha = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMulRange, Velocity.Size());

	// 환산된값 0~1 사이의 기준으로, Weapon Min,Max 값에 대응하는 값을 뽑는다.
	// min :2, max:6, 2~6 사이의 적절한값으로 환산해준다.
	float CrosshairVelocityFactor = FMath::Lerp(CrosshairSpreadMin, CrosshairSpreadMax, CrosshairVelocityAlpha);

	// 최종값 : 이동 Factor + Fire Factor
	float CrosshairFireFactor = 0;	//@TODO : 총기마다 벌어지는 크로스헤어 값
	float TotalCrosshairFactor = CrosshairVelocityFactor + CrosshairFireFactor;

	// 최종적으로 HUD 객체에 Spread 정보를 전달한다.
	HUD->SetCrosshairSpread(TotalCrosshairFactor);
}

void ATPSPlayer::UpdateAimOffset(float DeltaTime)
{
	FVector Velocity = GetVelocity();
	float Speed = Velocity.Size2D();  // 높이에 대한 속도는 무시하고, 수평속도만 계산
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	// 가만히 있을때 AO_Yaw 계산
	if (Speed == 0.f && !bIsInAir)
	{
		// 현재 회전값
		float CurrentYaw = GetController()->GetControlRotation().Yaw;

		// -180~180 사이의 차이값으로 정규화해서 넘겨준다.
		float DeltaYaw = FMath::FindDeltaAngleDegrees(AO_StartYaw, CurrentYaw);

		// 시작 AO YAW 값을 기준으로 차이점을 계산한다.
		AO_Yaw = DeltaYaw;  //CurrentYaw - AO_StartYaw;

		// 에임오프셋이 적용될때는, 컨트롤러가 회전해도 캐릭터는 제자리에 서있어야 한다.
		bUseControllerRotationYaw = false;
	}
	else
	{
		AO_Yaw = 0;
		AO_StartYaw = GetController()->GetControlRotation().Yaw;  // 이동 중에는 시작값을 계속 갱신

		// 달리기 시작하면, TPS 장르처럼 카메라 방향으로 캐릭터가 회전해야한다.
		bUseControllerRotationYaw = true;
	}

	// 액터의 눈의 위치로 Pitch를 계산하면 편하다
	AO_Pitch = GetBaseAimRotation().Pitch;
}

void ATPSPlayer::OnGameOver_Implementation()
{
	// 게임 오버 시 일시 정지
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
}