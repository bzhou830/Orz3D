#include "App.h"


// ��Ҫ_In_���, ɾ��ʱ����C28251����WinMain"��ע��һ�µľ������
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,          // handle to current instance
	_In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
	_In_ LPSTR lpCmdLine,              // command line
	_In_ int nCmdShow                  // show state
)
{
	try 
	{
		return App{800, 600, "Orz.3D"}.Run();
	}
	catch (const BzException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) 
	{
		MessageBox(nullptr, e.what(), "std::exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
