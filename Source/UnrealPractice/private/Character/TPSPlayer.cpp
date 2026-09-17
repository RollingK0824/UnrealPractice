#include "Character/TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerMove.h"
#include "Character/PlayerFire.h"
#include "../UnrealPractice.h"
#include <Kismet/GameplayStatics.h>

ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/.SKM_Quinn_Simple'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	if (SpringArmComp)
	{
		SpringArmComp->SetupAttachment(RootComponent);
		SpringArmComp->SetRelativeLocation(FVector(0, 70, 90));
		SpringArmComp->TargetArmLength = 400;
		SpringArmComp->bUsePawnControlRotation = true;
	}

	TPSCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamComp"));
	if (TPSCamComp)
	{
		TPSCamComp->SetupAttachment(SpringArmComp);
		TPSCamComp->bUsePawnControlRotation = false;
	}

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	PlayerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	PlayerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_TPS, 0);
		}
	}

	Hp = InitialHP;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAimOffset(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		OnInputBindingDelegate.Broadcast(PlayerInput);
	}
}

void ATPSPlayer::OnHitEvent()
{
	PRINT_LOG(TEXT("Damaged!"));
	--Hp;
	if (Hp <= 0)
	{
		PRINT_LOG(TEXT("Player Is Dead"));
		OnGameOver();
	}
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
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

