#include "GameMode/PandorasPlayerState.h"
#include "Item/ItemBase.h"
#include "Net/UnrealNetwork.h"

//TArray<TSubclassOf<AItemBase>> APandorasPlayerState::GetItemClasses()
//{
//    return ItemClasses;
//}
//
//void APandorasPlayerState::AddItemClass(TSubclassOf<AItemBase> item)
//{
//    ItemClasses.AddUnique(item);
//    //__debugbreak();
//}

void APandorasPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ItemClasses ������ ���� ��Ͽ� �߰�
    DOREPLIFETIME(APandorasPlayerState, SwordAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, BowAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, GloveAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, HelmetAbilityClasses);
    DOREPLIFETIME(APandorasPlayerState, ArmorAbilityClasses);
}
