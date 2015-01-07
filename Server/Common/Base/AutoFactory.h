
//-----------------------------------------------------------------------------
//!\brief �Զ��೧
//!
//!	ʹ����ע�ᶼ�ܷ�����Զ��೧
//!	ʹ��Register( TBinder<YOUR_CLASS>(), uint32_t dwID )�ĸ�ʽ��ע��
//!	Ȼ��ʹ��Create( uint32_t dwID )���������ʵ��
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! ��һЩ�����ӿڰ󶨵�ָ����
//-----------------------------------------------------------------------------
class BinderAbstract
{ 	
public:
	virtual void* Create()=0;	// ��������Ľӿ�
	virtual void Destroy(void*)=0;	// ɾ������Ľӿ�
};


//-----------------------------------------------------------------------------
//! ��create,destroy�󶨵�ָ����,���೧�����ʹ��
//-----------------------------------------------------------------------------
template<typename concrete_class>class TBinder : public BinderAbstract
{
public:
	void* Create() { return (new concrete_class); }
	void Destroy(void* p) { delete((concrete_class*)p); }
};


//-----------------------------------------------------------------------------
//! ������������������Ϣ��ģ����
//-----------------------------------------------------------------------------
template<typename concrete_class>class TWrap{};

template<typename abstract_class>	// ������Ҫ��ָ��������ģ�����
class TAutoFactory
{
public:
	//! ע�������
	template<typename concrete_class>
	bool Register(TWrap<concrete_class>, uint32_t dwID)
	{ 
		TBinder<concrete_class>* pBinder = new TBinder<concrete_class>;
		if( NULL == pBinder )
			return false;

		Assert( m_map.find(dwID) == m_map.end() );	// �������ظ�ע��
		m_map.insert(std::pair<uint32_t, BinderAbstract*>(dwID, pBinder));
		return true;
	}

	template<typename concrete_class>
	bool Unregister(TWrap<concrete_class>, uint32_t dwID)
	{
		std::map<uint32_t, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter == m_map.end() )
			return false;

		delete((TBinder<concrete_class>*)(iter->second));
		m_map.erase(dwID);
		return true;
	}

	//! ����ID��������ʵ��
	abstract_class* Create(uint32_t dwID)
	{
		std::map<uint32_t, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter == m_map.end() )
			return NULL;

		return (abstract_class*)iter->second->Create();
	}

	//! ɾ��ID��������ʵ��
	void Destroy(void* p, uint32_t dwID)
	{
		std::map<uint32_t, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter != m_map.end() )
			iter->second->Destroy(p);
	}

	~TAutoFactory() 
	{	
		std::map<uint32_t, BinderAbstract*>::iterator iter;
		for( iter=m_map.begin(); iter!=m_map.end(); ++iter )
			delete(iter->second);
	}

private:
	bstd::map<uint32_t, BinderAbstract*> m_map;
};


