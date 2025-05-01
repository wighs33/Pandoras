#include "ItemBox/ItemBox.h"
#include "Components/BoxComponent.h"

// ������
// ���ø����̼� ���
AItemBox::AItemBox()
{
    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    Chest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Chest"));
    PlayerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerCollider"));
    Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Lid"));

    RootComponent = DefaultSceneRoot;
    Chest->SetupAttachment(DefaultSceneRoot);
    PlayerCollider->SetupAttachment(Chest);
    Lid->SetupAttachment(Chest);

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
