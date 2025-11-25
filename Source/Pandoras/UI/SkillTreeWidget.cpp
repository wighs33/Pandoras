#include "UI/SkillTreeWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"

#include "Common/Structs.h"
#include "GA/GA_Pandoras.h"
#include "AttributeSet/BaseActorAttributes.h"
#include "UI/SkillDetailsPanelWidget.h"
#include "UI/SkillButtonWidget.h"
#include "UI/SkillTreeLinesWidget.h"

#include "Interface/CharacterInterface.h"

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeTree(DTSurvivalSkills, VBSkillTree1);
	InitializeTree(DTCombatSkills, VBSkillTree2);
	InitializeTree(DTMagicSkills, VBSkillTree3);
}

// 마우스를 클릭해야 게임 입력모드로 바뀌면서 이벤트가 들어옴
void USkillTreeWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

    // 마우스 클릭 플래그 체크
    if (!MouseDown)
    {
        return;
    }

    // 마우스 클릭 플래그 OFF
    MouseDown = false;

    // 스킬 세부정보 창 숨기기
    if (WDG_SkillDetailsPanel)
    {
        WDG_SkillDetailsPanel->Hide();
    }
}

void USkillTreeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 마우스 클릭 플래그 체크
    if (!MouseDown)
    {
        return;
    }

    APlayerController* PC = GetOwningPlayer();
    if (!PC)
    {
        return;
    }

    float MouseX = 0.f;
    float MouseY = 0.f;
    if (!PC->GetMousePosition(MouseX, MouseY))
    {
        return;
    }

    const FVector2D CurrentMousePos(MouseX, MouseY);

    // [현재 마우스 위치] - [캐시된 마우스 위치]
    const FVector2D Delta = CurrentMousePos - CachedMousePosition;

    // VBMain의 CanvasPanelSlot 위치에 Delta만큼 위치 조정
    if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VBMain->Slot))
    {
        const FVector2D SlotPos = CanvasSlot->GetPosition();
        const FVector2D NewPos = SlotPos + Delta;
        CanvasSlot->SetPosition(NewPos);
    }

    // 캐시된 마우스 위치 갱신
    CachedMousePosition = CurrentMousePos;
}

void USkillTreeWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (WDG_SkillDetailsPanel)
    {
        WDG_SkillDetailsPanel->OnActivateButtonClick.AddDynamic(
            this,
            &USkillTreeWidget::OnActivateButtonClickEvent
        );
    }
}

FReply USkillTreeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    const FReply ParentReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    // 뷰포트의 마우스 위치를 저장
    CachedMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

    // 마우스 클릭 플래그 ON
    MouseDown = true;

    return ParentReply;
}

void USkillTreeWidget::OnActivateButtonClickEvent()
{
    RefreshSkillTree();
}

void USkillTreeWidget::InitializeTree_Implementation(UDataTable* InDTSkills, UVerticalBox* InVerticalBox)
{
    if (!InDTSkills)
    {
        return;
    }

    int32 MaxLevel = 0;

    // --- 데이터 테이블의 내부 데이터 각각 --------------------------------------------
    TArray<FName> RowNames = InDTSkills->GetRowNames();

    for (const FName& RowName : RowNames)
    {
        // --- 해당 데이터에서 레벨 가져오기 ---------------------------------------------
        static const FString Context(TEXT("SkillTreeWidget::InitializeTree"));

        FSkill* SkillRow = InDTSkills->FindRow<FSkill>(RowName, Context, /*bWarnIfRowMissing=*/false);

        if (!SkillRow)
        {
            continue;
        }

        const int32 Level = SkillRow->Level;

        // --- 최대 레벨(층)을 넘으면 갱신 ---------------------------------------
        if (MaxLevel < Level)
        {
            MaxLevel = Level;
        }
    }

    // --- 최대 레벨만큼 반복 ----------------------------------
    for (int32 i = 0; i <= MaxLevel; ++i)
    {
        // --- 수평박스 하나 생성하여 수직박스 안에 추가 후 가운데 정렬 ------------------
        UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this);
        UVerticalBoxSlot* VBSlot = InVerticalBox->AddChildToVerticalBox(HorizontalBox);
        VBSlot->SetHorizontalAlignment(HAlign_Center);
    }

    // --- 데이터 테이블의 내부 데이터 각각 ----------------------------------
    for (const FName& RowName : RowNames)
    {
        if (!InVerticalBox || !SkillButtonWidgetClass)
        {
            continue;
        }

        // --- 해당 데이터 가져오기 ----------------------------------------
        const FSkill* SkillRow = InDTSkills->FindRow<FSkill>(RowName, TEXT("InitializeTree"));
        if (!SkillRow)
        {
            continue;
        }

        const int32 Level = SkillRow->Level;
        UTexture2D* Texture = SkillRow->Texture;

        // --- 스킬 버튼 생성하여 (이름-버튼)쌍으로 스킬버튼 리스트에 추가 --------------
        USkillButtonWidget* SkillButton = CreateWidget<USkillButtonWidget>(
            GetWorld(),
            SkillButtonWidgetClass
        );
        if (!SkillButton)
        {
            continue;
        }
        SkillButton->GetSkillData() = *SkillRow;
        SkillButtonMap.Add(RowName, SkillButton);

        // --- 해당 인덱스의 수평박스 얻기 --------------------------------------------------------
        UWidget* LevelWidget = InVerticalBox->GetChildAt(Level);
        UHorizontalBox* LevelHorizontalBox = Cast<UHorizontalBox>(LevelWidget);
        if (!LevelHorizontalBox)
        {
            continue;
        }

        // --- 수평박스에 버튼 추가 ----------------------------------------------------------------
        UHorizontalBoxSlot* ButtonSlot = LevelHorizontalBox->AddChildToHorizontalBox(SkillButton);

        // --- 버튼 간 간격과 이미지 설정 ------------------------------------------
        if (ButtonSlot)
        {
            ButtonSlot->SetPadding(FMargin(50.f, 50.f, 50.f, 50.f));
        }

        SkillButton->ChangeButtonImage(Texture);

        // --- 스킬버튼 누르는 시점에 스킬 세부 정보 표시 -------------------------
        SkillButton->OnClickDispatch.AddDynamic(this, &USkillTreeWidget::ClickedShowSkillDetails);

        // --- 스킬 버튼에 커서 올리는 시점에 피킹 ------------------------------
        SkillButton->OnHoverDispatch.AddDynamic(this, &USkillTreeWidget::PeekSkillDetails);
    }

    // --- 데이터 테이블의 내부 데이터 각각 ----------------------------------
    for (const FName& RowName : RowNames)
    {
        // --- 해당 데이터의 선행스킬 리스트의 각각의 선행스킬 ---------------------------
        const FSkill* SkillRow = InDTSkills->FindRow<FSkill>(RowName, TEXT("InitializeTree_DependencyPass"));
        if (!SkillRow)
        {
            continue;
        }
        const TArray<FName>& Dependencies = SkillRow->Dependencies;
        for (const FName& DependencyName : Dependencies)
        {
            // --- 스킬 버튼 리스트 내에 해당 선행스킬이 있으면 라인 시작 버튼 리스트에 추가 ----------------------------
            if (USkillButtonWidget* const* StartButtonPtr = SkillButtonMap.Find(DependencyName))
            {
                LineStartButtons.Add(*StartButtonPtr);

                // --- 스킬 버튼 리스트 내에 해당 스킬이 있으면 라인 끝 버튼 리스트에 추가 --------------------------
                if (USkillButtonWidget* const* EndButtonPtr = SkillButtonMap.Find(RowName))
                {
                    LineEndButtons.Add(*EndButtonPtr);
                }
            }
        }
    }

    RefreshSkillTree();
}

void USkillTreeWidget::UpdateSkillStatus_Implementation(USkillButtonWidget* SkillButton)
{
    bool bSkillAlreadyApplied = true;
    bool bDependenciesActivated = true;

    if (!SkillButton) return;

    // --- 스킬 버튼 사용가능 ---------------------------
    SkillButton->ToggleSkillLocked(false);

    // --- 스킬데이터의 어빌리티의 유효성 체크 ------------------
    const FSkill& SkillData = SkillButton->GetSkillData();
    TSubclassOf<UGA_Pandoras> SkillAbilityClass = SkillData.SkillGameplayAbility;
    if (!SkillAbilityClass)
    {
        // 스킬 버튼 잠김
        SkillButton->ToggleSkillLocked(true);
        return;
    }

    // --- 스킬 어빌리티 내 적용된 GE 리스트의 GE 각각 --------------------
    UGA_Pandoras* AbilityCDO = SkillAbilityClass->GetDefaultObject<UGA_Pandoras>();
    if (!AbilityCDO) return;

    const TArray<TSubclassOf<UGameplayEffect>>& AppliedGameplayEffects = AbilityCDO->GetAppliedGameplayEffects();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    UAbilitySystemComponent* ASC = nullptr;

    if (!PlayerPawn) return;

    if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(PlayerPawn))
    {
        ASC = AbilitySystemInterface->GetAbilitySystemComponent();
        if (!ASC) return;
    }

    // --- 폰의 ASC에 해당 GE의 태그가 없다면 플래그 OFF -------------------------
    for (TSubclassOf<UGameplayEffect> EffectClass : AppliedGameplayEffects)
    {
        if (!EffectClass) continue;

        const UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
        if (!EffectCDO) continue;

        FGameplayTagContainer GrantedTags = EffectCDO->GetGrantedTags();

        if (GrantedTags.IsEmpty()) continue;

        if (!ASC->HasAnyMatchingGameplayTags(GrantedTags))
        {
            bSkillAlreadyApplied = false;
            break;
        }
    }

    // --- 이미 적용된 스킬이면 이후 포인트 스킬버튼을 활성화 표시 ------------------------
    if (bSkillAlreadyApplied)
    {
        SkillButton->MarkAsActivated();
        return;
    }
    auto BaseAttributes = ICharacterInterface::Execute_GetBaseActorAttribute(PlayerPawn);
    if (!BaseAttributes)
    {
        SkillButton->ToggleSkillLocked(true);
        return;
    }

    const float CurrentSkillPoints = BaseAttributes->SkillPoints.GetCurrentValue();
    const int32 SkillPointCost = SkillData.SkillPointCost;

    if (CurrentSkillPoints < static_cast<float>(SkillPointCost))
    {
        SkillButton->ToggleSkillLocked(true);
        return;
    }

    // --- 스킬 각각의 선행스킬마다 -----------------------------------------------
    const TArray<FName>& Dependencies = SkillData.Dependencies;

    for (const FName& DependencyName : Dependencies)
    {
        // --- 스킬 버튼 리스트에 해당 선행스킬이 있는 지 체크 -----------------------------
        USkillButtonWidget* const* FoundButtonPtr = SkillButtonMap.Find(DependencyName);
        const bool bFound = (FoundButtonPtr && *FoundButtonPtr);

        // --- 선행 스킬 비활성화일 때 활성화 플래그 OFF -------------------------------------
        if (!bFound || !(*FoundButtonPtr)->GetSkillActivated())
        {
            bDependenciesActivated = false;
        }
    }

    // --- 선행 스킬 비활성화일 때 스킬 버튼 잠김-------------------------------------
    if (!bDependenciesActivated)
    {
        SkillButton->ToggleSkillLocked(true);
    }
}

void USkillTreeWidget::RefreshSkillTree_Implementation()
{
    // 선 시작 & 끝 버튼 저장 (WDG_SkillTreeLines 쪽으로 전달)
    if (WDG_SkillTreeLines)
    {
        WDG_SkillTreeLines->LineStartButtons = LineStartButtons;
        WDG_SkillTreeLines->LineEndButtons = LineEndButtons;
    }

    // 스킬 버튼마다 스킬 상태 갱신
    for (auto& Pair : SkillButtonMap)
    {
        USkillButtonWidget* SkillButton = Pair.Value;
        if (!SkillButton) continue; // BP에서 None 들어오는 경우 방어

        UpdateSkillStatus(SkillButton);
    }
}

void USkillTreeWidget::ShowSkillDetails_Implementation(USkillButtonWidget* SkillButton)
{
    if (!WDG_SkillDetailsPanel) return;

    WDG_SkillDetailsPanel->SetVisibility(ESlateVisibility::Visible);
    WDG_SkillDetailsPanel->InitializeData(SkillButton);

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WDG_SkillDetailsPanel->Slot);
    if (!CanvasSlot) return;

    // 버튼 기준 위치
    const FVector2D ButtonPos = GetPosition(SkillButton, FVector2D(0.5f, 1.0f));

    // 뷰포트 크기 구하기
    FVector2D ViewportSize(0.f, 0.f);

    if (UWorld* World = GetWorld())
    {
        if (UGameViewportClient* ViewportClient = World->GetGameViewport())
        {
            ViewportClient->GetViewportSize(ViewportSize);
        }
    }

    const FVector2D HalfViewport = ViewportSize / 2.0f;

    // 스킬버튼X값 - 디테일 패널X값으로 조정
    const FVector2D PanelSize = CanvasSlot->GetSize();

    // 뷰포트 크기의 절반을 넘기는 지 체크 후 상황에 따라 디테일 패널 위치 조정
    FVector2D Pos = ButtonPos;
    if (ButtonPos.X > HalfViewport.X)
    {
        Pos.X -= PanelSize.X;
    }
    if (ButtonPos.Y > HalfViewport.Y)
    {
        Pos.Y -= PanelSize.Y;
    }

    // --- 캔버스 내 위치 이동 -----------
    CanvasSlot->SetPosition(Pos);
}

void USkillTreeWidget::ClickedShowSkillDetails_Implementation(USkillButtonWidget* SkillButton)
{
    WDG_SkillDetailsPanel->SetClickedShow(true);
    ShowSkillDetails(SkillButton);
}

void USkillTreeWidget::PeekSkillDetails_Implementation(USkillButtonWidget* SkillButton, bool Hovering)
{
    if (!WDG_SkillDetailsPanel) return;

    if (WDG_SkillDetailsPanel->GetClickedShow()) return;

    if (Hovering) ShowSkillDetails(SkillButton);
    else WDG_SkillDetailsPanel->Hide();
}

FVector2D USkillTreeWidget::GetPosition(UWidget* Button, const FVector2D& OffsetScale)
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
        this,
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