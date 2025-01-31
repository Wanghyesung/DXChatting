#pragma once

template <typename T>
class CSingleton
{
	//prviate���� �ϸ� �ڽĿ��� ����� �Ұ�����
protected:
	CSingleton() {};
	virtual ~CSingleton() {};

public:
	static T* GetInst();
	static void Delete();

private:
	static T* m_Inst;
};

template <typename T>
T* CSingleton<T>::m_Inst = nullptr;

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (m_Inst == nullptr)
		m_Inst = new T();

	return m_Inst;
}

template<typename t>
inline void CSingleton<t>::Delete()
{
	delete m_Inst;
	m_Inst = nullptr;
}
