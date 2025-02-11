// Fill out your copyright notice in the Description page of Project Settings.


#include "GHBaseCharacter.h"

// Sets default values
AGHBaseCharacter::AGHBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

