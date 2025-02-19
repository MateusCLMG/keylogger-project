# Projeto de Keylogger

## Requisitos

- Win32 API

## Descrição

Esse código usa um hook para escutar eventos de baixo nível globalmente na máquina Windows do usuário logado. Resumidamente, o hook é um mecanismo do Windows feito para escutar determinados eventos. Nesse caso, dados de entrada de um teclado.

`HHOOK hHook;`
`hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, GetModuleHandle(NULL), 0);`

Declaramos uma handle que instancia nosso hook. Simplificando o conceito de uma handle, podemos dizer que ele serve como o ponteiro do nosso hook, e é com ela que vamos manipular nosso hook. A função `SetWindowsHookEx` recebe um indicador para escutar mensagens de baixo nível do teclado, chamar a função HookCallback quando o sistema detectar uma entrada tal, e zero é usado para indicar que o hook escuta globalmente no usuário em que está logado. O atributo que leva GetModuleHandle pode ser nulo, pois o hook chama uma função declarada neste mesmo código. Isso funciona para hooks de baixo nível como esse que recebem `WH_KEYBOARD_LL` ou `WH_MOUSE_LL`. Caso contrário, seria necessário especificar a DLL que contém a função que vamos chamar, e especificar a aplicação que estamos "hookando" (que já entraria no campo de injeção de hooks). Como HookCallBack foi declarada aqui, e estamos escutando eventos de baixo nível, esse atributo pode receber NULL.

`while (GetMessage(&msg, NULL, 0, 0))`

Esse laço mantém o hook ativo e escutando por eventos, pois GetMessage só vai retornar zero quando o evento que ele escutar do sistema for uma mensagem de `WM_QUIT`, que é a solicitação disparada quando se quer fechar um aplicativo, como fechando uma janela ou apertando CTRL+C em um terminal, por exemplo.

```
   LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LONG_PTR lParam) {
      if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) { 
          KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*) lParam;         
          std::cout << "Tecla pressionada: " << kbdStruct->vkCode << std::endl;
          std::cout << "Horário do sistema: " << kbdStruct->time << std::endl;
      }
      return CallNextHookEx(hHook, nCode, wParam, lParam);
```

`HookCallback` recebe alguns atributos da struct de uma mensagem do sistema, e os dados relevantes para nosso hook operar são `wParam` e `lParam`. nCode determina se a operação tem permissão para executar na ordem de processos do sistema. Se sim, e o `wParam` indicar que o evento é de fato de uma tecla, passamos o resto das informações sobre essa tecla para uma struct `KBDLLHOOKSTRUCT`.
Sempre que se pressiona uma tecla é gerado uma struct `KBDLLHOOKSTRUCT` para armazenar informações como qual foi a tecla, em que horário, etc. Porém, nós só recebemos um ponteiro pra essa struct, e não a struct em si. Esse ponteiro é o `lParam` da nossa função. Por isso usamos este ponteiro com um casting para `KBDLLHOOKSTRUCT`. Assim, podemos acessar as informações da tecla pressionada. 

As informações pertinentes a um keylogger são o código virtual da tecla pressionada. Você pode extrapolar também que o horário do sistema é necessário para discernir quando dados digitados terminam e acabam, olhando pela diferença de tempo entre certas palavras sendo digitadas. Portanto, os atributos que extraímos dessa struct são o `vkCode` e o `time`, e ambos são imprimidos na tela.

## Documentação
[Windows API Hooks](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowshookexa)


