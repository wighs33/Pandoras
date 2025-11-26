#include "UI/DialogueWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/RichTextBlock.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"

void UDialogueWidget::ChangeImage_Implementation(UTexture2D* Image)
{
    if (!CharacterImage) return;

    CharacterImage->SetBrushFromTexture(Image, /*bMatchSize=*/true);
}

void UDialogueWidget::ShowNext_Implementation()
{
    // 입력 잠금
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetIgnoreMoveInput(true);
        PC->SetIgnoreLookInput(true);
    }

    ++TalkIndex;

    // 인덱스 유효성 검사
    if (!DialogueData.IsValidIndex(TalkIndex))
    {
        // 알림 보내고 인덱스 초기화
        TalkEnd.Broadcast();
        TalkIndex = -1;

        // 입력 잠금 해제
        if (PC)
        {
            PC->ResetIgnoreMoveInput();
            PC->ResetIgnoreLookInput();
        }
        return;
    }

    // 현재 대화 레코드
    const FDialogue& Row = DialogueData[TalkIndex];

    // 본문 텍스트
    if (TalkText)
    {
        TalkText->SetText(FText::FromString(Row.TalkContent));
    }

    // 캐릭터 이미지
    ChangeImage(Row.CharacterImage);

    // 캐릭터 이름
    if (CharacterName)
    {
        CharacterName->SetText(FText::FromString(Row.CharacterName));
    }

    // 기존 보이스가 유효하면 멈추기
    if (IsValid(CurrentVoice))
    {
        CurrentVoice->Stop();
        CurrentVoice = nullptr;
    }

    // 플레이어 폰 Root에 보이스 스폰
    if (Row.Voice)
    {
        APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
        USceneComponent* AttachTo = Pawn ? Pawn->GetRootComponent() : nullptr;

        if (AttachTo)
        {
            CurrentVoice = UGameplayStatics::SpawnSoundAttached(
                Row.Voice,
                AttachTo,
                NAME_None,
                FVector::ZeroVector,
                FRotator::ZeroRotator,
                EAttachLocation::KeepRelativeOffset,
                /*bStopWhenAttachedToDestroyed=*/ false,
                /*VolumeMultiplier=*/ 1.f,
                /*PitchMultiplier=*/ 1.f,
                /*StartTime=*/ 0.f,
                /*AttenuationSettings=*/ nullptr,
                /*ConcurrencySettings=*/ nullptr,
                /*bAutoDestroy=*/ true
            );
        }
        else
        {
            // 폰이 없으면 2D로라도 재생
            UGameplayStatics::PlaySound2D(this, Row.Voice, 1.f, 1.f, 0.f);
        }
    }

    // Trigger == true 일 때만 추가 분기
    // 캐릭터 이름이 "디엘 이브"면 NameWindow -> Visible
    if (Row.Trigger)
    {
        const bool bIsSpecialName = (Row.CharacterName == TEXT("디엘 이브"));
        if (bIsSpecialName)
        {
            if (NameWindow)
            {
                NameWindow->SetVisibility(ESlateVisibility::Visible);
            }

            if (PC)
            {
                if (AHUD* HUD = PC->GetHUD())
                {
                    if (HUD->GetClass()->ImplementsInterface(UHudInterface::StaticClass()))
                    {
                        // Execute_를 써야 BP override를 탈 수 있음
                        IHudInterface::Execute_ToggleUIInput(HUD, /*bInput=*/true);
                    }
                }
            }
        }
    }

}
