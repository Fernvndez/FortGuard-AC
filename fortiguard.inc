/*
 * FortiGuard Anti-Cheat System Main Include
 * Enhanced Real-time Protection
 */

#if !defined _INC_FORTIGUARD
    #define _INC_FORTIGUARD
#else
    #endinput
#endif

// Include all modules
#include <fortiguard/constants>
#include <fortiguard/utils>
#include <fortiguard/logging>
#include <fortiguard/player_state>
#include <fortiguard/aim_detection>
#include <fortiguard/memory_protection>
#include <fortiguard/packet_validation>
#include <fortiguard/weapon_validation>
#include <fortiguard/teleport>
#include <fortiguard/speed>

// Initialize all systems
public OnGameModeInit() {
    FG_InitializePlayerState();
    FG_InitializeAimDetection();
    FG_InitializeMemoryProtection();
    FG_InitializeTeleportCheck();
    FG_InitializeSpeedCheck();
    
    #if defined FG_OnGameModeInit
        return FG_OnGameModeInit();
    #else
        return 1;
    #endif
}

// Main update hook
public OnPlayerUpdate(playerid) {
    if(!IsPlayerConnected(playerid)) return 0;
    
    // Validate packet first
    if(!FG_ValidatePacket(playerid)) return 0;
    
    // Run all checks
    if(FG_CheckAimbot(playerid) ||
       FG_CheckTeleportHack(playerid) ||
       FG_CheckSpeedHack(playerid)) {
        
        new violations = FG_AddViolation(playerid);
        if(violations >= VIOLATION_THRESHOLD) {
            new name[MAX_PLAYER_NAME];
            GetPlayerName(playerid, name, sizeof(name));
            Ban(playerid);
            FG_Log(LOG_LEVEL_ERROR, "Player %s banned for multiple violations", name);
            return 0;
        }
    }
    
    #if defined FG_OnPlayerUpdate
        return FG_OnPlayerUpdate(playerid);
    #else
        return 1;
    #endif
}

// Weapon shot validation
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
    if(!FG_ValidateWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ)) {
        FG_AddViolation(playerid);
        return 0;
    }
    
    #if defined FG_OnPlayerWeaponShot
        return FG_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    #else
        return 1;
    #endif
}