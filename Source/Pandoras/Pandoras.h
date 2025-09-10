// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"   // GEngine, AddOnScreenDebugMessage
#include "Misc/Crc.h"

// 함수 시그니처 문자열 매크로
#if defined(__clang__) || defined(__GNUC__)
#define FUNC_SIG __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC_SIG __FUNCSIG__
#else
#define FUNC_SIG __FUNCTION__
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// 실패 시 화면에 "현재 함수 시그니처"를 띄우고 false를 반환하는 식 매크로
#if !UE_BUILD_SHIPPING
#define SCREEN_WARN(Cond)                                                     \
    ( (Cond) ? true : ([&](){                                                           \
        if (GEngine) {                                                                  \
          const int32 Key = (int32)FCrc::StrCrc32(__FILE__ ":" STR(__LINE__));          \
          GEngine->AddOnScreenDebugMessage(                                             \
            Key, 5.f, FColor::Red, FString::Printf(TEXT("%s"), ANSI_TO_TCHAR(FUNC_SIG)) \
          );                                                                            \
        }                                                                               \
        return false;                                                                   \
      }()) )
#else
  // Shipping에서는 부작용 없이 조건만 평가
#define SCREEN_WARN(Cond) (bool)(Cond)
#endif