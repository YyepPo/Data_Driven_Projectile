// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManagerSubSystem.h"

#include "NiagaraFunctionLibrary.h"

UProjectileManagerSubSystem::UProjectileManagerSubSystem()
{
	
}

TStatId UProjectileManagerSubSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UProjectileManagerSubsystem, STATGROUP_Game);
}

void UProjectileManagerSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<int32> ToRemove;

	for (int32 i = 0; i < Projectiles.Num(); ++i)
	{
		FProjectileData& Bullet = Projectiles[i];

		// Apply gravity to Z-velocity
		Bullet.ZVelocity += Gravity * DeltaTime;

		// Calculate new location & add bullet drop
		const FVector StartLocation = Bullet.Location;
		const FVector ProjectileVelocity = Bullet.Direction * Bullet.Speed;
		const FVector NewLocation = StartLocation + (ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity * Bullet.Mass)) * DeltaTime;

		// Line trace to detect hit, if hit destroy projectile, otherwise keep updating movement
		FHitResult HitResult;
		const bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult,
			StartLocation,
			NewLocation,
			ECC_Visibility);

		if (bHasHit)
		{
			// Deal damage
			//if (AActor* HitActor = HitResult.GetActor())
			//{
			//	UE_LOG(LogTemp, Log, TEXT("Bullet hit actor: %s"), *HitActor->GetName());
			//}

			// Destroy Niagara component. Optionally spawn impact vfx
			if (Bullet.VFXComponent)
			{
				Bullet.VFXComponent->DestroyComponent();
			}
		}
		else
		{
			// Update location
			Bullet.Location = NewLocation;

			if (Bullet.VFXComponent)
			{
				Bullet.VFXComponent->SetWorldLocation(NewLocation);
				// Rotation follows velocity
				FVector CombinedVelocity = ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity);
				Bullet.VFXComponent->SetWorldRotation(CombinedVelocity.Rotation());
			}

			//DrawDebugLine(GetWorld(), StartLocation, NewLocation, FColor::Red, true, 5.f);
		}
	}
}

void UProjectileManagerSubSystem::SpawnProjectile(const FVector& Location, const FVector& Direction,
	const FProjectileData& NewProjectileData)
{
	if(NewProjectileData.ProjectileVFX == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Projectile Manager: Niagara System is not select. Pls Select one")));
		return;
	}

	if(FMath::IsNearlyZero(NewProjectileData.Speed))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Projectile Manager: Projectiles bullet speed is 0")));
	}
	
	FProjectileData ProjectileData = NewProjectileData;
	ProjectileData.Location = Location;
	ProjectileData.Direction = Direction;	
		
	ProjectileData.VFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ProjectileData.ProjectileVFX,
		ProjectileData.Location,
		FRotator::ZeroRotator,
		FVector(1),
		true,
		true,
		ENCPoolMethod::AutoRelease);
	
	Projectiles.Emplace(ProjectileData);
}
