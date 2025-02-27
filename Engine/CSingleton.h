#pragma once


template <typename T>
class CSingleton
{
	//prviate으로 하면 자식에서 사용이 불가능함
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
	{
		m_Inst = new T();
		atexit(reinterpret_cast<void(*)()>(&CSingleton<T>::Delete));
	}
		

	return m_Inst;
}

template<typename T>
inline void CSingleton<T>::Delete()
{
	delete m_Inst;
	m_Inst = nullptr;
}
