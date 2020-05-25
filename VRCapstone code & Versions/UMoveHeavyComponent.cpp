// Fill out your copyright notice in the Description page of Project Settings.

#include "UMoveHeavyComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


// Sets default values for this component's properties
UUMoveHeavyComponent::UUMoveHeavyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UUMoveHeavyComponent::BeginPlay()
{
	RegisterComponent();
	Super::BeginPlay();
	m_originalOddsOfShooting = m_oddsOfShootingInPercent;
	m_state = CharacterState::Standing;
	m_spawnLocation = GetOwner()->GetActorLocation();
	m_timer = m_timeBetweenMovingOrShooting;
	m_bulletsShot = 0;
}

void UUMoveHeavyComponent::SetGoalLocation()
{
	float x = FMath::RandRange((m_spawnLocation.X - m_wanderDistance), (m_spawnLocation.X + m_wanderDistance));
	float y = FMath::RandRange((m_spawnLocation.Y - m_wanderDistance), (m_spawnLocation.Y + m_wanderDistance));
	float z = GetOwner()->GetActorLocation().Z;

	m_goalLocation = FVector(x, y, z);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Goal Location: %f, %f, %f"), x, y, z));
}

void UUMoveHeavyComponent::RotateTowardLocation(float DeltaTime, FVector Location)
{
	FRotator NewRotation = GetOwner()->GetActorRotation();

	//Determine if Location is clockwise or counter-clockwise of actor
	//FVector normalizedLocation = Location;
	//normalizedLocation.Normalize();

	FVector vectorToLocation = Location - GetOwner()->GetActorLocation();
	vectorToLocation.Normalize();

	/*FVector normalizedActorLocation = GetOwner()->GetActorLocation();
	normalizedActorLocation.Normalize();*/

	//the difference between the two angles 0-2PI of normalizedLocation and the forward vector of the actor
	float diff = FMath::Atan2(vectorToLocation.Y, vectorToLocation.X) - FMath::Atan2(GetOwner()->GetActorForwardVector().Y, GetOwner()->GetActorForwardVector().X);
	//float diff = FMath::Atan2(GetOwner()->GetActorForwardVector().Y, GetOwner()->GetActorForwardVector().X) - FMath::Atan2(normalizedLocation.Y, normalizedLocation.X);
	if ((diff >= 0 && diff <= PI) || diff <= -PI)
	{
		NewRotation.Yaw += m_rotationSpeed * DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("rotate CounterClockwise")));
	}
	//else if (FMath::Abs(diff) < FMath::Abs((m_rotationSpeed * DeltaTime)))
	//{
	//	NewRotation.Yaw += diff * 10;
	//	//HACK: Do whatever is also in the switch statement below. May need a switch statement of it's own to make sure we don't get infinite loop
	//	//switch (m_state)
	//	//{
	//	//case RotatingTowardGoal:
	//	//	m_timer = 2;
	//	//	m_state = Moving;
	//	//	break;
	//	//case RotatingTowardPlayer:
	//	//	m_timer = m_timeBetweenMovingOrShooting;
	//	//	m_state = Standing;
	//	//	break;
	//	//}
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("add difference")));
	//}
	else
	{
		NewRotation.Yaw -= m_rotationSpeed * DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("rotate Clockwise")));
	}

	GetOwner()->SetActorRotation(NewRotation);
}

void UUMoveHeavyComponent::MoveTowardGoalLocation(float DeltaTime)
{
	FVector newLoc = GetOwner()->GetActorLocation();
	newLoc += GetOwner()->GetActorForwardVector() * (m_movementSpeed * DeltaTime);

	GetOwner()->SetActorLocation(newLoc);
}

bool UUMoveHeavyComponent::CheckRotationIsNearlyZero(float DeltaTime, FVector Location)
{
	//Location.Normalize();
	FVector vectorToLocation = Location - GetOwner()->GetActorLocation();
	vectorToLocation.Normalize();


	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("vectorToLocation.X: %f"), vectorToLocation.X));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("actor.X: %f"), GetOwner()->GetActorForwardVector().X));

	//if (FMath::IsNearlyZero(FMath::Acos(FVector::DotProduct(vectorToLocation, GetOwner()->GetActorForwardVector())), m_rotationSpeed * DeltaTime))
	//{
	//	return true;
	//}

	//return FMath::IsNearlyZero(FMath::Acos(FVector::DotProduct(vectorToLocation, GetOwner()->GetActorForwardVector())));
	return FMath::IsNearlyZero(FMath::Atan2(vectorToLocation.Y, vectorToLocation.X) - FMath::Atan2(GetOwner()->GetActorForwardVector().Y, GetOwner()->GetActorForwardVector().X), 0.01f);

}


// Called every frame
void UUMoveHeavyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_timer -= DeltaTime;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Timer: %f"), m_timer));

	//RotateTowardLocation(DeltaTime, GetOwner()->GetActorLocation());

	switch (m_state)
	{
	case CharacterState::Standing:
		if (m_timer < 0)
		{
			SetGoalLocation();

				if (FMath::RandRange(1, 100) <= m_oddsOfShootingInPercent)
				{
					m_state = CharacterState::Shooting;
					m_oddsOfShootingInPercent -= m_oddsOfShootingOffset;
				}
				else
				{
					m_state = CharacterState::RotatingTowardGoal;
					m_oddsOfShootingInPercent = m_originalOddsOfShooting;
				}
			
			//m_timer = m_timeBetweenMovingOrShooting;
		}
		break;
	case CharacterState::Moving:
		/*MoveTowardGoalLocation(DeltaTime);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ActorLocation().X: %f"), GetOwner()->GetActorLocation().X));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("m_goalLocation.X: %f"), m_goalLocation.X));

		if (FMath::IsNearlyEqual(GetOwner()->GetActorLocation().X, m_goalLocation.X, m_movementSpeed/10.0f)
			&& FMath::IsNearlyEqual(GetOwner()->GetActorLocation().Y, m_goalLocation.Y, m_movementSpeed/10.0f))
		{
			m_state = CharacterState::RotatingTowardPlayer;
		}*/

		m_state = CharacterState::Shooting;

		break;
	case CharacterState::RotatingTowardGoal:
		RotateTowardLocation(DeltaTime, m_goalLocation);
		if (CheckRotationIsNearlyZero(DeltaTime, m_goalLocation))
		{
			//m_timer = 2;
			m_state = CharacterState::Moving;
		}
		break;
	case CharacterState::RotatingTowardPlayer:
		RotateTowardLocation(DeltaTime, m_player->GetActorLocation());
		if (CheckRotationIsNearlyZero(DeltaTime, m_player->GetActorLocation()))
		{
			m_timer = m_timeBetweenMovingOrShooting;
			m_state = CharacterState::Standing;
		}
		break;
	case CharacterState::Shooting:
		if (m_timer <= 0)
		{
			Shoot();
		}
		break;
	}
}

void UUMoveHeavyComponent::Shoot()
{
	if (m_bulletsShot >= m_numberOfBulletsToShoot)
	{
		m_bulletsShot = 0;
		m_state = CharacterState::Standing;
		m_timer = m_timeBetweenMovingOrShooting;
	}
	else
	{
		OnShoot.Broadcast();
		m_bulletsShot++;
		m_timer = m_timeBetweenEachBullet;
	}
}