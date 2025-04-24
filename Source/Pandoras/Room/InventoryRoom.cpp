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

// 컴포넌트들을 생성하고 계층정립, 세부 설정은 에디터에서 처리
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

    // 캡슐 클릭 시점 바인딩
    RotateCapsule->OnClicked.AddDynamic(this, &AInventoryRoom::OnCapsuleClicked);
    RotateCapsule->OnReleased.AddDynamic(this, &AInventoryRoom::OnCapsuleReleased);
}

// 플레이 시작 시 플레이어 컨트롤러의 입력 활성화
void AInventoryRoom::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* player_controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (player_controller)
    {
        // 입력 활성화
        EnableInput(player_controller);
    }
}

// 인벤토리에 입장
// 인벤토리에 입장한 [플레이어의 캐릭터]는 입력이 비활성화되고 컨트롤러의 [클릭 이벤트]가 활성화 된다.
// [인벤토리 오픈 상태] true
// 캐릭터의 위치는 그대로 유지 하며 컴포넌트의 위치만 [인벤토리 내 씬 컴포넌트]의 위치로 이동하고 이동 전/후의 [상대 트랜스폼]을 둘다 저장
// [플레이어 컨트롤러의 뷰타겟]은 [캐릭터 카메라]에서 [인벤토리의 카메라]로 이동
// HUD를 통해 인벤토리 UI를 화면에 띄우고 UI 입력을 활성화
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

// 인벤토리에서 퇴장
// [플레이어 캐릭터]의 메시를 원래 맵에 있던 위치로 이동
// [플레이어 캐릭터]의 입력 다시 활성화하고 뷰타겟을 [캐릭터의 카메라]로 복귀
// [인벤토리 오픈 상태] false
// HUD를 통해 인벤토리 UI를 화면에서 제거하고 UI 입력을 비활성화
// 컨트롤러의 [클릭 이벤트]가 비활성화
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

// 카메라 포커싱 변경
// [포커싱 키워드]에 따라 스프링암 전환
// 스프링암에 KeepWorld 규칙으로 카메라 부착
// 카메라를 원래 위치에서 [스프링암의 부착 원점]으로 0.4초동안 자연스럽게 이동
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
        EAttachmentRule::KeepWorld,  // 위치 규칙
        EAttachmentRule::KeepWorld,  // 회전 규칙
        EAttachmentRule::KeepWorld,  // 스케일 규칙
        true                         // WeldSimulatedBodies 옵션
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

// 캡슐을 마우스로 누르고 있을 때
void AInventoryRoom::OnCapsuleClicked(UPrimitiveComponent* ClickedComp, FKey ButtonPressed)
{
    bMeshRotating = true;
}

// 캡슐을 눌렀던 걸 뗄 때
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
