// Fill out your copyright notice in the Description page of Project Settings.


#include "BackPackSystem/BackPackSlot.h"
#include <string>

#include "BackPack_Cpp/BackPack_CppCharacter.h"
#include "E:\Unreal Engine 5\UE_5.4\Engine\Source\Runtime\UMG\Public\Blueprint\DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UBackPackSlot::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<ABackPack_CppCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UBackPackSlot::SetSlot()
{
	/* 主要去渲染格子内容，通过Number判断 */
	if (Number > 1)
	{
		// 需要显示出右下角数字
		Number_Text->SetText(FText::FromString(FString(std::to_string(Number).c_str())));
		Number_Text->SetOpacity(1.0f);
	}
	else
	{
		// 不需要显示数字，直接隐藏掉
		Number_Text->SetOpacity(0.0f);
	}
	// 将Texture2D类型传递给自定义的Image组件去渲染
	FSlateBrush InBrush;
	InBrush.SetResourceObject(Image);
	SlotImage->SetBrush(InBrush);
}

FReply UBackPackSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Player->BackPackComponent->bHasOperateUI = false;
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton )
	{
		Player->BackPackComponent->SetOperateUI(Index);  // 调用背包组件中的SetOpetateUI函数
		return FReply::Handled(); 
	}
	else
	{
		Player->BackPackComponent->bHasOperateUI = true;
		Player->BackPackComponent->SetOperateUI(Index);  // 调用背包组件中的SetOpetateUI函数
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
}

void UBackPackSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (Index == -1) return;

	UDragDropOperation* DragOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragOp)
	{
		DragOp->Payload = this;
		DragOp->DefaultDragVisual =SlotImage;
		DragOp->Pivot = EDragPivot::BottomRight;  // 不要设置到居中不然鼠标指针落点会判定不上
		DragOp->Offset = FVector2D(0.f, 0.f);
	}
	OutOperation = DragOp;  // 直接赋值给输出参数
}

bool UBackPackSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )
{
	if (!InOperation || !InOperation->Payload) return false;
	
	// BackPackSlot是拖动的目标格子，不是拖动的那个函数
	UBackPackSlot* BackPackSlot = Cast<UBackPackSlot>(InOperation->Payload);
	if (!BackPackSlot) return false;
	
	// 检查边界（如果放置的格子Index小于0无效）
	if (BackPackSlot->Index < 0 || this->Index < 0) return false;
	
	UWorld* World = GetWorld();
	if (!World) return false;
	Player =Cast<ABackPack_CppCharacter>(World->GetFirstPlayerController()->GetCharacter());
	if (Player && Player->BackPackComponent)
	{
		Player->BackPackComponent->SwapItem(BackPackSlot->Index, this->Index);
		return true;  // !!
	}
	
	return false;
}







