// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRCAPSTONE_API UUMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUMoveComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float wanderDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveWaitTime;

private:

	float timer;
	FVector spawnLocation;
	FVector direction;

	enum ActorState { Standing, Moving };
	ActorState m_state;
	
	float speed;
	FVector goalLocation;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetLocation();
	void Move();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
