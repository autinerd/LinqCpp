#pragma once
#ifndef LINQ_HPP
#define LINQ_HPP
#include <vector>
#include <functional>
#include <typeinfo>
#include <exception>

template <class T>
class IEqualityComparer;

template <class T>
class Queryable
{
public:
	Queryable(std::vector<T> vector_);
	Queryable();
	~Queryable();
	Queryable<T> operator=(Queryable<T> second);
	T &operator[](size_t index);
	T const & operator[](size_t index) const;
	bool operator==(Queryable<T> second);
	void Add(T item);
	void AddRange(std::vector<T> & list);
	void AddRange(Queryable<T> && list);
	T Aggregate(std::function<T(T, T)> func);
	template <class TAggregate>
	TAggregate Aggregate(TAggregate seed, std::function<TAggregate(TAggregate, T)> func);
	bool Any();
	bool Any(std::function<bool(T)> predicate);
	bool All(std::function<bool(T)> predicate);
	double Average(std::function<int(T)> selector);
	template <class TResult>
	Queryable<TResult> Cast();
	Queryable<T> Concat(std::vector<T> second);
	Queryable<T> Concat(Queryable<T> second);
	template <class TOutput>
	Queryable<TOutput> ConvertAll(std::function<TOutput(T)> converter);
	bool Contains(T const & item) const;
	size_t Count();
	size_t Count() const;
	size_t Count(std::function<bool(T)> predicate) const;
	Queryable<T> Distinct();
	T ElementAt(int number);
	Queryable<T> Except(Queryable<T> second);
	T &First();
	T &First(std::function<bool(T)> predicate);
	int IndexOf(T item);
	void Insert(int index, T item);
	Queryable<T> Intersect(Queryable<T> second);
	T &Last();
	T &Last(std::function<bool(T)> predicate);
	void Remove(T item);
	void RemoveAll(T item);
	void RemoveAt(int index);
	T Sum();
	bool SequenceEqual(Queryable<T> second);
	bool SequenceEqual(Queryable<T> second, IEqualityComparer<T> comp);
	T &Single();
	Queryable<T> Skip(int number);
	Queryable<T> SkipWhile(std::function<bool(T)> predicate);
	Queryable<T> Take(int number);
	Queryable<T> TakeWhile(std::function<bool(T)> predicate);
	std::vector<T> ToVector() const;
	std::vector<T> ToVector();
	Queryable<T> Union(Queryable<T> const & second) const;
	Queryable<T> Where(std::function<bool(T)> predicate);
	template <class TSecond, class TResult>
	Queryable<TResult> Zip(Queryable<TSecond> second, std::function<TResult(T, TSecond)> resultSelector);
private:
	std::vector<T> _vector;
};

template<class T>
inline Queryable<T>::Queryable(std::vector<T> vector_) : _vector(vector_)
{
}

template<class T>
inline Queryable<T>::Queryable()
{
	_vector = std::vector<T>();
}

template <class T>
Queryable<T>::~Queryable()
{
}

template<class T>
inline Queryable<T> Queryable<T>::operator=(Queryable<T> second)
{
	return Queryable<T>(second.ToVector());
}

template<class T>
inline T &Queryable<T>::operator[](size_t index)
{
	if (index >= this->Count())
	{
		throw std::out_of_range("Index out of range");
	}
	return _vector[index];
}

template<class T>
inline T const & Queryable<T>::operator[](size_t index) const
{
	if (index >= this->Count())
	{
		throw std::out_of_range("Index out of range");
	}
	return _vector[index];
}

template<class T>
inline bool Queryable<T>::operator==(Queryable<T> second)
{
	return this->SequenceEqual(second);
}

template<class T>
inline void Queryable<T>::Add(T item)
{
	_vector.push_back(item);
}

template<class T>
inline void Queryable<T>::AddRange(std::vector<T> & list)
{
	for (T item : list)
	{
		this->Add(item);
	}
}

template<class T>
inline void Queryable<T>::AddRange(Queryable<T> && list)
{
	for (T item : list.ToVector())
	{
		this->Add(item);
	}
}

template<class T>
inline T Queryable<T>::Aggregate(std::function<T(T, T)> func)
{
	T sum;
	size_t ti = typeid(T).hash_code();
	Queryable<size_t> q({ typeid(int).hash_code(), typeid(double).hash_code(), typeid(short).hash_code(), typeid(long).hash_code() });
	if (q.Any([ti](size_t x) {return x == ti; }))
	{
		sum = 0;
	}
	else
	{
		sum = T();
	}

	for (T item : _vector)
	{
		sum = func(sum, item);
	}
	return sum;
}

template<class T>
template<class TAggregate>
TAggregate Queryable<T>::Aggregate(TAggregate seed, std::function<TAggregate(TAggregate, T)> func)
{
	TAggregate v = seed;
	for (T item : _vector)
	{
		v = func(v, item);
	}
	return v;
}

template<class T>
template<class TResult>
inline Queryable<TResult> Queryable<T>::Cast()
{
	Queryable<TResult> q(std::vector<TResult>);
	for (T item : _vector)
	{
		q.Add((TResult)item);
	}
	return q;
}

template<class T>
template<class TOutput>
inline Queryable<TOutput> Queryable<T>::ConvertAll(std::function<TOutput(T)> converter)
{
	Queryable<TOutput> q;
	for (T item : _vector)
	{
		q.Add(converter(item));
	}
	return q;
}

template<class T>
inline bool Queryable<T>::Any()
{
	if (_vector.size() > 0)
		return true;
	else
		return false;
}

template<class T>
inline bool Queryable<T>::Any(std::function<bool(T)> predicate)
{
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			return true;
		}
	}
	return false;
}

template<class T>
inline bool Queryable<T>::All(std::function<bool(T)> predicate)
{
	for (T item : _vector)
	{
		if (predicate(item) == false)
			return false;
	}
	return true;
}

template<class T>
inline double Queryable<T>::Average(std::function<int(T)> selector)
{
	int sum = 0;
	for (T item : _vector)
	{
		sum += selector(item);
	}
	return (double)sum / _vector.size();
}

template<class T>
inline Queryable<T> Queryable<T>::Concat(std::vector<T> second)
{
	std::vector<T> v(_vector);
	for (T item : second)
	{
		v.insert(v.end(), item);
	}
	return Queryable<T>(v);
}

template<class T>
inline Queryable<T> Queryable<T>::Concat(Queryable<T> second)
{
	return this->Concat(second.ToVector());
}

template<class T>
inline bool Queryable<T>::Contains(T const & item) const
{
	for (T listitem : _vector)
	{
		if (listitem == item)
		{
			return true;
		}
	}
	return false;
}

template<class T>
inline size_t Queryable<T>::Count()
{
	return _vector.size();
}

template<class T>
inline size_t Queryable<T>::Count() const
{
	return _vector.size();
}

template<class T>
inline size_t Queryable<T>::Count(std::function<bool(T)> predicate) const
{
	int i = 0;
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			i++;
		}
	}
	return i;
}

template<class T>
inline Queryable<T> Queryable<T>::Distinct()
{
	Queryable<T> q(std::vector<T>);
	for (T item : _vector)
	{
		if (!q.Contains(item))
		{
			q.Add(item);
		}
	}
	return q;
}

template<class T>
inline T Queryable<T>::ElementAt(int number)
{
	return _vector[number];
}

template<class T>
inline Queryable<T> Queryable<T>::Except(Queryable<T> second)
{
	Queryable<T> q(std::vector<T>);
	for (T item : _vector)
	{
		if (!second.Contains(item) && !q.Contains(item))
		{
			q.Add(item);
		}
	}
	return q;
}

template<class T>
inline T &Queryable<T>::First()
{
	return _vector[0];
}

template<class T>
inline T &Queryable<T>::First(std::function<bool(T)> predicate)
{
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			return item;
		}
	}
}

template<class T>
inline int Queryable<T>::IndexOf(T item)
{
	int i = 0;
	for (T vitem : _vector)
	{
		if (item == vitem)
		{
			return i;
		}
		else
		{
			i++;
		}
	}
	return -1;
}

template<class T>
inline void Queryable<T>::Insert(int index, T item)
{
	_vector.insert(_vector.begin() + index, item);
}

template<class T>
inline Queryable<T> Queryable<T>::Intersect(Queryable<T> second)
{
	Queryable<T> q(std::vector<T>);
	for (T item : _vector)
	{
		if (second.Contains(item))
		{
			q.Add(item);
		}
	}
	return q;
}

template<class T>
inline T &Queryable<T>::Last()
{
	return _vector[_vector.size() - 1];
}

template<class T>
inline T &Queryable<T>::Last(std::function<bool(T)> predicate)
{
	T last;
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			last = item;
		}
		return last;
	}
}

template<class T>
inline void Queryable<T>::Remove(T item)
{
	for (int i = 0; i < _vector.size(); i++)
	{
		if (_vector[i] == item)
		{
			_vector.erase(_vector.begin() + i);
			return;
		}
	}
}

template<class T>
inline void Queryable<T>::RemoveAll(T item)
{
	for (int i = 0; i < _vector.size(); i++)
	{
		if (_vector[i] == item)
		{
			_vector.erase(_vector.begin() + i);
		}
	}
}

template<class T>
inline void Queryable<T>::RemoveAt(int index)
{
	_vector.erase(_vector.begin() + index);
}

template<class T>
inline T Queryable<T>::Sum()
{
	T sum;
	size_t ti = typeid(T).hash_code();
	Queryable<size_t> q({ typeid(int).hash_code(), typeid(double).hash_code(), typeid(short).hash_code(), typeid(long).hash_code() });
	if (q.Any([ti](size_t x) {return x == ti; }))
	{
		sum = 0;
	}
	else
	{
		sum = T();
	}
	for (T item : _vector)
	{
		sum += item;
	}
	return sum;
}

template<class T>
inline bool Queryable<T>::SequenceEqual(Queryable<T> second)
{
	std::vector<T> secvec = second.ToVector();
	if (_vector.size() != secvec.size())
	{
		return false;
	}
	for (int i = 0; i < _vector.size(); i++)
	{
		if (_vector[i] != secvec[i])
		{
			return false;
		}
	}
	return true;
}

template<class T>
inline bool Queryable<T>::SequenceEqual(Queryable<T> second, IEqualityComparer<T> comp)
{
	std::vector<T> secvec = second.ToVector();
	if (_vector.size() != secvec.size())
	{
		return false;
	}
	for (int i = 0; i < _vector.size(); i++)
	{
		if (!comp.Equals(_vector[i], secvec[i]))
		{
			return false;
		}
	}
	return true;
}

template<class T>
inline T &Queryable<T>::Single()
{
	if (_vector.size() > 1)
	{
		throw std::length_error("More than one element");
	}
	return _vector[0];
}

template<class T>
inline Queryable<T> Queryable<T>::Skip(int number)
{
	std::vector<T> v;
	for (int i = number - 1; i < _vector.size(); i++)
	{
		v.insert(v.end(), _vector[i]);
	}
	return Queryable<T>(v);
}

template<class T>
inline Queryable<T> Queryable<T>::SkipWhile(std::function<bool(T)> predicate)
{
	int index = 0;
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			index++;
		}
		else
		{
			break;
		}
	}
	std::vector<T> v;
	for (int i = index; i < _vector.size(); i++)
	{
		v.insert(v.end(), _vector[i]);
	}
	return Queryable<T>(v);
}

template<class T>
inline Queryable<T> Queryable<T>::Take(int number)
{
	std::vector<T> v;
	for (int i = 0; i < number; i++)
	{
		v.insert(v.end(), _vector[i]);
	}
	return Queryable(v);
}

template<class T>
inline Queryable<T> Queryable<T>::TakeWhile(std::function<bool(T)> predicate)
{
	std::vector<T> v;
	for (T item : _vector)
	{
		if (predicate(item) == false)
		{
			break;
		}
		else
		{
			v.insert(v.end(), item);
		}
	}
	return Queryable<T>(v);
}
template<class T>
inline std::vector<T> Queryable<T>::ToVector()
{
	return _vector;
}
template<class T>
inline std::vector<T> Queryable<T>::ToVector() const
{
	return _vector;
}

template<class T>
inline Queryable<T> Queryable<T>::Union(Queryable<T> const & second) const
{
	Queryable<T> q;
	for (T item : this->ToVector())
	{
		if (!q.Contains(item))
		{
			q.Add(item);
		}
	}
	for (T item : second.ToVector())
	{
		if (!q.Contains(item))
		{
			q.Add(item);
		}
	}
	return q;
}

template<class T>
inline Queryable<T> Queryable<T>::Where(std::function<bool(T)> predicate)
{
	std::vector<T> v;
	for (T item : _vector)
	{
		if (predicate(item) == true)
		{
			v.insert(v.end(), item);
		}
	}
	return Queryable<T>(v);
}

template<class T>
template<class TSecond, class TResult>
inline Queryable<TResult> Queryable<T>::Zip(Queryable<TSecond> second, std::function<TResult(T, TSecond)> resultSelector)
{
	int min = 0;
	if (_vector.size() > second.ToVector().size())
	{
		min = second.ToVector().size();
	}
	else
	{
		min = _vector.size();
	}
	std::vector<TResult> v;
	for (int i = 0; i < min; i++)
	{
		v.insert(v.end(), resultSelector(_vector[i], second.ToVector()[i]));
	}
	return Queryable<TResult>(v);
}

template <class T>
class IEqualityComparer
{
public:
	virtual bool Equals(T first, T second) = 0;
};

template <class T>
Queryable<T> from_vector(std::vector<T> vector)
{
	return Queryable<T>(vector);
}

template <class TKey, class TValue>
class KeyValuePair
{
public:
	KeyValuePair(TKey key_, TValue value_);
	TKey key;
	TValue value;
private:

};

template<class TKey, class TValue>
inline KeyValuePair<TKey, TValue>::KeyValuePair(TKey key_, TValue value_) : key(key_), value(value_)
{
}

template <class TKey, class TValue>
class Dictionary : public Queryable<KeyValuePair<TKey, TValue>>
{
public:
	Dictionary();
	Dictionary(std::vector<KeyValuePair<TKey, TValue>> vector);
	void Add(TKey key, TValue value);
	void Add(KeyValuePair<TKey, TValue> item);
private:
	std::vector<KeyValuePair<TKey, TValue>> _vector;
};

template<class TKey, class TValue>
inline Dictionary<TKey, TValue>::Dictionary()
{
	_vector = std::vector<KeyValuePair<TKey, TValue>>();
}

template<class TKey, class TValue>
inline Dictionary<TKey, TValue>::Dictionary(std::vector<KeyValuePair<TKey, TValue>> vector) : _vector(vector)
{
}

template<class TKey, class TValue>
inline void Dictionary<TKey, TValue>::Add(TKey key, TValue value)
{
	this->Add(KeyValuePair<TKey, TValue>(key, value));
}

template<class TKey, class TValue>
inline void Dictionary<TKey, TValue>::Add(KeyValuePair<TKey, TValue> item)
{
	_vector.push_back(item);
}

template<class T1, class T2>
class Tuple
{
public:
	Tuple(T1 first, T2 second);
	T1 First();
	T2 Second();
private:
	T1 _first;
	T2 _second;
};

template<class T1, class T2>
inline Tuple<T1, T2>::Tuple(T1 first, T2 second) : _first(first), _second(second)
{
}

template<class T1, class T2>
inline T1 Tuple<T1, T2>::First()
{
	return _first;
}

template<class T1, class T2>
inline T2 Tuple<T1, T2>::Second()
{
	return _second;
}
#endif // !LINQ_HPP
