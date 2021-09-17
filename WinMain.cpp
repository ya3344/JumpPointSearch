// JumpPointSearch.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "WinMain.h"
#include "Visualization.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
Visualization gVisualization;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_JUMPPOINTSEARCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPPOINTSEARCH));

    MSG msg;

    msg.message = WM_NULL;

    bool isInit = false;
    DWORD oldTime = timeGetTime();

    // FPS(Frame Per Second) 출력해보기.
    TCHAR strFPS[32] = L"";
    WORD fps = 0;
    DWORD oldFps = timeGetTime();

    // Visualization 타일 세팅 및 렌더링 초기화
    isInit = gVisualization.Initialize();

    // 기본 메시지 루프입니다.
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (isInit)
            {
                //dwCurTime = GetTickCount();
                if (oldFps + 1000 < timeGetTime())
                {
                    oldFps = timeGetTime();

                    swprintf_s(strFPS, L"FPS: %d", fps);
                    fps = 0;

                    // SetWindowText: 현재 윈도우 타이틀바에 문자열을 출력하는 함수.
                    SetWindowText(gHWnd, strFPS);
                }
                if (oldTime + 10 < timeGetTime())
                {
                    oldTime = timeGetTime();
                    gVisualization.AStarWorking();
                    ++fps;
                }
            }

        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUMPPOINTSEARCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_JUMPPOINTSEARCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    gHWnd = CreateWindowEx(0, szTitle, szTitle, WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

    if (gHWnd == NULL)
        return FALSE;

    ShowWindow(gHWnd, SW_SHOW);
    UpdateWindow(gHWnd);
    SetFocus(gHWnd);

    AdjustWindowRectEx(&gWindowRect, GetWindowStyle(gHWnd), GetMenu(gHWnd) != NULL,
        GetWindowExStyle(gHWnd));

    int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (WINDOW_WIDTH / 2); //화면의 크기를 얻어서 정 중앙으로
    int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (WINDOW_HEIGHT / 2);

    MoveWindow(gHWnd, x, y,
        gWindowRect.right - gWindowRect.left, gWindowRect.bottom - gWindowRect.top, TRUE);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static RectInfo rectInfo;
    static BYTE rButtonCount = 0;
    static bool isClip = false;

    switch (message)
    {
        case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                // 메뉴 선택을 구문 분석합니다:
                switch (wmId)
                {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        case WM_LBUTTONDOWN:
            {
                isClip = true;
                rectInfo.point.x = LOWORD(lParam);
                rectInfo.point.y = HIWORD(lParam);
                //ScreenToClient(gHWnd, &rectInfo.point);
                wprintf(L"X: %d Y: %d\n", rectInfo.point.x, rectInfo.point.y);
                rectInfo.nodeIndex = BLOCK_INDEX;
                gVisualization.SetTilePicking(rectInfo);
            }
            break;
        case WM_LBUTTONUP:
            {
                isClip = false;
            }
            break;
        case WM_RBUTTONDOWN:
            {
                if (rButtonCount >= 2)
                {
                    rButtonCount = 0;
                }
                rectInfo.point.x = LOWORD(lParam);
                rectInfo.point.y = HIWORD(lParam);
                //ScreenToClient(gHWnd, &rectInfo.point);
                wprintf(L"X: %d Y: %d\n", rectInfo.point.x, rectInfo.point.y);
                if (rButtonCount == 0)
                    rectInfo.nodeIndex = START_INDEX;
                else if (rButtonCount == 1)
                    rectInfo.nodeIndex = FINISH_INDEX;
                gVisualization.SetTilePicking(rectInfo);
                ++rButtonCount;
            }
            break;
        case WM_MOUSEMOVE:
            {
                if (isClip)
                {
                    rectInfo.point.x = LOWORD(lParam);
                    rectInfo.point.y = HIWORD(lParam);
                    //ScreenToClient(gHWnd, &rectInfo.point);
                    wprintf(L"X: %d Y: %d\n", rectInfo.point.x, rectInfo.point.y);
                    rectInfo.nodeIndex = BLOCK_INDEX;
                    gVisualization.SetTilePicking(rectInfo);
                }
            }
            break;
        case WM_MOUSEWHEEL:
            {
                gVisualization.SetBlockIndexClear();
            }
            break;
        case WM_MBUTTONUP:
            {
                gIsNodeTrade = !gIsNodeTrade;
                wprintf(L"gIsNodeTrade: %d\n", gIsNodeTrade);
            }
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
                EndPaint(hWnd, &ps);
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
