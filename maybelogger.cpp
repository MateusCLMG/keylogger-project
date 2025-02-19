#include <windows.h>
#include <iostream>

HHOOK hHook; //handle (ponteiro) global que vai armazenar o hook do windows


LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LONG_PTR lParam) {
    if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) { //se o processo pode ser executado e uma tecla for pressionada, passa pela condicao

        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*) lParam;         //cast no ponteiro lparam para uma struct de teclado de baixo nivel, para poder ler seus atributos
        std::cout << "Tecla pressionada: " << kbdStruct->vkCode << std::endl; //retira da struct criada as informacoes do identificador da tecla pressionada e o momento que foi pressionada
        std::cout << "Horário do sistema: " << kbdStruct->time << std::endl;
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam); //permite que outros processos possam ser executados em seguida, para o hook continuar funcionando
}

void SetHook() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0); //instancia um hook que (respectivamente), escuta eventos do teclado, chama a funcao HookCallback quando escuta, NULL pois nosso hook nao esta localizado em uma DLL externa, e zero para "hookar" threads globalmente
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { 

        //loop para manter o hook funcionando. Escapa quando a mensagem do sistema for WM_QUIT

    }
}

int main() {
    SetHook();
    return 0;
}