#include <iostream>
#include <Windows.h>
#include <math.h>
#include "Vector3.h"

struct playerOffset{
    uint32_t player_health = 0xF8; // uint32_t -> Assure que la valeur est unsigned.
    uint32_t player_ammo = 0x150;
    uint32_t player_ammo_pistolgun = 0x13C;
    uint32_t player_kev = 0xFC;
    uint32_t player_pos_x = 0x4;
    uint32_t player_pos_y = 0x8;
    uint32_t player_pos_z = 0xC;
    uint32_t player_pos_head = 0x08;
    uint32_t player_x_mouse = 0x40;
    uint32_t player_y_mouse = 0x44;
    uintptr_t player_team_1 = 0x204;
    uintptr_t player_team_2 = 0x32C;
}offsets; // Nom du type.

struct baseAddress{
    uintptr_t player_base = 0x509B74;
    uintptr_t entityplayer = 0x50F4F8;
    uintptr_t entitypointer; // Entitypointer va nous permettre d'avoir les vecteurs.
}adress;

/* Struct for the main player */

struct playerData{
    int player_health;
    int player_ammo; 
    int player_team_1;
    int player_team_2;
    int player_kev;
    float player_pos_x;
    float player_pos_y;
    float player_pos_z;
    float player_pos_head;
    float player_x_mouse;
    float player_y_mouse;
};

/* Struct for the entity player */

struct entityData{
    int player_health;
    int player_ammo; 
    int player_team_1;
    int player_team_2;
    int player_kev;
    float player_pos_x;
    float player_pos_y;
    float player_pos_z;
    float player_pos_head;
    float player_x_mouse;
    float player_y_mouse;
}entity;

class Bypass { // Création d'une classe.
    public:
    Bypass(); // Initialisation du constructor.

    ~Bypass(); // Initialisation du destructor.

    bool Attach(); // DWORD : 8 Octets -> Permet de récupérer le PID.
    bool Read(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesRead = NULL); // uintptr_t permet de prendre en paramètre un pointeur.
    bool Write(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesWritten = NULL);
    bool readDataPlayer(uintptr_t IpBaseAddress, playerData *player);
    bool findPID();
    bool readDataEntity(uintptr_t IpBaseAddress, entityData *player);
    void aimbot(uintptr_t IpBaseAddress, entityData entity, playerData player);
    bool getOrigin(uintptr_t IpBaseAddress, playerData *player);
    float getDistance(entityData entity, playerData player); // Permet d'avoir la distance (hypotenuse).

    private:
    HANDLE m_hProcess = NULL;
    DWORD pid = 0;
    HWND hwnds;
};

Bypass::Bypass(){ // Constructor

}
Bypass::~Bypass(){ // Si hors du scope alors on close.
    if (m_hProcess != NULL){ // RAII -> Life time.
        CloseHandle(m_hProcess);
    }
}

/*
    - FindWindowA va nous retourner un handle de la fenêtre (donc trouvé).
    Sinon FindWindowA nous retourne un NULL.

*/


bool Bypass::findPID(){
    hwnds = FindWindowA(NULL, LPCSTR("AssaultCube"));
    if (hwnds != NULL){
        std::cout << "[*] - Windows find." << std::endl;
    }
    else{
        std::cout << "[X] - Failure, we could not find the Windows." << std::endl;
        system("pause");
    }
    GetWindowThreadProcessId(hwnds, &pid);
    if (pid != 0){
        std::cout << "[*] - ProcessID find." << std::endl;
    }
    else{
        std::cout << "[X] - No ProcessID finds." << std::endl;
        system("pause");
    }
    

}


bool Bypass::Attach(){
    m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid);
    if (m_hProcess != NULL){
        std::cout << "[*] - Opening the process.\n" << std::endl;
        return true;
    }
    else{
        std::cout << "[X] - The process is invalid, error : " << GetLastError() << std::endl;
        return false;
    }
}

// Méthode Read.

bool Bypass::Read(uintptr_t IpBaseAddress, void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesRead){ 
    if (ReadProcessMemory(m_hProcess, (LPCVOID) IpBaseAddress,IpBuffer, nSize, IpNumberOfBytesRead)){
        return true;
    }
    else{
        return false;
    }
}

// Méthode Write.

bool Bypass::Write(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesWritten){
    if (WriteProcessMemory(m_hProcess, (LPVOID) IpBaseAddress, IpBuffer, nSize, IpNumberOfBytesWritten)){
        return true;
    }
    else{
        return false;
    }
}

/*
    - On lui donne l'handle pour qu'il puisse se rattacher à la classe.
    - On lui donne la baseadresse .
    - On lui dit ou stocker la valeur, ici dans une structure.
    - On lui dit la taille de la structure (sa sera toujours un int).

    - player est un pointeur pointant sur le type playerData et donc sur la structure. (-> permet d'avoir accès à une structure).
*/


bool Bypass::readDataPlayer(uintptr_t IpBaseAddress, playerData *player){ 
    Read((IpBaseAddress + offsets.player_health), &player->player_health, sizeof(player->player_health));
    Read((IpBaseAddress + offsets.player_kev), &player->player_kev, sizeof(player->player_kev));
    Read((IpBaseAddress + offsets.player_pos_x), &player->player_pos_x, sizeof(player->player_pos_x));
    Read((IpBaseAddress + offsets.player_pos_y), &player->player_pos_y, sizeof(player->player_pos_z));
    Read((IpBaseAddress + offsets.player_pos_z), &player->player_pos_z, sizeof(player->player_pos_z));
    Read((IpBaseAddress + offsets.player_pos_head), &player->player_pos_head, sizeof(player->player_pos_head));
    Read((IpBaseAddress + offsets.player_x_mouse), &player->player_x_mouse, sizeof(player->player_x_mouse));
    Read((IpBaseAddress + offsets.player_y_mouse), &player->player_y_mouse, sizeof(player->player_y_mouse));
    Read((IpBaseAddress + offsets.player_team_1), &player->player_team_1, sizeof(player->player_team_1)); // Check Team.
    Read((IpBaseAddress + offsets.player_team_2), &player->player_team_2, sizeof(player->player_team_2));
}


/* Fonction permettant de voir les coordonnées de plusieurs entités à la fois */

bool Bypass::readDataEntity(uintptr_t IpBaseAddress, entityData *entity){
    Read((IpBaseAddress + offsets.player_pos_x), &entity->player_pos_x, sizeof(entity->player_pos_x));
    Read((IpBaseAddress + offsets.player_pos_y), &entity->player_pos_y, sizeof(entity->player_pos_y));
    Read((IpBaseAddress + offsets.player_pos_z), &entity->player_pos_z, sizeof(entity->player_pos_z));
    Read((IpBaseAddress + offsets.player_pos_head), &entity->player_pos_head, sizeof(entity->player_pos_head));
    Read((IpBaseAddress + offsets.player_health), &entity->player_health, sizeof(entity->player_health));
    Read((IpBaseAddress + offsets.player_team_1), &entity->player_team_1, sizeof(entity->player_team_1)); 
    Read((IpBaseAddress + offsets.player_team_2), &entity->player_team_2, sizeof(entity->player_team_2)); 
}

float Bypass::getDistance(entityData entity, playerData player){
    float positionSubX = (entity.player_pos_x - player.player_pos_x);
    float positionSubY = (entity.player_pos_y - player.player_pos_y);
    float positionSubZ = (entity.player_pos_z - player.player_pos_z);

    float hypotenuse =sqrt((positionSubX * positionSubX) + (positionSubY * positionSubY) + (positionSubZ * positionSubZ));

    return hypotenuse;
}


/* 
- Tout d'abord il faut trouver l'angle de vue de notre joueur afin
qu'il puisse tirer sur l'ennemie 

- On a alors besoin de soustraire la position de l'ennemie avec la notre. (Permet d'être à l'origine)
- 3.14 * 180 + 180 permet de rendre le résultat en degrée.
- On a besoin de l'angle A (l'origine de notre joueur) On utilise Pythagor.


*/
double PI = 3.14159265358;

void Bypass::aimbot(uintptr_t IpBaseAddress,entityData entity, playerData player){

    view mouse;

    float positionSubX = (entity.player_pos_x - player.player_pos_x);
    float positionSubY = (entity.player_pos_y - player.player_pos_y);
    float positionSubZ = (entity.player_pos_z - player.player_pos_z);

    // On calcul l'hypothénuse :

    float hypotenuse =sqrt((positionSubX * positionSubX) + (positionSubY * positionSubY) + (positionSubZ * positionSubZ));

    mouse.x = -atan2(positionSubX,positionSubY) / PI * 180.0f + 180.0f;
    mouse.y = asin(positionSubZ / hypotenuse)  * 180.0f / PI;
    mouse.z = 0.0f;



    Write((IpBaseAddress + offsets.player_x_mouse), &mouse.x, sizeof(float));
    Write((IpBaseAddress + offsets.player_y_mouse), &mouse.y, sizeof(float));

}