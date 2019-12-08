#include <iostream>
#include <Windows.h>

struct playerOffset{
    uint32_t player_health = 0xF8; // uint32_t -> Assure que la valeur est unsigned.
    uint32_t player_ammo = 0x150;
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

    bool Attach(DWORD dwPid); // DWORD : 8 Octets -> Permet de récupérer le PID.
    bool Read(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesRead = NULL); // uintptr_t permet de prendre en paramètre un pointeur.
    bool Write(uintptr_t IpBaseAddress,void *IpBuffer, SIZE_T nSize, SIZE_T *IpNumberOfBytesWritten = NULL);


    private:
    HANDLE m_hProcess = NULL;
};

Bypass::Bypass(){ // Constructor

}
Bypass::~Bypass(){ // Si hors du scope alors on close.
    if (m_hProcess != NULL){ // RAII -> Life time.
        CloseHandle(m_hProcess);
    }
}

bool Bypass::Attach(DWORD dwPid){
    m_hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwPid);
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
