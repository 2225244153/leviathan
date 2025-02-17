// Copyright Ali El Saleh 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GHDebugLogSettings.generated.h"

UENUM(BlueprintType)
enum EDebugLogType
{
	DL_Info		UMETA(DisplayName = "Info"),
    DL_Success	UMETA(DisplayName = "Success"),
    DL_Warning	UMETA(DisplayName = "Warning"),
    DL_Error	UMETA(DisplayName = "Error"),
    DL_Fatal	UMETA(DisplayName = "Fatal")
};

UENUM(BlueprintType)
enum ELoggingOptions
{
	LO_Viewport UMETA(DisplayName = "Viewport"),
    LO_Console	UMETA(DisplayName = "Console"),
    LO_Both		UMETA(DisplayName = "Viewport and Console"),
    LO_NoLog	UMETA(DisplayName = "Disabled")
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class LEVIATHAN_API UGHDebugLogSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UGHDebugLogSettings(const FObjectInitializer& ObjectInitializer);

	// Should we use Two's Compliment when converting a decimal number to a hexadecimal value on negative numbers?
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
    uint8 bUseTwosComplimentForDecimalToHexConversionOnNegativeNumbers : 1;

	// Should the game crash if any UAC_Log functions are called in a shipping build configuration?
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
    uint8 bCrashGameInShippingBuildConfiguration : 1;

	// Writes all log messages called from this plugin to a custom log file
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
    uint8 bSaveLogMessagesToCustomLogFile : 1;
	
	// The color to use when logging a standard message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings", DisplayName = "Default Color")
	FColor InfoColor = FLinearColor(0.0f, 0.66f, 1.0f).ToFColor(true);
	
	// The color to use when logging a error message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	FColor ErrorColor = FColor::Red;
	
	// The color to use when logging a warning message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	FColor WarningColor = FColor::Yellow;
	
	// The color to use when logging a success message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	FColor SuccessColor = FColor::Green;

	// The key to use when calling any logging functions that specify its corresponding name (case-insensitive)
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
    TMap<FName, int32> ViewportLogKeys;
	
	// If true, add a specifier text before the final output log message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	uint8 bEnableSpecifiers : 1;

	// The place to display the category specifier text along side the final log message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings", meta = (EditCondition = "bEnableSpecifiers"))
	TEnumAsByte<ELoggingOptions> DisplaySpecifiersIn = LO_Console;

	// The tag specifers to use when logging in these categories
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings", EditFixedSize = 5, meta = (EditCondition = "bEnableSpecifiers"))
	TMap<TEnumAsByte<EDebugLogType>, FString> LogCategorySpecifiers;

	// Display the context of a log message?
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	uint8 bLogContext : 1;

	// The place to display the context along side the final log message
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings", meta = (EditCondition = "bLogContext"))
	TEnumAsByte<ELoggingOptions> DisplayContextIn = LO_Console;
	
	// 禁止Log输出到屏幕上
	UPROPERTY(EditAnywhere, config, Category = "GH|LogSettings")
	uint8 bDisableLogToViewport : 1;

	UPROPERTY(EditAnywhere, Config, Category="LevelRandomNode")
	uint8 bEnableRNPLog : 1;
	
};
