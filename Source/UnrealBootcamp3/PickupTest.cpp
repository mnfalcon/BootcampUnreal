// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupTest.h"
#include <UnrealBootcamp3\UnrealBootcamp3Character.h>
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

// Sets default values
APickupTest::APickupTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	//this->BoxCollider->bGenerateOverlapEvents;: 1;
	this->BoxCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	this->BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickupTest::OnOverlapBegin);
}


void APickupTest::OnInteract()
{
	FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Yellow, pickup);

	AUnrealBootcamp3Character* player = Cast<AUnrealBootcamp3Character>
		(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{

		player->AddToInventory(this);

		Destroy();
	}

}


void APickupTest::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->ActorHasTag(FName(TEXT("isPlayer")))))
	{
		AUnrealBootcamp3Character* player = Cast<AUnrealBootcamp3Character>
			(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (player->InventoryLumber < 5)
		{
			OnInteract();

			player->InventoryLumber += 1;
		}
		else if (player->InventoryLumber == 5)
		{
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Orange, "Inventory is full.");
		}
	}
}


// Called when the game starts or when spawned
void APickupTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

