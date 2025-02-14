// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseCharacter.h"

int32 AGHBaseCharacter::ID = 0;

// Sets default values
AGHBaseCharacter::AGHBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		ID++;
	}
}

// Called when the game starts or when spawned
void AGHBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGHBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AGHBaseCharacter::GetID()
{
	return ID;
}

