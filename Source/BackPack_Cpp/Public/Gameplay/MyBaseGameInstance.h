// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructManager.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyBaseGameInstance.generated.h"

/**
 * 将背包物品保存在游戏实例中，因为这个背包物品数组是全局实时变化的
 */
class UMeshComponent;

UCLASS()
class BACKPACK_CPP_API UMyBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FItem_Struct> BackPack_Array;  // 游戏实例中的背包物品，可全局保存和使用
    
};


