#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeLinesWidget.generated.h"

class USkillButtonWidget;

UCLASS()
class PANDORAS_API USkillTreeLinesWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;

	virtual void PaintSkillLines(FPaintContext& InContext) const;

private:
	FVector2D GetPosition(USkillButtonWidget* SkillButton, const FVector2D& OffsetScale) const;

// º¯¼ö
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineStartButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<TObjectPtr<USkillButtonWidget>> LineEndButtons;
};
