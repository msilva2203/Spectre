// © 2024 Marco Silva. All Rights Reserved.


#include "Components/ChatComponent.h"
#include "Statics/CatalystStatics.h"
#include "GameModeCatalyst.h"
#include "PlayerControllerCatalyst.h"

// Sets default values for this component's properties
UChatComponent::UChatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UChatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UChatComponent::SendChatMessage(FChatMessage ChatMessage)
{
	Server_SendChatMessage(ChatMessage);
}

void UChatComponent::Client_ReceiveMessage_Implementation(FChatMessage ChatMessage)
{
	OnMessageReceived.Broadcast(ChatMessage);
}

void UChatComponent::Server_SendChatMessage_Implementation(FChatMessage ChatMessage)
{
	auto GM = UCatalystStatics::GetGameModeCatalyst(GetWorld());

	if (GM)
	{
		GM->BroadcastChatMessage(ChatMessage, ChatID);
	}
}

bool UChatComponent::Server_SendChatMessage_Validate(FChatMessage ChatMessageHandle)
{
	return true;
}
