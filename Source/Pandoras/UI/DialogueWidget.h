// 대화창 위젯

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/Structs.h"
#include "DialogueWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UBorder;
class UEditableText;
class URichTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTalkEnd);

UCLASS()
class PANDORAS_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 이미지 변경
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ChangeImage(UTexture2D* Image);

	// 다음 대화 보여주기
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "C++")
	void ShowNext();

// 위젯
protected:
	// BindWidget: 이름만 맞춰두면 엔진이 자동으로 연결
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> CharacterImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> DialogueImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> NameButtonCancel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> NameButtonOk;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> NameWindow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableText> PlayerNameInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> TalkText;

// 변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UAudioComponent> CurrentVoice;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	int32 TalkIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	TArray<FDialogue> DialogueData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++")
	FString PlayerNickname = TEXT("플레이어");

// 델리게이트
protected:
	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "C++")
	FOnTalkEnd TalkEnd;
};