// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableTest.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "GameFramework/Character.h"

// Sets default values
AThrowableTest::AThrowableTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallTest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallTest"));
	BallTest->SetSimulatePhysics(true);
	RootComponent = BallTest;

	bHolding = false;
	bGravity = true;

}

// Called when the game starts or when spawned
void AThrowableTest::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	PlayerCamera = MyCharacter->FindComponentByClass<UCameraComponent>();

	TArray<USceneComponent*> Components;

	MyCharacter->GetComponents(Components);

	if (Components.Num() > 0)
	{
		for (auto& Comp : Components)
		{
			if (Comp->GetName() == "HoldingComponent")
			{
				HoldingComp = Cast<USceneComponent>(Comp);
			}
		}

	}
	
}

// Called every frame
void AThrowableTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bHolding && HoldingComp)
	{
		SetActorLocationAndRotation(HoldingComp->GetComponentLocation(), HoldingComp->GetComponentRotation());
	}
}

void AThrowableTest::Pickup()
{
	bHolding = !bHolding;
	bGravity = !bGravity;
	BallTest->SetEnableGravity(bGravity);
	BallTest->SetSimulatePhysics(bHolding ? false : true);
	BallTest->SetCollisionEnabled(bHolding ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (!bHolding)
	{
		ForwardVector = PlayerCamera->GetForwardVector();
		BallTest->AddForce(ForwardVector * 100000 * BallTest->GetMass());
	}
}


