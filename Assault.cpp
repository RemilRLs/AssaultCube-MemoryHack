#include <iostream>
#include <Windows.h>
#include "MemoryWriteRead.h"



int main(){
    DWORD pid = 0;
    playerData player; // player à le type playerData
    int intRead;
    int activateCheat;
    int godMod = 1000;
    int playerKev = 1000;
    int ammo = 20;
    int ammoPistolGun = 10;

    std::cout << "[*] - ProcessID search..." << std::endl;
    
    /*
        - FindWindowA va nous retourner un handle de la fenêtre (donc trouvé).
        Sinon FindWindowA nous retourne un NULL.

    */


    Bypass* bypass = new Bypass(); // On crée la classe.
    if(!bypass->findPID()){

    }

    if(!bypass->Attach()){ // On génère les permissions souhaité sur le processus en question.
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

        if(!bypass->readDataPlayer((baseAddress), &player)){
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

        if(!bypass->readDataPlayer((baseAddress), &player)){
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
            if(bypass->Write((baseAddress + offsets.player_ammo_pistolgun), &ammoPistolGun, sizeof(ammoPistolGun))){
            }
        }
    }
    delete bypass;
}

