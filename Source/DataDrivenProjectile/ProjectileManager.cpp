#include "ProjectileManager.h"
#include "NiagaraFunctionLibrary.h"

AProjectileManager::AProjectileManager()
{
	PrimaryActorTick.bCanEverTick = true;

	bEnableDebug = false;
}

void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TArray<int32> ToRemove;
	const float Gravity = -970.f;  // cm/s², downward gravity

	for (int32 i = 0; i < Projectiles.Num(); ++i)
	{
		FProjectileData& Bullet = Projectiles[i];

		// Apply gravity to Z-velocity
		Bullet.ZVelocity += Gravity * DeltaTime;

		// Calculate new location & add bullet drop
		const FVector StartLocation = Bullet.Location;
		const FVector ProjectileVelocity = Bullet.Direction * Bullet.Speed;
		const FVector NewLocation = StartLocation + (ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity)) * DeltaTime;

		// Line trace to detect hit, if hit destroy projectile, otherwise keep updating movement
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		const bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, NewLocation, ECC_Visibility, Params);

		if (bHasHit)
		{
			// Deal damage
			if (AActor* HitActor = HitResult.GetActor())
			{
				UE_LOG(LogTemp, Log, TEXT("Bullet hit actor: %s"), *HitActor->GetName());
			}

			// Destroy Niagara component. Optionally spawn impact vfx
			if (Bullet.VisualComponent)
			{
				Bullet.VisualComponent->DestroyComponent();
			}
		}
		else
		{
			// Update location
			Bullet.Location = NewLocation;

			if (Bullet.VisualComponent)
			{
				Bullet.VisualComponent->SetWorldLocation(NewLocation);
				// Rotation follows velocity
				FVector CombinedVelocity = ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity);
				Bullet.VisualComponent->SetWorldRotation(CombinedVelocity.Rotation());
			}
			
			if (bEnableDebug)
			{
				DrawDebugLine(GetWorld(), StartLocation, NewLocation, FColor::Red, true, 5.f);
			}
		}
	}
}

void AProjectileManager::SpawnProjectile(const FVector& StartLocation, const FVector& StartDirection,
	const float BulletSpeed,UNiagaraSystem* NiagaraEffect)
{
	if(NiagaraEffect == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Projectile Manager: Niagara System is not select. Pls Select one")));
		return;
	}

	if(FMath::IsNearlyZero(BulletSpeed))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Projectile Manager: Projectiles bullet speed is 0")));
	}
	
	FProjectileData NewProjectile;
	NewProjectile.Location = StartLocation;
	NewProjectile.Direction = StartDirection.GetSafeNormal();
	NewProjectile.Speed = BulletSpeed;
	
	NewProjectile.VisualComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraEffect, StartLocation, FRotator::ZeroRotator,
		FVector(1),true,true,ENCPoolMethod::AutoRelease);
	
	Projectiles.Add(NewProjectile);
}

