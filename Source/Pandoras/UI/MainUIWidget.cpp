#include "UI/MainUIWidget.h"
#include "UI/TabButtonWidget.h"
#include "Components/Button.h"

void UMainUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 클릭 시점 바인딩
    TabBtnMenu->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnMenuClicked);
    TabBtnSkills->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnSkillsClicked);
    TabBtnInventory->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnInventoryClicked);
    TabBtnMap->OnTabButtonClicked.AddDynamic(this, &UMainUIWidget::OnTabBtnMapClicked);
}
