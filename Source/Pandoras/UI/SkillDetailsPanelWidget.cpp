#include "UI/SkillDetailsPanelWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"
#include "Kismet/GameplayStatics.h"

#include "GA/GA_Pandoras.h"
#include "UI/FillButtonWidget.h"
#include "UI/SkillButtonWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Interface/GASInterface.h"
#include "Interface/HudInterface.h"

void USkillDetailsPanelWidget::NativeConstruct()
{
    Super::NativeConstruct();

	if (FillButtonActivate)
	{
		FillButtonActivate->OnFilled.AddDynamic(this, &USkillDetailsPanelWidget::Activate);
	}

	if (BCancel)
	{
		BCancel->OnClicked.AddDynamic(this, &USkillDetailsPanelWidget::HandleCancelClicked);
	}
}

void USkillDetailsPanelWidget::HandleCancelClicked()
{
    Hide();
}

void USkillDetailsPanelWidget::Hide()
{
    // 숨기기
    SetVisibility(ESlateVisibility::Hidden);
    ClickedShow = false;
}

void USkillDetailsPanelWidget::Activate()
{
    // --- 폰에게 해당 스킬 활성화 어빌리티 부여하고 실행 ---------------------------------
    APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
    TSubclassOf<UGA_Pandoras> AbilityClass = SkillData.SkillGameplayAbility;
    if (!AbilityClass)
    {
        return;
    }
    IGASInterface::Execute_GiveAndActivateAbility_Replicate(Pawn, AbilityClass);

    // --- 스킬 활성화 어빌리티에서 GE목록 뽑아서 활성화 시점에만 캐릭터에 적용하기 ----------------
    UGA_Pandoras* AbilityCDO = AbilityClass->GetDefaultObject<UGA_Pandoras>();

	if (AbilityCDO)
	{
		const TArray<TSubclassOf<UGameplayEffect>>& AppliedEffects = 
            AbilityCDO->GetAppliedGameplayEffects();

		for (TSubclassOf<UGameplayEffect> EffectClass : AppliedEffects)
		{
			IGASInterface::Execute_ApplyGameplayEffect_Replicate(Pawn, EffectClass);
		}
	}

    // 패널 숨기기
    Hide();

    // 캐릭터 레벨UI 잠시 보이기
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        AHUD* HUD = PC->GetHUD();
        if (HUD)
        {
            IHudInterface::Execute_ShowCharacterLevel(HUD, 50.f);
        }
    }

    // 브로드캐스트
    CallOnActivateButtonClick();
}

void USkillDetailsPanelWidget::InitializeData_Implementation(USkillButtonWidget* SkillButton)
{
    if (!SkillButton)
    {
        return;
    }

    // --- 버튼의 스킬데이터 저장 ---------------------------------
    SkillData = SkillButton->GetSkillData();
    const FSkill& Skill = SkillData;

    // --- 스킬 이름을 얻어서 텍스트에 기입 --------------------------
    if (TextSkillName)
    {
        TextSkillName->SetText(FText::FromString(Skill.SkillName));
    }

    // --- 스킬 이미지를 얻어서 넣기 ------------------------------------
    if (ImgSkill)
    {
        ImgSkill->SetBrushFromTexture(Skill.Texture, false);
    }

    // --- 스킬 포인트 비용 텍스트 설정 ------------------------------------
    if (TextSkillCost)
    {
        TextSkillCost->SetText(FText::AsNumber(Skill.SkillPointCost));
    }

    // --- 스킬 설명 추가 --------------------------------------------
    if (TextSkillDescription)
    {
        TextSkillDescription->SetText(Skill.SkillDescription);
    }

    // --- 스킬 버튼의 활성화 여부에 따라 Activate 버튼 활성화 -------------
    if (FillButtonActivate)
    {
        FillButtonActivate->Reset();

        const bool bLockedOrActivated =
            (SkillButton->GetLocked() || SkillButton->GetSkillActivated());

        FillButtonActivate->SetButtonEnabled(!bLockedOrActivated);
    }

    // --- 캐릭터 레벨 UI 잠시 보이기 ------------------------------------------
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AHUD* HUD = PC->GetHUD())
        {
            IHudInterface::Execute_ShowCharacterLevel(HUD, 50.f);
        }
    }
}