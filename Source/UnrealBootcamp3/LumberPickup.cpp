// Fill out your copyright notice in the Description page of Project Settings.


#include "LumberPickup.h"
#include <UnrealBootcamp3\UnrealBootcamp3Character.h>
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

// Sets default values
ALumberPickup::ALumberPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALumberPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALumberPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

