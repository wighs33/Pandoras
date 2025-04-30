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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Actor Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseActorAttributes, Health)

//public:
//    // === Health ==========================================================
//    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = GAMEPLAYATTRIBUTE_REPNOTIFY_FUNC(UBaseActorAttributes, Health), Category = "Base")
//    FGameplayAttributeData Health;
//    ATTRIBUTE_ACCESSORS(UBaseActorAttributes, Health)
//
//    // === Overrides =======================================================
//    virtual void PreAttributeChange(const FGameplayAttribute& Attr, float NewValue) override;
//    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
//
//protected:
//    UFUNCTION()
//    void OnRep_Health(const FGameplayAttributeData& OldHealth)
//    {
//        GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseActorAttributes, Health, OldHealth);
//    }
//
//void UBaseActorAttributes::PreAttributeChange(const FGameplayAttribute& Attr, float NewValue)
//	{
//		if (Attr == GetHealthAttribute())
//		{
//			// 예: 최대체력 클램프
//			NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
//		}
//	}
};
