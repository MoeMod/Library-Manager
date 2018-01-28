#pragma once

#include <memory>
#include <string>
#include <unordered_map>

template<class T>
class weak_ptr_less
{
public:
	bool operator()(const std::weak_ptr<T> &lhs, const std::weak_ptr<T> &rhs) const
	{
		return std::less<std::shared_ptr<T>>()(lhs.lock(), rhs.lock());
	}
};

template<class T>
class SearchTable
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> hashmap;
public:
	std::shared_ptr<T> get(const std::string &key)
	{
		auto iter = hashmap.find(key);
		if (iter == hashmap.end())
			return nullptr;
		return iter->second;
	}
	void set(const std::string &key, std::shared_ptr<T> value)
	{
		hashmap[key] = value;
	}
	void erase(const std::string &key)
	{
		auto iter = hashmap.find(key);
		if (iter == hashmap.end())
			return void();
		hashmap.erase(iter);
	}
	size_t size() const
	{
		return hashmap.size();
	}
};