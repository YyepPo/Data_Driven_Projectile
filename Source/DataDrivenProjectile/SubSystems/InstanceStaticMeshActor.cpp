#include "InstanceStaticMeshActor.h"

#include "Components/InstancedStaticMeshComponent.h"

AInstanceStaticMeshActor::AInstanceStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("InstancedStaticMeshComponent"));
	InstancedStaticMeshComponent->SetupAttachment(RootComponent);
	InstancedStaticMeshComponent->RegisterComponent();
	
	InstancedStaticMeshComponent->bAffectDistanceFieldLighting = false;
	InstancedStaticMeshComponent->bUseAttachParentBound = true;
	InstancedStaticMeshComponent->SetCanEverAffectNavigation(false);
	InstancedStaticMeshComponent->bDisableCollision = true;
	InstancedStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

