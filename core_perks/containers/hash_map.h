// Core Perks (https://github.com/smogpill/core_perks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once
#include "core_perks/memory/allocators.h"

namespace cp
{
	template <class T>
	struct HashMapHash
	{
		uint32 operator()(const T& key) const { return cp::hash::fast(key); }
	};

	struct HashMapHash<const void*>
	{
		CP_FORCE_INLINE uint32 operator()(const void* key) const { return cp::hash::faster(key); }
	};

	template <class Key, class Value, class Hash = HashMapHash<Key>>
	class HashMap
	{
	public:
		explicit HashMap(AllocTag tag = AllocTag()) : tag_(tag) {}
		~HashMap();
		Value& operator[](const Key& key);
		//Value& operator[](Key&& key);
		Value* find(const Key& key);
		const Value* find(const Key& key) const;
		bool contains(const Key& key) const { return find() != nullptr; }
		bool erase(const Key& key);
		void clear();
		uint32 size() const { return size_; }
		void reserve(uint32 size);

	private:
		struct Node
		{
			union
			{
				Key key_;
				uint32 block_count_;
			};
			uint32 next_;
			Value value_;
		};

		static constexpr uint16 node_count_per_block = page_size / sizeof(Node);
		static constexpr uint16 block_index_stride = std::bit_ceil(node_count_per_block);

		Node& get_bucket_for_hash(uint32 hash) { return buckets_[hash & bucket_mask_]; }
		uint32 get_bucket_count() const { return bucket_mask_ + 1; }
		uint32& first_free_node_id() { return node_blocks_[0][0].next_; }
		uint32 first_free_node_id() const { return CP_AS_CONST(first_free_node_id)(); }
		Node& secondary_node(uint32 id) { return node_blocks_[id / block_index_stride][id % block_index_stride]; }
		const Node& secondary_node(uint32 id) const { return node_blocks_[id / block_index_stride][id % block_index_stride]; }
		
		Node* buckets_ = nullptr;
		Node** node_blocks_ = nullptr;
		uint32 size_ = 0;
		uint32 bucket_mask_ = 0;
		AllocTag tag_ = 0;
	};

	template <class Key, class Value, class Hash>
	HashMap<Key, Value, Hash>::~HashMap()
	{
		clear();
		free(buckets_);
	}

	template <class Key, class Value, class Hash>
	Value& HashMap<Key, Value, Hash>::operator[](const Key& key)
	{
		const uint32 hash = Hash()(key);
		Node* node = &get_bucket_for_hash(hash);
		do
		{
			if (node->key_ == key)
				return node->value_;
			node = node->next_;
		} while (node);
		const uint32 new_size = size_ + 1;
		reserve(new_size);
		size_ = new_size;

		Node& bucket = get_bucket_for_hash(hash);
		node = new (allocate<Node>(tag_))();
		node->key_ = key;
		node->next_ = &bucket;
		bucket = node;
		return node->value_;
	}

	template <class Key, class Value, class Hash>
	bool HashMap<Key, Value, Hash>::erase(const Key& key)
	{
		if (!size_)
			return false;
		Hash hash_fn;
		uint32 hash = hash_fn(key);
		Node& bucket = get_bucket_for_hash(hash);
		Node* node = &bucket;
		Node* prev = nullptr;
		while (node)
		{
			if (node->key_ == key)
			{
				if (prev)
					prev->next_ = node->next_;
				else
					bucket = node->next_;
				node->~Node();
				free(node, tag_);
				size_--;
				return true;
			}
			prev = node;
			node = node->next_;
		}
		return false;
	}

	template <class Key, class Value, class Hash>
	void HashMap<Key, Value, Hash>::clear()
	{
		if (!size_)
			return;
		const uint32 bucket_count = get_bucket_count();
		for (uint16 i = 0; i < bucket_count; ++i)
		{
			Node* node = buckets_[i];
			while (node)
			{
				Node* next = node->next_;
				node->~Node();
				free(node);
				node = next;
			}
			buckets_[i] = nullptr;
		}
		size_ = 0;
	}

	template <class Key, class Value, class Hash>
	void HashMap<Key, Value, Hash>::reserve(uint32 size)
	{
		if (size * 2 <= bucket_count_)
			return;
		uint32 bucket_count = max
		if (bucket_count <= bucket_count_)
			return;
		Node** new_buckets = static_cast<Node**>(calloc(bucket_count, sizeof(Node*), tag_));
		for (uint16 i = 0; i < bucket_count_; ++i)
		{
			Node* node = buckets_[i];
			while (node)
			{
				Node* next = node->next_;
				uint16 new_bucket_index = node->hash_ % bucket_count;
				node->next_ = new_buckets[new_bucket_index];
				new_buckets[new_bucket_index] = node;
				node = next;
			}
		}
		free(buckets_, tag_);
		buckets_ = new_buckets;
		bucket_count_ = bucket_count;
	}

	template <class Key, class Value, class Hash>
	Value* HashMap<Key, Value, Hash>::find(const Key& key)
	{
		if (size_ == 0)
			return nullptr;
		const uint32 hash = Hash()(key);
		Node* node = &get_bucket_for_hash(hash);
		while (node)
		{
			if (node->key_ == key)
				return &node->value_;
			node = node->next_;
		}
		return nullptr;
	}
}
