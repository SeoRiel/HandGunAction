#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/02_Player/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));

		if (PlayerMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(PlayerMesh.Object);
			GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
			GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		}

		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));
		SpringArm->TargetArmLength = 300.0f;
		SpringArm->bEnableCameraLag = true;
		SpringArm->CameraLagSpeed = 10.0f;
		SpringArm->AddRelativeLocation(FVector(0.0f, 0.0f, 60.0f));

		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("AcutalCamera"));
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

		AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	// InputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerYawInput);
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::JumpStart);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::JumpStop);

	InputComponent->BindAction("Dash", IE_Pressed, this, &APlayerCharacter::DashMoveStart);
	InputComponent->BindAction("Dash", IE_Released, this, &APlayerCharacter::DashMoveStop);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}

void APlayerCharacter::DashMoveStart()
{
	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
	MovementComponent->MaxWalkSpeed = 220.0f;
}

void APlayerCharacter::DashMoveStop()
{
	MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
	MovementComponent->MaxWalkSpeed = 0.0f;
}

void APlayerCharacter::JumpStart()
{
	bPressedJump = true;
}

void APlayerCharacter::JumpStop()
{
	bPressedJump = false;
}

void APlayerCharacter::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Input Attack"));
}

void APlayerCharacter::Dodge()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Input Dodge"));
}

