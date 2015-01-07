#ifndef BASE_CONTAINER_H
#define BASE_CONTAINER_H
//////////////////////////////////////////////////////////////////////////
template<typename Type, int32_t SizeMax>
class TArray
{
public:
	typedef Type* TypePtr;
	typedef const Type* ConstTypePtr;
public:
	Type& operator[](int32_t idx)
	{
		Assert((idx >= 0 && idx < SizeMax));
		return m_ElemList[idx];
	}

	const Type& operator[](int32_t idx) const
	{
		Assert((idx >= 0 && idx < SizeMax));
		return m_ElemList[idx];
	}

	int32_t Capacity() const { return SizeMax; }

	TypePtr Begin(){ return &m_ElemList[0]; }
	ConstTypePtr Begin() const { return &m_ElemList[0]; }

	TypePtr End(){ return &m_ElemList[SizeMax]; }
	ConstTypePtr End() const { return &m_ElemList[SizeMax]; }
private:
	Type m_ElemList[SizeMax];
};
//////////////////////////////////////////////////////////////////////////
template<typename Type>
class TVector : public bstd::vector<Type>
{
public:
	typedef bstd::vector<Type> ImplVector;
	typedef typename ImplVector::reference reference;
	typedef typename ImplVector::const_reference const_reference;
	typedef typename ImplVector::size_type size_type;
public:
	reference operator[](size_type idx)
	{
		Assert((idx >=0 && idx < size()));
		return ImplVector::operator [](idx);
	}

	const_reference operator[](size_type idx) const
	{
		Assert((idx >=0 && idx < size()));
		return ImplVector::operator [](idx);
	}

	void EraseIndex(size_type idx) 
	{
		Assert((idx >=0 && idx < size()));
		ImplVector::erase(begin()+idx);
	}
};
//////////////////////////////////////////////////////////////////////////
template<typename Type, typename LockType = Mutex> 
class TSList
{
public:
	typedef boost::container::list<Type>  _implist;
public:
	// 添加元素到队列尾
	void PushBack(Type& val)
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.push_back(val);
		++m_Size;
	}

	// 添加元素到队列首部
	void PushFront(Type& val)
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.push_front(val);
		++m_Size;
	}

	// 从队列头取元素
	Type PopFront()
	{
		Type val;
		AutoLock_T lock(m_Lock);
		if( m_Size > 0 )
		{
			val = m_ImplList.front();
			m_ImplList.pop_front();
			m_Size--;
		}
		return val;
	}

	// 从队列头取元素
	bool PopFront(Type& val)
	{
		AutoLock_T lock(m_Lock);
		if( m_Size > 0 )
		{
			val = m_ImplList.front();
			m_ImplList.pop_front();
			m_Size--;
			return true;
		}
		return false;
	}

	// 删除指定元素
	bool Erase(Type& val)
	{
		std::list<Type>::iterator it;
		AutoLock_T lock(m_Lock);
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it)
		{
			if( val == *it )
			{
				m_ImplList.erase(it);
				--m_Size;
				return true;
			}
		}
		return false;
	}


	// 检查指定元素是否存在
	bool IsExist(Type& val)
	{
		bool bFound = false;

		AutoLock_T lock(m_Lock);

		std::list<Type>::iterator it;
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it)
		{
			if( val == *it )
			{
				bFound = true;
				break;
			}
		}
		
		return bFound;
	}

	// 清空所有元素
	void Clear()
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.clear();
		m_Size=0;
	}

	// 得到元素数目,std::list.size()并不能保证线程安全,
	// 所以我们自己保存一份个数数据
	int32_t	SizeUnSafe() { return m_Size;	}
	int32_t	Size() 
	{ 
		AutoLock_T lock(m_Lock);
		return m_Size;	
	}

	// 安全的将数据导入一个普通std::list,返回数据个数
	template<class OutList>
	int32_t Exportall(OutList& listDest)
	{
		int32_t n=0;
		AutoLock_T lock(m_Lock);
		_implist::iterator it;
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it, ++n)
			listDest.push_back(*it);

		return n;
	}


	explicit TSList():m_Size(0) {  }
	~TSList(){ }

private:
	int32_t				m_Size;
	LockType			m_Lock;
	_implist			m_ImplList;
};
//////////////////////////////////////////////////////////////////////////
template<typename Type> 
class TList
{
public:
	typedef boost::container::list<Type>  _implist;
	typedef typename _implist::iterator TListIterator;

public:
	// 添加元素到队列尾
	void PushBack(Type val)
	{
		m_ImplList.push_back(val);
		++m_Size;
	}

	// 从队列头取元素
// 	Type PopFront()
// 	{
// 		Type val;
// 		if( m_Size > 0 )
// 		{
// 			val = m_ImplList.front();
// 			m_ImplList.pop_front();
// 			m_Size--;
// 		}
// 
// 		return val;
// 	}

	bool  PopFront(Type& rVal)
	{
		if( m_Size > 0 )
		{
			rVal = m_ImplList.front();
			m_ImplList.pop_front();
			m_Size--;
			return true;
		}

		return false;
	}

	// 添加元素到队列头
	void PushFront(Type val)
	{
		m_ImplList.push_front(val);
		++m_Size;
	}

	// 得到第一个元素的值，但并不取出来
// 	Type Front()
// 	{
// 		Type val;
// 		if( m_Size > 0 )
// 			val = m_ImplList.front();
// 
// 		return val;
// 	}

	bool Front(Type& rVal)
	{
		if( m_Size > 0 )
		{
			rVal = m_ImplList.front();
			return true;
		}

		return false;
	}

	// 删除指定元素
	bool Erase(Type& val)
	{
		TListIterator it;
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it)
		{
			if( val == *it )
			{
				m_ImplList.erase(it);
				--m_Size;	// 需要处理nItemNum,所以不能直接用list::remove
				return true;
			}
		}
		return false;
	}

	// 检查指定元素是否存在
	bool IsExist(Type& val)
	{
		TListIterator it;
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it)
		{
			if( val == *it )
				return true;
		}
		return false;
	}

	// 清空所有元素
	void Clear()
	{
		m_ImplList.clear();
		m_Size=0;
	}

	// 得到元素数目,std::list.size()并不能保证线程安全,
	// 所以我们自己保存一份个数数据
	int32_t	Size() { return m_Size;	}
	bool Empty() { return (0 == m_Size); }


	//! 从list里面随机读一个元素
	bool RandPeek(Type& val, bool pop=false)
	{
		if( m_ImplList.empty() )
			return false;

		int32_t nRand = rand() % m_Size;
		TListIterator it = m_ImplList.begin();
		for(int32_t n=0; n<nRand; n++)
			++it;

		val = *it;
		if( pop )
		{
			m_ImplList.erase(it);
			m_Size--;
		}

		return true;
	}


	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool PeekNext(Type& value)
	{
		if( m_Iterator == m_ImplList.end() )
			return false;
		value = *m_Iterator;
		++m_Iterator;
		return true;
	}

	//! 取出外部的迭代器指向的元素，并后移迭代器
	bool PeekNext(TListIterator& it, Type& value)
	{
		if( it == m_ImplList.end() )
			return false;
		value = *it;
		++it;
		return true;
	}

	//! 将内部的迭代器初始化到map的开始
	void ResetIterator()
	{ m_Iterator = m_ImplList.begin();	}

	//! 得到指向list开始位置的迭代器
	TListIterator Begin()
	{
		return m_ImplList.begin();
	}

	std::list<Type>& ListImpl() { return m_ImplList; }


	void operator=(_implist& list) { m_ImplList = list;	}
	void operator=(TList<Type>& list) { m_ImplList = list.listImpl(); m_Size = m_ImplList.size(); m_Iterator = m_ImplList.end(); }


	TList():m_Size(0) {}
	TList(_implist& list){ m_ImplList = list; }
	~TList(){}

private:
	_implist					m_ImplList;
	int32_t						m_Size;
	typename _implist::iterator	m_Iterator;
};
//////////////////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType, typename LockType = Mutex> 
class TSMap
{
public:
	typedef boost::container::map<KeyType, ValueType> _mapImpl;
public:
	//! 添加元素
	void Add(KeyType key, ValueType value)
	{
		AutoLock_T lock(m_Lock);
		m_Map.insert(std::pair<KeyType, ValueType>(key, value));
	}

	//! 读取元素
	ValueType Peek(KeyType key)
	{	
		AutoLock_T lock(m_Lock);

		ValueType temp;
		TMapIterator it = m_Map.find(key);
		if( it != m_Map.end() )
		{
			temp = it->second;
		}

		return temp;
	}

	//! 改变指定key对应的值
	bool ChangeValue(KeyType key, ValueType new_value)
	{
		AutoLock_T lock(m_Lock);
		TMapIterator it = m_Map.find(key);
		if( it != m_Map.end() )
		{
			it->second = new_value;
			return true;
		}
		return false;
	}

	//! 检查指定元素是否存在
	bool IsExist(KeyType key)
	{
		AutoLock_T lock(m_Lock);
		TMapIterator it = m_Map.find(key);
		return it != m_Map.end();
	}


	//! 删除指定元素
	bool Erase(KeyType key)
	{
		AutoLock_T lock(m_Lock);

		TMapIterator it = m_Map.find(key);
		if( it != m_Map.end() )
		{
			m_Map.erase(it);
			return true;
		}
		
		return false;
	}


	//! 清空所有元素
	void Clear()
	{
		AutoLock_T lock(m_Lock);
		m_Map.clear();
	}

	//! 得到元素个数
	int32_t	SizeUnSafe() { return (int32_t)m_Map.size(); }
	int32_t	Size() 
	{ 
		AutoLock_T lock(m_Lock);
		return (int32_t)m_Map.size(); 
	}


	//! 将所有key导出到一个给定std::list,并返回导出的个数
	template<class OutList>
	int32_t ExportAllKey(OutList& listDest)
	{
		AutoLock_T lock(m_Lock);
		
		int32_t n=0;
		TMapIterator it;
		for(it = m_Map.begin(); it != m_Map.end(); ++it, ++n)
			listDest.push_back(it->first);
		
		return n;
	}

	//! 将所有value导出到一个给定std::list,并返回导出的个数
	template<class OutList>
	int32_t ExportAllValue(OutList& listDest)
	{
		AutoLock_T lock(m_Lock);

		int32_t n=0;
		TMapIterator it;
		for(it = m_Map.begin(); it != m_Map.end(); ++it, ++n)
			listDest.push_back(it->second);

		return n;
	}


	explicit TSMap() {  }
	~TSMap()	{ }


private:
	_mapImpl		 m_Map;
	LockType		 m_Lock;
};

//////////////////////////////////////////////////////////////////////////
template<class KeyType, class ValueType> 
class TMap
{
public:
	typedef boost::container::map<KeyType, ValueType> _mapImpl;
	typedef typename _mapImpl::iterator TMapIterator;

public:
	//! 添加元素
	void Add(KeyType key, ValueType value)
	{ m_Map.insert(std::make_pair(key, value)); }

	//! 读取元素
	ValueType Peek(KeyType key)
	{
		ValueType val;
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return val;
		else
			return it->second;
	}

	//! 改变指定key对应的值
	bool ChangeValue(KeyType key, ValueType new_value)
	{
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;

		it->second = new_value;
		return true;
	}

	//! 累加指定key对应的值，如果key不存在，则直接添加（要求ValueType必须有operator+和operator=重载）
	void ModifyValue(KeyType key, ValueType mod_value)
	{
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
		{
			m_Map.insert(std::make_pair(key, mod_value));
		}
		else
		{
			it->second = it->second + mod_value;
		}
	}

	//! 检查指定元素是否存在
	bool IsExist(KeyType key)
	{
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;
		else
			return true;
	}

	//! 删除一个指定元素
	bool Erase(KeyType key)
	{ 
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;

		m_Map.erase(it);
		return true;
	}

	//! 清空所有元素
	void Clear() { m_Map.clear(); }

	//! 得到元素个数
	int32_t	Size() { return (int32_t)m_Map.size(); }

	bool Empty() { return m_Map.empty(); }

	//! 将内部的迭代器初始化到map的开始
	void ResetIterator()
	{ m_Iterator = m_Map.begin(); }

	// ! 得到map的头
	TMapIterator Begin()
	{ return m_Map.begin(); }

	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool PeekNext(ValueType& value)
	{
		if( m_Iterator == m_Map.end() )
			return false;
		value = m_Iterator->second;
		++m_Iterator;
		return true;
	}

	//! 取出指定迭代器指向的元素，并后移该迭代器
	bool PeekNext(TMapIterator& it, ValueType& value)
	{
		if( it == m_Map.end() )
			return false;
		value = it->second;
		++it;
		return true;
	}

	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool PeekNext(KeyType& key, ValueType& value)
	{
		if( m_Iterator == m_Map.end() )
			return false;
		key = m_Iterator->first;
		value = m_Iterator->second;
		++m_Iterator;
		return true;
	}

	//! 取出指定的迭代器指向的元素，并后移迭代器
	bool PeekNext(TMapIterator& it, KeyType& key, ValueType& value)
	{
		if( it == m_Map.end() )
			return false;
		key = it->first;
		value = it->second;
		++it;
		return true;
	}

	//! 从list里面随机抽取一个元素
	bool RandPeek(KeyType& key, ValueType& value)
	{
		int32_t nSize = m_Map.size();
		if( nSize <= 0 )
			return false;

		int32_t nRand = rand() % nSize;

		TMapIterator it = m_Map.begin();
		for(int32_t n=0; n<nRand; n++)
			++it;

		key = it->first;
		value = it->second;
		return true;
	}


	//! 将所有key导出到一个给定std::list
	void ExportAllKey(std::list<KeyType>& listDest)
	{
		TMapIterator it;
		for(it = m_Map.begin(); it != m_Map.end(); ++it)
			listDest.push_back(it->first);
	}

	//! 将所有value导出到一个给定std::list
	void ExportAllValue(std::list<ValueType>& listDest)
	{
		TMapIterator it;
		for(it = m_Map.begin(); it != m_Map.end(); ++it)
			listDest.push_back(it->second);
	}

private:
	_mapImpl					m_Map;
	typename _mapImpl::iterator m_Iterator;
};
//////////////////////////////////////////////////////////////////////////
#endif