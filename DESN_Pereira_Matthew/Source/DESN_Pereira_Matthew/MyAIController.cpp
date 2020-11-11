// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Assets/EnemyCharacter/EnemyAI/EnemyAIBehaviourTree.EnemyAIBehaviourTree'"));
	if (obj.Succeeded())
	{
		BehaviorTree = obj.Object;
	}
	BehaviorTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AMyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AMyAIController::GetBlackboard() const
{
	return blackboard;
}
