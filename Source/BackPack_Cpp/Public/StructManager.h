// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumManager.h"
#include "StructManager.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct FItem_Struct : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString Name;  // 物品名
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Index;  // 索引
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* Icon;  // 图标
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Count;  // 当前数量
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Max_Count;  // 最大数量
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Can_Stack;  // 是否可堆叠
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EItem_type Item_Type;  // 物品类型
};
UCLASS()
class BACKPACK_CPP_API UStructManager : public UObject
{
	GENERATED_BODY()
	
};
