// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "ThrowableTest.generated.h"

UCLASS()
class UNREALBOOTCAMP3_API AThrowableTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere);
	UStaticMeshComponent* BallTest;

	UPROPERTY(EditAnywhere);
	USceneComponent* HoldingComp;

	UFUNCTION()
	void Pickup();

	bool bHolding;
	bool bGravity;

	ACharacter* MyCharacter;
	UCameraComponent* PlayerCamera;
	FVector ForwardVector;

};
