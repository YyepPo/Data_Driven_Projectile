#include "ProjectileManagerSubSystem.h"
#include "InstanceStaticMeshActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

TStatId UProjectileManagerSubSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UProjectileManagerSubsystem, STATGROUP_Game);
}

void UProjectileManagerSubSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(InstancedStaticMeshComponent) == false)
	{
		AInstanceStaticMeshActor* Actor = Cast<AInstanceStaticMeshActor>(UGameplayStatics::GetActorOfClass(GetWorld(),AInstanceStaticMeshActor::StaticClass()));
		if (Actor != nullptr)
		{
			InstancedStaticMeshComponent = Actor->InstancedStaticMeshComponent;
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("AInstanceStaticMeshActor is valid")));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1,
				5.f,
				FColor::Red,
				TEXT("Invalid AInstanceStaticMeshActor! Please create an AInstanceStaticMeshActor and assign a Static Mesh to its InstancedStaticMeshComponent."));
		}
	}

	TArray<int32> ToRemove;

	TArray<FTransform> ProjectileTransforms;
	ProjectileTransforms.Reserve(Projectiles.Num());

	for (int32 i = 0; i < Projectiles.Num(); ++i)
	{
		FProjectileData& Bullet = Projectiles[i];

		// Apply gravity to Z-velocity
		Bullet.ZVelocity += Gravity * DeltaTime;

		// Calculate new location & add bullet drop overtime
		const FVector StartLocation = Bullet.Location;
		const FVector ProjectileVelocity = Bullet.Direction * Bullet.Speed;
		const FVector CombinedVelocity = ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity * Bullet.Mass);
		const FVector NewLocation = StartLocation + CombinedVelocity * DeltaTime;
		Bullet.Location = NewLocation;
		// Line trace to detect hit, if hit destroy projectile, otherwise keep updating movement
		FHitResult HitResult;
		const bool bHasHit = GetWorld()->LineTraceSingleByChannel(HitResult,
			StartLocation,
			NewLocation,
			ECC_Visibility);

		if (bHasHit)
		{			
			if (AActor* HitActor = HitResult.GetActor())
			{
				// Deal damage
				// Add impulse etc.
			}

			ToRemove.Add(i);
		}
		else
		{
			// Update location
			Bullet.Location = NewLocation;
			const FRotator BulletOrientation = (ProjectileVelocity + FVector(0, 0, Bullet.ZVelocity)).Rotation();
			const FTransform NewTransform = FTransform(BulletOrientation,Bullet.Location,FVector(1));
			ProjectileTransforms.Add(NewTransform);
	
			//DrawDebugLine(GetWorld(), StartLocation, NewLocation, FColor::Red, true, 5.f);
		}
	}

	InstancedStaticMeshComponent->ClearInstances();
	
	if (ProjectileTransforms.IsEmpty() == false)
	{
		for (const FTransform& ProjectileTransform : ProjectileTransforms)
		{
			InstancedStaticMeshComponent->AddInstance(ProjectileTransform);
		}
		//DrawDebugSphere(GetWorld(),ProjectileTransforms[0].GetLocation(),10,10,FColor::Red);
	}
	
	// Remove projectiles that has been destroyed
	if (ToRemove.IsEmpty() == false)
	{
		ToRemove.Sort(TGreater<int32>());
	
		for (int32 Index : ToRemove)
		{
			if (Projectiles.IsValidIndex(Index))
			{
				// RemoveAtSwap is much more efficient than RemoveAt (O(Count) instead of O(ArrayNum)), but does not preserve the order.
				Projectiles.RemoveAtSwap(Index, EAllowShrinking::No);
			}
		}
	}
}

void UProjectileManagerSubSystem::SpawnProjectile(const FVector& Location, const FVector& Direction,
                                                  const FProjectileData& NewProjectileData)
{
	if(FMath::IsNearlyZero(NewProjectileData.Speed))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Projectile Manager: Projectiles bullet speed is 0")));
	}
	
	FProjectileData ProjectileData = NewProjectileData;
	ProjectileData.Location = Location;
	ProjectileData.Direction = Direction;	
	
	Projectiles.Emplace(ProjectileData);
}
