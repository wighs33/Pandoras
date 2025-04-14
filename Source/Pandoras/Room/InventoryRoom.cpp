// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/InventoryRoom.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "GameMode/PandorasHud.h"

// Sets default values
AInventoryRoom::AInventoryRoom()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    MainSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Main"));
    WeaponSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SP_Weapon"));
    DomeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Dome"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    PlayerPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPosition"));

    RootComponent = DefaultSceneRoot;
    DomeMesh->SetupAttachment(DefaultSceneRoot);
    PlayerPosition->SetupAttachment(DefaultSceneRoot);
    MainSpringArm->SetupAttachment(PlayerPosition);
    WeaponSpringArm->SetupAttachment(PlayerPosition);
    Camera->SetupAttachment(MainSpringArm);
}

// Called when the game starts or when spawned
void AInventoryRoom::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        // �Է� Ȱ��ȭ
        EnableInput(PC);
    }
}

void AInventoryRoom::EnterInventoryMode()
{
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player) return;
    APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!pc) return;

    // �÷��̾��� �Է� ��Ȱ��ȭ�ϰ� Ŭ�� �̺�Ʈ �ѱ�
    Player->DisableInput(pc);
    pc->bEnableClickEvents = true;

    bInventoryOpen = true;

    USkeletalMeshComponent* player_mesh = Player->GetMesh();
    FTransform player_scene_transform = PlayerPosition->GetComponentTransform();
    FVector loc = player_scene_transform.GetLocation();
    FRotator rot = player_scene_transform.GetRotation().Rotator();

    // ������Ʈ�� ���� ��ġ�� �ű� ���� ��ġ ����
    MeshRelativeTransformOrigin = player_mesh->GetRelativeTransform();
    player_mesh->SetWorldLocationAndRotation(loc, rot);
    MeshRelativeTransformInventory = player_mesh->GetRelativeTransform();

    pc->SetViewTargetWithBlend(this);

    APandorasHud* hud = CastChecked<APandorasHud>(pc->GetHUD());
    hud->SwitchToInventory(true, this);
    hud->ToggleUIInput(true);
}

void AInventoryRoom::ExitInventoryMode()
{
    if (!Player) return;
    APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!pc) return;

    USkeletalMeshComponent* PlayerMesh = Player->GetMesh();
    PlayerMesh->SetRelativeLocationAndRotation(MeshRelativeTransformOrigin.GetLocation(), MeshRelativeTransformOrigin.GetRotation().Rotator());

    Player->EnableInput(pc);
    pc->SetViewTargetWithBlend(Player);

    bInventoryOpen = false;

    APandorasHud* hud = CastChecked<APandorasHud>(pc->GetHUD());
    hud->SwitchToInventory(false, this);
    hud->ToggleUIInput(false);
}

void AInventoryRoom::ChangeFocusPoint(ECharacterFocusPoint focus_point)
{
    USpringArmComponent* comp_to_attach = nullptr;

    if (focus_point == ECharacterFocusPoint::Main)
    {
        comp_to_attach = MainSpringArm;
    }
    else if (focus_point == ECharacterFocusPoint::Weapon)
    {
        comp_to_attach = WeaponSpringArm;
    }

    if (!comp_to_attach) return;

    FAttachmentTransformRules AttachmentRules(
        EAttachmentRule::KeepWorld,  // ��ġ ��Ģ
        EAttachmentRule::KeepWorld,  // ȸ�� ��Ģ
        EAttachmentRule::KeepWorld,  // ������ ��Ģ
        true                         // WeldSimulatedBodies �ɼ�
    );

    Camera->AttachToComponent(comp_to_attach, AttachmentRules);

    FLatentActionInfo latent_info;
    latent_info.CallbackTarget = this; // ���� ���͸� �ݹ� ���
    latent_info.ExecutionFunction = FName("OnCameraMoveFinished"); // �̵� �Ϸ� �� ȣ��� �Լ� �̸�
    latent_info.UUID = 1; // ���� �ĺ��� (���� �׼��� ���� ��� ���� �޶�� ��)
    latent_info.Linkage = 0;

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
