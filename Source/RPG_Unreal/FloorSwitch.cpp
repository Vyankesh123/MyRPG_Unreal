// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
 

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = triggerBox;

	triggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	triggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	triggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	triggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	triggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));


	floorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	floorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	SwitchTime = 2.f;
	bCharacterOnSwitch = false;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	triggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	triggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = floorSwitch->GetComponentLocation();

}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
	if (!bCharacterOnSwitch) { bCharacterOnSwitch = true; }
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End"));
	if (bCharacterOnSwitch) bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchHandle,this,&AFloorSwitch::ClosedDoor,SwitchTime);

	
}
void AFloorSwitch::UpdateDoorLocation(float Z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += Z;
	Door->SetWorldLocation(NewLocation);
}
void AFloorSwitch::UpdateFloorSwitchLocation(float Z)
{
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += Z;
	floorSwitch->SetWorldLocation(NewLocation);

}
void AFloorSwitch::ClosedDoor()
{
	if (!bCharacterOnSwitch)
	{
	 LowerDoor();
	 RaiseFloorSwitch();

	}
}