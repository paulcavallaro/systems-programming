#include <string>
#include <thread>

#include "absl/memory/memory.h"
#include "benchmark/benchmark.h"
#include "gtest/gtest.h"

namespace sysprog {
namespace {

using uint64 = uint64_t;

struct SomeData {
  std::string foo;
  uint64 bar;
};

constexpr uintptr_t kDirtyBit = 0x8000000000000000;
constexpr uintptr_t kPtrMask = 0x7fffffffffffffff;
static_assert(sizeof(uintptr_t) == 8, "Only works on 64-bit systems");

template <typename T>
uintptr_t MarkDirty(T* t) {
  return reinterpret_cast<uintptr_t>(t) | kDirtyBit;
}

bool IsDirty(uintptr_t t) { return t & kDirtyBit; }

template <typename T>
T* GetPointer(uintptr_t t) {
  return reinterpret_cast<T*>(t & kPtrMask);
}

TEST(PointerTagging, RoundTrip) {
  auto data = absl::make_unique<SomeData>();
  SomeData* raw = data.get();
  raw->foo = "abc";
  raw->bar = 123;

  uintptr_t d = MarkDirty(raw);
  EXPECT_TRUE(IsDirty(d));

  raw = GetPointer<SomeData>(d);
  EXPECT_EQ(raw->foo, "abc");
  EXPECT_EQ(raw->bar, 123);
}

void BM_RoundTrip(benchmark::State& state) {
  SomeData* data = new SomeData();
  data->foo = "abc";
  data->bar = 123;
  for (auto _ : state) {
    uintptr_t d = MarkDirty(data);
    if (IsDirty(d)) {
      SomeData* data = GetPointer<SomeData>(d);
      data->foo = "def";
      data->bar = 456;
    }
  }
}

BENCHMARK(BM_RoundTrip);

}  // namespace
}  // namespace sysprog
