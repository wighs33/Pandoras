#include "GameMode/PandorasHud.h"
#include "Blueprint/UserWidget.h"
#include "Room/InventoryRoom.h"
#include "UI/MainUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/PlayerStateInterface.h"
#include "GameMode/PandorasPlayerState.h"

// �÷��� ����
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

// ȭ��UI�� �ʱ�ȭ
void APandorasHud::InitWidget()
{
	WDG_InGameHud = MainUIWidget->GetInGameHud();
	WDG_InventoryMain = MainUIWidget->GetInventoryMain();
	WDG_WeaponList = MainUIWidget->GetWeaponList();
}

// �κ��丮 UI�� ��ȯ
// �κ��丮 ���� ���ٸ� �Ű������� �κ��丮 ���� ����
// ���� ������ ����� �κ��丮 ���� �������� ��ȯ
// ������ġ�� ��Ŀ��
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

// UI �Է� ���
// �÷��� on �� ��
// ����&UI �Է� ���� �����ϰ� ���콺 Ŀ�� ����
// �÷��� off �� ��
// ���� ���� �Է� ���� �����ϰ� ���콺 Ŀ�� �����
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

// ���� ����Ʈ UI�� ��ȯ
// ���� ������ ����� ���� ����Ʈ �������� ��ȯ
// �ӽ� �ʱ� ���� ����
// ���� ��ġ�� ��Ŀ��
void APandorasHud::SwitchToWeaponListUI()
{
	MainUIWidget->SwitchWidget(WDG_WeaponList);
	IPlayerStateInterface* player_state = Cast<IPlayerStateInterface>(Cast<APandorasPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)));
	TArray<TSubclassOf<AItemBase>> ItemClasses = player_state->GetItemClasses();
	BP_InitItems(ItemClasses);
	InventoryRoom->ChangeFocusPoint(ECharacterFocusPoint::Weapon);
}

// �ΰ��� HUD�� ��ȯ
void APandorasHud::SwitchToInGameHud()
{
	MainUIWidget->SwitchWidget(WDG_InGameHud);
}
