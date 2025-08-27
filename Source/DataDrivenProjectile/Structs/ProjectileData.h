#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "ProjectileData.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UNiagaraSystem> ProjectileVFX;
	// Mass in kg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Mass = 1.f;

	FVector Location;
	FVector Direction;
	float ZVelocity;
	UPROPERTY()
		TObjectPtr<UNiagaraComponent> VFXComponent;
};
