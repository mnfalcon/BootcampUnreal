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
	//this->BoxCollider->AttachToComponent(this->RootComponent,
		//FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}
void APickupTest::Show(bool visible)
{
	/*ECollisionEnabled::Type collision = visible ?
		ECollisionEnabled::QueryAndPhysics :
		ECollisionEnabled::NoCollision;

	this->SetActorTickEnabled(visible);

	this->ItemMesh->SetVisibility(visible);
	this->ItemMesh->SetCollisionEnabled(collision);

	this->BoxCollider->SetCollisionEnabled(collision);*/
}



void APickupTest::OnInteract()
{
	FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::White, pickup);

	AUnrealBootcamp3Character* player = Cast<AUnrealBootcamp3Character>
		(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{
		Show(false);

		player->AddToInventory(this);

		player->ProjectileCount += 1;

		Destroy();
	}

}


void APickupTest::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (OtherActor->ActorHasTag(FName(TEXT("isPlayer")))))
	{
		OnInteract();
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

