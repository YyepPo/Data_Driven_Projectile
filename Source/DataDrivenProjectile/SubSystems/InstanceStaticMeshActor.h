// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstanceStaticMeshActor.generated.h"

UCLASS()
class DATADRIVENPROJECTILE_API AInstanceStaticMeshActor : public AActor
{
	GENERATED_BODY()

public:
	AInstanceStaticMeshActor();
	
	UPROPERTY(EditAnywhere)
		TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

};
