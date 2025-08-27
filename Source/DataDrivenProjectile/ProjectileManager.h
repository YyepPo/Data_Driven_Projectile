#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ProjectileData.h"
#include "ProjectileManager.generated.h"

UCLASS()
class DATADRIVENPROJECTILE_API AProjectileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AProjectileManager();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SpawnProjectile(const FVector& StartLocation,const FVector& StartDirection, const float BulletSpeed,UNiagaraSystem* NiagaraEffect);

	//~ Getters
	UFUNCTION(BlueprintCallable,BlueprintPure)
		FORCEINLINE TArray<FProjectileData> GetProjectiles() const {return Projectiles;}	
	//~ End Getters
	
protected:
	
	virtual void BeginPlay() override;

private:
	
	TArray<FProjectileData> Projectiles;

	UPROPERTY(EditAnywhere)
		bool bEnableDebug = false;
};
