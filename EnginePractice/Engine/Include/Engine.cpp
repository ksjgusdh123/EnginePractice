#include "Engine.h"
#include "GameFramework.h"

DEFINITION_SINGLE(CEngine)
bool CEngine::m_loop = true;

CEngine::CEngine()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(100);
}

CEngine::~CEngine()
{
}

bool CEngine::Init(HINSTANCE hInst, int windowWidth, int windowHeight)
{
    m_hInst = hInst;

    m_resolution.width = windowWidth;
    m_resolution.height = windowHeight;

    Register();

    m_gameFramework = new CGameFramework(m_hInst, m_hWnd, windowWidth, windowHeight);

    Create();

    return true;
}

int CEngine::Run()
{
    MSG msg;

    while (m_loop)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // TODO: 컨텐츠 로직
            m_gameFramework->Render();
        }
    }

    return (int)msg.wParam;
}

CGameFramework* CEngine::GetFramework()
{
    return m_gameFramework;
}

void CEngine::Register()
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ENGINE));    // 실행파일에 사용할 아이콘 등록
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // 마우스 커서 모양
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY220428);  // 메뉴를 사용할 것인지
    wcex.lpszClassName = TEXT("EnginePractice");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));   // 윈도우창 좌상단에 표시할 작은 아이콘

    RegisterClassExW(&wcex);
}

bool CEngine::Create()
{
    m_hWnd = CreateWindowW(TEXT("EnginePractice"), TEXT("EnginePractice"), WS_OVERLAPPEDWINDOW, 
        100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hWnd)
        return false;

    m_gameFramework = new CGameFramework(m_hInst, m_hWnd, m_resolution.width, m_resolution.height);
    m_gameFramework->Init();

    RECT    rc = { 0, 0, m_resolution.width, m_resolution.height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    MoveWindow(m_hWnd, 50, 50, rc.right - rc.left, rc.bottom - rc.top, TRUE);   // 윈도우 창의 크기를 변경한다.
    ShowWindow(m_hWnd, SW_SHOW);    // 윈도우 창을 보여준다.

    // 클라이언트 영역 갱신에 실패했을 경우 0을 반환한다.
    if (!UpdateWindow(m_hWnd))
        return false;

    return true;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // 윈도우가 종료될때 들어오는 메세지이다.
        m_loop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
