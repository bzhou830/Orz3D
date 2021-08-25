#pragma once
#include "BzWin.h"
#include <vector>
#include <wrl.h>
#include <string>
#include <dxgidebug.h>

// ʹ��ComPtr�����Com�ӿں����Ĺ���
#include <wrl.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() {};
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
private:
	unsigned long long next = 0u;
	ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};

