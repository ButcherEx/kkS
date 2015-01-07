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
		_Assert0((idx >= 0 && idx < SizeMax));
		return array_[idx];
	}

	const Type& operator[](int32_t idx) const
	{
		_Assert0((idx >= 0 && idx < SizeMax));
		return array_[idx];
	}

	int32_t capacity() const { return SizeMax; }

	TypePtr begin(){ return &array_[0]; }
	ConstTypePtr begin() const { return &array_[0]; }

	TypePtr end(){ return &array_[SizeMax]; }
	ConstTypePtr end() const { return &array_[SizeMax]; }
private:
	Type array_[SizeMax];
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
		_Assert0((idx >=0 && idx < size()));
		return ImplVector::operator [](idx);
	}

	const_reference operator[](size_type idx) const
	{
		_Assert0((idx >=0 && idx < size()));
		return ImplVector::operator [](idx);
	}

	void eraseIndex(size_type idx) 
	{
		_Assert0((idx >=0 && idx < size()));
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
	void pushBack(Type& val)
	{
		ScopedLock lock(lock_);
		list_.push_back(val);
		++size_;
	}

	// ���Ԫ�ص������ײ�
	void pushFront(Type& val)
	{
		ScopedLock lock(lock_);
		list_.push_front(val);
		++size_;
	}

	// �Ӷ���ͷȡԪ��
	Type popFront()
	{
		Type val;
		ScopedLock lock(lock_);
		if( size_ > 0 )
		{
			val = list_.front();
			list_.pop_front();
			size_--;
		}
		return val;
	}

	// �Ӷ���ͷȡԪ��
	bool popFront(Type& val)
	{
		ScopedLock lock(lock_);
		if( size_ > 0 )
		{
			val = list_.front();
			list_.pop_front();
			size_--;
			return true;
		}
		return false;
	}

	// ɾ��ָ��Ԫ��
	bool erase(Type& val)
	{
		std::list<Type>::iterator it;
		ScopedLock lock(lock_);
		for(it=list_.begin(); it!=list_.end(); ++it)
		{
			if( val == *it )
			{
				list_.erase(it);
				--size_;
				return true;
			}
		}
		return false;
	}


	// ���ָ��Ԫ���Ƿ����
	bool isExist(Type& val)
	{
		bool bFound = false;

		ScopedLock lock(lock_);

		std::list<Type>::iterator it;
		for(it=list_.begin(); it!=list_.end(); ++it)
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
	void clear()
	{
		ScopedLock lock(lock_);
		list_.clear();
		size_=0;
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,
	// ���������Լ�����һ�ݸ�������
	int32_t	sizeUnSafe() { return size_;	}
	int32_t	size() 
	{ 
		ScopedLock lock(lock_);
		return size_;	
	}

	// ��ȫ�Ľ����ݵ���һ����ͨstd::list,�������ݸ���
	template<class OutList>
	int32_t exportall(OutList& listDest)
	{
		int32_t n=0;
		ScopedLock lock(lock_);
		_implist::iterator it;
		for(it=list_.begin(); it!=list_.end(); ++it, ++n)
			listDest.push_back(*it);

		return n;
	}


	explicit TSList():size_(0) {  }
	~TSList(){ }

private:
	int32_t				size_;
	LockType			lock_;
	_implist			list_;
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
	void pushBack(Type val)
	{
		list_.push_back(val);
		++size_;
	}

	// �Ӷ���ͷȡԪ��
	Type popFront()
	{
		Type val;
		if( size_ > 0 )
		{
			val = list_.front();
			list_.pop_front();
			size_--;
		}

		return val;
	}

	// ���Ԫ�ص�����ͷ
	void pushFront(Type val)
	{
		list_.push_front(val);
		++size_;
	}

	// �õ���һ��Ԫ�ص�ֵ��������ȡ����
	Type front()
	{
		Type val;
		if( size_ > 0 )
			val = list_.front();

		return val;
	}

	// ɾ��ָ��Ԫ��
	bool erase(Type& val)
	{
		TListIterator it;
		for(it=list_.begin(); it!=list_.end(); ++it)
		{
			if( val == *it )
			{
				list_.erase(it);
				--size_;	// ��Ҫ����nItemNum,���Բ���ֱ����list::remove
				return true;
			}
		}
		return false;
	}

	// ���ָ��Ԫ���Ƿ����
	bool isExist(Type& val)
	{
		TListIterator it;
		for(it=list_.begin(); it!=list_.end(); ++it)
		{
			if( val == *it )
				return true;
		}
		return false;
	}

	// �������Ԫ��
	void clear()
	{
		list_.clear();
		size_=0;
	}

	// �õ�Ԫ����Ŀ,std::list.size()�����ܱ�֤�̰߳�ȫ,
	// ���������Լ�����һ�ݸ�������
	int32_t	size() { return size_;	}
	BOOL empty() { return (0 == size_); }


	//! ��list���������һ��Ԫ��
	BOOL randPeek(Type& val, BOOL pop=false)
	{
		if( list_.empty() )
			return false;

		int32_t nRand = rand() % size_;
		TListIterator it = list_.begin();
		for(int32_t n=0; n<nRand; n++)
			++it;

		val = *it;
		if( pop )
		{
			list_.erase(it);
			size_--;
		}

		return true;
	}


	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool peekNext(Type& value)
	{
		if( iterHold_ == list_.end() )
			return false;
		value = *iterHold_;
		++iterHold_;
		return true;
	}

	//! ȡ���ⲿ�ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool peekNext(TListIterator& it, Type& value)
	{
		if( it == list_.end() )
			return false;
		value = *it;
		++it;
		return true;
	}

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ
	void resetIterator()
	{ iterHold_ = list_.begin();	}

	//! �õ�ָ��list��ʼλ�õĵ�����
	TListIterator begin()
	{
		return list_.begin();
	}

	std::list<Type>& listImpl() { return list_; }


	void operator=(_implist& list) { list_ = list;	}
	void operator=(TList<Type>& list) { list_ = list.listImpl(); size_ = list_.size(); iterHold_ = list_.end(); }


	TList():size_(0) {}
	TList(_implist& list){ list_ = list; }
	~TList(){}

private:
	_implist					list_;
	int32_t						size_;
	typename _implist::iterator	iterHold_;
};
//////////////////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType, typename LockType = Mutex> 
class TSMap
{
public:
	typedef boost::container::map<KeyType, ValueType> _mapImpl;
public:
	//! ���Ԫ��
	void add(KeyType key, ValueType value)
	{
		ScopedLock lock(lock_);
		map_.insert(std::pair<KeyType, ValueType>(key, value));
	}

	//! ��ȡԪ��
	ValueType peek(KeyType key)
	{	
		ScopedLock lock(lock_);

		ValueType temp;
		TMapIterator it = map_.find(key);
		if( it != map_.end() )
		{
			temp = it->second;
		}

		return temp;
	}

	//! �ı�ָ��key��Ӧ��ֵ
	bool changeValue(KeyType key, ValueType new_value)
	{
		ScopedLock lock(lock_);
		TMapIterator it = map_.find(key);
		if( it != map_.end() )
		{
			it->second = new_value;
			return true;
		}
		return false;
	}

	//! ���ָ��Ԫ���Ƿ����
	bool isExist(KeyType key)
	{
		ScopedLock lock(lock_);
		TMapIterator it = map_.find(key);
		return it != map_.end();
	}


	//! ɾ��ָ��Ԫ��
	bool erase(KeyType key)
	{
		ScopedLock lock(lock_);

		TMapIterator it = map_.find(key);
		if( it != map_.end() )
		{
			map_.erase(it);
			return true;
		}
		
		return false;
	}


	//! �������Ԫ��
	void clear()
	{
		ScopedLock lock(lock_);
		map_.clear();
	}

	//! �õ�Ԫ�ظ���
	int32_t	sizeUnSafe() { return (int32_t)map_.size(); }
	int32_t	size() 
	{ 
		ScopedLock lock(lock_);
		return (int32_t)map_.size(); 
	}


	//! ������key������һ������std::list,�����ص����ĸ���
	template<class OutList>
	int32_t exportAllKey(OutList& listDest)
	{
		ScopedLock lock(lock_);
		
		int32_t n=0;
		TMapIterator it;
		for(it = map_.begin(); it != map_.end(); ++it, ++n)
			listDest.push_back(it->first);
		
		return n;
	}

	//! ������value������һ������std::list,�����ص����ĸ���
	template<class OutList>
	int32_t exportAllValue(OutList& listDest)
	{
		ScopedLock lock(lock_);

		int32_t n=0;
		TMapIterator it;
		for(it = map_.begin(); it != map_.end(); ++it, ++n)
			listDest.push_back(it->second);

		return n;
	}


	explicit TSMap() {  }
	~TSMap()	{ }


private:
	_mapImpl		 map_;
	LockType		 lock_;
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
	void add(KeyType key, ValueType value)
	{ m_map.insert(std::make_pair(key, value)); }

	//! ��ȡԪ��
	ValueType peek(KeyType key)
	{
		ValueType val;
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return val;
		else
			return it->second;
	}

	//! �ı�ָ��key��Ӧ��ֵ
	bool changeValue(KeyType key, ValueType new_value)
	{
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;

		it->second = new_value;
		return true;
	}

	//! �ۼ�ָ��key��Ӧ��ֵ�����key�����ڣ���ֱ����ӣ�Ҫ��ValueType������operator+��operator=���أ�
	void modifyValue(KeyType key, ValueType mod_value)
	{
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
		{
			m_map.insert(std::make_pair(key, mod_value));
		}
		else
		{
			it->second = it->second + mod_value;
		}
	}

	//! ���ָ��Ԫ���Ƿ����
	bool isExist(KeyType key)
	{
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;
		else
			return true;
	}

	//! ɾ��һ��ָ��Ԫ��
	bool erase(KeyType key)
	{ 
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;

		m_map.erase(it);
		return true;
	}

	//! �������Ԫ��
	void clear() { m_map.clear(); }

	//! �õ�Ԫ�ظ���
	int32_t	size() { return (int32_t)m_map.size(); }

	bool empty() { return m_map.empty(); }

	//! ���ڲ��ĵ�������ʼ����map�Ŀ�ʼ
	void resetIterator()
	{ m_it = m_map.begin(); }

	// ! �õ�map��ͷ
	TMapIterator begin()
	{ return m_map.begin(); }

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool peekNext(ValueType& value)
	{
		if( m_it == m_map.end() )
			return false;
		value = m_it->second;
		++m_it;
		return true;
	}

	//! ȡ��ָ��������ָ���Ԫ�أ������Ƹõ�����
	bool peekNext(TMapIterator& it, ValueType& value)
	{
		if( it == m_map.end() )
			return false;
		value = it->second;
		++it;
		return true;
	}

	//! ȡ���ڲ��ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool peekNext(KeyType& key, ValueType& value)
	{
		if( m_it == m_map.end() )
			return false;
		key = m_it->first;
		value = m_it->second;
		++m_it;
		return true;
	}

	//! ȡ��ָ���ĵ�����ָ���Ԫ�أ������Ƶ�����
	bool peekNext(TMapIterator& it, KeyType& key, ValueType& value)
	{
		if( it == m_map.end() )
			return false;
		key = it->first;
		value = it->second;
		++it;
		return true;
	}

	//! ��list���������ȡһ��Ԫ��
	bool randPeek(KeyType& key, ValueType& value)
	{
		int32_t nSize = m_map.size();
		if( nSize <= 0 )
			return false;

		int32_t nRand = rand() % nSize;

		TMapIterator it = m_map.begin();
		for(int32_t n=0; n<nRand; n++)
			++it;

		key = it->first;
		value = it->second;
		return true;
	}


	//! ������key������һ������std::list
	void exportAllKey(std::list<KeyType>& listDest)
	{
		TMapIterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it)
			listDest.push_back(it->first);
	}

	//! ������value������һ������std::list
	void exportAllValue(std::list<ValueType>& listDest)
	{
		TMapIterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it)
			listDest.push_back(it->second);
	}

private:
	_mapImpl					m_map;
	typename _mapImpl::iterator m_it;
};
//////////////////////////////////////////////////////////////////////////
#endif