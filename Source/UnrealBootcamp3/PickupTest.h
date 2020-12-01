// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Runtime\Engine\Classes\Components\BoxComponent.h>
#include "PickupTest.generated.h"

UCLASS()
class UNREALBOOTCAMP3_API APickupTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Show(bool visible);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Pickup)
		UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
		UTexture2D* Image;

	virtual void OnInteract();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
