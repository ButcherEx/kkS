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
	// ���Ԫ�ص�����β
	void PushBack(Type& val)
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.push_back(val);
		++m_Size;
	}

	// ���Ԫ�ص������ײ�
	void PushFront(Type& val)
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.push_front(val);
		++m_Size;
	}

	// �Ӷ���ͷȡԪ��
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

	// �Ӷ���ͷȡԪ��
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

	// ɾ��ָ��Ԫ��
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


	// ���ָ��Ԫ���Ƿ����
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

	// �������Ԫ��
	void Clear()
	{
		AutoLock_T lock(m_Lock);
		m_ImplList.clear();
		m_Size=0;
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,
	// ���������Լ�����һ�ݸ�������
	int32_t	SizeUnSafe() { return m_Size;	}
	int32_t	Size() 
	{ 
		AutoLock_T lock(m_Lock);
		return m_Size;	
	}

	// ��ȫ�Ľ����ݵ���һ����ͨstd::list,�������ݸ���
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
	// ���Ԫ�ص�����β
	void PushBack(Type val)
	{
		m_ImplList.push_back(val);
		++m_Size;
	}

	// �Ӷ���ͷȡԪ��
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

	// ���Ԫ�ص�����ͷ
	void PushFront(Type val)
	{
		m_ImplList.push_front(val);
		++m_Size;
	}

	// �õ���һ��Ԫ�ص�ֵ��������ȡ����
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

	// ɾ��ָ��Ԫ��
	bool Erase(Type& val)
	{
		TListIterator it;
		for(it=m_ImplList.begin(); it!=m_ImplList.end(); ++it)
		{
			if( val == *it )
			{
				m_ImplList.erase(it);
				--m_Size;	// ��Ҫ����nItemNum,���Բ���ֱ����list::remove
				return true;
			}
		}
		return false;
	}

	// ���ָ��Ԫ���Ƿ����
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

	// �������Ԫ��
	void Clear()
	{
		m_ImplList.clear();
		m_Size=0;
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,
	// ���������Լ�����һ�ݸ�������
	int32_t	Size() { return m_Size;	}
	bool Empty() { return (0 == m_Size); }


	//! ��list���������һ��Ԫ��
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


	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool PeekNext(Type& value)
	{
		if( m_Iterator == m_ImplList.end() )
			return false;
		value = *m_Iterator;
		++m_Iterator;
		return true;
	}

	//! ȡ���ⲿ�ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool PeekNext(TListIterator& it, Type& value)
	{
		if( it == m_ImplList.end() )
			return false;
		value = *it;
		++it;
		return true;
	}

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ
	void ResetIterator()
	{ m_Iterator = m_ImplList.begin();	}

	//! �õ�ָ��list��ʼλ�õĵ�����
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
	//! ���Ԫ��
	void Add(KeyType key, ValueType value)
	{
		AutoLock_T lock(m_Lock);
		m_Map.insert(std::pair<KeyType, ValueType>(key, value));
	}

	//! ��ȡԪ��
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

	//! �ı�ָ��key��Ӧ��ֵ
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

	//! ���ָ��Ԫ���Ƿ����
	bool IsExist(KeyType key)
	{
		AutoLock_T lock(m_Lock);
		TMapIterator it = m_Map.find(key);
		return it != m_Map.end();
	}


	//! ɾ��ָ��Ԫ��
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


	//! �������Ԫ��
	void Clear()
	{
		AutoLock_T lock(m_Lock);
		m_Map.clear();
	}

	//! �õ�Ԫ�ظ���
	int32_t	SizeUnSafe() { return (int32_t)m_Map.size(); }
	int32_t	Size() 
	{ 
		AutoLock_T lock(m_Lock);
		return (int32_t)m_Map.size(); 
	}


	//! ������key������һ������std::list,�����ص����ĸ���
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

	//! ������value������һ������std::list,�����ص����ĸ���
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
	//! ���Ԫ��
	void Add(KeyType key, ValueType value)
	{ m_Map.insert(std::make_pair(key, value)); }

	//! ��ȡԪ��
	ValueType Peek(KeyType key)
	{
		ValueType val;
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return val;
		else
			return it->second;
	}

	//! �ı�ָ��key��Ӧ��ֵ
	bool ChangeValue(KeyType key, ValueType new_value)
	{
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;

		it->second = new_value;
		return true;
	}

	//! �ۼ�ָ��key��Ӧ��ֵ�����key�����ڣ���ֱ����ӣ�Ҫ��ValueType������operator+��operator=���أ�
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

	//! ���ָ��Ԫ���Ƿ����
	bool IsExist(KeyType key)
	{
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;
		else
			return true;
	}

	//! ɾ��һ��ָ��Ԫ��
	bool Erase(KeyType key)
	{ 
		TMapIterator it = m_Map.find(key);
		if( it == m_Map.end() )
			return false;

		m_Map.erase(it);
		return true;
	}

	//! �������Ԫ��
	void Clear() { m_Map.clear(); }

	//! �õ�Ԫ�ظ���
	int32_t	Size() { return (int32_t)m_Map.size(); }

	bool Empty() { return m_Map.empty(); }

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ
	void ResetIterator()
	{ m_Iterator = m_Map.begin(); }

	// ! �õ�map��ͷ
	TMapIterator Begin()
	{ return m_Map.begin(); }

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool PeekNext(ValueType& value)
	{
		if( m_Iterator == m_Map.end() )
			return false;
		value = m_Iterator->second;
		++m_Iterator;
		return true;
	}

	//! ȡ��ָ��������ָ���Ԫ�أ������Ƹõ�����
	bool PeekNext(TMapIterator& it, ValueType& value)
	{
		if( it == m_Map.end() )
			return false;
		value = it->second;
		++it;
		return true;
	}

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool PeekNext(KeyType& key, ValueType& value)
	{
		if( m_Iterator == m_Map.end() )
			return false;
		key = m_Iterator->first;
		value = m_Iterator->second;
		++m_Iterator;
		return true;
	}

	//! ȡ��ָ���ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool PeekNext(TMapIterator& it, KeyType& key, ValueType& value)
	{
		if( it == m_Map.end() )
			return false;
		key = it->first;
		value = it->second;
		++it;
		return true;
	}

	//! ��list���������ȡһ��Ԫ��
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


	//! ������key������һ������std::list
	void ExportAllKey(std::list<KeyType>& listDest)
	{
		TMapIterator it;
		for(it = m_Map.begin(); it != m_Map.end(); ++it)
			listDest.push_back(it->first);
	}

	//! ������value������һ������std::list
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