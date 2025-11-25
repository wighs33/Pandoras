#include "UI/ItemListWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "Engine/DataTable.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "GameplayTagAssetInterface.h"

#include "UI/ItemButtonWidget.h"
#include "UI/ItemDetailsPanel.h"
#include "Item/ItemBase.h"
#include "Interface/HudInterface.h"
#include "Interface/ItemInterface.h"
#include "Interface/GASInterface.h"
#include "Interface/ItemWielderInterface.h"

void UItemListWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 클릭 시점 바인딩
	Back->OnClicked.AddDynamic(this, &UItemListWidget::OnBackButtonClicked);
}

// 인벤토리 위젯으로 전환
void UItemListWidget::OnBackButtonClicked()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	AHUD* HUD = PC->GetHUD();
	if (!HUD) return;

	IHudInterface::Execute_SwitchToInventoryUI(HUD, nullptr);
}

void UItemListWidget::InitItems_Implementation(const TArray<TSubclassOf<UGA_Equip>>& New_GA_Equip_Classes)
{
    // --- 아이템 선택 표시 없애기 ------------------------------------------
    ChangeSelectedItem(nullptr);

    // --- 장착 GA들 저장 -------------------------------------------------
    GA_Equip_Classes = New_GA_Equip_Classes;

    // --- 아이템 버튼들을 모두 숨김 처리 -------------------------------
    for (UItemButtonWidget* iItemButton : ItemButtons)
    {
        if (!iItemButton) continue; // BP에서 null이면 함수 호출이 무시되는 것과 동일하게 처리

        iItemButton->SetVisibility(ESlateVisibility::Hidden);
    }

    // --- 장착 GA 각각 ------------------------------------------------------
    for (int32 Index = 0; Index < GA_Equip_Classes.Num(); ++Index)
    {
        TSubclassOf<UGA_Equip> EquipGAClass = GA_Equip_Classes[Index];

        // --- 장착 GA 개수만큼 아이템 버튼 개수가 충분한 지 체크 -----------------
        if (ItemButtons.IsValidIndex(Index))
        {
            // --- 해당 인덱스의 아이템 버튼 저장 후 시각화 ------------------------
            ItemButton = ItemButtons[Index];
            ItemButton->SetVisibility(ESlateVisibility::Visible);

            // --- 로컬 플레이어가 GA와 맞는 아이템을 장착 중인 지 체크 ----------------
            if (IsAlreadyEquipped(EquipGAClass))
            {
                // --- 해당 아이템 버튼에 선택 표시 ------------------------------------
                ChangeSelectedItem(ItemButton);
            }
        }
        else
        {
            // --- 아이템 버튼 위젯 하나 생성 후 저장 --------------------------------------
            UItemButtonWidget* NewItemButton = CreateWidget<UItemButtonWidget>(GetOwningPlayer(), UItemButtonWidget::StaticClass());
            ItemButton = NewItemButton;

            // --- 버튼리스트에 추가 -------------------------------------
            ItemButtons.Add(ItemButton);

            // --- 그리드패널에 3열로 맞춰서 추가 ----------------------------------
            if (ItemPanel && ItemButton)
            {
                const int32 Row = Index / 3;
                const int32 Column = Index % 3;

                ItemPanel->AddChildToUniformGrid(ItemButton, Row, Column);
            }
        }

        // --- 아이템에 맞는 버튼 이미지를 아이템 데이터테이블에서 가져와서 설정------------------
        if (!DT_ItemTable)
        {
            continue;
        }

        // GA 기본값에서 ItemClass 가져오기
        UGA_Equip* EquipCDO = Cast<UGA_Equip>(EquipGAClass->GetDefaultObject());
        if (!EquipCDO || !EquipCDO->GetItemClass())
        {
            continue;
        }

        TSubclassOf<AItemBase> ItemClass = EquipCDO->GetItemClass();

        // 클래스 디스플레이 이름에서 "_C" 제거해서 데이터테이블 RowName 으로 사용
        const FString ClassDisplayName = ItemClass->GetName();
        FString RowNameString = ClassDisplayName;
        RowNameString.ReplaceInline(TEXT("_C"), TEXT(""), ESearchCase::IgnoreCase);
        const FName RowName(*RowNameString);                                                       // Conv_StringToName

        // 데이터테이블에서 ItemData 로우 가져오기
        const FString ContextString = TEXT("UItemListWidget::SetupItemButton");
        const FItemData* ItemDataRow = DT_ItemTable
            ? DT_ItemTable->FindRow<FItemData>(RowName, ContextString, /*bWarnIfMissing=*/false)
            : nullptr;

        if (ItemDataRow)
        {
            // 버튼에 아이템 데이터 초기화
            ItemButton->InitializeItemData(*ItemDataRow);

            // --- 버튼에 번호 부여 --------------------------------
            ItemButton->SetButtonIndex(Index);

            // --- 클릭/Hover 델리게이트에 버튼 이벤트 구독 --------------------------
            ItemButton->OnClickDispatch.Clear();
            ItemButton->OnClickDispatch.AddDynamic(this,&UItemListWidget::OnItemButtonPressed);
            ItemButton->OnHoverDispatch.AddDynamic(this,&UItemListWidget::PeekItemDetails);
        }
        else
        {
            const FString Msg = FString::Printf(
                TEXT("ItemData Not Found for Row: %s"),
                *RowName.ToString()
            );

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, Msg);
            }
        }
    }
}

// 선택 표시 아이템 변경
void UItemListWidget::ChangeSelectedItem_Implementation(UItemButtonWidget* NewSelectedItem)
{
    // --- [old 선택된 아이템] 유효성 체크 + 선택 표시 끄기 ---------------------
    if (IsValid(SelectedItem))
    {
        SelectedItem->ToggleSelect(false);
    }

    // --- [new 선택된 아이템] 유효성 체크 ------------------------------------
    if (!IsValid(NewSelectedItem))
    {
        return;
    }

    // --- 저장 후 선택 표시 켜기 ---------------------------
    SelectedItem = NewSelectedItem;
    SelectedItem->ToggleSelect(true);
}

bool UItemListWidget::IsAlreadyEquipped_Implementation(TSubclassOf<UGA_Equip> EquipGA)
{
    if (!EquipGA)
    {
        return false;
    }

    const UGA_Equip* EquipCDO = EquipGA->GetDefaultObject<UGA_Equip>();
    if (!EquipCDO || !EquipCDO->GetItemClass())
    {
        return false;
    }

    AItemBase* ItemCDO = EquipCDO->GetItemClass()->GetDefaultObject<AItemBase>();
    if (!ItemCDO)
    {
        return false;
    }

    const FGameplayTagContainer& ItemTags = IItemInterface::Execute_GetItemTag(ItemCDO);

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (!PlayerCharacter)
    {
        return false;
    }

    const AActor* PlayerActor = PlayerCharacter;

    UAbilitySystemComponent* ASC =
        UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerActor);

    if (!ASC)
    {
        return false;
    }

    return ASC->HasAllMatchingGameplayTags(ItemTags);
}

void UItemListWidget::PeekItemDetails_Implementation(UItemButtonWidget* InItemButton, bool Hovered)
{
    if (!WDG_ItemDetailsPanel) return;

    if (!Hovered)
    {
        WDG_ItemDetailsPanel->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    WDG_ItemDetailsPanel->SetVisibility(ESlateVisibility::Visible);
    WDG_ItemDetailsPanel->InitializeData(InItemButton);

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WDG_ItemDetailsPanel->Slot);
    if (!CanvasSlot) return;

    // 버튼 기준 위치
    const FVector2D ButtonPos = GetPosition(InItemButton, FVector2D(0.5f, 1.0f));

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

void UItemListWidget::OnItemButtonPressed(int32 WeaponIndex)
{
    // --- 인덱스에 맞는 아이템 선택을 변경 ---------------------
    UItemButtonWidget* NewSelectedItem = nullptr;
    if (ItemButtons.IsValidIndex(WeaponIndex))
    {
        NewSelectedItem = ItemButtons[WeaponIndex];
    }
    ChangeSelectedItem(NewSelectedItem);

    // --- 해당 인덱스의 장착GA 가져와서 저장 -----------------------
    TSubclassOf<UGA_Equip> NewSelectedGA = nullptr;
    if (GA_Equip_Classes.IsValidIndex(WeaponIndex))
    {
        NewSelectedGA = GA_Equip_Classes[WeaponIndex];
    }
    Selected_GA_Equip = NewSelectedGA;

    if (!Selected_GA_Equip)
    {
        return;
    }

    UGA_Equip* EquipCDO = Selected_GA_Equip->GetDefaultObject<UGA_Equip>();
    if (!EquipCDO)
    {
        return;
    }

    TSubclassOf<AItemBase> ItemClass = EquipCDO->GetItemClass();
    if (!ItemClass)
    {
        return;
    }

    AItemBase* ItemCDO = ItemClass->GetDefaultObject<AItemBase>();
    if (!ItemCDO)
    {
        return;
    }

    const EItem ItemType = IItemInterface::Execute_GetItemType(ItemCDO);

    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    if (!PlayerCharacter)
    {
        return;
    }

    UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerCharacter);
    FGameplayTagContainer UnequipTag;
    UnequipTag.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Event.Unequip")));

    auto GiveAndActivateSelectedAbility = [this, PlayerCharacter]()
        {
            IGASInterface::Execute_GiveAndActivateAbility_Replicate(
                PlayerCharacter,
                Selected_GA_Equip
            );
        };

    // 아이템 타입 스위치
    switch (ItemType)
    {
        // 무기
    case EItem::Sword:
    case EItem::GreatSword:
    case EItem::Bow:
    {
        // --- 무기 장착 해제 실행 후 성공 여부 체크 ------------------------------

        bool bActivatedUnequip = false;
        if (ASC)
        {
            bActivatedUnequip = ASC->TryActivateAbilitiesByTag(UnequipTag, true);
        }

        if (bActivatedUnequip)
        {
            if (UWorld* World = GetWorld())
            {
                FTimerHandle TimerHandle;
                FTimerDelegate TimerDel;
                TimerDel.BindLambda(GiveAndActivateSelectedAbility);

                World->GetTimerManager().SetTimer(
                    TimerHandle,
                    TimerDel,
                    1.5f,
                    /*bLoop=*/false
                );
            }
        }
        else
        {
            GiveAndActivateSelectedAbility();
        }

        break;
    }

    // 무기 아닌 장비
    case EItem::Helmet:
    case EItem::Armor:
    case EItem::Glove:
    case EItem::Shoes:
    {
        // --- 아이템 삭제 ----------------------------------------------
        IItemWielderInterface::Execute_DestroyItem(PlayerCharacter, ItemType);
        GiveAndActivateSelectedAbility();
        break;
    }

    case EItem::UnArmed:
    default:
        break;
    }
}

FVector2D UItemListWidget::GetPosition(UWidget* Button, const FVector2D& OffsetScale)
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