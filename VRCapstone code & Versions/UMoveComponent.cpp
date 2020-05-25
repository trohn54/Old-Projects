// Fill out your copyright notice in the Description page of Project Settings.

#include "UMoveComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include <iostream>

// Sets default values for this component's properties
UUMoveComponent::UUMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UUMoveComponent::BeginPlay()
{
	RegisterComponent();
	Super::BeginPlay();
	spawnLocation = GetOwner()->GetActorLocation();
	// ...
	UE_LOG(LogTemp, Warning, TEXT("Component Running"))
}

void UUMoveComponent::SetLocation()
{
	float x = FMath::RandRange((spawnLocation.X - wanderDistance), (spawnLocation.X + wanderDistance));
	float y = FMath::RandRange((spawnLocation.Y - wanderDistance), (spawnLocation.Y + wanderDistance));
	float z = GetOwner()->GetActorLocation().Z;
	 goalLocation = FVector(x , y, z);

	 GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("X: %f, Y: %f"), x, y));

}

void UUMoveComponent::Move()
{
	
	//TODO make uptime and downtime functionality
	//FVector Direction = goalLocation - GetOwner()->GetActorLocation();
	//Direction.Normalize();
	//FVector NewLocation += Direction * GetOwner()->GetActorLocation();
	GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(), goalLocation, 0.05f), true);
}


// Called every frame
void UUMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	timer -= DeltaTime;

	if (timer <= 0)
	{
		//Change state to Moving
		m_state = Moving;
		timer = moveWaitTime;
		SetLocation();
	}

	if (FMath::IsNearlyEqual(goalLocation.X, GetOwner()->GetActorLocation().X, 0.1f) &&
		FMath::IsNearlyEqual(goalLocation.Y, GetOwner()->GetActorLocation().Y, 0.1f))
	{

		m_state = Standing;

	}
		
	switch (m_state)
	{
	case UUMoveComponent::Moving:
		UE_LOG(LogTemp, Warning, TEXT("Location is %s"), *GetOwner()->GetActorLocation().ToString());

		Move();

		
	case UUMoveComponent::Standing:
		//SetLocation();
		UE_LOG(LogTemp, Warning, TEXT("AI Standing"))
		break;
	default:
		break;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Component Running"))
}

