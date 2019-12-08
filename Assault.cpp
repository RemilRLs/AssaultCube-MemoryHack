#include <iostream>
#include <Windows.h>
#include "MemoryWriteRead.h"

/*
    - On lui donne l'handle pour qu'il puisse se rattacher à la classe.
    - On lui donne la baseadresse .
    - On lui dit ou stocker la valeur, ici dans une structure.
    - On lui dit la taille de la structure (sa sera toujours un int).

    - player est un pointeur pointant sur le type playerData et donc sur la structure. (-> permet d'avoir accès à une structure).
*/

bool readDataPlayer(Bypass * handle, uintptr_t IpBaseAddress, playerData *player, SIZE_T nSize){ 
    handle->Read((IpBaseAddress + offsets.player_health), &player->player_health, sizeof(player->player_health));
    handle->Read((IpBaseAddress + offsets.player_kev), &player->player_kev, sizeof(player->player_kev));
    handle->Read((IpBaseAddress + offsets.player_pos_x), &player->player_pos_x, sizeof(player->player_pos_x));
    handle->Read((IpBaseAddress + offsets.player_pos_y), &player->player_pos_y, sizeof(player->player_pos_z));
    handle->Read((IpBaseAddress + offsets.player_pos_z), &player->player_pos_z, sizeof(player->player_pos_z));
    handle->Read((IpBaseAddress + offsets.player_pos_head), &player->player_pos_head, sizeof(player->player_pos_head));
    handle->Read((IpBaseAddress + offsets.player_x_mouse), &player->player_x_mouse, sizeof(player->player_x_mouse));
    handle->Read((IpBaseAddress + offsets.player_y_mouse), &player->player_y_mouse, sizeof(player->player_y_mouse));
}



int main(){
    DWORD pid = 0;
    playerData player; // player à le type playerData
    int intRead;
    int activateCheat;
    int godMod = 1000;
    int playerKev = 1000;
    int ammo = 20;

    std::cout << "[*] - ProcessID search..." << std::endl;
    /*
        - FindWindowA va nous retourner un handle de la fenêtre (donc trouvé).
        Sinon FindWindowA nous retourne un NULL.

    */
    HWND hwnd = FindWindowA(NULL, LPCSTR("AssaultCube"));

    if (hwnd != NULL){
        std::cout << "[*] - ProcessID find." << std::endl;
    }
    else{
        std::cout << "[X] - Failure, we could not find the process" << std::endl;
        system("pause");
    }
    GetWindowThreadProcessId(hwnd, &pid); // La variable pid va reçevoir le ProcessID.
    if (pid != 0){
        std::cout << "[*] - ProcessID find." << std::endl;
    }
    else{
        std::cout << "[X] - No ProcessID finds." << std::endl;
        system("pause");
    }

    Bypass* bypass = new Bypass(); // On crée la classe.
    if(!bypass->Attach(pid)){ // On génère les permissions souhaité sur le processus en question.
        std::cout << "[X] - Exit." << std::endl;
    } 

    uintptr_t baseAddress = adress.player_base;
    DWORD baseReference;

    if(!bypass->Read(baseAddress, &baseReference, sizeof(baseReference))){ 
        std::cout << "[X] - The base address could not be found. " << baseReference << std::endl;
    }



    baseAddress = baseReference; // On a la base adress.
    bool exit = false;
    bool godmod = false;
    bool infinite_ammo = false;

    // Menu
    std::cout << "==============================================================" << std::endl;
    std::cout << "[*] - Press F1 to leave." << std::endl;
    std::cout << "[*] - Press F2 to activate the God Mod([!] - Only works solo)." << std::endl;
    std::cout << "[*] - Press F3 to activate infinite ammo ([!] - Only works solo)." << std::endl;
    std::cout << "==============================================================\n\n[*] - Press a key : ";

    while(!exit){

        if(!readDataPlayer(bypass, (baseAddress), &player, sizeof(player))){
            std::cout << "[X] - We could not read the player's data." << std::endl;
        }


        if(GetAsyncKeyState(VK_F1)){
            exit = true;
            std::cout << "\n[*] - Exit." << std::endl;
        }
        
        if(GetAsyncKeyState(VK_F2)){
            godmod = true;
        }
        if(GetAsyncKeyState(VK_F3)){
            infinite_ammo = true;
        }

        if(!readDataPlayer(bypass, (baseAddress), &player, sizeof(player))){
            std::cout << "[X] We could not recover the player's values.\n[X]Exiting..." << std::endl;
            break;
        }
        if(godmod){
             if(bypass->Write((baseAddress + offsets.player_health), &godMod, sizeof(player.player_health))){ // Health.
            }
            if(bypass->Write((baseAddress + offsets.player_kev), &playerKev, sizeof(playerKev))){ // Kevlar.
            }

        }
        if(infinite_ammo){
            if(bypass->Write((baseAddress + offsets.player_ammo), &ammo, sizeof(ammo))){
            }
        }
    }
    delete bypass;
}

