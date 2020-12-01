// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime\Engine\Classes\Components\SphereComponent.h>
#include <Runtime\Engine\Classes\GameFramework\ProjectileMovementComponent.h>
#include "Materials/MaterialInstanceDynamic.h"
#include "ProjectileTest.generated.h"



UCLASS()
class UNREALBOOTCAMP3_API AProjectileTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Sphere collision component
	UPROPERTY(EditAnywhere, Category = Projectile)
		USphereComponent* CollisionComponent;

	//Projectile movement component
	UPROPERTY(EditAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//Function to launch projectile
	void FireInDirection(const FVector& ShootDirection);

	//Projectile mesh
	UPROPERTY(EditAnywhere, Category = Projectile)
		UStaticMeshComponent* ProjectileMeshComponent;

	//Projectile material
	UPROPERTY(EditAnywhere, Category = Movement)
		UMaterialInstanceDynamic* ProjectileMaterialInstance;

	//Function called when projectile hits something
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			FVector NormalImpulse, const FHitResult& Hit);

};
