#include <windows.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>

#define ID_DISPLAY 100
#define ID_BTN_0 200
#define ID_BTN_1 201
#define ID_BTN_2 202
#define ID_BTN_3 203
#define ID_BTN_4 204
#define ID_BTN_5 205   
#define ID_BTN_6 206
#define ID_BTN_7 207
#define ID_BTN_8 208
#define ID_BTN_9 209
#define ID_BTN_ADD 210
#define ID_BTN_SUB 211
#define ID_BTN_MUL 212
#define ID_BTN_DIV 213
#define ID_BTN_EQ  214
#define ID_BTN_CLR 215

class Calculadora {
public:
    Calculadora(HINSTANCE hInstance);
    int Run(int nCmdShow);

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void OnCreate(HWND hwnd);
    void OnCommand(WPARAM wParam);

    void AppendDigit(char digit);
    void SetOperator(char op);
    void CalculateResult();
    void Clear();

    static Calculadora* instancia;

    HWND hwndMain;
    HWND hDisplay;
    HINSTANCE hInst;

    char operando[2][100];
    char operador;
    int fase;
    bool resultadoMostrado;
};

Calculadora* Calculadora::instancia = nullptr;

Calculadora::Calculadora(HINSTANCE hInstance) 
    : hInst(hInstance), hwndMain(NULL), hDisplay(NULL), operador(0), fase(0), resultadoMostrado(false) 
{
    operando[0][0] = '\0';
    operando[1][0] = '\0';
    instancia = this;
}

int Calculadora::Run(int nCmdShow) {
    const char CLASS_NAME[] = "CalculadoraAjustada";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    hwndMain = CreateWindowExA(
        0, CLASS_NAME, "Calculadora OO C++", 
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 330,
        NULL, NULL, hInst, NULL
    );

    if (!hwndMain) return 0;

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK Calculadora::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        if (instancia) instancia->OnCreate(hwnd);
        break;

    case WM_COMMAND:
        if (instancia) instancia->OnCommand(wParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Calculadora::OnCreate(HWND hwnd) {
    hDisplay = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
                            20, 20, 220, 30, hwnd, (HMENU)ID_DISPLAY, hInst, NULL);

    const char* botoes[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"C", "0", "=", "+"}
    };

    int idMap[4][4] = {
        {ID_BTN_7, ID_BTN_8, ID_BTN_9, ID_BTN_DIV},
        {ID_BTN_4, ID_BTN_5, ID_BTN_6, ID_BTN_MUL},
        {ID_BTN_1, ID_BTN_2, ID_BTN_3, ID_BTN_SUB},
        {ID_BTN_CLR, ID_BTN_0, ID_BTN_EQ, ID_BTN_ADD}
    };

    int startX = 20, startY = 70;
    int btnW = 50, btnH = 40;
    int padding = 10;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            CreateWindowA("BUTTON", botoes[row][col], WS_VISIBLE | WS_CHILD | BS_CENTER,
                startX + col * (btnW + padding), startY + row * (btnH + padding),
                btnW, btnH, hwnd, (HMENU)idMap[row][col], hInst, NULL);
        }
    }
}

void Calculadora::OnCommand(WPARAM wParam) {
    int id = LOWORD(wParam);

    if (id >= ID_BTN_0 && id <= ID_BTN_9) {
        AppendDigit('0' + (id - ID_BTN_0));
    }
    else if (id == ID_BTN_ADD || id == ID_BTN_SUB || id == ID_BTN_MUL || id == ID_BTN_DIV) {
        SetOperator(
            (id == ID_BTN_ADD) ? '+' :
            (id == ID_BTN_SUB) ? '-' :
            (id == ID_BTN_MUL) ? '*' : '/'
        );
    }
    else if (id == ID_BTN_EQ) {
        CalculateResult();
    }
    else if (id == ID_BTN_CLR) {
        Clear();
    }
}

void Calculadora::AppendDigit(char digit) {
    if (resultadoMostrado) {
        operando[0][0] = '\0';
        operando[1][0] = '\0';
        operador = 0;
        fase = 0;
        resultadoMostrado = false;
    }

    char* atual = operando[fase];
    int len = strlen(atual);
    if (len < 99) {
        atual[len] = digit;
        atual[len + 1] = '\0';
        SetWindowTextA(hDisplay, atual);
    }
}

void Calculadora::SetOperator(char op) {
    if (strlen(operando[0]) == 0) return;
    operador = op;
    fase = 1;
    SetWindowTextA(hDisplay, "");
    resultadoMostrado = false;
}

void Calculadora::CalculateResult() {
    if (strlen(operando[0]) && strlen(operando[1]) && operador) {
        float n1 = static_cast<float>(atof(operando[0]));
        float n2 = static_cast<float>(atof(operando[1]));
        char resultado[100];

        if (operador == '/' && n2 == 0) {
            strcpy(resultado, "Erro: divisao por zero!");
        }
        else {
            float r = 0;
            switch (operador) {
                case '+': r = n1 + n2; break;
                case '-': r = n1 - n2; break;
                case '*': r = n1 * n2; break;
                case '/': r = n1 / n2; break;
            }
            sprintf(resultado, "%.2f", r);
        }

        SetWindowTextA(hDisplay, resultado);
        strcpy(operando[0], resultado);
        operando[1][0] = '\0';
        fase = 0;
        operador = 0;
        resultadoMostrado = true;
    }
}

void Calculadora::Clear() {
    operando[0][0] = '\0';
    operando[1][0] = '\0';
    operador = 0;
    fase = 0;
    resultadoMostrado = false;
    SetWindowTextA(hDisplay, "");
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    Calculadora calc(hInstance);
    return calc.Run(nCmdShow);
}
