// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackSystem/BackPackComponent.h"

#include "BackPackSystem/BackPackUI.h"
#include "BackPackSystem/ItemBase.h"

// Sets default values for this component's properties
UBackPackComponent::UBackPackComponent()
{

}

void UBackPackComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBackPackComponent::AddItem(FItem_Struct NewItem)
{
	UWorld*world = GetWorld();
	if (!world) return;
	UMyBaseGameInstance* MyGameInstance = Cast<UMyBaseGameInstance>(GetWorld()->GetGameInstance());
	if (!MyGameInstance) return;
	TArray<FItem_Struct>& BackpackArray = MyGameInstance->BackPack_Array;
    
	// 查找背包中是否有相同名称的物品
	for (auto& Item : BackpackArray)
	{
		if (Item.Name == NewItem.Name && Item.Can_Stack && Item.Count < Item.Max_Count)
		{
			Item.Count += NewItem.Count;
			return;
		}
	}
	// 如果没有找到匹配项或者物品不可堆叠，则作为新物品添加
	NewItem.Index = BackpackArray.Num();
	BackpackArray.Add(NewItem);
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Add！"));
}


void UBackPackComponent::ThrowItem(int32 SlotIndex)
{
	if (!IsValid(GetWorld())) return;
	UMyBaseGameInstance* MyGameInstance = Cast<UMyBaseGameInstance>(GetWorld()->GetGameInstance());
    
	// 直接使用 GameInstance 的数组，不要复制
	TArray<FItem_Struct>& BackPackArray = MyGameInstance->BackPack_Array;
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("SlotIndex:%d,Num:%d"), SlotIndex,BackPackArray.Num()));
	// 加强边界检查
	if (SlotIndex < 0 || SlotIndex >= BackPackArray.Num()) return;
    
	if (BackPackArray.Num() > 0)
	{
		FString Name = BackPackArray[SlotIndex].Name;
		AActor* SpawnActor = SpawnActorClass(Name);
        
		if (SpawnActor)
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				APawn* PlayerPawn = PlayerController->GetPawn();
				if (PlayerPawn)
				{
					FVector Location = PlayerPawn->GetActorLocation();
					FVector SpawnLocation = Location + FVector(0.f, 0.f, -80.f);
					SpawnActor->SetActorLocation(SpawnLocation);
				}
                
				// 修改原始数组
				BackPackArray[SlotIndex].Count--;
				if (BackPackArray[SlotIndex].Count <= 0) BackPackArray.RemoveAt(SlotIndex);
			}
			// 更新 UI
			if (BackPackUI)
			{
				BackPackUI->BackPackTransfer = BackPackArray;
				BackPackUI->RefreshBackPack();
			}
		}
	}
}
AActor* UBackPackComponent::SpawnActorClass(const FString& Name)
{
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Yellow, TEXT("DoSpawn!"));
	if (!IsValid(GetWorld())) return nullptr;

	// 通过物品名称加载蓝图路径
	FString BlueprintPath = FString::Printf(TEXT("/Game/BackPackSystem/Item/%s.%s_C"),*Name,*Name);

	// 加载对应蓝图类
	UClass*ActorClass = LoadClass<AItemBase>(nullptr,*BlueprintPath);

	if (ActorClass)
	{

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass);

		return SpawnedActor;
	}
	return nullptr;
}

void UBackPackComponent::SwapItem(int32 DragIndex, int32 DropIndex)
{
	// DragIndex:拖动的格子的Index ； DropIndex:放置的格子的Index

	// 边界检查1：如果两个格子的Index都小于零直接返回
	if (DragIndex < 0 || DropIndex < 0) return;

	// 边界检查2：如果两个格子的Index相等直接返回
	if (DragIndex == DropIndex) return;
	
	if (!IsValid(GetWorld())) return;
	UMyBaseGameInstance* MyGameInstance = Cast<UMyBaseGameInstance>(GetWorld()->GetGameInstance());
	TArray<FItem_Struct>& BackPackArray = MyGameInstance->BackPack_Array;
	
	FItem_Struct TempItem = BackPackArray[DragIndex];
	BackPackArray[DragIndex] = BackPackArray[DropIndex];  // !!!
	BackPackArray[DropIndex] = TempItem;
	MyGameInstance->BackPack_Array = BackPackArray;
	if (BackPackUI)
	{
		BackPackUI->BackPackTransfer = BackPackArray;
		BackPackUI->RefreshBackPack();
	}
}


void UBackPackComponent::SetOperateUI(int32 SlotIndex)
{
	if (!IsValid(GetWorld())) return;
	APlayerController*OwningPlayer = GetWorld()->GetFirstPlayerController();
	TSubclassOf<UOperateUI> WidgetClass = LoadClass<UOperateUI>(nullptr, TEXT("/Game/BackPackSystem/UMG/UMG_OperateUI.UMG_OperateUI_C"));
	
	if (!bHasOperateUI && SlotIndex >= 0)
	{
		if (IsValid(OperateUI)) OperateUI->RemoveFromParent();
		OperateUI = CreateWidget<UOperateUI>(OwningPlayer, WidgetClass);
		if (OperateUI)
		{
			OperateUI->AddToViewport();
			float MouseX,MouseY;
			OwningPlayer->GetMousePosition(MouseX,MouseY);
			OperateUI->SetPositionInViewport(FVector2D(MouseX,MouseY),true);
			OperateUI->SlotIndex = SlotIndex;
			
		}
	}
	else
	{
		if (OperateUI)
		{
			OperateUI->RemoveFromParent();
			OperateUI = nullptr;
			
		}
	}
}


void UBackPackComponent::BackPackUIController()
{
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("CreateBackPack!!!"));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();  // 获取玩家控制器
	if (!bIsOpenBackPack)
	{
		if (!IsValid(GetWorld())) return;
		APlayerController*OwningPlayer = GetWorld()->GetFirstPlayerController();
		TSubclassOf<UUserWidget> WidgetClass = LoadClass<UBackPackUI>(nullptr, TEXT("/Game/BackPackSystem/UMG/UMG_BackPack.UMG_BackPack_C"));
		
		BackPackUI = CreateWidget<UBackPackUI>(OwningPlayer, WidgetClass);
		if (BackPackUI)
		{
			BackPackUI->BackPackTransfer = Cast<UMyBaseGameInstance>(GetWorld()->GetGameInstance())->BackPack_Array;  // GameInstance里存放的数组
			BackPackUI->AddToViewport();
			bIsOpenBackPack = true;
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		if (BackPackUI)
		{
			BackPackUI->RemoveFromParent();
		}
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		bIsOpenBackPack = false;
		if (OperateUI) OperateUI->RemoveFromParent();
	}
}








