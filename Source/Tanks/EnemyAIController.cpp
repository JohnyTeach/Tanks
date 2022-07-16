#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Waypoints.Empty();
	
	TankPawn = Cast<ATankPawn>(InPawn);

	if (!TankPawn)
	{
		return;
	}

	TArray<AActor*> WaypointActors;
	
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), TankPawn->WaypointTag, WaypointActors);

	WaypointActors.Sort([](const AActor& a, const AActor& b)
	{
		auto WPA = CastChecked<AWaypoint>(&a);
		auto WPB = CastChecked<AWaypoint>(&b);
		return WPA->Order > WPB ->Order;
	});

	for (auto Waypoint : WaypointActors)
	{
		Waypoints.Add(Waypoint->GetActorLocation());		
	}	
	
	NextWaypoint = 0;
	
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!TankPawn || Waypoints.Num() <= 0)
	{
		return;
	}

	if (NextWaypoint >= Waypoints.Num())
	{
		NextWaypoint = 0;
	}

	auto Waypoint = Waypoints[NextWaypoint];	
	
	if (2500 < FVector::DistSquared2D(Waypoint, TankPawn->GetActorLocation()))
	{
		TankPawn->MoveForward(1);
	}
	else
	{
		NextWaypoint++;
	}
		
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), Waypoint);	
	auto Rotation = TankPawn->GetActorRotation();
	auto Direction = FRotator::NormalizeAxis(TargetRotation.Yaw - Rotation.Yaw);
	auto Diff = FMath::Abs(Direction);
	
	if (Diff >= 5)
	{
		if (Diff > 90)
		{
			TankPawn->MoveForward(0);
		}
		TankPawn->RotateRight(Direction > 0 ? 1 : -1);
	}
	else
	{
		TankPawn->RotateRight(0);
	}
}

FVector AEnemyAIController::GetTargetLocation() const
{
	if (TankPawn)
	{
		auto BestTarget = TankPawn->GetBestTarget();
		
		if (BestTarget && TankPawn->EnemyVisibility().Actor == BestTarget)
		{
			TankPawn->Cannon->AIFire();
			return BestTarget->GetActorLocation();
		}
		return  TankPawn->GetActorLocation() + TankPawn->GetActorForwardVector() * 500;
	}
	return FVector::ZeroVector;
}