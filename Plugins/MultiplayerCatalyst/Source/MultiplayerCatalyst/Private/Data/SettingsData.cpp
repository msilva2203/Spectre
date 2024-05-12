// Copyright (c) 2024, Marco Silva. All rights reserved.


#include "Data/SettingsData.h"

//uint32 GetTypeHash(const FSettingOption& Thing)
//{
//    return FCrc::MemCrc32(&Thing, sizeof(FSettingOption));
//}
//
uint32 GetTypeHash(const FSetting& Thing)
{
    return FCrc::MemCrc32(&Thing, sizeof(FSetting));
}

int32 FSetting::GetValueOptionIndex(FString Value)
{
    for (int i = 0; i < Options.Num(); i++)
    {
        if (Options[i].Value == Value) return i;
    }
    return 0;
}
