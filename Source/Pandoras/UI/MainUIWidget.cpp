#include "UI/MainUIWidget.h"
#include "UI/TabButtonWidget.h"
#include "Components/Button.h"

void UMainUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư Ŭ�� ���� ���ε�
    TabBtnMenu->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnMenuClicked);
    TabBtnSkills->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnSkillsClicked);
    TabBtnInventory->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnInventoryClicked);
    TabBtnMap->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnMapClicked);
}
