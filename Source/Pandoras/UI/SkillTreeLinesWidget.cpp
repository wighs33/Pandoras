#include "UI/SkillTreeLinesWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "UI/SkillButtonWidget.h"

int32 USkillTreeLinesWidget::NativePaint(
    const FPaintArgs& Args,
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId,
    const FWidgetStyle& InWidgetStyle,
    bool bParentEnabled
) const
{
    const int32 SuperLayerId = Super::NativePaint(
        Args,
        AllottedGeometry,
        MyCullingRect,
        OutDrawElements,
        LayerId,
        InWidgetStyle,
        bParentEnabled
    );

    FPaintContext Context(
        AllottedGeometry,
        MyCullingRect,
        OutDrawElements,
        SuperLayerId,
        InWidgetStyle,
        bParentEnabled
    );

    PaintSkillLines(Context);

    return Context.MaxLayer;
}


void USkillTreeLinesWidget::PaintSkillLines(FPaintContext& InContext) const
{
    const int32 Num = LineStartButtons.Num();

    for (int32 i = 0; i < Num; ++i)
    {
        USkillButtonWidget* StartButton = LineStartButtons[i];
        USkillButtonWidget* EndButton = LineEndButtons[i];

        if (!StartButton || !EndButton) continue;

        // Start: 버튼 아래쪽 중앙 (OffsetScale = (0.5, 1.0))
        const FVector2D PositionA = GetPosition(StartButton, FVector2D(0.5f, 1.0f));

        // End: 버튼 위쪽 중앙 (OffsetScale = (0.5, 0.0))
        const FVector2D PositionB = GetPosition(EndButton, FVector2D(0.5f, 0.0f));

        // 선으로 연결
        UWidgetBlueprintLibrary::DrawLine(
            InContext,
            PositionA,
            PositionB,
            /*색*/FLinearColor::White,
            /*AntiAlias*/true,
            /*두께*/1.0f
        );
    }
}

FVector2D USkillTreeLinesWidget::GetPosition(USkillButtonWidget* Button, const FVector2D& OffsetScale) const
{
    if (!Button)
    {
        return FVector2D::ZeroVector;
    }

    // --- 캐시된 Geometry 를 가져와서 (0,0) 로컬 좌표를 뷰포트 좌표로 변환 ---------------
    const FGeometry& Geometry = Button->GetCachedGeometry();

    FVector2D PixelPosition;    // 뷰포트 픽셀 좌표 (DPI 적용 전)
    FVector2D ViewportPosition;

    USlateBlueprintLibrary::LocalToViewport(
        GetWorld(),
        Geometry,
        FVector2D::ZeroVector,
        /*out*/ PixelPosition,
        /*out*/ ViewportPosition
    );

    // DPI / 뷰포트 스케일 보정
    const float ViewportScale =
        UWidgetLayoutLibrary::GetViewportScale(this);

    // 픽셀 위치 / 뷰포트 스케일 = 실제 화면 좌표
    const float SafeScale = FMath::Max(ViewportScale, KINDA_SMALL_NUMBER);
    const FVector2D PosInViewportSpace = PixelPosition / SafeScale;

    // 버튼의 원하는 사이즈 * OffsetScale = 오프셋
    const FVector2D DesiredSize = Button->GetDesiredSize();
    const FVector2D Offset = DesiredSize * OffsetScale;

    // 최종 위치 = (픽셀 위치 / 뷰포트 스케일) + (버튼 사이즈 * OffsetScale)
    return PosInViewportSpace + Offset;
}