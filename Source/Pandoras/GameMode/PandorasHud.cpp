#include "GameMode/PandorasHud.h"
#include "Blueprint/UserWidget.h"
#include "Room/InventoryRoom.h"
#include "UI/MainUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/PlayerStateInterface.h"
#include "GameMode/PandorasPlayerState.h"

// 플레이 시작
void APandorasHud::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = GetOwningPlayerController();
	if (!ensure(pc)) return;

	MainUIWidget = CreateWidget<UMainUIWidget>(pc, MainUIWidgetClass);
	if (!ensure(MainUIWidget)) return;

	MainUIWidget->AddToViewport();
	InitWidget();
}

// 화면UI들 초기화
void APandorasHud::InitWidget()
{
	WDG_InGameHud = MainUIWidget->GetInGameHud();
	WDG_InventoryMain = MainUIWidget->GetInventoryMain();
	WDG_WeaponList = MainUIWidget->GetWeaponList();
}

// 인벤토리 UI로 전환
// 인벤토리 룸이 없다면 매개변수의 인벤토리 룸을 저장
// 메인 위젯을 사용해 인벤토리 메인 위젯으로 전환
// 메인위치에 포커싱
void APandorasHud::SwitchToInventoryUI(AActor* inventory_room)
{
	if (!InventoryRoom)
	{
		if (inventory_room)
		{
			InventoryRoom = CastChecked<AInventoryRoom>(inventory_room);
			if (!ensure(InventoryRoom)) return;
		}
	}
	MainUIWidget->SwitchWidget(WDG_InventoryMain);
	InventoryRoom->ChangeFocusPoint(ECharacterFocusPoint::Main);
}

// UI 입력 토글
// 플래그 on 일 때
// 게임&UI 입력 모드로 변경하고 마우스 커서 등장
// 플래그 off 일 때
// 게임 전용 입력 모드로 변경하고 마우스 커서 사라짐
void APandorasHud::ToggleUIInput(bool b_input)
{
	APlayerController* pc = GetOwningPlayerController();
	if (!ensure(pc)) return;

	if (b_input)
	{
		pc->SetInputMode(FInputModeGameAndUI());
		pc->bShowMouseCursor = true;
	}
	else
	{
		pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = false;
	}
}

// 무기 리스트 UI로 전환
// 메인 위젯을 사용해 무기 리스트 위젯으로 전환
// 임시 초기 무기 설정
// 무기 위치에 포커싱
void APandorasHud::SwitchToWeaponListUI()
{
	MainUIWidget->SwitchWidget(WDG_WeaponList);
	IPlayerStateInterface* player_state = Cast<IPlayerStateInterface>(Cast<APandorasPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)));
	TArray<TSubclassOf<AItemBase>> ItemClasses = player_state->GetItemClasses();
	BP_InitItems(ItemClasses);
	InventoryRoom->ChangeFocusPoint(ECharacterFocusPoint::Weapon);
}

// 인게임 HUD로 전환
void APandorasHud::SwitchToInGameHud()
{
	MainUIWidget->SwitchWidget(WDG_InGameHud);
}
