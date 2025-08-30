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
	// Mass in kg
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Mass = 1.f;

	FVector Location;
	FVector Direction;
	float ZVelocity;
};
