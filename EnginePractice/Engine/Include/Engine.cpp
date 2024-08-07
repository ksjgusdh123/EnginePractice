#include "Engine.h"
#include "GameFramework.h"
#include "SceneManager.h"
#include "CommandQueue.h"

DEFINITION_SINGLE(CEngine)
bool CEngine::m_loop = true;

CEngine::CEngine()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(100);
}

CEngine::~CEngine()
{
    m_gameFramework->OnDestroy();
    delete m_gameFramework;
}

bool CEngine::Init(HINSTANCE hInst, int windowWidth, int windowHeight)
{
    m_hInst = hInst;

    m_resolution.width = windowWidth;
    m_resolution.height = windowHeight;

    Register();
    Create();

    if (!CSceneManager::GetInst()->Init(m_gameFramework->GetDevice()))
        return false;

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
            // TODO: ������ ����
            Logic();
        }
    }

    return (int)msg.wParam;
}

CGameFramework* CEngine::GetFramework()
{
    return m_gameFramework;
}

void CEngine::Logic()
{
    float elapsedTime = 0.f;

    // Scene�� ��ü�� ��� ó������ �ٽ� ���۽�Ų��
    if (Update(elapsedTime))
        return;

    if (PostUpdate(elapsedTime))
        return;

    Render(elapsedTime);
}

bool CEngine::Update(float elapsedTime)
{
    return CSceneManager::GetInst()->Update(elapsedTime);
}

bool CEngine::PostUpdate(float elapsedTime)
{
    return CSceneManager::GetInst()->PostUpdate(elapsedTime);
}

void CEngine::Render(float elapsedTime)
{
    m_gameFramework->RenderBegin();
    CSceneManager::GetInst()->Render(m_gameFramework->GetcmdQueue()->GetCmdList(), elapsedTime);
    m_gameFramework->RenderEnd();
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
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ENGINE));    // �������Ͽ� ����� ������ ���
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // ���콺 Ŀ�� ���
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY220428);  // �޴��� ����� ������
    wcex.lpszClassName = TEXT("EnginePractice");
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));   // ������â �»�ܿ� ǥ���� ���� ������

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
    MoveWindow(m_hWnd, 50, 50, rc.right - rc.left, rc.bottom - rc.top, TRUE);   // ������ â�� ũ�⸦ �����Ѵ�.
    ShowWindow(m_hWnd, SW_SHOW);    // ������ â�� �����ش�.

    // Ŭ���̾�Ʈ ���� ���ſ� �������� ��� 0�� ��ȯ�Ѵ�.
    if (!UpdateWindow(m_hWnd))
        return false;

    return true;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // �����찡 ����ɶ� ������ �޼����̴�.
        m_loop = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
