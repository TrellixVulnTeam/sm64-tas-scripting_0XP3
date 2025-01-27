#pragma once
#include <unordered_map>
#include <vector>
#include "tasfw/Resource.hpp"

#ifndef LIBSM64_H
#define LIBSM64_H

constexpr int pagesize = 4096;
class LibSm64Mem
{
public:
#if defined(_WIN32)
	std::vector<uint8_t> buf1;
	std::vector<uint8_t> buf2;
#else
	std::unordered_map<void*, std::array<uint8_t, pagesize>> changed_regions;
	uint64_t region_count_at_save_time=0;
#endif
};

class LibSm64 : public Resource<LibSm64Mem>
{
public:
	SharedLib dll;
	std::vector<SegVal> segment;

#if !defined(_WIN32)
	std::vector<uint8_t> original_buf1;
	std::vector<uint8_t> original_buf2;
#endif

	LibSm64(const std::filesystem::path& dllPath);
	void save(LibSm64Mem& state) const;
	void load(const LibSm64Mem& state);
	void advance();
	void* addr(const char* symbol) const;
	std::size_t getStateSize(const LibSm64Mem& state) const;
	uint32_t getCurrentFrame() const;
};

#endif