// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackSystem/OperateUI.h"

#include "BackPack_Cpp/BackPack_CppCharacter.h"

void UOperateUI::NativeConstruct()
{
	Super::NativeConstruct();

	UseButton->OnClicked.AddDynamic(this,&UOperateUI::OnUseButtonClick);
	ThrowButton->OnClicked.AddDynamic(this,&UOperateUI::OnThrowButtonClick);
}

void UOperateUI::OnUseButtonClick()
{
	// 调用组件中的UseItem函数
	// 与GAS绑定
	//RemoveFromParent();if (!IsValid(GetWorld())) return;
	UMyBaseGameInstance* MyGameInstance = Cast<UMyBaseGameInstance>(GetWorld()->GetGameInstance());
    
	// 直接使用 GameInstance 的数组，不要复制
	TArray<FItem_Struct>& BackPackArray = MyGameInstance->BackPack_Array;
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("SlotIndex:%d,Num:%d"), SlotIndex,BackPackArray.Num()));
}

void UOperateUI::OnThrowButtonClick()
{
	if (!IsValid(GetWorld())) return;
	ABackPack_CppCharacter* Player = Cast<ABackPack_CppCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UBackPackComponent* BackPackComponent = Player->BackPackComponent;
    
	if (SlotIndex >= 0)  // 基本的边界检查
	{
		BackPackComponent->ThrowItem(SlotIndex);
	}
	RemoveFromParent();
}
