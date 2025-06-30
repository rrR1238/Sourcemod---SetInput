#if defined _setinput_included
    #endinput
#endif
#define _setinput_included

void SetInput(int ent, const char[] inputName, const char[] param)
{
    if (!IsValidEntity(ent))
        return;

    char targetname[64];
    Format(targetname, sizeof(targetname), "__temp_target_%d", ent);
    DispatchKeyValue(ent, "targetname", targetname);

    int relay = CreateEntityByName("logic_relay");
    if (relay == -1)
        return;

    char relayName[64], output1[128];
    Format(relayName, sizeof(relayName), "relay_autocall_%d", relay);

    if (param[0] != '\0')
        Format(output1, sizeof(output1), "%s,%s,%s,0,-1", targetname, inputName, param);
    else
        Format(output1, sizeof(output1), "%s,%s,,0,-1", targetname, inputName);

    DispatchKeyValue(relay, "targetname", relayName);
    DispatchKeyValue(relay, "OnUser1", output1);
    DispatchSpawn(relay);

    int timer = CreateEntityByName("logic_timer");
    if (timer == -1)
    {
        if (IsValidEntity(relay))
            RemoveEntity(relay);
        return;
    }

    char output2[128];
    Format(output2, sizeof(output2), "%s,FireUser1,,0,-1", relayName);
    DispatchKeyValue(timer, "RefireTime", "0.01");
    DispatchKeyValue(timer, "StartDisabled", "0");
    DispatchKeyValue(timer, "OnTimer", output2);
    DispatchSpawn(timer);

    CreateTimer(1.0, Timer_RemoveInputEntities, relay, TIMER_FLAG_NO_MAPCHANGE);
    CreateTimer(1.0, Timer_RemoveInputEntities, timer, TIMER_FLAG_NO_MAPCHANGE);
}

public Action Timer_RemoveInputEntities(Handle timer, any ent)
{
    if (IsValidEntity(ent))
        RemoveEntity(ent);
    return Plugin_Stop;
}
