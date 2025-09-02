// 기본 어트리튜트 세트

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseActorAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PANDORAS_API UBaseActorAttributes : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Base Actor Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, Health)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Base Actor Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Base Actor Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, Stamina)
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Defence, Category = "Base Actor Attributes")
	FGameplayAttributeData Defence;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, Defence)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_XPPoints, Category = "Base Actor Attributes")
	FGameplayAttributeData XPPoints;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, XPPoints);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CharacterLevel, Category = "Base Actor Attributes")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, CharacterLevel)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SkillPoints, Category = "Base Actor Attributes")
	FGameplayAttributeData SkillPoints;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, SkillPoints);

protected:
	// 변수 복제를 위해 반드시 GetLifetimeReplicatedProps 를 오버라이드
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 속성값 변경 직전
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, Health, OldHealth);}

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, MaxHealth, OldMaxHealth);}

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, Stamina, OldStamina);}

	UFUNCTION()
	virtual void OnRep_Defence(const FGameplayAttributeData& OldDefence) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, Defence, OldDefence);}

	UFUNCTION()
	virtual void OnRep_XPPoints(const FGameplayAttributeData& OldXPPoints) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, XPPoints, OldXPPoints); }

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, CharacterLevel, OldCharacterLevel); }

	UFUNCTION()
	virtual void OnRep_SkillPoints(const FGameplayAttributeData& OldSkillPoints) { GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, SkillPoints, OldSkillPoints); }
};
