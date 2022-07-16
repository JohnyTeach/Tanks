#include "Waypoint.h"

AWaypoint::AWaypoint()
{ 	
	PrimaryActorTick.bCanEverTick = false;
}

void AWaypoint::BeginPlay()
{
	Super::BeginPlay();	
}

void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

