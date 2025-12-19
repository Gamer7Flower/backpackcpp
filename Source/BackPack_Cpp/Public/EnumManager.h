// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumManager.generated.h"

/**
 * 
 */
UENUM(Blueprintable)
enum class EItem_type : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Prop UMETA(DisplayName = "Prop"),
	Food UMETA(DisplayName = "Food")
};
UCLASS()
class BACKPACK_CPP_API UEnumManager : public UObject
{
	GENERATED_BODY()
	
};
