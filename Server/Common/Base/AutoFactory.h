
//-----------------------------------------------------------------------------
//!\brief 自动类厂
//!
//!	使用与注册都很方便的自动类厂
//!	使用Register( TBinder<YOUR_CLASS>(), uint32_t dwID )的格式来注册
//!	然后使用Create( uint32_t dwID )来产生类的实体
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! 把一些基本接口绑定到指定类
//-----------------------------------------------------------------------------
class BinderAbstract
{ 	
public:
	virtual void* Create()=0;	// 创建对象的接口
	virtual void Destroy(void*)=0;	// 删除对象的接口
};


//-----------------------------------------------------------------------------
//! 把create,destroy绑定到指定类,与类厂等配合使用
//-----------------------------------------------------------------------------
template<typename concrete_class>class TBinder : public BinderAbstract
{
public:
	void* Create() { return (new concrete_class); }
	void Destroy(void* p) { delete((concrete_class*)p); }
};


//-----------------------------------------------------------------------------
//! 单纯用来传递类型信息的模板类
//-----------------------------------------------------------------------------
template<typename concrete_class>class TWrap{};

template<typename abstract_class>	// 以你需要的指针类型做模板参数
class TAutoFactory
{
public:
	//! 注册各种类
	template<typename concrete_class>
	bool Register(TWrap<concrete_class>, uint32_t dwID)
	{ 
		TBinder<concrete_class>* pBinder = new TBinder<concrete_class>;
		if( NULL == pBinder )
			return false;

		Assert( m_map.find(dwID) == m_map.end() );	// 不允许重复注册
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

	//! 产生ID相符的类的实体
	abstract_class* Create(uint32_t dwID)
	{
		std::map<uint32_t, BinderAbstract*>::iterator iter = m_map.find(dwID);
		if( iter == m_map.end() )
			return NULL;

		return (abstract_class*)iter->second->Create();
	}

	//! 删除ID相符的类的实体
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


