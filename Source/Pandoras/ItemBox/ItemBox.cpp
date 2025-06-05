#include "ItemBox/ItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/MaterialBillboardComponent.h"
#include "Components/WidgetComponent.h"

// ������
// ���ø����̼� ���
AItemBox::AItemBox()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Chest"));
    PlayerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerCollider"));
    Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Lid"));
    MaterialBillboard = CreateDefaultSubobject<UMaterialBillboardComponent>(TEXT("MaterialBillboard"));
    Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));

    RootComponent = DefaultSceneRoot;
    Chest->SetupAttachment(DefaultSceneRoot);
    PlayerCollider->SetupAttachment(Chest);
    Lid->SetupAttachment(Chest);
    MaterialBillboard->SetupAttachment(Chest);
    Widget->SetupAttachment(MaterialBillboard);

	bReplicates = true;
}

void AItemBox::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // ĸ�� Ŭ�� ���� ���ε�
    PlayerCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnColliderBeginOverlap);
    PlayerCollider->OnComponentEndOverlap.AddDynamic(this, &AItemBox::OnColliderEndOverlap);
}

// �÷��� ����
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AItemBox::Collect_Server_Validate(AActor* OwnerActor)
{
    return true;
}

void AItemBox::Collect_Server_Implementation(AActor* OwnerActor)
{
    BP_Collect_Server(OwnerActor);
}

void AItemBox::Collect_Multicast_Implementation(AActor* OwnerActor)
{
    BP_Collect_Multicast(OwnerActor);
}
