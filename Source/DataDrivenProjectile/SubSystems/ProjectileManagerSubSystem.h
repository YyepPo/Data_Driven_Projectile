#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DataDrivenProjectile/Structs/ProjectileData.h"
#include "ProjectileManagerSubSystem.generated.h"

UCLASS()
class DATADRIVENPROJECTILE_API UProjectileManagerSubSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	UProjectileManagerSubSystem();

	// Required function from FTickableGameObject. If not implemented a crash will occur
	virtual TStatId GetStatId() const override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SpawnProjectile(const FVector& Location,
			const FVector& Direction,
			const FProjectileData& NewProjectileData);

	//~ Getters
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FORCEINLINE TArray<FProjectileData> GetProjectiles() const {return Projectiles;}	
	//~ End Getters

private:

	// Projectiles that are spawned
	TArray<FProjectileData> Projectiles;
	
	const float Gravity = -970.f;
};
