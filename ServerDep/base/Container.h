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
	// 添加元素到队列尾
	void pushBack(Type& val)
	{
		ScopedLock lock(lock_);
		list_.push_back(val);
		++size_;
	}

	// 添加元素到队列首部
	void pushFront(Type& val)
	{
		ScopedLock lock(lock_);
		list_.push_front(val);
		++size_;
	}

	// 从队列头取元素
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

	// 从队列头取元素
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

	// 删除指定元素
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


	// 检查指定元素是否存在
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

	// 清空所有元素
	void clear()
	{
		ScopedLock lock(lock_);
		list_.clear();
		size_=0;
	}

	// 得到元素数目,std::list.size()并不能保证线程安全,
	// 所以我们自己保存一份个数数据
	int32_t	sizeUnSafe() { return size_;	}
	int32_t	size() 
	{ 
		ScopedLock lock(lock_);
		return size_;	
	}

	// 安全的将数据导入一个普通std::list,返回数据个数
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
	// 添加元素到队列尾
	void pushBack(Type val)
	{
		list_.push_back(val);
		++size_;
	}

	// 从队列头取元素
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

	// 添加元素到队列头
	void pushFront(Type val)
	{
		list_.push_front(val);
		++size_;
	}

	// 得到第一个元素的值，但并不取出来
	Type front()
	{
		Type val;
		if( size_ > 0 )
			val = list_.front();

		return val;
	}

	// 删除指定元素
	bool erase(Type& val)
	{
		TListIterator it;
		for(it=list_.begin(); it!=list_.end(); ++it)
		{
			if( val == *it )
			{
				list_.erase(it);
				--size_;	// 需要处理nItemNum,所以不能直接用list::remove
				return true;
			}
		}
		return false;
	}

	// 检查指定元素是否存在
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

	// 清空所有元素
	void clear()
	{
		list_.clear();
		size_=0;
	}

	// 得到元素数目,std::list.size()并不能保证线程安全,
	// 所以我们自己保存一份个数数据
	int32_t	size() { return size_;	}
	BOOL empty() { return (0 == size_); }


	//! 从list里面随机读一个元素
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


	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool peekNext(Type& value)
	{
		if( iterHold_ == list_.end() )
			return false;
		value = *iterHold_;
		++iterHold_;
		return true;
	}

	//! 取出外部的迭代器指向的元素，并后移迭代器
	bool peekNext(TListIterator& it, Type& value)
	{
		if( it == list_.end() )
			return false;
		value = *it;
		++it;
		return true;
	}

	//! 将内部的迭代器初始化到map的开始
	void resetIterator()
	{ iterHold_ = list_.begin();	}

	//! 得到指向list开始位置的迭代器
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
	//! 添加元素
	void add(KeyType key, ValueType value)
	{
		ScopedLock lock(lock_);
		map_.insert(std::pair<KeyType, ValueType>(key, value));
	}

	//! 读取元素
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

	//! 改变指定key对应的值
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

	//! 检查指定元素是否存在
	bool isExist(KeyType key)
	{
		ScopedLock lock(lock_);
		TMapIterator it = map_.find(key);
		return it != map_.end();
	}


	//! 删除指定元素
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


	//! 清空所有元素
	void clear()
	{
		ScopedLock lock(lock_);
		map_.clear();
	}

	//! 得到元素个数
	int32_t	sizeUnSafe() { return (int32_t)map_.size(); }
	int32_t	size() 
	{ 
		ScopedLock lock(lock_);
		return (int32_t)map_.size(); 
	}


	//! 将所有key导出到一个给定std::list,并返回导出的个数
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

	//! 将所有value导出到一个给定std::list,并返回导出的个数
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
	//! 添加元素
	void add(KeyType key, ValueType value)
	{ m_map.insert(std::make_pair(key, value)); }

	//! 读取元素
	ValueType peek(KeyType key)
	{
		ValueType val;
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return val;
		else
			return it->second;
	}

	//! 改变指定key对应的值
	bool changeValue(KeyType key, ValueType new_value)
	{
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;

		it->second = new_value;
		return true;
	}

	//! 累加指定key对应的值，如果key不存在，则直接添加（要求ValueType必须有operator+和operator=重载）
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

	//! 检查指定元素是否存在
	bool isExist(KeyType key)
	{
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;
		else
			return true;
	}

	//! 删除一个指定元素
	bool erase(KeyType key)
	{ 
		TMapIterator it = m_map.find(key);
		if( it == m_map.end() )
			return false;

		m_map.erase(it);
		return true;
	}

	//! 清空所有元素
	void clear() { m_map.clear(); }

	//! 得到元素个数
	int32_t	size() { return (int32_t)m_map.size(); }

	bool empty() { return m_map.empty(); }

	//! 将内部的迭代器初始化到map的开始
	void resetIterator()
	{ m_it = m_map.begin(); }

	// ! 得到map的头
	TMapIterator begin()
	{ return m_map.begin(); }

	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool peekNext(ValueType& value)
	{
		if( m_it == m_map.end() )
			return false;
		value = m_it->second;
		++m_it;
		return true;
	}

	//! 取出指定迭代器指向的元素，并后移该迭代器
	bool peekNext(TMapIterator& it, ValueType& value)
	{
		if( it == m_map.end() )
			return false;
		value = it->second;
		++it;
		return true;
	}

	//! 取出内部的迭代器指向的元素，并后移迭代器
	bool peekNext(KeyType& key, ValueType& value)
	{
		if( m_it == m_map.end() )
			return false;
		key = m_it->first;
		value = m_it->second;
		++m_it;
		return true;
	}

	//! 取出指定的迭代器指向的元素，并后移迭代器
	bool peekNext(TMapIterator& it, KeyType& key, ValueType& value)
	{
		if( it == m_map.end() )
			return false;
		key = it->first;
		value = it->second;
		++it;
		return true;
	}

	//! 从list里面随机抽取一个元素
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


	//! 将所有key导出到一个给定std::list
	void exportAllKey(std::list<KeyType>& listDest)
	{
		TMapIterator it;
		for(it = m_map.begin(); it != m_map.end(); ++it)
			listDest.push_back(it->first);
	}

	//! 将所有value导出到一个给定std::list
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