// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileTest.h"
#include <Runtime\Engine\Classes\GameFramework\DefaultPawn.h>

// Sets default values
AProjectileTest::AProjectileTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Collision and root component

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	if (!CollisionComponent)
		{

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileTest::OnHit);
	CollisionComponent->InitSphereRadius(7.5f);
	RootComponent = CollisionComponent;
		}



	//Projectile Movement
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>
		(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.5f;
	ProjectileMovementComponent->ProjectileGravityScale = 1;

	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>
			(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh
		(TEXT("'/Game/Sphere/Sphere.Sphere'"));

		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material
	(TEXT("'/Game/Sphere/SphereMaterial.SphereMaterial'"));

	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create
		(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//Lifespan set to 3 seconds
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AProjectileTest::BeginPlay()
{
	Super::BeginPlay();

	FString teststring = FString::Printf(TEXT("Projectile spawned"));

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::White, teststring);
	
}

// Called every frame
void AProjectileTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileTest::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = 
		ShootDirection * ProjectileMovementComponent->InitialSpeed;

}

void AProjectileTest::OnHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	FString hit_test = FString::Printf(TEXT("Projectile: Hit"));

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::White, hit_test);

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity
			* 1.0f, Hit.ImpactPoint);


	}




	Destroy();
}

