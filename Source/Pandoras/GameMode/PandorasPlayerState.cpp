#include "GameMode/PandorasPlayerState.h"
#include "Net/UnrealNetwork.h"

#include "Item/ItemBase.h"
#include "GA/GA_Equip.h"

void APandorasPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ItemClasses 변수를 복제 목록에 추가
    DOREPLIFETIME(APandorasPlayerState, SwordAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, BowAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, HelmetAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, ArmorAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, GlovesAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, ShoesAbilityClasses);
}

void APandorasPlayerState::AddItemClass_Implementation(TSubclassOf<UGA_Equip> itemAbility)
{
    if (!itemAbility) return;

    // GA에 매칭된 아이템 클래스의 유효성 체크----------------------------------------------------------------
    UGA_Equip* GA_CDO = itemAbility->GetDefaultObject<UGA_Equip>();
    TSubclassOf<AItemBase> ItemClass = GA_CDO ? GA_CDO->GetItemClass() : nullptr;
    if (!ItemClass) return;

    // 타입 별로 매칭되는 배열에 저장-----------------------------------------------------------------------
    AItemBase* ItemCDO = ItemClass->GetDefaultObject<AItemBase>();
    if (!ItemCDO) return;

    EItem ItemType = EItem::UnArmed;
    if (ItemCDO->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
    {
        ItemType = IItemInterface::Execute_GetItemType(ItemCDO);
    }

    switch (ItemType)
    {
    case EItem::Sword:
    case EItem::GreatSword: // BP에서 둘 다 Sword 배열로 분기
        SwordAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::Bow:
        BowAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::Helmet:
        HelmetAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::Armor:
        ArmorAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::Glove:
        GlovesAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::Shoes:
        ShoesAbilityClasses.AddUnique(itemAbility);
        break;

    case EItem::UnArmed:
    default:
        break;
    }
}

TArray<TSubclassOf<UGA_Equip>> APandorasPlayerState::GetItemAbilityClassesByType_Implementation(EItem ItemType)
{
    switch (ItemType)
    {
    case EItem::Sword:
    case EItem::GreatSword:
        return SwordAbilityClasses;

    case EItem::Bow:
        return BowAbilityClasses;

    case EItem::Helmet:
        return HelmetAbilityClasses;

    case EItem::Armor:
        return ArmorAbilityClasses;

    case EItem::Glove:
        return GlovesAbilityClasses;

    case EItem::Shoes:
        return ShoesAbilityClasses;

    case EItem::UnArmed:
    default:
        return {};
    }
}

TArray<TSubclassOf<UGA_Equip>> APandorasPlayerState::GetAllItemAbilities_Implementation()
{
    // 모든 아이템들을 저장하는 통합 아이템 리스트 채우기----------------------------------------
    TArray<TSubclassOf<UGA_Equip>> AllClasses;

    AllClasses.Append(SwordAbilityClasses);
    AllClasses.Append(BowAbilityClasses);
    AllClasses.Append(HelmetAbilityClasses);
    AllClasses.Append(ArmorAbilityClasses);
    AllClasses.Append(GlovesAbilityClasses);
    AllClasses.Append(ShoesAbilityClasses);

    return AllClasses;
}