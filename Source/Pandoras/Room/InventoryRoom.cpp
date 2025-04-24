#include "Room/InventoryRoom.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/HUD.h"
#include "Interface/HudInterface.h"

// ������Ʈ���� �����ϰ� ��������, ���� ������ �����Ϳ��� ó��
AInventoryRoom::AInventoryRoom()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    MainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Main"));
    WeaponSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Weapon"));
    DomeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Dome"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RotateCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RotateCapsule"));
    PlayerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPosition"));

    RootComponent = DefaultSceneRoot;
    DomeMesh->SetupAttachment(DefaultSceneRoot);
    PlayerPosition->SetupAttachment(DefaultSceneRoot);
    MainSpringArm->SetupAttachment(PlayerPosition);
    WeaponSpringArm->SetupAttachment(PlayerPosition);
    Camera->SetupAttachment(MainSpringArm);
    RotateCapsule->SetupAttachment(Camera);

    // ĸ�� Ŭ�� ���� ���ε�
    RotateCapsule->OnClicked.AddDynamic(this, &AInventoryRoom::OnCapsuleClicked);
    RotateCapsule->OnReleased.AddDynamic(this, &AInventoryRoom::OnCapsuleReleased);
}

// �÷��� ���� �� �÷��̾� ��Ʈ�ѷ��� �Է� Ȱ��ȭ
void AInventoryRoom::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (player_controller)
    {
        // �Է� Ȱ��ȭ
        EnableInput(player_controller);
    }
}

// �κ��丮�� ����
// �κ��丮�� ������ [�÷��̾��� ĳ����]�� �Է��� ��Ȱ��ȭ�ǰ� ��Ʈ�ѷ��� [Ŭ�� �̺�Ʈ]�� Ȱ��ȭ �ȴ�.
// [�κ��丮 ���� ����] true
// ĳ������ ��ġ�� �״�� ���� �ϸ� ������Ʈ�� ��ġ�� [�κ��丮 �� �� ������Ʈ]�� ��ġ�� �̵��ϰ� �̵� ��/���� [��� Ʈ������]�� �Ѵ� ����
// [�÷��̾� ��Ʈ�ѷ��� ��Ÿ��]�� [ĳ���� ī�޶�]���� [�κ��丮�� ī�޶�]�� �̵�
// HUD�� ���� �κ��丮 UI�� ȭ�鿡 ���� UI �Է��� Ȱ��ȭ
void AInventoryRoom::EnterInventoryMode()
{
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter) return;
    APlayerController* player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!player_controller) return;

    PlayerCharacter->DisableInput(player_controller);
    player_controller->bEnableClickEvents = true;

    bInventoryOpen = true;

    USkeletalMeshComponent* player_mesh = PlayerCharacter->GetMesh();
    FTransform scene_transform = PlayerPosition->GetComponentTransform();
    FVector scene_loc = scene_transform.GetLocation();
    FRotator scene_rot = scene_transform.GetRotation().Rotator();

    MeshRelativeTransformOrigin = player_mesh->GetRelativeTransform();
    player_mesh->SetWorldLocationAndRotation(scene_loc, scene_rot);
    MeshRelativeTransformInventory = player_mesh->GetRelativeTransform();

    player_controller->SetViewTargetWithBlend(this);

    //player_controller->GetHUD()->bLostFocusPaused = true;

    IHudInterface* hud = CastChecked<IHudInterface>(player_controller->GetHUD());
    hud->SwitchToInventoryUI(this);
    hud->ToggleUIInput(true);
}

// �κ��丮���� ����
// [�÷��̾� ĳ����]�� �޽ø� ���� �ʿ� �ִ� ��ġ�� �̵�
// [�÷��̾� ĳ����]�� �Է� �ٽ� Ȱ��ȭ�ϰ� ��Ÿ���� [ĳ������ ī�޶�]�� ����
// [�κ��丮 ���� ����] false
// HUD�� ���� �κ��丮 UI�� ȭ�鿡�� �����ϰ� UI �Է��� ��Ȱ��ȭ
// ��Ʈ�ѷ��� [Ŭ�� �̺�Ʈ]�� ��Ȱ��ȭ
void AInventoryRoom::ExitInventoryMode()
{
    if (!PlayerCharacter) return;
    APlayerController* player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!player_controller) return;

    USkeletalMeshComponent* player_mesh = PlayerCharacter->GetMesh();
    FVector orgin_loc = MeshRelativeTransformOrigin.GetLocation();
    FRotator orgin_rot = MeshRelativeTransformOrigin.GetRotation().Rotator();
    player_mesh->SetRelativeLocationAndRotation(orgin_loc, orgin_rot);

    PlayerCharacter->EnableInput(player_controller);
    player_controller->SetViewTargetWithBlend(PlayerCharacter);

    bInventoryOpen = false;

	IHudInterface* hud = CastChecked<IHudInterface>(player_controller->GetHUD());
    hud->SwitchToInGameHud();
	hud->ToggleUIInput(false);

    player_controller->bEnableClickEvents = false;
}

// ī�޶� ��Ŀ�� ����
// [��Ŀ�� Ű����]�� ���� �������� ��ȯ
// �������Ͽ� KeepWorld ��Ģ���� ī�޶� ����
// ī�޶� ���� ��ġ���� [���������� ���� ����]���� 0.4�ʵ��� �ڿ������� �̵�
void AInventoryRoom::ChangeFocusPoint(ECharacterFocusPoint focus_point)
{
    ResetMeshRotation();

    USpringArmComponent* current_sp_arm = nullptr;

    if (focus_point == ECharacterFocusPoint::Main)
    {
        current_sp_arm = MainSpringArm;
    }
    else if (focus_point == ECharacterFocusPoint::Weapon)
    {
        current_sp_arm = WeaponSpringArm;
    }

    if (!current_sp_arm) return;

    FAttachmentTransformRules AttachmentRules(
        EAttachmentRule::KeepWorld,  // ��ġ ��Ģ
        EAttachmentRule::KeepWorld,  // ȸ�� ��Ģ
        EAttachmentRule::KeepWorld,  // ������ ��Ģ
        true                         // WeldSimulatedBodies �ɼ�
    );

    Camera->AttachToComponent(current_sp_arm, AttachmentRules);

    FLatentActionInfo latent_info(
        __LINE__,       /* Linkage */
        __LINE__,              /* UUID */
        TEXT(__FUNCTION__), /* ExecutionFunction */
        this            /* CallbackTarget */
    );

    UKismetSystemLibrary::MoveComponentTo(
        Camera,
        FVector(0.f, 0.f, 0.f),
        FRotator(0.f, 0.f, 0.f),
        true,
        false,
        0.4f,
        false,
        EMoveComponentAction::Move,
        latent_info
    );
}

// ĸ���� ���콺�� ������ ���� ��
void AInventoryRoom::OnCapsuleClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    bMeshRotating = true;
}

// ĸ���� ������ �� �� ��
void AInventoryRoom::OnCapsuleReleased(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    bMeshRotating = false;
}

void AInventoryRoom::ResetMeshRotation()
{
    if (!bInventoryOpen) return;

    if (!ensure(PlayerCharacter)) return;
    USkeletalMeshComponent* player_mesh = PlayerCharacter->GetMesh();
    FVector inventory_loc = MeshRelativeTransformInventory.GetLocation();
    FRotator inventory_rot = MeshRelativeTransformInventory.GetRotation().Rotator();

    FLatentActionInfo latent_info(
        __LINE__,       /* Linkage */
        __LINE__,              /* UUID */
        TEXT(__FUNCTION__), /* ExecutionFunction */
        this            /* CallbackTarget */
    );

    UKismetSystemLibrary::MoveComponentTo(
        player_mesh,
        inventory_loc,
        inventory_rot,
        true,
        true,
        0.8f,
        false,
        EMoveComponentAction::Move,
        latent_info
    );
}
