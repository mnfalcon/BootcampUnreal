// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickupTest.h"
#include "ProjectileTest.h"
#include "UnrealBootcamp3Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate,
const TArray<APickupTest*>&, InventoryItems);


UCLASS(config = Game)
class AUnrealBootcamp3Character : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	//Third person camera test
	UPROPERTY(EditAnywhere/*, BlueprintReadOnly, Category = Camera , meta = (AllowPrivateAcess = "true")*/)
		class USpringArmComponent* CameraFace;
	UPROPERTY(EditAnywhere/*, BlueprintReadOnly, Category = Camera , meta = (AllowPrivateAcess = "true")*/)
		class UCameraComponent* TPCamera;



	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* l_hand;

	UPROPERTY(EditAnywhere)
		class USceneComponent* HoldingComponent;
public:
	AUnrealBootcamp3Character();


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere)
		class AThrowableTest* CurrentItem;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Fire Projectile Testing*/

	virtual void Tick(float DeltaSeconds) override;


private:
	TArray<APickupTest*> _inventory;

public:
	void AddToInventory(APickupTest* actor);

	UFUNCTION(BlueprintCallable)
		void PrintInventory();

	UFUNCTION(BlueprintCallable)
		void UpdateInventory();

	UPROPERTY(BlueprintAssignable, Category = "Pickup")
		FUpdateInventoryDelegate OnUpdateInventory;

	virtual void ShowMouseFunction();



protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class USpringArmComponent* GetCameraFace() const { return CameraFace; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetTPCamera() const { return TPCamera; }

	UFUNCTION()
	void Fire();

	void CameraSwitch();

	//Offset from the camera location to fire projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	//Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectileTest> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ProjectileCount;






};

