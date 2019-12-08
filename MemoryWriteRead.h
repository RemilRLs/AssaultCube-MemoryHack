#include <iostream>
#include <Windows.h>

struct playerOffset{
    uint32_t player_health = 0xF8; // uint32_t -> Assure que la valeur est unsigned.
    uint32_t player_ammo = 0x150;
    uint32_t player_ammo_pistolgun = 0x13C;
    uint32_t player_kev = 0xFC;
    uint32_t player_pos_x = 0x04;
    uint32_t player_pos_y = 0x3C;
    uint32_t player_pos_z = 0x0C;
    uint32_t player_pos_head = 0x08;
    uint32_t player_x_mouse = 0x44;
    uint32_t player_y_mouse = 0x40;
}offsets; // Nom du type.

struct baseAddress{
    uintptr_t player_base = 0x509B74;
}adress;

struct playerData{
    int player_health;
    int player_ammo; 
    int player_kev;
    float player_pos_x;
    float player_pos_y;
    float player_pos_z;
    float player_pos_head;
    float player_x_mouse;
    float player_y_mouse;
};


class Bypass { // Création d'une classe.
    public:
    Bypass(); // Initialisation du constructor.

    ~Bypass(); // Initialisation du destructor.

    bool Attach(); // DWORD : 8 Octets -> Permet de récupérer le PID.
    bool Read(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesRead = NULL); // uintptr_t permet de prendre en paramètre un pointeur.
    bool Write(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesWritten = NULL);
    bool readDataPlayer(uintptr_t IpBaseAddress, playerData *player);
    bool findPID();

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
}