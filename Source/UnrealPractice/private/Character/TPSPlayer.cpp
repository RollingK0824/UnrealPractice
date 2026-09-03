#include "Character/TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Weapon/Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Character/PlayerAnim.h"
#include "Kismet/GameplayStatics.h"

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

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundCue'/Game/Assets/MilitaryWeapSilver/Sound/Rifle/Cues/RifleA_Fire_Cue.RifleA_Fire_Cue'"));
	if (tempSound.Succeeded())
	{
		BulletSound = tempSound.Object;
	}

}

void ATPSPlayer::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void ATPSPlayer::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	direction.X = value.X;
	direction.Y = value.Y;
}

void ATPSPlayer::InputJump(const struct FInputActionValue& inputValue)
{
	Jump();
}

void ATPSPlayer::InputFire(const struct FInputActionValue& inputValue)
{
	UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShake);
	}

	auto anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
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
		params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		if (bHit)
		{
			FTransform bulletTrans;
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				BulletEffectFactory,
				hitInfo.ImpactPoint);
		}
	}
}

void ATPSPlayer::ChangeToAssaultRifle(const struct FInputActionValue& inputValue)
{
	bUsingAssaultRifle = true;
	SniperGunComp->SetVisibility(false);
	GunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperRifle(const struct FInputActionValue& inputValue)
{
	bUsingAssaultRifle = false;
	SniperGunComp->SetVisibility(true);
	GunMeshComp->SetVisibility(false);
}

void ATPSPlayer::InputRun()
{
	auto movement = GetCharacterMovement();

	if (movement->MaxWalkSpeed > WalkSpeed)
	{
		movement->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		movement->MaxWalkSpeed = RunSpeed;
	}
}

void ATPSPlayer::PlayerMove()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direciton * WalkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_TPS, 0);
		}
	}

	ChangeToSniperRifle(FInputActionValue());
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ATPSPlayer::InputJump);
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ATPSPlayer::InputFire);
		PlayerInput->BindAction(IA_AssaultRifle, ETriggerEvent::Triggered, this, &ATPSPlayer::ChangeToAssaultRifle);
		PlayerInput->BindAction(IA_SniperRifle, ETriggerEvent::Triggered, this, &ATPSPlayer::ChangeToSniperRifle);
		PlayerInput->BindAction(IA_PlayerRun, ETriggerEvent::Started, this, &ATPSPlayer::InputRun);
		PlayerInput->BindAction(IA_PlayerRun, ETriggerEvent::Completed, this, &ATPSPlayer::InputRun);
	}
}

