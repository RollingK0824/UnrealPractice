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
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	GunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (TempGunMesh.Succeeded())
	{
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		//GunMeshComp->SetRelativeLocation(FVector(-14, 11, 138));
	}

	SniperGunComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SniperGunComp"));
	SniperGunComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempSniperMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/MilitaryWeapSilver/Weapons/Sniper_Rifle_A.Sniper_Rifle_A'"));
	if (TempSniperMesh.Succeeded())
	{
		SniperGunComp->SetSkeletalMesh(TempSniperMesh.Object);
		//SniperGunComp->SetRelativeLocation(FVector(-22, 31, 128));
	}

	PlayerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	//PlayerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
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
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		OnInputBindingDelegate.Broadcast(PlayerInput);
		/*PlayerMove->SetupInputBinding(PlayerInput);
		PlayerFire->SetupInputBinding(PlayerInput);*/
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

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

