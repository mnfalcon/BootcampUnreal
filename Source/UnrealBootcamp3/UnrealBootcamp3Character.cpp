// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealBootcamp3Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"



//////////////////////////////////////////////////////////////////////////
// AUnrealBootcamp3Character

AUnrealBootcamp3Character::AUnrealBootcamp3Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Creating a first person camera
	CameraFace = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraFace"));
	CameraFace->SetupAttachment(AUnrealBootcamp3Character::GetMesh(), FName(TEXT("head")));
	CameraFace->TargetArmLength = 0.0f; // The camera follows at this distance behind the character	
	CameraFace->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	FPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	FPCamera->SetupAttachment(CameraFace, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FPCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FPCamera->bAutoActivate = false;



	ProjectileCount = 0;
	isFPCameraActive = false;

	SprintSpeedMultiplier = 1.75;

	PlayerStamina = 100.0;

}

void AUnrealBootcamp3Character::AddToInventory(APickupTest* actor)
{
	_inventory.Add(actor);
}

void AUnrealBootcamp3Character::PrintInventory()
{
	FString sInventory = "";
	for (APickupTest* actor : _inventory)
	{
		sInventory.Append(actor->Name);
		sInventory.Append(" | ");
	}
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Yellow, *sInventory);
}

void AUnrealBootcamp3Character::UpdateInventory()
{
	OnUpdateInventory.Broadcast(_inventory);
}

void AUnrealBootcamp3Character::ShowMouseFunction()
{

	APlayerController* playerController = Cast<APlayerController>
		(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (playerController)
	{
		playerController->bShowMouseCursor = true;
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
	}
}
void AUnrealBootcamp3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerStamina == 100.0 && !bIsSprinting)
	{
		GetWorldTimerManager().ClearTimer(SprintTimer);
	}

	

}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealBootcamp3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUnrealBootcamp3Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnrealBootcamp3Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnrealBootcamp3Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnrealBootcamp3Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUnrealBootcamp3Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUnrealBootcamp3Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AUnrealBootcamp3Character::OnResetVR);


	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUnrealBootcamp3Character::Fire);

	//Camera test
	PlayerInputComponent->BindAction("CPPCameraToggle", IE_Pressed, this, &AUnrealBootcamp3Character::CameraSwitch);

	//Sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AUnrealBootcamp3Character::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AUnrealBootcamp3Character::StopSprinting);

}


void AUnrealBootcamp3Character::CameraSwitch()
{
	
	if (FollowCamera->IsActive())
	{
		FollowCamera->SetActive(false);
		FPCamera->SetActive(true);
		this->bUseControllerRotationYaw = true;
		isFPCameraActive = true;
	}
	else if(FPCamera->IsActive())
	{
		FPCamera->SetActive(false);
		FollowCamera->SetActive(true);
		this->bUseControllerRotationYaw = false;
		isFPCameraActive = false;
	}
}

void AUnrealBootcamp3Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AUnrealBootcamp3Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AUnrealBootcamp3Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AUnrealBootcamp3Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealBootcamp3Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AUnrealBootcamp3Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AUnrealBootcamp3Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AUnrealBootcamp3Character::Sprint()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
	GetWorldTimerManager().SetTimer(SprintTimer, this,
			&AUnrealBootcamp3Character::DepleteStamina, 1.0f, true);


}

void AUnrealBootcamp3Character::StopSprinting()
{
	GetWorldTimerManager().ClearTimer(SprintTimer);
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;

	GetWorldTimerManager().SetTimer(SprintTimer, this,
			&AUnrealBootcamp3Character::RegenerateStamina, 1.0f, true);

}


void AUnrealBootcamp3Character::DepleteStamina()
{
	PlayerStamina -= 5;
}

void AUnrealBootcamp3Character::RegenerateStamina()
{
	PlayerStamina += 1;
}
void AUnrealBootcamp3Character::Fire()
{


	if (ProjectileCount > 0)

	{
		if (ProjectileClass)
		{
			//Get camera transform
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			//Set MuzzleOffset tospawn projectiles slightly in front of the camera
			MuzzleOffset.Set(0.0f, 0.0f, -50.0f);

			//Transform MuzzleOffset from camera space to world space
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation)
				.TransformVector(MuzzleOffset);

			//Skew aim slightly upwards
			FRotator MuzzleRotation = CameraRotation;
			MuzzleRotation.Pitch += 10.0f;

			UWorld* World = GetWorld();

			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				//Spawn projectile at muzzle
				AProjectileTest* Projectile = World->SpawnActor<AProjectileTest>
					(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if (Projectile)
				{
					//Set projectile's initial trajectory
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					ProjectileCount -= 1;
					if (_inventory.Num() > 0)
					{
						AUnrealBootcamp3Character::_inventory.Pop();
					}
				}
			}
		}
	}
}
