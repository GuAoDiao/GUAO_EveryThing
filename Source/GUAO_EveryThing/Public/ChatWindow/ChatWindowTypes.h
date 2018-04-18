#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ChatWindowTypes.generated.h"


UENUM(BlueprintType)
enum class ESystemMessageType : uint8
{
	Normal,
	Alert,
	Error,
	Team,
	Success,
	Custom
};

USTRUCT(BlueprintType)
struct FChatMessageInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FChatMessageInfo() {}

	FChatMessageInfo(const FString& InMessage, const FLinearColor InDisplayColor, int32 InPlayerID = -1) : Message(InMessage), DisplayColor(InDisplayColor), PlayerID(InPlayerID) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Message;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PlayerID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor DisplayColor;
};

