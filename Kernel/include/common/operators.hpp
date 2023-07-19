void* operator new(size_t size) { return (void*)exec()->memory().alloc(size); }

void* operator new[](size_t size) { return (void*)exec()->memory().alloc(size); }

void operator delete(void* p) { exec()->memory().free((u64)p); }

void operator delete(void* p, size_t) { ::operator delete(p); }

void operator delete[](void* p) { exec()->memory().free((u64)p); }

void operator delete[](void* p, size_t) { ::operator delete[](p); }