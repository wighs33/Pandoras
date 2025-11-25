#include "UI/InventoryMainWidget.h"
#include "ItemButtonWidget.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"

void UInventoryMainWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BTN_Sword)
    {
        BTN_Sword->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnSwordButtonPressed);
    }

    if (BTN_Bow)
    {
        BTN_Bow->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnBowButtonPressed);
    }

    if (BTN_Helmet)
    {
        BTN_Helmet->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnHelmetButtonPressed);
    }

    if (BTN_Armor)
    {
        BTN_Armor->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnArmorButtonPressed);
    }

    if (BTN_Glove)
    {
        BTN_Glove->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnGloveButtonPressed);
    }

    if (BTN_Shoes)
    {
        BTN_Shoes->OnClickDispatch.AddDynamic(this, &UInventoryMainWidget::OnShoesButtonPressed);
    }
}

// 검 아이템 리스트로 전환
void UInventoryMainWidget::OnSwordButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Sword);
}

// 활 아이템 리스트로 전환
void UInventoryMainWidget::OnBowButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Bow);
}

// 투구 아이템 리스트로 전환
void UInventoryMainWidget::OnHelmetButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Helmet);
}

// 갑옷 아이템 리스트로 전환
void UInventoryMainWidget::OnArmorButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Armor);
}

// 장갑 아이템 리스트로 전환
void UInventoryMainWidget::OnGloveButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Glove);
}

// 신발 아이템 리스트로 전환
void UInventoryMainWidget::OnShoesButtonPressed(int32 Index)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AHUD* HUD = PC->GetHUD();
    if (!HUD) return;

    IHudInterface::Execute_SwitchToItemListUI(HUD, EItem::Shoes);
}
